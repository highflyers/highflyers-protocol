/*
 * highflyers-cpp-protocol
 *     Frame.cpp
 *
 *  Created on: 10 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#include "Frame.h"

using namespace HighFlyers::Protocol;
using namespace std;


vector<bool> Frame::PreParseData(const vector<byte>& data)
{
	vector<bool> fields;
	uint16 field_flag = BitConverter(Endianes::BIG_ENDIANA).from_bytes<uint16>(data, 3);

	for (size_t i = 0; i < get_field_count(); i++)
		fields[i] = (field_flag & (1 >> i)) != 0;

    return fields;
}


