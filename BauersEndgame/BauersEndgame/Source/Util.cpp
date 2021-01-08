#include <iostream>
#include "Util.h"

void Util::clearConsole() {
#if defined _WIN32 || _WIN64
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}
