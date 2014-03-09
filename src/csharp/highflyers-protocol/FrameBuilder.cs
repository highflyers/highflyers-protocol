using System;
using System.Collections.Generic;
using System.IO;

namespace HighFlyers.Protocol
{
    partial class FrameBuilder
    {
        private static void CheckBytes(List<byte> bytes)
        {
            UInt16 size = BitConverter.ToUInt16(bytes.ToArray(), 1);

            CheckCrcSum(BitConverter.ToUInt32(bytes.ToArray(), bytes.Count - 5));

            if (size != bytes.Count)
                throw new InvalidDataException("Invalid data length");
        }

        private static void CheckCrcSum(UInt32 crc)
        {
            if (crc != 12) // TODO fuck yeah, hardcoded crc!
                throw new InvalidDataException("Invalid crc sum!");
        }
    }
}
