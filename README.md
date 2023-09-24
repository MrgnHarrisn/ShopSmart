# Shop Smart
This is a Desktop Application that tells you the cheapest way for you to be able to
travel to stores and save money!

In this project we are using:<br>
-   cpr <br>
for communicating with API's on the web

# Documentation
[doxygen](https://codedocs.xyz/MrgnHarrisn/ShopSmart/md_README.html)

# How to build

## Requirements to build:
-   [vcpkg](https://vcpkg.io)
-   curl
-   cpr

If you have these libraries then you can build it with the Visual Studio Solution (Swapping to CMake/Make in the future)


# How does it work?
The way this works is:
You give the program your address.
It picks the countdown, pak'n save, and new world in your earea.
You then give it a grocery list, and it will do a price comparrison between the products in the list.

# How to use it?
When loading up the CLI tool it will ask you for your location (Only works for NZ)<br>



## Commands

-   "query":        "query item_name"
-   "query_list":   "query_list file_path"
-   "swap_market":  "swap_market"