#include "CodeGenerator.h"
#include "FrameBuilderGenerator.h"
#include "FramesGenerator.h"
#include "Structure.h"
#include "Enumeration.h"
#include <fstream>
#include <iterator>

using namespace HighFlyers::Protocol;
using namespace std;

CodeGenerator::CodeGenerator(const string& input_file_name, const string& frames_file_name, const string& builder_file_name)
	: curr_type(CurrentType::NONE),
	input_file_name(input_file_name),
	frames_file_name(frames_file_name),
	builder_file_name(builder_file_name)
{
}

void CodeGenerator::generate()
{
	read_from_file();
    prepare_data();

    save_to_file(builder_file_name, FrameBuilderGenerator().GenerateCode(objects_types));
    save_to_file(builder_file_name, FramesGenerator().GenerateCode(objects_types));
}

void CodeGenerator::read_from_file()
{
	ifstream input_file(input_file_name);
	copy(istream_iterator<string>(input_file), 
		istream_iterator<string>(), back_inserter(data));
}

void CodeGenerator::save_to_file(const string& file_name, const string& content)
{
	ofstream output(file_name);
	output << content;
}

void CodeGenerator::prepare_data()
{
}

void CodeGenerator::add_new_object_type()
{
	if (!was_start_bracket || curr_type == CurrentType::NONE)
		throw exception("Unexpected '}' token");
        
	switch (curr_type)
    {
	case CurrentType::STRUCTURE:
		objects_types.push_back(std::make_shared<Structure>(Structure(current_name, current_collector)));
		break;
	case CurrentType::ENUMERATION:
		objects_types.push_back(std::make_shared<Enumeration>(Enumeration(current_name, current_collector)));
		break;
	}

    was_start_bracket = false;
	curr_type = CurrentType::NONE;
}

void CodeGenerator::append_line(const std::string& line)
{

}