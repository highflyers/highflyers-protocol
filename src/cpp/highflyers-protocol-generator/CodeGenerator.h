#ifndef CODEGENERATOR_H_
#define CODEGENERATOR_H_

#include "ObjectType.h"
#include <string>
#include <vector>
#include <memory>

namespace HighFlyers {
namespace Protocol {

enum class CurrentType
{
	NONE,
    STRUCTURE,
    ENUMERATION
};

class CodeGenerator
{
private:
	std::string frames_file_name;
    std::string input_file_name;
    std::string builder_file_name;
	std::string current_name;
	CurrentType curr_type;
	bool was_start_bracket;
	std::vector<std::shared_ptr<ObjectType>> objects_types;
    std::vector<std::string> data;
	std::vector<std::vector<std::string>> current_collector;;
        
	void read_from_file();
    void save_to_file(const std::string& file_name, const std::string& content);
	void prepare_data();
	void add_new_object_type();

protected:
	void append_line(const std::string& line);
public:
	CodeGenerator(const std::string& input_file_name, const std::string& frames_file_name, const std::string& builder_file_name);
	void generate();
};

}
}
#endif