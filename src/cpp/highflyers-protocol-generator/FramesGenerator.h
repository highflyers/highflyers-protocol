#ifndef FRAMESGENERATOR_H_
#define FRAMESGENESATOR_H_

#include "ObjectType.h"
#include <sstream>
#include <vector>
#include <memory>

namespace HighFlyers {
namespace Protocol {

class FramesGenerator
{
private:
	std::stringstream builder;
	std::vector<std::shared_ptr<ObjectType>> types;

	void GenerateHeaders();
    void GenerateBottom();
    void GenerateFrameTypesEnum();
    void GenerateObjectTypes();

public:
	std::string GenerateCode(const std::vector<std::shared_ptr<ObjectType>>& types);
};

}
}

#endif