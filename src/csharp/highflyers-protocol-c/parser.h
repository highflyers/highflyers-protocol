#ifndef HIGHFLYERS_PROTOCOL_PARSER_H
#define HIGHFLYERS_PROTOCOL_PARSER_H

typedef unsigned char byte;
typedef unsigned char bool;

#define true 1
#define false 0

#define FRAMEPARSER_HELPER_MAXLENGTH 2048
// todo update defined values
#define FRAMEPARSER_HELPER_SENTINEL 00 
#define FRAMEPARSER_HELPER_ENDFRAME 00

typedef struct {
	bool prev_sentinel;
	byte bytes[FRAMEPARSER_HELPER_MAXLENGTH];
	int iterator;
} HighFlyersParser;

void append_bytes (HighFlyersParser* obj, byte bytes[], int size);
void append_byte (HighFlyersParser* obj, byte b);

#endif