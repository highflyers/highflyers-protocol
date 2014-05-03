#include "parser.h"
#include "TestFramework/TestFramework.h"
#include <stdio.h>

void check_frame_parser_helper_to_uint32 ()
{
	byte arr[] = {20, 1, 0, 0};
	uint32 v = frame_parser_helper_to_uint32 (arr, 0);
	ASSERT_EQ(v , 276, "%d")
}

int main (int argc, char *argv[])
{
	check_frame_parser_helper_to_uint32();
	
	TEST_SUMMARY
	
	return 0;
}

