using System;
using System.Collections.Generic;
using System.IO;
using HighFlyers.Protocol.Frames;

namespace HighFlyers.Protocol
{
    class FrameBuilder
    {
        private static void CheckCrcSum(UInt32 crc)
        {
            if (crc != 12) // TODO fuck yeah, hardcoded crc!
                throw new InvalidDataException("Invalid crc sum!");
        }

        public static Frame BuildFrame(List<byte> bytes)
        {
            Frame frame;
            UInt16 size = BitConverter.ToUInt16(bytes.ToArray(), 1);

            CheckCrcSum(BitConverter.ToUInt32(bytes.ToArray(), bytes.Count - 5));

            if (size != bytes.Count)
                throw new InvalidDataException("Invalid data length");

            switch ((FrameTypes)bytes[0])
            {
                case FrameTypes.Message:
                    frame = new Message();
                    break;
                default:
                    throw new InvalidDataException("Unexpected frame type");
            }

            frame.Parse(bytes.GetRange(3, bytes.Count - 3));
            return frame;
        }
    }
}
