#include "frame.h"
#include <string.h>

void frame_preparse_data(const byte* data, bool* output, int field_count)
{
	uint16 field_flags = frame_parser_helper_to_uint16(data, 0);
	int i;

	for (i = 0; i < field_count; i++)
		output[i] = (field_flags & (1 << i)) != 0;
}

int frame_serialize(const byte* serialized_data, int size, byte* output)
{
	uint32 crc = frame_parser_helper_calculate_crc(serialized_data, size);

	memcpy(output, serialized_data, size);

	frame_parser_helper_set_uint32 (output + size + 1, crc);

	return 0;



/*
 * 			var data = Serialize ();
			data.AddRange (BitConverter.GetBytes (Crc32.CalculateCrc32 (data.ToArray ())));
			var finishData = new List<byte> ();

			foreach (var d in data) {
				if (d == FrameParserHelper.EndFrame || d == FrameParserHelper.Sentinel)
					finishData.Add (FrameParserHelper.Sentinel);

				finishData.Add (d);
			}

			finishData.Add (FrameParserHelper.EndFrame);

			return finishData.ToArray ();
 */
}
