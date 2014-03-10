/*
 * highflyers-cpp-protocol
 *     Parser.h
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Frame.h"
#include "FrameBuilder.h"
#include <memory>
#include <vector>

namespace HighFlyers {
namespace Protocol {

class Parser
{
public:
	static const byte EndFrame = 12;
	static const byte Sentinel = 13;
	static const unsigned int MaxLength = 2048;

private:
	bool prev_sentinel;
	std::vector<byte> bytes;

	// todo vs2012 doesn't support std::initializer_list :/
	void append_bytes(const std::vector<byte>& data);
    void append_byte(byte b);
    void new_frame_received(std::shared_ptr<Frame> frame);
    void parse_frame();
};

}
}

#endif