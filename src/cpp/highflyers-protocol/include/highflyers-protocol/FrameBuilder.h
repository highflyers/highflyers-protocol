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
	static void check_bytes(const std::vector<byte>& bytes);
	static void check_crc_sum(uint32 crc);
	static std::shared_ptr<Frame> build_frame(const std::vector<byte>& bytes);
};

}
}

#endif

