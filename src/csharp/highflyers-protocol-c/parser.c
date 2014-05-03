#include "parser.h"

void parse_frame (HighFlyersParser* obj);

void append_bytes (HighFlyersParser* obj, byte bytes[], int size)
{
	int i;
	
	for (i = 0; i < size; i++) 
	{
		obj->bytes[obj->iterator++ % FRAMEPARSER_HELPER_MAXLENGTH] = bytes[i];
		// todo log somewhere about iterator's restart
	}
}

void append_byte (HighFlyersParser* obj, byte b)
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

}