#include "FrameBuilder.h"
#include "Frames.h"

using namespace HighFlyers::Protocol;

std::shared_ptr<Frame> FrameBuilder::build_frame(const std::vector<byte>& bytes)
{
	check_bytes(bytes);
	std::shared_ptr<Frame> frame;
	
	switch ((FrameTypes) bytes[0])
	{
	case FrameTypes::Minor:
		frame = std::make_shared<Minor>(Minor()); break;
	case FrameTypes::Message:
		frame = std::make_shared<Message>(Message()); break;
	default: throw std::logic_error("Unexpected frame type");
	}
	
	frame->parse(std::vector<byte>(bytes.begin() + 1, bytes.end()));
	return frame;
}