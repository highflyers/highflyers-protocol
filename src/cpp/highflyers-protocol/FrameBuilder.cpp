#include "FrameBuilder.h"
#include "Frames.h"

using namespace HighFlyers::Protocol;

void FrameBuilder::check_bytes(const std::vector<byte>& bytes)
{
	BitConverter converter(Endianes::BIG_ENDIANA);
	uint32 size = converter.from_bytes<uint32>(bytes, 1);

	check_crc_sum(converter.from_bytes<uint32>(bytes, bytes.size() - 5));

	if (size != bytes.size())
		throw std::length_error("Invalid data length");
}

void FrameBuilder::check_crc_sum(uint32 crc)
{
	if (crc != 12) // TODO fuck yeah, hardcoded crc!
		throw std::logic_error("Invalid crc sum!");
}

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