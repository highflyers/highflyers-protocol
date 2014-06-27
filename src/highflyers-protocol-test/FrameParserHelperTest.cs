using System;
using HighFlyers.Protocol;
using NUnit.Framework;

namespace HighFlyers.Test.Protocol
{
    [TestFixture]
    public class FrameParserHelperTest
    {
        [Test]
        public void ShouldNotReorderBytes()
        {
            var a = new byte[] {1, 2, 3, 4, 5, 6};
            var b = (byte[])(a.Clone());

            FrameParserHelper.EndianType type = BitConverter.IsLittleEndian
                ? FrameParserHelper.EndianType.Little
                : FrameParserHelper.EndianType.Big;
            FrameParserHelper.ReorderIfNeeded(type, a, 2, 3);

            Assert.AreEqual(b.Length, a.Length);
            for (int i = 0; i < b.Length; i++)
            {
                Assert.AreEqual(b[i], a[i]);
            }
        }

        [Test]
        public void ShouldReorderBytes()
        {
            var a = new byte[] {1, 2, 3, 4, 5, 6};
            var b = new byte[] {1, 2, 5, 4, 3, 6};

            FrameParserHelper.EndianType type = BitConverter.IsLittleEndian
                ? FrameParserHelper.EndianType.Big
                : FrameParserHelper.EndianType.Little;
            FrameParserHelper.ReorderIfNeeded(type, a, 2, 3);

            Assert.AreEqual(b.Length, a.Length);
            for (int i = 0; i < b.Length; i++)
            {
                Assert.AreEqual(b[i], a[i]);
            }
        }
    }
}