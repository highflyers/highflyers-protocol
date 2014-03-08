// GENERATED CODE

using System;
using System.Collections.Generic;

namespace HighFlyers.Protocol.Frames
{
    class TelemetryFrame : Frame
    {
        public double? Temperature = null;
        public int? DummyIntData = null;
        public double? WindSpeed = null;

        public int TotalSize
        {
            get { return sizeof(double) + sizeof(int) + sizeof(double); }
        }

        protected override int FieldCount
        {
            get { return 3; }
        }

        public override void Parse(List<byte> bytes)
        {
            byte[] data = bytes.ToArray();
            bool[] fields = PreParseData(data);
            int iterator = 0;

            if (fields[0])
            {
                Temperature = BitConverter.ToDouble(data, iterator + 5);
                iterator += Utils.GetSize(Temperature.GetValueOrDefault());
            }
            if (fields[1])
            {
                DummyIntData = BitConverter.ToInt32(data, iterator + 5);
                iterator += Utils.GetSize(DummyIntData.GetValueOrDefault());
            }
            if (fields[2])
            {
                WindSpeed = BitConverter.ToDouble(data, iterator + 5);
                iterator += Utils.GetSize(WindSpeed.GetValueOrDefault()); ;
            }

            CheckCrcSum(data[iterator]);
        }
    }
}
