#ifndef FRAMEBUILDERGENERATOR_H_
#define FRAMEBUILDERGENERATOR_H_

#include "ObjectType.h"
#include <sstream>
#include <vector>
#include <memory>

namespace HighFlyers {
namespace Protocol {

class FrameBuilderGenerator
{
private:
	std::stringstream builder;
	std::vector<std::shared_ptr<ObjectType>> types;

	void GenerateBuildMethod();
	void GenerateBottom();
	void GenerateHeader();

public:
	std::string GenerateCode(const std::vector<std::shared_ptr<ObjectType>>& types);
};

}
}

#endif