#include "frame_parser_helper.h"

byte crc_tab[256];

uint32 frame_parser_helper_calculate_crc (byte* bytes, int size);
uint32 frame_parser_helper_to_uint32 (byte* bytes, int index);

void init_highflyers_protocol ()
{
	uint32 i, j, poly = 0xedb88320, temp = 0;
	
	for (i = 0; i < 256; ++i)
	{
		temp = i;
		for (j = 8; j > 0; --j)
			temp = (temp & 1) == 1 ? (uint32)((temp >> 1) ^ poly) : temp >> 1;
		
		crc_tab [i] = temp;
	}
}

uint32 frame_parser_helper_calculate_crc (byte* bytes, int size)
{
	uint32 crc = 0xffffffff, i;
	for (i = 0; i < size; ++i)
	{
		byte index = (byte)(((crc) & 0xff) ^ bytes [i]);
		crc = (uint32)((crc >> 8) ^ crc_tab [index]);
	}
	
	return ~crc;
}

uint32 frame_parser_helper_to_uint32 (byte* bytes, int index)
{
	uint32 val = 0;
	int i = 0;
	
	for (i = 0; i < 4; i++)
		val = val | (bytes [i] << (i * 8));
}

bool frame_parser_helper_check_bytes (byte* bytes, int size)
{
	return frame_parser_helper_to_uint32 (bytes, size - 5) == frame_parser_helper_calculate_crc(bytes, size - 5);
}
