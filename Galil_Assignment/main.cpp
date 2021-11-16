//GalilTest
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <conio.h>

#include <stdlib.h>
#include <gclib.h>
#include <gclibo.h>
#include <gclib_errors.h>
#include <gclib_record.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <bitset>

#include "Galil.h"
#include <time.h>

#define ADDRESS "192.168.0.120 -d"

using namespace System;
using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

int main()
{
	EmbeddedFunctions * Funcs = new EmbeddedFunctions();

	Galil * g = new Galil(Funcs, ADDRESS);

	Console::WriteLine("--- Start of Task 4 ---");

	g->DigitalOutput(0);

	g->WriteEncoder();

	g->AnalogOutput(0, 0.5);

	time_t start = time(0);
	double seconds_passed = 0;

	do {
		seconds_passed = difftime(time(0), start);
		
		uint8_t encVal = g->ReadEncoder();

		g->DigitalByteOutput(false, encVal);

		if (_kbhit()) {
			std::bitset<8> output(encVal);
			std::cout << output << std::endl;
			g->WriteEncoder();
			_getch();
		}
	} while (seconds_passed < 10);
	
	Console::WriteLine("--- End of Task 4 ---");

	std::this_thread::sleep_for(std::chrono::seconds(1));

	Console::WriteLine("--- Start of Task 5 ---");

	g->DigitalOutput(0);

	uint16_t inputVal = 1;

	while (inputVal) {
		std::string s;
		std::cin >> s;
		inputVal = stoi(s, 0, 2);

		g->DigitalOutput(inputVal);
	}

	Console::WriteLine("--- End of Task 5 ---");

	delete g;

	Console::WriteLine("Terminating!");
	Console::ReadKey();

	return G_NO_ERROR;
}
