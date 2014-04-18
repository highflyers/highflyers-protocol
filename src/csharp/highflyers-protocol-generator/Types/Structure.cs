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

        private IEnumerable<string> GenerateCurrentSize()
        {
            yield return "public override int CurrentSize";
            yield return "{";
            yield return "\tget";
            yield return "\t{";
            yield return "\t\tint size = 0;";
            foreach (var words in Input)
            {
                string line = "\t\t";
                if (words[0].EndsWith("?"))
                    line += "if (" + words[1] + " != null) ";
                line += "size += " + GetSizeMethod(words[0], words[1]) + ";";
                yield return line;
            }
            yield return "\t\treturn size;";
            yield return "\t}";
            yield return "}";
        }

        protected override IEnumerable<string> GenerateBody()
        {
            yield return GenerateParser();
            foreach (var l in GenerateCurrentSize())
                yield return l;
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
                builder.AppendLine("\t\t\t\t" + line[1] + " = " + GetConversionMethod(line[0], line[1]) + ";");
                builder.AppendLine("\t\t\t\titerator += " + GetSizeMethod(line[0], line[1]) + ";");
                builder.AppendLine("\t\t\t}");
                if (!line[0].EndsWith("?"))
                    builder.AppendLine("\t\t\telse throw new Exception(\"" +
                                       "field "+line[1]+" must be enabled! It's not " +
                                       "an optional value!\");");
            }

            builder.AppendLine("\t\t}");

            return builder.ToString();
        }

        private readonly string[] nativeTypes =
        {
            "byte", "UInt16", "UInt32", "UInt64", "Int16", "Int32", "Int64", "Double",
            "Single"
        };
        
        string GetConversionMethod(string type, string name)
		{
			if (type.EndsWith ("?"))
				type = type.Remove (type.Length - 1);
            
			int index = Array.FindIndex (nativeTypes, t => t.IndexOf (type, StringComparison.InvariantCultureIgnoreCase) != -1);

			if (index == 0)
				return "data[iterator + 2]";
			if (index != -1)
				return "BitConverter.To" + nativeTypes [index] + "(data, iterator + 2)";

			return "new " + type + "(); " + name +
				".Parse(data.ToList().GetRange(iterator + 2, data.Length - iterator - 2))";
		}

        string GetSizeMethod(string type, string name)
        {
            if(string.IsNullOrEmpty(type))
                throw new Exception("Invalid empty type name!");

            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = Array.FindIndex(nativeTypes, t => t.IndexOf(type, StringComparison.InvariantCultureIgnoreCase) != -1);
            
            if (index != -1)
                return "sizeof(" + nativeTypes[index] + ")";

            return name + ".CurrentSize";
        }
    }
}
