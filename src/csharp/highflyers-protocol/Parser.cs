﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;

namespace HighFlyers.Protocol
{
    public class FrameParsedEventArgs : EventArgs
    {
        public FrameParsedEventArgs(Frame frame)
        {
            ParsedFrame = frame;
        }

        public Frame ParsedFrame { get; private set; }
    }

    public class Parser<T>
    {
        public delegate void FrameParsedHandler(object sender, FrameParsedEventArgs args);

        private readonly MethodInfo buildMethod;
        private readonly List<byte> bytes = new List<byte>();
        private bool prevSentinel;

        public Parser()
        {
            buildMethod = typeof(T).GetMethod("BuildFrame", new[] {typeof(List<byte>)});

            if (buildMethod == null)
                throw new Exception("Cannot find method GetMethod in a builder class");

            LastFrame = null;
        }

        public Frame LastFrame { get; private set; }
        public event FrameParsedHandler FrameParsed;

        protected void OnFrameParsed(FrameParsedEventArgs args)
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
            bool tmpSentinel = prevSentinel;
            if (prevSentinel)
            {
                if (b == FrameParserHelper.Sentinel || b == FrameParserHelper.EndFrame)
                    bytes.Add(b);
                else
                    throw new InvalidDataException("Unexpected token " + b);
                prevSentinel = false;
            }
            else if (b == FrameParserHelper.Sentinel)
                prevSentinel = true;
            else
                bytes.Add(b);

            if (b == FrameParserHelper.EndFrame && !tmpSentinel)
                ParseFrame();
            else if (FrameParserHelper.MaxLength == bytes.Count)
            {
                bytes.Clear();
                throw new WarningException("Too many bytes without end_frame sign. Dropping data...");
            }
        }

        private void ParseFrame()
        {
            LastFrame = buildMethod.Invoke(null, new object[] {bytes}) as Frame;
            OnFrameParsed(new FrameParsedEventArgs(LastFrame));
            bytes.Clear();
        }
    }
}