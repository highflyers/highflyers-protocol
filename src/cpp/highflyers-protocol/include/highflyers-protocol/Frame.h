/*
 * highflyers-cpp-protocol
 *     Frame.h
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef FRAME_H_
#define FRAME_H_

#include "BitConverter.h"
#include <vector>

namespace HighFlyers {
namespace Protocol {

class Frame
{
protected:
	size_t field_count;
	std::vector<bool> PreParseData(const std::vector<byte>& data);
	BitConverter converter;

public:
	Frame() : converter(Endianes::BIG_ENDIANA){}
	virtual ~Frame() {}
	virtual size_t get_field_count() = 0;
	virtual void parse(const std::vector<byte>& data) = 0;
	virtual size_t get_current_size() = 0;
};

}
}

#endif /* FRAME_H_ */
