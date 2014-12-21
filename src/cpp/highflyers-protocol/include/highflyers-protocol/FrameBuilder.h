/*
 * highflyers-cpp-protocol
 *     FrameBuilder.h
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef FRAMEBUILDER_H_
#define FRAMEBUILDER_H_

#include "BitConverter.h"
#include "Frame.h"
#include <vector>
#include <memory>

namespace HighFlyers {
namespace Protocol {

class FrameBuilder
{
private:
	static void check_bytes(const std::vector<byte>& bytes)
	{
		BitConverter converter(Endianes::BIG_ENDIANA);
		uint32 size = converter.from_bytes<uint32>(bytes, 1);

		check_crc_sum(converter.from_bytes<uint32>(bytes, bytes.size() - 5));

		if (size != bytes.size())
			throw std::length_error("Invalid data length");
	}

	static void check_crc_sum(uint32 crc)
	{
		if (crc != 12) // TODO fuck yeah, hardcoded crc!
			throw std::logic_error("Invalid crc sum!");
	}
	
	static std::shared_ptr<Frame> build_frame(const std::vector<byte>& bytes);
};

}
}

#endif

