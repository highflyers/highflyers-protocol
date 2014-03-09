using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;

namespace HighFlyers.Protocol
{
    public class FrameParsedArgs : EventArgs
    {
        public FrameParsedArgs(Frame frame)
        {
            ParsedFrame = frame;
        }

        public Frame ParsedFrame { get; private set; }
    }

    public class Parser
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

        private bool prevSentinel;

        private readonly List<byte> bytes = new List<byte>();

        public delegate void FrameParsedHandler(object sender, FrameParsedArgs args);
        public event FrameParsedHandler FrameParsed;

        protected void OnFrameParsed(FrameParsedArgs args)
        {
            if (FrameParsed != null) 
                FrameParsed(this, args);
        }

        public void AppendBytes(byte[] data)
        {
            foreach (byte b in data)
                AppendByte(b);
        }

        private void AppendByte(byte b)
        {
            if (prevSentinel)
            {
                if (b == Sentinel || b == EndFrame)
                    bytes.Add(b);
                else
                    throw new InvalidDataException("Unexpected token " + b);
                prevSentinel = false;
            }
            else if (b == Sentinel)
                prevSentinel = true;
            else
                bytes.Add(b);

            if (b == EndFrame)
                ParseFrame();
            else if (MaxLength == bytes.Count)
            {
                bytes.Clear();
                throw new WarningException("Too many bytes without end_frame sign. Dropping data...");
            }
        }

        private void ParseFrame()
        {
            Frame frame = FrameBuilder.BuildFrame(bytes);

            OnFrameParsed(new FrameParsedArgs(frame));
        }
    }
}
