#ifndef OBJECTTYPE_H_
#define OBJECTTYPE_H_

#include <string>
#include <vector>

namespace HighFlyers {
namespace Protocol {

class ObjectType
{
protected:
	std::vector<std::vector<std::string>> input;
    std::string name;

protected:
	ObjectType(std::string name, const std::vector<std::vector<std::string>>& input)
		: input(input), name(name) {}
	virtual std::string generate_header() = 0;
	virtual std::vector<std::string> generate_body() = 0;
	static std::string generate_bottom()
	{
		return "}";
	}

public:
	std::string get_name() { return name; }
	std::vector<std::string> generate_class()
	{
		std::vector<std::string> vect;
		vect.push_back(generate_header());
		
		for (auto line : generate_body())
			vect.push_back("\t" + line);

		vect.push_back(generate_bottom());
		return vect;
	}
};

}
}

#endif