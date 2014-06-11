#include "parser.h"
#include "frame.h"
#include "frames.h"
#include "TestFramework/TestFramework.h"
#include <stdio.h>
#include <stdlib.h>

void check_frame_parser_helper_to_uint32 ()
{
	byte arr[] = {20, 1, 0, 0};
	uint32 v = frame_parser_helper_to_uint32 (arr, 0);
	ASSERT_EQ(v , 276, "%d")
}

void simple_parser_test ()
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

void serialize_test ()
{
	int i;
	TestStruct* str = (TestStruct*)malloc(sizeof(TestStruct));
	str->Field1 = 32;
	str->Field3 = 13;
	str->Field4 = 108;

	int str_size = TestStruct_current_size(str);

	byte* data = (byte*)malloc(sizeof(byte)*str_size);

	TestStruct_serialize(str, data);

	byte* frame_bytes = (byte*)malloc(sizeof(byte)*str_size + 10 /*for crc and others*/);

	int frame_size = frame_serialize(data, str_size, frame_bytes);

	free(data);

	HighFlyersParser parser;

	parser_initialize(&parser);

	for (i = 0; i < frame_size; i++)
		parser_append_byte(&parser, frame_bytes[i]);

	ASSERT_TRUE(parser_has_frame(&parser));
	FrameProxy p = parser_get_last_frame_ownership(&parser);
	TestStruct* frame = (TestStruct*)p.pointer;
	ASSERT_TRUE(frame);
	ASSERT_EQ(str->Field1, frame->Field1, "%d");
	ASSERT_EQ(str->Field3, frame->Field3, "%d");
	ASSERT_EQ(str->Field4, frame->Field4, "%d");

	free(frame_bytes);
	free(str);
	frame_proxy_free(&p);
}

int main (int argc, char *argv[])
{
	check_frame_parser_helper_to_uint32();
	simple_parser_test ();
	serialize_test ();

	TEST_SUMMARY

	return 0;
}

