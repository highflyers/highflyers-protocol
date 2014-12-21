#ifndef ENUMERATION_H_
#define ENUMERATION_H_

#include "ObjectType.h"	
#include <vector>
#include <string>

namespace HighFlyers {
namespace Protocol {

class Enumeration : public ObjectType
{
protected:
	std::string generate_header() override;
	std::vector<std::string> generate_body() override;

public:
	Enumeration(const std::string& name, const std::vector<std::vector<std::string>>& input);

};

}
}

#endif