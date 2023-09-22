#include "SupermarketAPI.h"
#include <iostream>

/*!
 * \brief The constructor for the SupermarketAPI class.
 */
SupermarketAPI::SupermarketAPI() :
	PAKNSAVE_BASE_URL("https://www.paknsave.co.nz"),
	COUNTDOWN_BASE_URL("https://www.countdown.co.nz"),
	NEWWORLD_BASE_URL("https://www.newworld.co.nz"),
	ITEMS_PER_PAGE(10)
{
	// Initializing the clients with the cookies
	countdownClient = cpr::Session();
	countdownClient.SetHeader(countdownHeaders());
	countdownClient.SetTimeout(cpr::Timeout(3000));

	paknsaveClient = cpr::Session();
	paknsaveClient.SetHeader(paknsaveHeaders());
	paknsaveClient.SetTimeout(cpr::Timeout(3000));

	newWorldClient = cpr::Session();
	newWorldClient.SetHeader(newWorldHeaders());
	newWorldClient.SetTimeout(cpr::Timeout(3000));
}

/*!
 * \brief Returns common headers used in all supermarket requests.
 * \return Common headers as a cpr::Header object.
 */
cpr::Header SupermarketAPI::commonHeaders()
{
	cpr::Header headers;
	headers["accept"] = "application/json, text/plain, */*";
	headers["accept-language"] = "en-GB,en-US;q=0.9,en;q=0.8";
	headers["sec-ch-ua"] = "\"Not/A)Brand\";v=\"99\", \"Google Chrome\";v=\"115\", \"Chromium\";v=\"115\"";
	headers["sec-ch-ua-mobile"] = "?0";
	headers["sec-ch-ua-platform"] = "\"macOS\"";
	headers["sec-fetch-dest"] = "empty";
	headers["sec-fetch-mode"] = "cors";
	headers["sec-fetch-site"] = "same-origin";
	headers["user-agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36";
	return headers;
}

/*!
 * \brief Returns specific headers for the Countdown supermarket.
 * \return Countdown headers as a cpr::Header object.
 */
cpr::Header SupermarketAPI::countdownHeaders()
{
	cpr::Header headers = commonHeaders();
	headers["cache-control"] = "no-cache";
	headers["content-type"] = "application/json";
	headers["expires"] = "Sat, 01 Jan 2000 00:00:00 GMT";
	headers["pragma"] = "no-cache";
	headers["x-requested-with"] = "OnlineShopping.WebApp";
	headers["x-ui-ver"] = "7.21.138";
	
	return headers;
}

/*!
 * \brief Returns specific headers for the Pak'nSave supermarket.
 * \return Pak'nSave headers as a cpr::Header object.
 */
cpr::Header SupermarketAPI::paknsaveHeaders()
{
	cpr::Header headers = commonHeaders();
	headers["referrer"] = PAKNSAVE_BASE_URL;
	headers["referrerPolicy"] = "no-referrer-when-downgrade";
	headers["user-agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36";
	return headers;
}

/*!
 * \brief Returns specific headers for the New World supermarket.
 * \return New World headers as a cpr::Header object.
 */
cpr::Header SupermarketAPI::newWorldHeaders()
{
	cpr::Header headers = commonHeaders();
	headers["referrer"] = NEWWORLD_BASE_URL;
	headers["referrerPolicy"] = "no-referrer-when-downgrade";
	headers["user-agent"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36";
	return headers;
}

/*!
 * \brief Fetches a list of supermarkets from each platform's API.
 * \return Vector of maps containing supermarket information.
 */
std::vector<store_t> SupermarketAPI::fetchSupermarkets()
{
	std::vector<store_t> supermarkets;

	// Fetching for Countdown
	countdownClient.SetUrl(COUNTDOWN_BASE_URL + "/api/v1/addresses/pickup-addresses");

	auto countdownResponse = countdownClient.Get();
	if (countdownResponse.error) {
		std::cout << "Request error: " << countdownResponse.error.message << std::endl;
	}
	auto countdownData = nlohmann::json::parse(countdownResponse.text);
	
	for (const auto& store : countdownData["storeAreas"][0]["storeAddresses"])
	{
		supermarkets.push_back({ { "name", store["name"] }, { "type", "Countdown" }, { "id", std::to_string(store["id"].get<int>()) }, { "address", store["address"]}});
	}

	// Fetching for Pak'nSave
	paknsaveClient.SetUrl(PAKNSAVE_BASE_URL + "/CommonApi/Store/GetStoreList");
	auto paknsaveResponse = paknsaveClient.Post();
	auto paknsaveData = nlohmann::json::parse(paknsaveResponse.text);
	for (const auto& store : paknsaveData["stores"])
	{
		supermarkets.push_back({ { "name", store["name"] }, { "type", "Pak'nSave" }, { "id", store["id"] }, {"address", store["address"]}});
	}

	// Fetching for New World
	newWorldClient.SetUrl(NEWWORLD_BASE_URL + "/CommonApi/Store/GetStoreList");
	auto newWorldResponse = newWorldClient.Post();
	auto newWorldData = nlohmann::json::parse(newWorldResponse.text);
	for (const auto& store : newWorldData["stores"])
	{
		supermarkets.push_back({ { "name", store["name"] }, { "type", "New World" }, { "id", store["id"] }, {"address", store["address"]} });
	}

	return supermarkets;
}

// Selects a supermarket to perform subsequent operations. Changes the context in which subsequent API calls are made.

/*!
 * \brief Selects a supermarket to perform subsequent operations.
 * \details Changes the context in which subsequent API calls are made.
 * \param supermarket Map containing supermarket information.
 */

void SupermarketAPI::selectSupermarket(const store_t& supermarket)
{

	if (supermarket.at("type") == "Countdown")
	{
		countdownClient.SetUrl(COUNTDOWN_BASE_URL + "/api/v1/fulfilment/my/pickup-addresses");
		countdownClient.SetBody("addressId=" + supermarket.at("id"));
		countdownClient.Put();
	}
	else if (supermarket.at("type") == "Pak'nSave")
	{
		paknsaveClient.SetUrl(PAKNSAVE_BASE_URL + "/CommonApi/Store/ChangeStore?storeId=" + supermarket.at("id") + "&clickSource=list");
		paknsaveClient.Post();
	}
	else if (supermarket.at("type") == "New World")
	{
		newWorldClient.SetUrl(NEWWORLD_BASE_URL + "/CommonApi/Store/ChangeStore?storeId=" + supermarket.at("id") + "&clickSource=list");
		newWorldClient.Post();
	}
}

/*!
 * \brief Searches for products in the selected supermarket.
 * \details Returns the results from the specified page.
 * \param term The search term.
 * \param supermarket Map containing supermarket information.
 * \param page The page number.
 * \return Vector of maps containing product information.
 */
std::vector<store_t> SupermarketAPI::searchProduct(const std::string& term, const store_t& supermarket, int page)
{
	std::vector<store_t> products;
	if (supermarket.at("type") == "Countdown")
	{	
		countdownClient.SetUrl(COUNTDOWN_BASE_URL + "/api/v1/products?target=search&search=" + term + "&page=" + std::to_string(page) + "&inStockProductsOnly=false&size=" + std::to_string(ITEMS_PER_PAGE));
		auto response = countdownClient.Get();
		auto productData = nlohmann::json::parse(response.text);
		if (productData.contains("products") && productData["products"].contains("items")) {
			for (const auto& product : productData["products"]["items"]) {

				/* Check if the type is a product */
				if (product["type"].get<std::string>() == "Product") {
					try {
						std::string product_name = product["name"];
						double sale_price = product["price"]["salePrice"].get<double>();
						std::string sale_price_str = std::to_string(sale_price);

						products.push_back({
							{ "name", product_name },
							{ "price", sale_price_str }
							});
					}
					catch (...) {
						printf("Something went wrong\n");
						std::cout << product["type"] << std::endl;
						std::cout << product["name"] << std::endl;
						throw new std::exception();
					}
				}

			}
		}
		else {
			printf("Error\n");
		}
	}
	else if (supermarket.at("type") == "Pak'nSave")
	{
		paknsaveClient.SetUrl(PAKNSAVE_BASE_URL + "/next/api/products/search?q=" + term + "&s=popularity&pg=" + std::to_string(page) + "&storeId=" + supermarket.at("id") + "&publish=true&ps=" + std::to_string(ITEMS_PER_PAGE));
		std::cout << paknsaveClient.Get().url << std::endl;
		auto response = paknsaveClient.Get();
		auto productData = nlohmann::json::parse(response.text);

		if (productData["data"].contains("products")) {
			for (const auto& product : productData["data"]["products"]) {
				products.push_back({ { "name", product["name"] }, { "price", std::to_string(product["price"] / 100.0) } });
			}
		}
		else {
			std::cout << paknsaveClient.Get().reason << std::endl;
		}
	}
	else if (supermarket.at("type") == "New World")
	{	
		newWorldClient.SetUrl(NEWWORLD_BASE_URL + "/next/api/products/search?q=" + term + "&s=popularity&pg=" + std::to_string(page) + "&storeId=" + supermarket.at("id") + "&publish=true&ps=" + std::to_string(ITEMS_PER_PAGE));
		std::cout << newWorldClient.Get().url << std::endl;
		auto response = newWorldClient.Get();
		auto productData = nlohmann::json::parse(response.text);
		if (productData["data"].contains("products")) {
			for (const auto& product : productData["data"]["products"])
				products.push_back({ { "name", product["name"] }, { "price", std::to_string(product["price"] / 100.0) } });
		}
		else {
			std::cout << newWorldClient.Get().reason << std::endl;
		}
	}

	return products;
}