#ifndef STRUCTURE_H_
#define STRUCTURE_H_

#include "ObjectType.h"
#include <vector>
#include <string>

namespace HighFlyers {
namespace Protocol {

class Structure : public ObjectType
{
private:
	std::vector<std::string> generate_current_size();
	std::string generate_field_count();
	std::string generate_parser();

protected:
	std::string generate_header() override;
	std::vector<std::string> generate_body() override;

public:
	Structure(const std::string& name, const std::vector<std::vector<std::string>>& input);
};

}
}

#endif