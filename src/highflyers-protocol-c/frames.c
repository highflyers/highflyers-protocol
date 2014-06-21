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

void frame_finalise(const byte* data, int size, bool* output)
{
	int bytesAdded = 0;
	int i;

	for (i = 0; i < size; i++)
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

	frame_parser_helper_set_byte (output + size + bytesAdded,
	FRAMEPARSER_HELPER_ENDFRAME);
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
	int size = 3; //struct type + enable flags
	size += sizeof(uint32);
	if (value->Field2_enabled) size += sizeof(double);
	size += sizeof(byte);
	if (value->Field4_enabled) size += sizeof(uint32);

	return size;
}

int TestStruct_preserialize (const TestStruct* value, byte* output)
{
	uint16 must_be = 0;
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
	iterator += sizeof(byte);

	if (value->Field4_enabled)
	{
		must_be = (uint16)(must_be | (1 << 3));
		frame_parser_helper_set_int32 (output + iterator, value->Field4);
		iterator += sizeof(int32);
	}

	frame_parser_helper_set_uint16 (output + 1, must_be);

	return iterator;
}

void TestStruct_serialize (const TestStruct* value, byte* output)
{
	int frame_size = TestStruct_current_size (value) + sizeof(uint32); // struct + crc32

	byte* data = (byte*)malloc (frame_size);

	int iterator = TestStruct_preserialize(value, data);

	uint32 crc = frame_parser_helper_calculate_crc (data, iterator);

	frame_parser_helper_set_uint32 (data + iterator, crc);

	frame_finalise(data, frame_size, output);
	free(data);
}


SecondStruct* SecondStruct_parse (const byte* data, int size)
{
}

int SecondStruct_current_size (const SecondStruct* value)
{
	int size = 3; //struct type + enable flags
	size += TestStruct_current_size(&value->Field1);
	size += sizeof(byte);
	if (value->Field3_enabled) size += TestStruct_current_size(&value->Field3);

	return size;
}

int SecondStruct_preserialize (const SecondStruct* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 3;

	output[0] = 0;

	must_be = (uint16)(must_be | (1 << 0));

	byte* output_f1 = (byte*)malloc (TestStruct_current_size (&value->Field1));
	int size_f1 = TestStruct_preserialize(&value->Field1, output_f1);
	memcpy(output + iterator, output_f1, size_f1);
	free(output_f1);
	iterator += size_f1;

	must_be = (uint16)(must_be | (1 << 1));
	frame_parser_helper_set_byte (output + iterator, value->Field2);
	iterator += sizeof(byte);

	if (value->Field3_enabled)
	{
		must_be = (uint16)(must_be | (1 << 2));

		byte* output_f3 = (byte*)malloc (TestStruct_current_size (&value->Field3));
		int size_f3 = TestStruct_preserialize(&value->Field3, output_f3);
		memcpy(output + iterator, output_f3, size_f3);
		free(output_f3);
		iterator += size_f3;
	}

	frame_parser_helper_set_uint16 (output + 1, must_be);

	return iterator;
}

void SecondStruct_serialize (const SecondStruct* value, byte* output)
{
	int frame_size = SecondStruct_current_size (value) + sizeof(uint32); // struct + crc32

	byte* data = (byte*)malloc (frame_size);

	int iterator = SecondStruct_preserialize(value, data);

	uint32 crc = frame_parser_helper_calculate_crc (data, iterator);

	frame_parser_helper_set_uint32 (data + iterator, crc);

	frame_finalise(data, frame_size, output);
	free(data);
}
