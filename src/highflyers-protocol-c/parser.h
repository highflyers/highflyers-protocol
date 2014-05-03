#ifndef HIGHFLYERS_PROTOCOL_PARSER_H
#define HIGHFLYERS_PROTOCOL_PARSER_H

#include "frame_parser_helper.h"

typedef struct {
	bool prev_sentinel;
	byte bytes[FRAMEPARSER_HELPER_MAXLENGTH];
	int iterator;
} HighFlyersParser;

void parser_initialize (HighFlyersParser* obj);
void parser_append_bytes (HighFlyersParser* obj, byte bytes[], int size);
void parser_append_byte (HighFlyersParser* obj, byte b);

#endif