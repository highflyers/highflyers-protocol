using NUnit.Framework;
using HighFlyers.Protocol.Frames;

namespace HighFlyers.Test.Protocol
{
	[TestFixture()]
	public class SerializeTest
	{
		[Test()]
		public void SimpleSerialization ()
		{
			var ts = new TestStruct ();
			ts.Field1 = 32;
			ts.Field3 = 2;
			ts.Field4 = 108;

			byte[] data = ts.FullSerialize ();
		}
	}
}

