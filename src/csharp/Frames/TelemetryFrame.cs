// GENERATED CODE! DON'T MODIFY IT!
using System;
using System.Collections.Generic;
using System.Linq;
namespace HighFlyers.Protocol.Frames
{
    enum Enumeracja
    {
        PIERWSZY,
        DRUGI,
        TRZECI,
    }
    class Minor : Frame
    {
        public override void Parse(List<byte> bytes)
        {
            byte[] data = bytes.ToArray();
            bool[] fields = PreParseData(data);
            int iterator = 0;
            if (fields[0])
            {
                a = BitConverter.ToUInt16(data, iterator + 2);
                iterator += sizeof(UInt16);
            }
            if (fields[1])
            {
                b = BitConverter.ToDouble(data, iterator + 2);
                iterator += sizeof(Double);
            }
            else throw new Exception("field b must be enabled! It's not an optional value!");
            CheckCrcSum(data[iterator]);
        }

        public override int CurrentSize
        {
            get
            {
                int size = 0;
                if (a != null) size += sizeof(UInt16);
                size += sizeof(Double);
                return size;
            }
        }
        protected override int FieldCount { get { return 2; } }
        public int? a = null;
        public double b;
    }
    class Message : Frame
    {
        public override void Parse(List<byte> bytes)
        {
            byte[] data = bytes.ToArray();
            bool[] fields = PreParseData(data);
            int iterator = 0;
            if (fields[0])
            {
                x = BitConverter.ToUInt16(data, iterator + 2);
                iterator += sizeof(UInt16);
            }
            if (fields[1])
            {
                y = BitConverter.ToDouble(data, iterator + 2);
                iterator += sizeof(Double);
            }
            else throw new Exception("field y must be enabled! It's not an optional value!");
            if (fields[2])
            {
                sample_struct = new Minor(); sample_struct.Parse(data.ToList().GetRange(iterator + 2, data.Length - iterator - 2));
                iterator += sample_struct.CurrentSize;
            }
            else throw new Exception("field sample_struct must be enabled! It's not an optional value!");
            if (fields[3])
            {
                z = BitConverter.ToSingle(data, iterator + 2);
                iterator += sizeof(Single);
            }
            else throw new Exception("field z must be enabled! It's not an optional value!");
            CheckCrcSum(data[iterator]);
        }

        public override int CurrentSize
        {
            get
            {
                int size = 0;
                if (x != null) size += sizeof(UInt16);
                size += sizeof(Double);
                size += sample_struct.CurrentSize;
                size += sizeof(Single);
                return size;
            }
        }
        protected override int FieldCount { get { return 4; } }
        public int? x = null;
        public double y;
        public Minor sample_struct;
        public Single z;
    }
    enum FrameTypes
    {
        Minor,
        Message,
    }
}

