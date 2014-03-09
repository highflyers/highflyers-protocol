using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace HighFlyers.Test.Protocol
{
    [TestClass]
    public class Parser
    {
        [TestMethod]
        [ExpectedException(typeof(InvalidDataException), "Sentinel sign must be used with end frame or another sentinel sign.")]
        public void ShouldThrowExceptionBecauseOfBadData()
        {
            var parser = new HighFlyers.Protocol.Parser();
            parser.AppendBytes(new byte[] { HighFlyers.Protocol.Parser.Sentinel, 99 });
        }

        [TestMethod]
        public void UsingSentinelAndEndFrameAsAData()
        {
            var parser = new HighFlyers.Protocol.Parser();
            parser.AppendBytes(new byte[]
            {
                HighFlyers.Protocol.Parser.Sentinel, HighFlyers.Protocol.Parser.Sentinel, 85, 34,
                HighFlyers.Protocol.Parser.Sentinel, HighFlyers.Protocol.Parser.EndFrame, 37, 64,
                HighFlyers.Protocol.Parser.EndFrame
            });
        }
    }
}
