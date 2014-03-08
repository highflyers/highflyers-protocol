using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using HighFlyers.Protocol.Frames;

namespace HighFlyers.Protocol
{
    enum FrameType
    {
        Telemetry = 0
    }

    public abstract class Frame
    {
        protected abstract int FieldCount { get; }
        public abstract int CurrentSize { get; }

        protected void CheckCrcSum(UInt32 crc)
        {
            if (crc != 12) // TODO fuck yeah, hardcoded crc!
                throw new InvalidDataException("Invalid crc sum!");
        }

        protected bool[] PreParseData(byte[] data)
        {
            UInt16 fieldFlags = BitConverter.ToUInt16(data, 3);
            var fields = new bool[FieldCount];

            for (int i = 0; i < FieldCount; i++)
                fields[i] = (fieldFlags & (1 >> i)) != 0;

            return fields;
        }


        public abstract void Parse(List<byte> bytes);
    }

    class FrameBuilder
    {
        public static Frame BuildFrame(List<byte> bytes)
        {
            Frame frame;
            UInt16 size = BitConverter.ToUInt16(bytes.ToArray(), 1);

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
