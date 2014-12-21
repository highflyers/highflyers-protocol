// highflyers-protocol-generator.cpp : Defines the entry point for the console application.
//

#include "CodeGenerator.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace HighFlyers::Protocol;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " <input hfproto file> <output cs file> <output builder file>" << endl;
        return 0;
	}

    try
    {
		CodeGenerator generator(argv[0], argv[1], argv[2]);
		generator.generate();
	}
    catch (const exception& ex)
	{
		cout << ex.what() << endl;
	}

	system("Pause");
	return 0;
}

