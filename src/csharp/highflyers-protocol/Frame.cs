using System;
using System.Collections.Generic;

namespace HighFlyers.Protocol
{
    public abstract class Frame
    {
        protected abstract int FieldCount { get; }
        public abstract int CurrentSize { get; }

        protected bool[] PreParseData(byte[] data)
        {
            UInt16 fieldFlags = BitConverter.ToUInt16(data, 0);
            var fields = new bool[FieldCount];

            for (int i = 0; i < FieldCount; i++)
                fields[i] = (fieldFlags & (1 << i)) != 0;

            return fields;
        }

		protected abstract byte[] Serialize ();

		public byte[] FullSerialize ()
		{
			var data = new List<byte> (Serialize ());
			data.AddRange (BitConverter.GetBytes (FrameParserHelper.CalculateCrcSum (data)));
			var finishData = new List<byte> ();

			foreach (var d in data) {
				if (d == FrameParserHelper.EndFrame || d == FrameParserHelper.Sentinel)
					finishData.Add (FrameParserHelper.Sentinel);

				finishData.Add (d);
			}

			finishData.Add (FrameParserHelper.EndFrame);

			return finishData.ToArray ();
		}

        public abstract void Parse(List<byte> bytes, FrameParserHelper.EndianType endian);
    }

}
