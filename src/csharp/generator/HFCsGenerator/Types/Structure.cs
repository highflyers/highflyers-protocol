using System;
using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.Types
{
    class Structure : ObjectType
    {
        public Structure(string name, string[][] input)
            : base(name, input)
        {
        }

        protected override string GenerateHeader()
        {
            return "struct " + Name;
        }

        protected override IEnumerable<string> GenerateBody()
        {
            foreach (var words in Input)
            {
                if (words.Length != 2)
                    throw new Exception("Expected two words in line!");

                // todo check types (words[0])
                yield return string.Join(" ", words) + ";";
            }
        }
    }
}
