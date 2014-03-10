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
#include <vector>

namespace HighFlyers {
namespace Protocol {

class FrameBuilder
{
private:
	static void check_bytes(std::vector<byte> bytes);
	static void check_crc_sum(uint32 crc);
};

}
}

#endif

