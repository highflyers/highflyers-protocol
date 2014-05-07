#include "frame_builder.h"

FrameProxy frame_builder_build_frame (byte* bytes, int size)
{
	FrameProxy proxy;
	frame_proxy_initialize(&proxy);

	if (!frame_parser_helper_check_bytes (bytes, size))
		return proxy;

	byte endianes = bytes [0] & 128;
	FrameTypes frame_type = (FrameTypes)(bytes [0] & 127);
	void* frame;

	proxy.type = frame_type;
	switch (frame_type)
	{
	case T_TestStruct:
		proxy.pointer = (void*)TestStruct_parse (bytes + 1, size - 4);
		break;
	default: 
		break;
	}
	
	return proxy;
}

