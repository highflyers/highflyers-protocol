using System;
using System.Collections.Generic;
using System.IO;

namespace HighFlyers.Protocol
{
    public class FrameParserHelper
    {
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
            CheckCrcSum(BitConverter.ToUInt32(bytes.ToArray(), bytes.Count - 5));
        }

        public static void CheckCrcSum(UInt32 crc)
        {
            if (crc != 12) // TODO fuck yeah, hardcoded crc!
                throw new InvalidDataException("Invalid crc sum!");
        }
    }
}
