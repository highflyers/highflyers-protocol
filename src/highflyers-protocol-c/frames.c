// GENERATED CODE! DON'T MODIFY IT!
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
TestStruct* TestStruct_parse (const byte* data)
{
	bool fields[4];
	int iterator = 2;
	TestStruct* value;
	value = (TestStruct*)malloc(sizeof(TestStruct));
	frame_preparse_data(data, fields, 4);
	if (fields[0])
	{
		value->Field1 = frame_parser_helper_to_uint32(data, iterator);
		iterator += sizeof(uint32);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[1])
	{
		value->Field2 = frame_parser_helper_to_double(data, iterator);
		iterator += sizeof(double);
		value->Field2_enabled = true;
	}
	else value->Field2_enabled = false;
	if (fields[2])
	{
		value->Field3 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[3])
	{
		value->Field4 = frame_parser_helper_to_uint32(data, iterator);
		iterator += sizeof(uint32);
		value->Field4_enabled = true;
	}
	else value->Field4_enabled = false;
	return value;
}

int TestStruct_current_size (const TestStruct* value)
{
	int size = 2; //enable flags
	size += sizeof(uint32);
	if (value->Field2_enabled) size += sizeof(double);
	size += sizeof(byte);
	if (value->Field4_enabled) size += sizeof(uint32);
	return size;
}
int TestStruct_preserialize (const TestStruct* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 2;
	must_be = (uint16)(must_be | (1 << 0));
frame_parser_helper_set_uint32(output + iterator, value->Field1);
	iterator += sizeof(uint32);
	if (value->Field2_enabled)
	{
	must_be = (uint16)(must_be | (1 << 1));
frame_parser_helper_set_double(output + iterator, value->Field2);
	iterator += sizeof(double);
	}
	must_be = (uint16)(must_be | (1 << 2));
frame_parser_helper_set_byte(output + iterator, value->Field3);
	iterator += sizeof(byte);
	if (value->Field4_enabled)
	{
	must_be = (uint16)(must_be | (1 << 3));
frame_parser_helper_set_uint32(output + iterator, value->Field4);
	iterator += sizeof(uint32);
	}
	frame_parser_helper_set_uint16 (output, must_be);
	return iterator;
}
void TestStruct_serialize (const TestStruct* value, byte* output)
{
	int frame_size = 1 + TestStruct_current_size (value) + sizeof(uint32); // struct + crc32
	byte* data = (byte*)malloc (frame_size);
	data[0] = 0;
	int iterator = TestStruct_preserialize(value, data + 1);
	uint32 crc = frame_parser_helper_calculate_crc (data, iterator + 1);
	frame_parser_helper_set_uint32 (data + 1 + iterator, crc);
	frame_finalise(data, frame_size, output);
	free(data);
}
SecondStruct* SecondStruct_parse (const byte* data)
{
	bool fields[3];
	int iterator = 2;
	SecondStruct* value;
	value = (SecondStruct*)malloc(sizeof(SecondStruct));
	frame_preparse_data(data, fields, 3);
	if (fields[0])
	{
		TestStruct* str = TestStruct_parse(data + iterator);
		value->Field1 = *str;
		free(str);
		iterator += TestStruct_current_size(&value->Field1);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[1])
	{
		value->Field2 = data[iterator];
		iterator += sizeof(byte);
	}
	else
	{
		free (value);
		return NULL;
	}
	if (fields[2])
	{
		TestStruct* str = TestStruct_parse(data + iterator);
		value->Field3 = *str;
		free(str);
		iterator += TestStruct_current_size(&value->Field3);
		value->Field3_enabled = true;
	}
	else value->Field3_enabled = false;
	return value;
}

int SecondStruct_current_size (const SecondStruct* value)
{
	int size = 2; //enable flags
	size += TestStruct_current_size(&value->Field1);
	size += sizeof(byte);
	if (value->Field3_enabled) size += TestStruct_current_size(&value->Field3);
	return size;
}
int SecondStruct_preserialize (const SecondStruct* value, byte* output)
{
	uint16 must_be = 0;
	int iterator = 2;
	must_be = (uint16)(must_be | (1 << 0));
{
	byte* output_f = (byte*)malloc (TestStruct_current_size (&value->Field1));
	int size_f = TestStruct_preserialize(&value->Field1, output_f);
	memcpy(output + iterator, output_f, size_f);
	free(output_f);
	iterator += size_f;
}
	must_be = (uint16)(must_be | (1 << 1));
frame_parser_helper_set_byte(output + iterator, value->Field2);
	iterator += sizeof(byte);
	if (value->Field3_enabled)
	{
	must_be = (uint16)(must_be | (1 << 2));
{
	byte* output_f = (byte*)malloc (TestStruct_current_size (&value->Field3));
	int size_f = TestStruct_preserialize(&value->Field3, output_f);
	memcpy(output + iterator, output_f, size_f);
	free(output_f);
	iterator += size_f;
}
	}
	frame_parser_helper_set_uint16 (output, must_be);
	return iterator;
}
void SecondStruct_serialize (const SecondStruct* value, byte* output)
{
	int frame_size = 1 + SecondStruct_current_size (value) + sizeof(uint32); // struct + crc32
	byte* data = (byte*)malloc (frame_size);
	data[0] = 1;
	int iterator = SecondStruct_preserialize(value, data + 1);
	uint32 crc = frame_parser_helper_calculate_crc (data, iterator + 1);
	frame_parser_helper_set_uint32 (data + 1 + iterator, crc);
	frame_finalise(data, frame_size, output);
	free(data);
}

