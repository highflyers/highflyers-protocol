#include "frames.h"
#include "frame_parser_helper.h"
#include <stdlib.h>

void frame_preparse_data (const byte* data, bool* output, int field_count)
{
	uint16 field_flags = frame_parser_helper_to_uint16 (data, 0);
	int i;

	for (i = 0; i < field_count; i++)
		output[i] = (field_flags & (1 << i)) != 0;
}

TestStruct* TestStruct_parse (const byte* data, int size)
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
	int size = 0;
	size += sizeof(uint32);
	if (value->Field2_enabled) size += sizeof(double);
	size += sizeof(byte);
	if (value->Field4_enabled) size += sizeof(uint32);

	return size;
}

void TestStruct_serialize (const TestStruct* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 3;

	int frame_size = TestStruct_current_size (value) + sizeof(uint32) + 1
			+ sizeof(uint16); // struct + crc32 + first '0' bit + must_be

	byte* data = (byte*)malloc (frame_size);

	data[0] = 0;

	must_be = (uint16)(must_be | (1 << 0));
	frame_parser_helper_set_uint32 (data + iterator, value->Field1);
	iterator += sizeof(uint32);

	if (value->Field2_enabled)
	{
		must_be = (uint16)(must_be | (1 << 1));
		frame_parser_helper_set_double (data + iterator, value->Field2);
		iterator += sizeof(double);
	}

	must_be = (uint16)(must_be | (1 << 2));
	frame_parser_helper_set_byte (data + iterator, value->Field3);
	iterator += sizeof(byte);

	if (value->Field4_enabled)
	{
		must_be = (uint16)(must_be | (1 << 3));
		frame_parser_helper_set_int32 (data + iterator, value->Field4);
		iterator += sizeof(int32);
	}

	frame_parser_helper_set_uint16 (data + 1, must_be);

	uint32 crc = frame_parser_helper_calculate_crc (data, iterator);

	frame_parser_helper_set_uint32 (data + iterator, crc);

	int bytesAdded = 0;
	int i;

	for (i = 0; i < frame_size; i++)
	{
		if ((data[i] == FRAMEPARSER_HELPER_SENTINEL)
				|| (data[i] == FRAMEPARSER_HELPER_ENDFRAME))
		{
			frame_parser_helper_set_byte (output + i + bytesAdded,
			FRAMEPARSER_HELPER_SENTINEL);
			bytesAdded++;
		}

		frame_parser_helper_set_byte (output + i + bytesAdded, data[i]);
	}

	frame_parser_helper_set_byte (output + frame_size + bytesAdded,
	FRAMEPARSER_HELPER_ENDFRAME);
}
