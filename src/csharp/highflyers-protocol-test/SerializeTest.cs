using HighFlyers.Protocol;
using HighFlyers.Protocol.Frames;
using NUnit.Framework;

namespace HighFlyers.Test.Protocol
{
    [TestFixture]
    public class SerializeTest
    {
        [Test]
        public void SimpleSerialization()
        {
            var ts = new TestStruct();
            ts.Field1 = 32;
            ts.Field3 = 2;
            ts.Field4 = 108;

            byte[] data = ts.FullSerialize();

            var parser = new Parser<FrameBuilder>();
            parser.AppendBytes(data);

            var frame = parser.LastFrame as TestStruct;

            Assert.AreEqual(ts.Field1, frame.Field1);
            Assert.AreEqual(ts.Field2, frame.Field2);
            Assert.AreEqual(ts.Field3, frame.Field3);
            Assert.AreEqual(ts.Field4, frame.Field4);
        }
    }
}