/*
 * highflyers-cpp-protocol
 *     BitConverter.h
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef BITCONVERTER_H_
#define BITCONVERTER_H_

#include <vector>
#include <stdexcept>

namespace HighFlyers {
namespace Protocol {

enum class Endianes
{
	BIG_ENDIANA,
	LITTLE_ENDIANA
};


// fixme Fix this typedefs !
typedef unsigned char byte;
typedef unsigned short uint16;
typedef unsigned int uint32; 

class BitConverter
{
private:
	Endianes endianes;

public:
	BitConverter(Endianes endianes);

	template<typename R>
	R from_bytes(const std::vector<byte>& data, size_t start_index)	
	{
		R value = 0;

		if (data.size() < start_index + sizeof(R) - 1)
			throw std::out_of_range("Cannot convert data to uint16");

		if (endianes == Endianes::BIG_ENDIANA)
			for (size_t i = 0; i < sizeof(R); i++)
				*((byte*)(&value) + i) = data[start_index + i];
		else if (endianes == Endianes::LITTLE_ENDIANA)
			for (size_t i = 0; i < 8; i++)
				*((byte*)(&value) + sizeof(R) - 1 - i) = data[start_index + i];
		else
			throw std::runtime_error("Unsupported endianes");
	
		return value;
	}
};

}
}

#endif
