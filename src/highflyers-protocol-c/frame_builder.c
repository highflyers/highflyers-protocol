#include "frame_builder.h"
#include "frame_parser_helper.h"

FrameProxy frame_builder_build_frame (byte* bytes, int size)
{
	if (frame_parser_helper_check_bytes (bytes, size))
		; // todo so.. ALERT!
	
	FrameProxy proxy;
	byte endianes = bytes [0] & 128;
	FrameTypes frame_type = (FrameTypes)(bytes [0] & 127);
	void* frame;

	proxy.type = frame_type;
	switch (frame_type)
	{
	case T_TestStruct:
		proxy.pointer = (void*)TestStruct_parse (bytes + 1, size - 4);
		break;
	case T_SecondStruct:
		//proxy.pointer = (void*)SecondStruct_parse (bytes + 1, size - 4);
		break;
	default: 
		; // todo throw new InvalidDataException("Unexpected frame type");
	}
	
	return proxy;
}

