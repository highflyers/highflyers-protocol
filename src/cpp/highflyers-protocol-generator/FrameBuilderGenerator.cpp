#include "FrameBuilderGenerator.h"

using namespace HighFlyers::Protocol;

std::string FrameBuilderGenerator::GenerateCode(const std::vector<std::shared_ptr<ObjectType>>& types)
{
	builder.clear();
	this->types = types;

    GenerateHeader();
    GenerateBuildMethod();
    GenerateBottom();

	return builder.str();
}

void FrameBuilderGenerator::GenerateBuildMethod()
{

}

void FrameBuilderGenerator::GenerateBottom()
{
}

void FrameBuilderGenerator::GenerateHeader()
{
}
