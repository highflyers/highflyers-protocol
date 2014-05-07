#include "parser.h"
#include "frame_builder.h"

void parse_frame (HighFlyersParser* obj);

void parser_initialize (HighFlyersParser* obj)
{
	obj->prev_sentinel = false;
	obj->iterator = 0;
	obj->last_frame_actual = false;
}


// todo don't use this method. It should be removed!
void parser_append_bytes (HighFlyersParser* obj, byte bytes[], int size)
{
	int i;
	bool new_frame;
	for (i = 0; i < size; i++) 
	{
		parser_append_byte(obj, bytes[i]);
	}
}

void parser_append_byte (HighFlyersParser* obj, byte b)
{
	bool tmp_sentinel = obj->prev_sentinel;
	
	if (obj->prev_sentinel)
	{
		if (b == FRAMEPARSER_HELPER_SENTINEL || b == FRAMEPARSER_HELPER_ENDFRAME)
			obj->bytes[obj->iterator++ % FRAMEPARSER_HELPER_MAXLENGTH] = b;
		else
			; // todo throw new InvalidDataException ("Unexpected token " + b);
		
		obj->prev_sentinel = false;
	}
	else if (b == FRAMEPARSER_HELPER_SENTINEL)
		obj->prev_sentinel = true;
	else
		obj->bytes[obj->iterator++ % FRAMEPARSER_HELPER_MAXLENGTH] = b;

	if (b == FRAMEPARSER_HELPER_ENDFRAME && !tmp_sentinel)
		parse_frame (obj);
	else if (FRAMEPARSER_HELPER_MAXLENGTH == obj->iterator) 
	{
		obj->iterator = 0;
		// todo throw new WarningException ("Too many bytes without end_frame sign. Dropping data...");
	}
}

void parse_frame (HighFlyersParser* obj)
{
	FrameProxy p;
	p = frame_builder_build_frame (obj->bytes, obj->iterator);
	obj->last_frame = p;
	obj->last_frame_actual = true;
}
