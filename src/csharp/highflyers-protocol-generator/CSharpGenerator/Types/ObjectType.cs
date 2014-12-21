using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.CSharpGenerator.Types
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

        protected abstract string GenerateHeader();
        protected abstract IEnumerable<string> GenerateBody();

        private static string GenerateBottom()
        {
            return "}";
        }

        public IEnumerable<string> GenerateClass()
        {
            yield return GenerateHeader();

            foreach (string line in GenerateBody())
                yield return "\t" + line;

            yield return GenerateBottom();
        }
    }
}