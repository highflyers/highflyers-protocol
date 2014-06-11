#include "frame.h"
#include "frames.h"
#include "frame_parser_helper.h"
#include <stdlib.h>

TestStruct* TestStruct_parse(const byte* data, int size)
{
	bool fields[4];
	int iterator = 0;
	TestStruct* value;

	value = (TestStruct*)malloc(sizeof(TestStruct));
	frame_preparse_data(data, fields, 4);

	if (fields[0])
	{
		value->Field1 = frame_parser_helper_to_uint32(data, iterator + 2);
		iterator += sizeof(uint32);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[1])
	{
		value->Field2 = frame_parser_helper_to_double (data, iterator + 2);
		iterator += sizeof(double);
		value->Field2_enabled = true;
	}
	if (fields[2])
	{
		value->Field3 = data[iterator + 2];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[3])
	{
		value->Field4 = frame_parser_helper_to_uint32(data, iterator + 2);
		iterator += sizeof(uint32);
		value->Field4_enabled = true;
	}

	return value;
}

int TestStruct_current_size (const TestStruct* value)
{
	int size = 2; // enabled fields information
	size += sizeof(uint32);
	if (value->Field2_enabled) size += sizeof(double);
	size += sizeof(byte);
	if (value->Field4_enabled) size += sizeof(uint32);

	return size;
}

void TestStruct_serialize (const TestStruct* value, byte* output)
{
	uint32 must_be = 0;
	int iterator = 3;
	output[0] = 0;

	must_be = (uint16)(must_be | (1 << 0));
	frame_parser_helper_set_uint32 (output + iterator, value->Field1);
	iterator += sizeof(uint32);

	if (value->Field2_enabled)
	{
		must_be = (uint16)(must_be | (1 << 1));
		frame_parser_helper_set_double (output + iterator, value->Field2);
		iterator += sizeof(double);
	}

	must_be = (uint16)(must_be | (1 << 2));
	frame_parser_helper_set_byte (output + iterator, value->Field3);
	iterator += sizeof (byte);

	if (value->Field4_enabled)
	{
		must_be = (uint16)(must_be | (1 << 3));
		frame_parser_helper_set_int32 (output + iterator, value->Field4);
		iterator += sizeof(int32);
	}

	frame_parser_helper_set_uint16 (output + 1, must_be);
}
