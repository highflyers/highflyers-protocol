using System.Collections.Generic;

namespace HighFlyers.Protocol.Generator.Types
{
    abstract class ObjectType
    {
        protected readonly string[][] Input;
        protected readonly string Name;

        protected ObjectType(string name, string[][] input)
        {
            Input = input;
            Name = name;
        }

        protected abstract string GenerateHeader();
        protected abstract IEnumerable<string> GenerateBody();

        private static string GenerateBottom()
        {
            return "}";
        }

        public IEnumerable<string> GenerateClass()
        {
            yield return GenerateHeader();
    
            foreach (var line in GenerateBody())
                yield return "\t" + line;

            yield return GenerateBottom();
        }
    }
}
