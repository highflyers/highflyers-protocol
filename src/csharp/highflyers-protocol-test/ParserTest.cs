using NUnit.Framework;

namespace HighFlyers.Test.Protocol
{
	[TestFixture]
	public class Parser
	{
		[Test]
		[ExpectedException(typeof(System.IO.InvalidDataException))]
		public void ShouldThrowExceptionBecauseOfBadData()
		{
			var parser = new HighFlyers.Protocol.Parser();
			parser.AppendBytes(new byte[] { HighFlyers.Protocol.Parser.Sentinel, 99 });
		}

		[Test]
		public void UsingSentinelAndEndFrameAsAData()
		{
			var parser = new HighFlyers.Protocol.Parser ();
			parser.AppendBytes (new byte[] {
				HighFlyers.Protocol.Parser.Sentinel, HighFlyers.Protocol.Parser.Sentinel, 11, 0,
				HighFlyers.Protocol.Parser.Sentinel, HighFlyers.Protocol.Parser.EndFrame, 37, 64,
				57, 0, 0, 0, HighFlyers.Protocol.Parser.EndFrame
			});
		}
	}
}
