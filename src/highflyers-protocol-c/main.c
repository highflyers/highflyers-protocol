#include "parser.h"
#include "TestFramework/TestFramework.h"
#include <stdio.h>

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

int main (int argc, char *argv[])
{
	check_frame_parser_helper_to_uint32();
	simple_parser_test ();

	TEST_SUMMARY

	return 0;
}

