using System;
using System.Collections.Generic;
using System.Text;

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
            return "class " + Name + " : Frame\n\t{";
        }

        protected override IEnumerable<string> GenerateBody()
        {
            yield return GenerateParser();
            yield return GenerateFieldCount();

            foreach (var words in Input)
            {
                if (words.Length != 2)
                    throw new Exception("Expected two words in line!");

                var builder = new StringBuilder("public ");
                builder.Append(string.Join(" ", words));

                if (words[0].EndsWith("?"))
                    builder.Append(" = null");
                
                builder.Append(";");

                // todo check types (words[0])
                yield return builder.ToString();
            }
        }

        private string GenerateFieldCount()
        {
            return "protected override int FieldCount " +
                   "{ get { return " + Input.Length + "; } }";
        }

        private string GenerateParser()
        {
            var builder = new StringBuilder("public override void Parse(List<byte> bytes)\n");

            builder.AppendLine("\t\t{");
            builder.AppendLine("\t\t\tbyte[] data = bytes.ToArray();");
            builder.AppendLine("\t\t\tbool[] fields = PreParseData(data);");
            builder.AppendLine("\t\t\tint iterator = 0;");

            int i = 0;
            foreach (var line in Input)
            {
                builder.AppendLine("\t\t\tif (fields[" + i++ + "])");
                builder.AppendLine("\t\t\t{");
                builder.AppendLine("\t\t\t\t" + line[1] + " = Utils.ConvertFromBytes(data, iterator + 5)");
                builder.AppendLine("\t\t\t\titerator += Utils.GetSize(" + line[1].Trim() +
                                   (line[0].EndsWith("?") ? ".GetValueOrDefault()" : "") + ");");
                builder.AppendLine("\t\t\t}");
            }

            builder.AppendLine("\t\t\tCheckCrcSum(data[iterator]);");
            builder.AppendLine("\t\t}");

            return builder.ToString();
        }
    }
}
