#include "Structure.h"

using namespace HighFlyers::Protocol;
using namespace std;

vector<string> Structure::generate_current_size()
{
	return vector<string>();
}
	
string Structure::generate_field_count()
{
	return "";
}

string Structure::generate_parser()
{
	return "";
}

string Structure::generate_header()
{
	return "";
}

vector<string> Structure::generate_body()
{
	return vector<string>();
}

Structure::Structure(const string& name, const vector<vector<string>>& input)
	: ObjectType(name, input)
{
}
