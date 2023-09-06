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

	/* Here we will start the terminal feature */
	cout << "What location do you want to check for?" << endl;
	string location;
	cin >> location;

	for (auto market : supermarkets) {
		try {
			if (market.at("name") == ("Countdown Dunedin Central")) {
				printf("Found market!\n");
				sapi.selectSupermarket(market);
			}
		}
		catch (...) {
			printf("Something went wrong\n");
		}
	}
	string input;
	while (input != "quit")
	{
		printf(">>> ");
		getline(cin, input);
		cout << "You typed: " << input << endl;
	}

	return 0;

}