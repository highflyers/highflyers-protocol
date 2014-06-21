#ifndef HIGHFLYERS_PROTOCOL_FRAMES_H
#define HIGHFLYERS_PROTOCOL_FRAMES_H

#include "types.h"

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

typedef struct
{
	TestStruct Field1;
	byte Field2;
	TestStruct Field3;
	bool Field3_enabled;
} SecondStruct;

void frame_preparse_data (const byte* data, bool* output, int field_count);
void frame_finalise(const byte* data, int size, bool* output);

TestStruct* TestStruct_parse (const byte* data);
int TestStruct_current_size (const TestStruct* value);
int TestStruct_preserialize (const TestStruct* value, byte* output);
void TestStruct_serialize (const TestStruct* value, byte* output);

SecondStruct* SecondStruct_parse (const byte* data);
int SecondStruct_current_size (const SecondStruct* value);
int SecondStruct_preserialize (const SecondStruct* value, byte* output);
void SecondStruct_serialize (const SecondStruct* value, byte* output);

#endif
