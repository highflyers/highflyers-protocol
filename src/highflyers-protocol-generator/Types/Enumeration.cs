using System;
using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.Types
{
    internal class Enumeration : ObjectType
    {
        public Enumeration(string name, string[][] input)
            : base(name, input)
        {
        }

        protected override string GenerateHeader()
        {
            return "enum " + Name + "\n\t{  ";
        }

        protected override IEnumerable<string> GenerateBody()
        {
            foreach (var words in Input)
            {
                if (words.Length != 1)
                    throw new Exception("Expected one word in line!");

                yield return words[0] + ",";
            }
        }
    }
}