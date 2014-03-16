#include "Enumeration.h"

using namespace HighFlyers::Protocol;
using namespace std;

Enumeration::Enumeration(const string& name, const vector<vector<string>>& input)
	: ObjectType(name, input)
{}

string Enumeration::generate_header()
{
	return "";
}

vector<string> Enumeration::generate_body()
{
	vector<string> out;

	for (auto words : input)
	{
		if (words.size() != 1)
			throw std::runtime_error("Expected one word in line!");

		out.push_back(words[0] + ",");
	}

	return out;
}
