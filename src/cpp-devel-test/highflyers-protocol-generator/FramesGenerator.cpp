#include "FramesGenerator.h"

using namespace HighFlyers::Protocol;

std::string FramesGenerator::GenerateCode(const std::vector<std::shared_ptr<ObjectType>>& types)
{
	builder.clear();
	this->types = types;

	GenerateHeaders();
	GenerateFrameTypesEnum();
    GenerateObjectTypes();
    GenerateBottom();

	return builder.str();
}

void FramesGenerator::GenerateHeaders()
{
}
 
void FramesGenerator::GenerateBottom()
{
}

void FramesGenerator::GenerateFrameTypesEnum()
{
}

void FramesGenerator::GenerateObjectTypes()
{
}
