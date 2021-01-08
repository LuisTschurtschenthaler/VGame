#include <iostream>
#include <windows.h>
#include <wchar.h>
#include "Input.h"
#include "Output.h"
#include "IPv4.h"
#include "File.h"


void setFontSize(int x, int y) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX currentFont = new CONSOLE_FONT_INFOEX();

	currentFont->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(console, 0, currentFont);
	
	currentFont->dwFontSize.X = 0;
	currentFont->dwFontSize.Y = 36;
	SetCurrentConsoleFontEx(console, 0, currentFont);
}

void setWindowSize(int x, int y) {
	HWND console = GetConsoleWindow();
	RECT r;

	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, x, y, TRUE);
}


int main() {
	SetConsoleTitle(TEXT("IP-Calculator :)"));
	setFontSize(0, 26);
	setWindowSize(800, 900);

	while(true) {
		std::vector<pair> subnets = std::vector<pair>();
		
		std::string address = Input::getIPv4Address();
		std::string subnetmask = Input::getSubnetmask();
		IPv4 ipv4(address, subnetmask);

		do {

			std::vector<std::string> subnet = Input::getSubnet(subnets);
			subnets.push_back(pair{subnet[0], std::stoi(subnet[1])});

		} while(!Input::isFinished());

		std::sort(subnets.begin(), subnets.end(), [](const pair& x, const pair& y) {
			return x.second > y.second;
		});

		for(auto& subnet : subnets)
			ipv4.addSubnet(subnet.first, subnet.second);

		ipv4.print();
		std::cin.ignore();

		if(Input::saveResultsToFile())
			Output::printFileSaved(File::saveSolutions(ipv4));
	}

	return 0;
}
