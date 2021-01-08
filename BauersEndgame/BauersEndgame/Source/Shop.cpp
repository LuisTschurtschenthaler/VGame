#include <iostream>
#include "Shop.h"
#include "Util.h"
#include "IO.h"
#include "KeyCode.h"
#include "Color.h"


void Shop::init() {
	_items.insert(_items.begin(), {
		{ "Kebap", 4.5 },
		{ "Frische bio Banane", 0.15 },
		{ "Apfel", 0.50 },
		{ "Wassermelone", 10.45 },
		{ "Kind", 604 },
		{ "Meins", float(UINT64_MAX) }
	});
}

void Shop::open(const std::string& message) {
	bool leaveShop = false;

	do {
		Util::clearConsole();
		Output::writeLine(message);

		for(int i = 0; i < _items.size(); i++) {
			if(_selectedItem == i) {
				Output::setBackgroundColor(BG_COL_WHITE);
				Output::setForegroundColor(FG_COL_BLACK);
				Output::writeLine(_items[i].toString());
				Output::resetColors();
			}
			else Output::writeLine(_items[i].toString());
		}

		KeyCode pressedKey = Input::readKey();
		switch(pressedKey) {
			case KeyCode::KEY_W:
			case KeyCode::KEY_w:
				if(_selectedItem - 1 >= 0)
					_selectedItem--;
				break;

			case KeyCode::KEY_S:
			case KeyCode::KEY_s:
				if(_selectedItem + 1 < _items.size())
					_selectedItem++;
				break;

			case KeyCode::KEY_ESC:
				leaveShop = true;
				break;
		}

	} while(!leaveShop);
}

std::string Shop::getValue() {
	return _items[_selectedItem].name;
}
