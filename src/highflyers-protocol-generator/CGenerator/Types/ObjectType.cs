using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.CGenerator.Types
{
    internal abstract class ObjectType
    {
        protected readonly string[][] Input;

        protected ObjectType(string name, string[][] input)
        {
            Input = input;
            Name = name;
        }

        public string Name { get; private set; }

        protected abstract IEnumerable<string> GenerateBody();

        public IEnumerable<string> GenerateClass()
        {
            foreach (string line in GenerateBody())
                yield return line;
        }
    }
}