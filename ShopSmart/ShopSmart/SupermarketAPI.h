#ifndef SUPERMARKETAPI_H
#define SUPERMARKETAPI_H

#include <cpr/cpr.h>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

#define store_t std::map<std::string, std::string>

class SupermarketAPI {
private:
    const std::string PAKNSAVE_BASE_URL;
    const std::string COUNTDOWN_BASE_URL;
    const std::string NEWWORLD_BASE_URL;
    const int ITEMS_PER_PAGE;

    cpr::Session countdownClient, paknsaveClient, newWorldClient;


    /*!
     * \brief Returns common headers used in all supermarket requests.
     * \return Common headers as a cpr::Header object.
     */
    cpr::Header commonHeaders();


    /*!
     * \brief Returns specific headers for the Countdown supermarket.
     * \return Countdown headers as a cpr::Header object.
     */
    cpr::Header countdownHeaders();

    /*!
     * \brief Returns specific headers for the Pak'nSave supermarket.
     * \return Pak'nSave headers as a cpr::Header object.
     */
    cpr::Header paknsaveHeaders();

    /*!
     * \brief Returns specific headers for the New World supermarket.
     * \return New World headers as a cpr::Header object.
     */
    cpr::Header newWorldHeaders();

    /*!
    * \brief Get's Authorization token for the CommonAPI for PaknSave and NewWorld
    * \return Get's the latest Authorization token
    */
    std::string getAuth(const std::string& BASE_URL);

public:

    /*!
     * \brief The constructor for the SupermarketAPI class.
     */
    SupermarketAPI();


    /*!
     * \brief Fetches a list of supermarkets from each platform's API.
     * \return Vector of maps containing supermarket information.
     */
    std::vector<store_t> fetchSupermarkets();

    /*!
     * \brief Selects a supermarket to perform subsequent operations.
     * \details Changes the context in which subsequent API calls are made.
     * \param supermarket Map containing supermarket information.
     */
    void selectSupermarket(const store_t& supermarket);

     /*!
     * \brief Searches for products in the selected supermarket.
     * \details Returns the results from the specified page.
     * \param term The search term.
     * \param supermarket Map containing supermarket information.
     * \param page The page number.
     * \return Vector of maps containing product information.
     */
    std::vector<store_t> searchProduct(const std::string& term, const store_t& supermarket, int page = 1);
};

#endif // SUPERMARKETAPI_H