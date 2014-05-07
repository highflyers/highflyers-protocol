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
	
	HighFlyersParser parser;
	parser_initialize(&parser);
	parser_append_bytes(&parser, bytes, 28);		
	ASSERT_TRUE(parser.last_frame_actual);
	FrameProxy p = parser.last_frame;
	ASSERT_EQ(T_TestStruct, p.type, "%d");
	TestStruct* str = (TestStruct*)p.pointer;
	ASSERT_EQ(256 + FRAMEPARSER_HELPER_ENDFRAME, str->Field1, "%d");
	ASSERT_EQ(2, str->Field3, "%d");

	free(p.pointer);
}

int main (int argc, char *argv[])
{
	check_frame_parser_helper_to_uint32();
	simple_parser_test ();
	
	TEST_SUMMARY
	
	return 0;
}

