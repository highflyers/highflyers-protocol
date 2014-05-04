#ifndef HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H
#define HIGHFLYERS_PROTOCOL_FRAME_PARSER_HELPER_H

#include <string.h>

#define FRAMEPARSER_HELPER_MAXLENGTH 2048
#define FRAMEPARSER_HELPER_SENTINEL 13 
#define FRAMEPARSER_HELPER_ENDFRAME 12

typedef unsigned char byte;
typedef unsigned char bool;
typedef unsigned int uint32;
typedef unsigned short uint16;

#define true 1
#define false 0

enum EndianType
{
	LITTLE,
	BIG
};

void init_highflyers_protocol ();
bool frame_parser_helper_check_bytes (byte* bytes, int size);

uint32 frame_parser_helper_to_uint32 (const byte* bytes, int index);
uint16 frame_parser_helper_to_uint16 (const byte* bytes, int index);
double frame_parser_helper_to_double (const byte* bytes, int index);

#define CONVERT_TO_DATATYPE( type ) \
type frame_parser_helper_to_##type (const byte* bytes, int index) \
{ \
	type val = 0; \
	memcpy(&val, bytes+index, sizeof( type )); \
	return val; \
}


#endif
