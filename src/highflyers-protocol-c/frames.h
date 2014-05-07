#ifndef HIGHFLYERS_PROTOCOL_FRAMES_H
#define HIGHFLYERS_PROTOCOL_FRAMES_H

#include "frame_parser_helper.h"

typedef enum 
{
	T_TestStruct,
	T_SecondStruct
} FrameTypes;

typedef struct
{
	uint32 Field1;
	double Field2;
	bool Field2_enabled;
	byte Field3;
	int Field4;
	bool Field4_enabled;
} TestStruct;

TestStruct* TestStruct_parse (const byte* data, int size);
int TestStruct_current_size (const TestStruct* value);

typedef struct
{
	TestStruct Field1;
	byte Field2;
	TestStruct Field3;
	bool Field3_enabled;
} SecondStruct; 

SecondStruct SecondStruct_parse (const byte* bytes, int size);
int Second_current_size (const SecondStruct* value);

#endif
