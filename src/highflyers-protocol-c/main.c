#include "parser.h"
#include "frames.h"
#include "TestFramework/TestFramework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_frame_parser_helper_to_uint32 ()
{
	byte arr[] = {20, 1, 0, 0};
	uint32 v = frame_parser_helper_to_uint32 (arr, 0);
	ASSERT_EQ(v , 276, "%d")
}

void test_struct_parser_test ()
{
	byte bytes[] = { 0, 255, 255,
			FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_ENDFRAME, 1, 0, 0,
			FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_SENTINEL,
			64, 23, 3, 11, 5, 2, 4,	2,
			FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_SENTINEL, 4, 2, 1,
			114, 84, 5, 19,
			FRAMEPARSER_HELPER_ENDFRAME
	};
	int i;
	HighFlyersParser parser;

	parser_initialize(&parser);

	for (i = 0; i < 28; i++)
		parser_append_byte(&parser, bytes[i]);

	ASSERT_TRUE(parser_has_frame(&parser));
	FrameProxy p = parser_get_last_frame_ownership(&parser);
	ASSERT_EQ(T_TestStruct, p.type, "%d");
	TestStruct* str = (TestStruct*)p.pointer;
	ASSERT_TRUE(str);
	ASSERT_EQ(256 + FRAMEPARSER_HELPER_ENDFRAME, str->Field1, "%d");
	ASSERT_EQ(2, str->Field3, "%d");
	frame_proxy_free(&p);
}

void test_struct_serialize_parser_test ()
{
	int i;
	TestStruct* str = (TestStruct*)malloc(sizeof(TestStruct));
	str->Field1 = 32;
	str->Field3 = 13;
	str->Field4 = 108;

	str->Field2_enabled = 0;
	str->Field4_enabled = 1;

	int frame_size = 30;

	byte* data = (byte*)malloc(frame_size);

	TestStruct_serialize(str, data);

	HighFlyersParser parser;

	parser_initialize (&parser);

	for (i = 0; i < frame_size; i++)
	{
		parser_append_byte(&parser, data[i]);
		if (parser_has_frame(&parser))
			break;
	}

	ASSERT_TRUE(parser_has_frame(&parser));
	FrameProxy p = parser_get_last_frame_ownership(&parser);
	TestStruct* frame = (TestStruct*)p.pointer;
	ASSERT_TRUE(frame);
	ASSERT_EQ(str->Field1, frame->Field1, "%d");
	ASSERT_EQ(str->Field3, frame->Field3, "%d");
	ASSERT_EQ(str->Field4, frame->Field4, "%d");

	free(data);
	free(str);
	frame_proxy_free(&p);
}

void test_struct_serialize_test ()
{
	byte bytes[] =
	{ 0, 15, 0,
	FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_ENDFRAME, 1, 0, 0,
	10, 64, 23, 3, 11, 5,
			2, 4, 2,
			FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_SENTINEL, 4, 2, 1,
			114, 84, 5, 19,
			FRAMEPARSER_HELPER_ENDFRAME };
	int i;
	TestStruct* str = (TestStruct*)malloc (sizeof(TestStruct));
	str->Field1 = 256 + FRAMEPARSER_HELPER_ENDFRAME;
	memcpy(&str->Field2, bytes + 8, sizeof(double));
	str->Field3 = 2;
	str->Field4 = 16909325;

	str->Field2_enabled = 1;
	str->Field4_enabled = 1;

	int frame_size = sizeof(bytes);

	byte* data = (byte*)malloc (sizeof(byte) * frame_size);

	TestStruct_serialize (str, data);

	bool passed = 1;
	for (i = 0; i < sizeof(bytes); i++)
	{
		if (i > (sizeof(bytes) - 6)) continue; // skip crc32 check

		if (bytes[i] != data[i])
		{
			passed = 0;
			break;
		}
	}

	ASSERT_TRUE(passed);

	free (data);
	free (str);
}

void second_struct_serialize_test ()
{
	byte bytes[] =
	{ 0, 7, 0, 0, FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_SENTINEL, 0,
			101, 0, 0, 0, 243, 32, 0, 0, 0, 55, 0, FRAMEPARSER_HELPER_SENTINEL,
			FRAMEPARSER_HELPER_SENTINEL, 0, 43, 0, 0, 0, 223, 210, 4, 0, 0, 75,
			255, 255, 255, FRAMEPARSER_HELPER_ENDFRAME };
	int i;
	SecondStruct* str = (SecondStruct*)malloc (sizeof(SecondStruct));
	str->Field1.Field1 = 101;
	str->Field1.Field2_enabled = 0;
	str->Field1.Field3 = 243;
	str->Field1.Field4_enabled = 1;
	str->Field1.Field4 = 32;

	str->Field2 = 55;

	str->Field3.Field1 = 43;
	str->Field3.Field2_enabled = 0;
	str->Field3.Field3 = 223;
	str->Field3.Field4_enabled = 1;
	str->Field3.Field4 = 1234;
	str->Field3_enabled = 1;

	int frame_size = sizeof(bytes);

	byte* data = (byte*)malloc (sizeof(byte) * frame_size);

	SecondStruct_serialize (str, data);

	bool passed = 1;
	for (i = 0; i < sizeof(bytes); i++)
	{
		if (i > (sizeof(bytes) - 6)) continue; // skip crc32 check

		if (bytes[i] != data[i])
		{
			passed = 0;
			break;
		}
	}

	ASSERT_TRUE(passed);

	free (data);
	free (str);
}

int main (int argc, char *argv[])
{
	init_highflyers_protocol ();
	check_frame_parser_helper_to_uint32 ();

	test_struct_parser_test ();
	test_struct_serialize_test ();
	test_struct_serialize_parser_test ();

	//second_struct_parser_test ();
	second_struct_serialize_test ();
	//second_struct_serialize_parser_test ();

	TEST_SUMMARY

	return 0;
}

