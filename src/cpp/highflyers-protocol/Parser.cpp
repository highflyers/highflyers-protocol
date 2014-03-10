/*
 * highflyers-cpp-protocol
 *     Parser.cpp
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Parser.h"

using namespace HighFlyers::Protocol;

void Parser::append_bytes(const std::vector<byte>& data)
{
	for (auto b : data)
		append_byte(b);
}

void Parser::append_byte(byte b)
{
	if (prev_sentinel)
    {
		if (b == Sentinel || b == EndFrame)
			bytes.push_back(b);
		else
			throw new std::logic_error("Unexpected token " + b);
			
		prev_sentinel = false;
	}
	else if (b == Sentinel)
		prev_sentinel = true;
	else
		bytes.push_back(b);

	if (b == EndFrame)
		parse_frame();
	else if (MaxLength == bytes.size())
    {
		bytes.clear();
		// todo log warning or sth. No warning exception defined: throw new std::warning_exception("Too many bytes without end_frame sign. Dropping data...");
	}
}

void Parser::new_frame_received(std::shared_ptr<Frame> frame)
{
	// todo notifier!
}

void Parser::parse_frame()
{
	std::shared_ptr<Frame> frame(FrameBuilder::build_frame(bytes));
	
	new_frame_received(frame);
}
