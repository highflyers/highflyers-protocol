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

void test_crc32()
{
	byte input[] = {1,3,0,13,0,5,1,0,0,80,52,0,0,0,98};
	uint32 expected = 2250716664;

	uint32 result = frame_parser_helper_calculate_crc(input, sizeof(input));

	ASSERT_EQ(expected, result, "%u");
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
	{ 0,
	FRAMEPARSER_HELPER_SENTINEL,
	FRAMEPARSER_HELPER_SENTINEL, 0,
	FRAMEPARSER_HELPER_SENTINEL,
	FRAMEPARSER_HELPER_ENDFRAME, 1, 0, 0, 2, 47, 1, 0, 0, 221, 254, 140, 25,
	FRAMEPARSER_HELPER_ENDFRAME };
	int i;
	TestStruct* str = (TestStruct*)malloc (sizeof(TestStruct));
	str->Field1 = 256 + FRAMEPARSER_HELPER_ENDFRAME;
	str->Field3 = 2;
	str->Field4 = 303;

	str->Field2_enabled = 0;
	str->Field4_enabled = 1;

	int frame_size = sizeof(bytes);

	byte* data = (byte*)malloc (sizeof(byte) * frame_size);

	TestStruct_serialize (str, data);

	bool passed = 1;
	for (i = 0; i < sizeof(bytes); i++)
	{
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


void second_struct_parser_test ()
{
	byte bytes[] =
	{
			1, 1  + 2, 0,
			FRAMEPARSER_HELPER_SENTINEL, 1 + 4 + 8, 0,
			5, 1, 0, 0,
			80,
			52, 0, 0, 0,
			98,
			248, 53, 39, 134,
			FRAMEPARSER_HELPER_ENDFRAME
	};

	int i;
	HighFlyersParser parser;

	parser_initialize(&parser);

	for (i = 0; i < sizeof(bytes); i++)
		parser_append_byte(&parser, bytes[i]);

	ASSERT_TRUE(parser_has_frame(&parser));
	FrameProxy p = parser_get_last_frame_ownership(&parser);
	ASSERT_EQ(T_SecondStruct, p.type, "%d");
	SecondStruct* str = (SecondStruct*)p.pointer;
	ASSERT_TRUE(str);
	ASSERT_EQ(256 + 5, str->Field1.Field1, "%d");
	ASSERT_TRUE(!str->Field1.Field2_enabled);
	ASSERT_EQ(80, str->Field1.Field3, "%d");
	ASSERT_EQ(52, str->Field1.Field4, "%d");
	ASSERT_EQ(98, str->Field2, "%d");
	ASSERT_TRUE(!str->Field3_enabled);

	frame_proxy_free(&p);
}

void second_struct_serialize_test ()
{
	byte bytes[] =
	{ 1, 7, 0, FRAMEPARSER_HELPER_SENTINEL, FRAMEPARSER_HELPER_SENTINEL, 0,
			101, 0, 0, 0, 243, 32, 0, 0, 0, 55, FRAMEPARSER_HELPER_SENTINEL,
			FRAMEPARSER_HELPER_SENTINEL, 0, 43, 0, 0, 0, 223, 210, 4, 0, 0, 182,
			153, 208, 66, FRAMEPARSER_HELPER_ENDFRAME };
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

void second_struct_serialize_parser_test ()
{
	int i;
	SecondStruct* str = (SecondStruct*)malloc(sizeof(SecondStruct));
	TestStruct ts;
	ts.Field1 = 256 + 5;
	ts.Field3 = 80;
	ts.Field4 = 52;
	ts.Field2_enabled = 0;
	ts.Field4_enabled = 1;
	str->Field1 = ts;

	str->Field2 = 98;
	str->Field3_enabled = 0;

	int frame_size = 22;

	byte* data = (byte*)malloc (sizeof(byte) * frame_size);

	SecondStruct_serialize (str, data);

	HighFlyersParser parser;

	parser_initialize(&parser);

	for (i = 0; i < frame_size; i++)
		parser_append_byte(&parser, data[i]);

	ASSERT_TRUE(parser_has_frame(&parser));
	FrameProxy p = parser_get_last_frame_ownership(&parser);
	ASSERT_EQ(T_SecondStruct, p.type, "%d");
	SecondStruct* frame = (SecondStruct*)p.pointer;
	ASSERT_TRUE(frame);
	ASSERT_EQ(frame->Field1.Field1, str->Field1.Field1, "%d");
	ASSERT_EQ(frame->Field1.Field3, str->Field1.Field3, "%d");
	ASSERT_EQ(frame->Field1.Field4, str->Field1.Field4, "%d");
	ASSERT_EQ(frame->Field2, str->Field2, "%d");

	frame_proxy_free(&p);
}

int main (int argc, char *argv[])
{
	init_highflyers_protocol ();
	check_frame_parser_helper_to_uint32 ();
	test_crc32();

	test_struct_parser_test ();
	test_struct_serialize_test ();
	test_struct_serialize_parser_test ();

	second_struct_parser_test ();
	second_struct_serialize_test ();
	second_struct_serialize_parser_test ();

	TEST_SUMMARY

	return 0;
}

