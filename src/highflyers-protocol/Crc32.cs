using System;

namespace HighFlyers.Protocol
{
	public static class Crc32
	{
		static uint[] crc_tab;

		static Crc32 ()
		{
			uint poly = 0xedb88320;
			crc_tab = new uint[256];
			uint temp = 0;
			for (uint i = 0; i < crc_tab.Length; ++i) {
				temp = i;
				for (int j = 8; j > 0; --j) {
					temp = (temp & 1) == 1 ? (uint)((temp >> 1) ^ poly) : temp >> 1;
				}
				crc_tab [i] = temp;
			}
		}

		public static uint CalculateCrc32 (byte[] bytes)
		{
			uint crc = 0xffffffff;
			for (int i = 0; i < bytes.Length; ++i) {
				byte index = (byte)(((crc) & 0xff) ^ bytes [i]);
				crc = (uint)((crc >> 8) ^ crc_tab [index]);
			}
			return ~crc;
		}
	}
}

