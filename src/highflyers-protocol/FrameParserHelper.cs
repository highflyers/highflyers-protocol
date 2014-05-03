using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace HighFlyers.Protocol
{
    public class FrameParserHelper
    {
		public enum EndianType
		{
			Little,
			Big
		}

		public static byte EndFrame
		{
			get { return 12; }
		}
		public static byte Sentinel
		{
			get { return 13; }
		}
		public static uint MaxLength
		{
			get { return 2048; }
		}

        public static void CheckBytes(List<byte> bytes)
        {
			if (BitConverter.ToUInt32 (bytes.ToArray (), bytes.Count - 5) != Crc32.CalculateCrc32(bytes.Take (bytes.Count - 5).ToArray ()))
				throw new InvalidDataException("Invalid crc sum!");

        }

        public static void CheckCrcSum(UInt32 crc, byte[] bytes)
		{}
				
		public static void ReorderIfNeeded (EndianType endian, IList<byte> a, int start, int size)
		{
			if (endian == FrameParserHelper.EndianType.Little && BitConverter.IsLittleEndian || endian == FrameParserHelper.EndianType.Big && !BitConverter.IsLittleEndian)
				return;

			for (int i = 0; i < size / 2; i++) {
				byte tmp = a [i + start];
				a [i + start] = a [start + size - i - 1];
				a [start + size - i - 1] = tmp;
			}
		}
    }
}
