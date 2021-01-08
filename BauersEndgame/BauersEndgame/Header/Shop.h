#ifndef SHOP_H
#define SHOP_H

#include <vector>
#include <string>


struct ShopItem {
	std::string name;
	float prize;

	std::string toString() {
		std::string pr = std::to_string(prize);
		return (name + " " + pr.substr(0, pr.find(".") + 2));
	}
};


class Shop {

private:
	int _selectedItem = 0;
	std::vector<ShopItem> _items;

public:
	void init();
	void open(const std::string& message);
	std::string getValue();

};

#endif // SHOP_H
