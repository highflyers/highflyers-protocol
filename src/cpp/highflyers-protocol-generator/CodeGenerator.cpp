#include "CodeGenerator.h"

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

//    save_to_file(builder_file_name, new FrameBuilderGenerator().GenerateCode(objectsTypes));
//    save_to_file(builder_file_name, new FramesGenerator().GenerateCode(objectsTypes));
}

void CodeGenerator::read_from_file()
{
}

void CodeGenerator::save_to_file(const string& file_name, const string& content)
{
}

void CodeGenerator::prepare_data()
{
}

void CodeGenerator::add_new_object_type()
{
	if (!was_start_bracket || curr_type == CurrentType::NONE)
		throw exception("Unexpected '}' token");
        
	/*switch (curr_type)
    {
	case CurrentType::STRUCTURE:
		objects_types.push_back(new Structure(currentName, currentCollector.ToArray()));
		break;
	case CurrentType::ENUMERATION:
		objects_types.push_back(new Enumeration(currentName, currentCollector.ToArray()));
		break;
	}*/

    was_start_bracket = false;
	curr_type = CurrentType::NONE;
}