#include <iostream>
#include <string>
#include <stdint.h>

#include "SupermarketAPI.h"



using namespace std;

/*!
 * \brief converts a string to lowercase
 * \return lowercase version of string
 */
string tolower(const string& s)
{
	string output = "";
	for (char c : s) {
		output += tolower(c);
	}

	return output;

}

/*!
* \brief checks if a word contains a subtring of another word
* \return bool
*/
bool containsWord(const std::string& str, const std::string& word) {

	if (tolower(str).find(tolower(word)) != -1) {
		return true;
	}

	return false;

}
/*!
* \brief selects a market for the user to search through
* \return returns a store_t (map<string, string>)
*/
store_t selectMarket(vector<map<string, string>>& stores, SupermarketAPI& sapi)
{


	string city;
	/* Will then give suggestions for stores */
	vector<map<string, string>> new_stores;

	do {
		/* Ask for area to search for */
		printf("Please enter the area you are in: ");
		getline(cin, city);

		/* Filter stores */
		for (auto s : stores) {
			if (containsWord(s["address"], city)) {
				new_stores.push_back(s);
			}
		}

		if (new_stores.size() == 0) {
			cout << "Uh oh Looks like we couldn't find a store in that area :(" << endl;
		}

	} while (new_stores.size() == 0);

	printf("Original Stores: %i\nFiltered: %i\n", stores.size(), new_stores.size());

	/* Display found stores */
	int index = 0;
	for (auto& store : new_stores) {
		cout << "No. " << index << endl;
		cout << store["name"] << endl;
		cout << store["address"] << endl;
		cout << "=========================================" << endl;
		index++;
	}
	cout << "looking for: " << city << endl;
	cout << "Found: " << new_stores.size() << endl;

	index = -1;

	do {
		printf("Pick store: ");
		cin >> index;
		cout << endl;
	} while (index < 0 || index > new_stores.size());

	return new_stores[index];

}

/*!
* \breif parses the user input through the comandline interface
*/
void parse(string& input, SupermarketAPI& sapi, store_t& store)
{
	vector<string> split_input;
	size_t pos = 0;

	while ((pos = input.find(" ")) != string::npos)
	{
		split_input.push_back(input.substr(0, pos));
		input.erase(0, pos + 1);
	}

	// add the last part if any remains
	if (!input.empty())
	{
		split_input.push_back(input);
	}
	
	for (int i = 0; i < split_input.size(); i++) {


		/* This needs to be turned into a switch statment */

		if (split_input[i] == "query") {
			sapi.selectSupermarket(store);
			vector<store_t> products = sapi.searchProduct(split_input[i+1], store);
			for (store_t prod : products) {
				cout << prod["name"] << endl;
				cout << prod["price"] << endl;
			}
			i++;
		}
		/* For querying a list of items */
		else if (split_input[i] == "query_list") {
			/* query a lsit of products from a given csv file */
			sapi.selectSupermarket(store);

			/* File for us to search through */
			fstream file;
			file.open(split_input[i + 1]);
			string product;
			while (true) {
				if (!getline(file, product)) {
					break;
				}
				/* Storing all the products */
				vector<store_t> products = sapi.searchProduct(product, store, 1);
				for (store_t prod : products) {
					cout << prod["name"] << endl;
					cout << prod["price"] << endl;
				}
			}
			file.close();
			i++;
		}
		else if (split_input[i] == "clear") {
			system("CLS");
		}
		else if (split_input[i] == "swap_market") {
			auto stores = sapi.fetchSupermarkets();
			store = selectMarket(stores, sapi);
		}
	}

}




int main(int argc, char* argv[])
{
	/* Most of this should be in it's own class */

	SupermarketAPI sapi;
	vector<store_t> supermarkets = sapi.fetchSupermarkets();

	if (supermarkets.empty()) {
		printf("No super markets found!\n");
		return 1;
	}

	/* Here we will start the terminal feature */

	/* Filter to area etc. */
	store_t filtered = selectMarket(supermarkets, sapi);

	auto market = supermarkets[0];

	/* Actual Terminal interactivity */
	string input = "";

	while (input != "quit")
	{
		printf(">>> ");
		getline(cin, input);
		parse(input, sapi, filtered);
	}
	
	/* Return 0 */
	return 0;

}