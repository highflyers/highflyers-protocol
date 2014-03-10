/*
 * highflyers-cpp-protocol
 *     Frame.h
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <vector>

namespace HighFlyers {
namespace Protocol {

typedef unsigned char byte;
typedef unsigned short uint16;

class Frame
{
protected:
	bool PreParseData(const std::vector<byte>& data);

public:
	virtual ~Frame() {}

	virtual void Parse(const std::vector<byte>& data) = 0;
};

}
}

#endif /* FRAME_H_ */
