#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include "SupermarketAPI.h"

using namespace std;
using namespace sf;

const float RESOLUTION = 16 / 9;
const int WIDTH = 1000;
const int HEIGHT = 563;

int imgui_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

int main()
{

	SupermarketAPI sapi;

	/* This is used to see if we actually close the window or not */
	bool opened = true;

	/* Get all supermarkets */
	auto supermarkets = sapi.fetchSupermarkets();

	// SFML BAD! MUST CHANGE
	/* Uses too many resources for this */
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Shop Smart", Style::Close | Style::None);

	ImGui::SFML::Init(window);
	/* We need this probably for deltaTime stuff inside ImGui::SFML */
	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{

			ImGui::SFML::ProcessEvent(event);

			if (event.type == Event::Closed || !opened) {
				window.close();
			}
		}

		/* Render Window Stuff*/
		ImGui::SFML::Update(window, clock.restart());

		ImGui::Begin("ShopSmart", &opened, imgui_flags);
			ImGui::SetWindowSize(ImVec2(WIDTH, HEIGHT));
			ImGui::SetWindowPos(ImVec2(0, 0));
			ImGui::Button("Click Me");
		ImGui::End();

		window.clear();
		ImGui::SFML::Render(window);
		window.display();

	}

	ImGui::SFML::Shutdown();

	//SupermarketAPI sapi;
	//auto supermarkets = sapi.fetchSupermarkets();
	//if (supermarkets.empty()) {
	//	printf("No super markets found!\n");
	//	return 1;
	//}

	///*for (const auto& market : supermarkets) {
	//	cout << market.at("name") << " | " << market.at("id") << endl;
	//}*/
	///* 176 Countdown  */
	///* ___ Pak'n Save */

	//vector<map<string, string>> broken_places;
	//for (int i = 0; i < supermarkets.size(); i++) {
	//	try {
	//		sapi.selectSupermarket(supermarkets[i]);
	//	}
	//	catch (...) {
	//		// printf("Broke at index: %i\n", i);
	//		cout << i << endl;
	//		broken_places.push_back(supermarkets[i]);
	//	}

	//	try {
	//		auto products = sapi.searchProduct("Milk", supermarkets[i]);
	//	}
	//	catch (...) {
	//		/* Something went wrong */
	//	}

	//}

	//// Step 5: Print out the results
	///*std::cout << "Search Results for 'milk' in " << supermarkets[index]["name"] << ":" << std::endl;
	//for (const auto& product : products) {
	//	std::cout << "Product Name: " << product.at("name") << ", Price: $" << product.at("price") << std::endl;
	//}*/

	//if (!broken_places.empty()) {
	//	for (int i = 0; i < broken_places.size(); i++) {
	//		cout << broken_places[i].at("name") << endl;
	//		cout << broken_places[i].at("id") << endl;
	//	}
	//}

	//printf("We good\n");
	return 0;

}