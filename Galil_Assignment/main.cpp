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

#include "Galil.h"

#define ADDRESS "192.168.0.120 -d"

using namespace System;
using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

GCon g = 0; //var used to refer to a unique connection

int main()
{
	EmbeddedFunctions * Funcs = new EmbeddedFunctions();

	Galil * g = new Galil(Funcs, ADDRESS);

	std::cout << *g;

	delete g;

	Console::WriteLine("Terminating!");
	Console::ReadKey();

	return G_NO_ERROR;
}
