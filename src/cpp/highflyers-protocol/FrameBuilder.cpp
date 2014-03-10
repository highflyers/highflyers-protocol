#include "FrameBuilder.h"

using namespace HighFlyers::Protocol;

void FrameBuilder::check_bytes(std::vector<byte> bytes)
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