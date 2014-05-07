#ifndef HIGHFLYERS_PROTOCOL_PARSER_H
#define HIGHFLYERS_PROTOCOL_PARSER_H

#include "frames.h"

typedef struct {
	FrameTypes type;
	void* pointer;
} FrameProxy;

typedef struct {
	bool prev_sentinel;
	byte bytes[FRAMEPARSER_HELPER_MAXLENGTH];
	int iterator;
	FrameProxy last_frame;
	bool last_frame_actual;
} HighFlyersParser;


void parser_initialize (HighFlyersParser* obj);
void parser_append_bytes (HighFlyersParser* obj, byte bytes[], int size);
void parser_append_byte (HighFlyersParser* obj, byte b);

#endif
