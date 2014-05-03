#ifndef HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H
#define HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H

#define FRAMEPARSER_HELPER_MAXLENGTH 2048
#define FRAMEPARSER_HELPER_SENTINEL 13 
#define FRAMEPARSER_HELPER_ENDFRAME 12

typedef unsigned char byte;
typedef unsigned char bool;
typedef unsigned int uint32;

#define true 1
#define false 0

enum EndianType
{
	LITTLE,
	BIG
};

void init_highflyers_protocol ();
bool frame_parser_helper_check_bytes (byte* bytes, int size);

#endif
