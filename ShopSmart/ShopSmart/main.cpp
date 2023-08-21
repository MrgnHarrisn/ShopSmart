#include <iostream>
#include <string>
#include "SupermarketAPI.h"

using namespace std;

int main()
{
	SupermarketAPI sapi;
	auto supermarkets = sapi.fetchSupermarkets();
	if (supermarkets.empty()) {
		printf("No super markets found!\n");
		return 1;
	}

	/*for (const auto& market : supermarkets) {
		cout << market.at("name") << " | " << market.at("id") << endl;
	}*/
	/* 176 Countdown  */
	/* ___ Pak'n Save */

	vector<map<string, string>> broken_places;
	for (int i = 0; i < supermarkets.size(); i++) {
		try {
			sapi.selectSupermarket(supermarkets[i]);
		}
		catch (...) {
			// printf("Broke at index: %i\n", i);
			cout << i << endl;
			broken_places.push_back(supermarkets[i]);
		}

		try {
			auto products = sapi.searchProduct("Milk", supermarkets[i]);
		}
		catch (...) {
			/* Something went wrong */
		}

	}

	// Step 5: Print out the results
	/*std::cout << "Search Results for 'milk' in " << supermarkets[index]["name"] << ":" << std::endl;
	for (const auto& product : products) {
		std::cout << "Product Name: " << product.at("name") << ", Price: $" << product.at("price") << std::endl;
	}*/

	if (!broken_places.empty()) {
		for (int i = 0; i < broken_places.size(); i++) {
			cout << broken_places[i].at("name") << endl;
			cout << broken_places[i].at("id") << endl;
		}
	}

	printf("We good\n");
	return 0;

}