using System;
using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.CGenerator.Types
{
    internal class Enumeration : ObjectType
    {
        public Enumeration(string name, string[][] input)
            : base(name, input)
        {
        }

        protected override IEnumerable<string> GenerateBody()
        {
            yield return "enum " + Name + "\n\t{  ";
            foreach (var words in Input)
            {
                if (words.Length != 1)
                    throw new Exception("Expected one word in line!");

                yield return words[0] + ",";
            }
        }
    }
}