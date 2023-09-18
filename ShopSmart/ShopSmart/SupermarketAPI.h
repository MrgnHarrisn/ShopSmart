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

    cpr::Header commonHeaders();
    cpr::Header countdownHeaders();
    cpr::Header paknsaveHeaders();
    cpr::Header newWorldHeaders();

public:
    SupermarketAPI();

    std::vector<store_t> fetchSupermarkets();
    void selectSupermarket(const store_t& supermarket);
    std::vector<store_t> searchProduct(const std::string& term, const store_t& supermarket, int page = 1);
};

#endif // SUPERMARKETAPI_H