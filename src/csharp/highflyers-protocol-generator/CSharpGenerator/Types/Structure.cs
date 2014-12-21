using System;
using System.Collections.Generic;
using System.Text;

namespace HighFlyers.Protocol.Generator.CSharpGenerator.Types
{
    internal class Structure : ObjectType
    {
        private readonly string[] nativeTypes =
        {
            "byte", "UInt16", "UInt32", "UInt64", "Int16", "Int32", "Int64", "Double",
            "Single"
        };

        private byte id;

        public Structure(string name, string[][] input, byte id)
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
            yield return "\t\tint size = 2; // enabled fields information";
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

        private IEnumerable<string> GenerateSerialize()
        {
            yield return "public override List<byte> Serialize ()";
            yield return "{";
            yield return "\tushort mustBe = 0;";
            yield return "\tvar bytes = new List<byte> ();";
            yield return "\tbytes.Add(" + id + ");";
            int i = 0;
            foreach (var words in Input)
            {
                yield return GetSerializeMethod(words[0], words[1], i++);
            }
            yield return "var mb = BitConverter.GetBytes (mustBe); bytes.Insert (1, mb [0]);bytes.Insert (2, mb [1]);";
            yield return "\treturn bytes;";
            yield return "}";
        }

        protected override IEnumerable<string> GenerateBody()
        {
            yield return GenerateParser();
            foreach (string l in GenerateCurrentSize())
                yield return l;
            yield return GenerateFieldCount();

            foreach (string l in GenerateSerialize())
                yield return l;

            foreach (var words in Input)
            {
                if (words.Length != 2)
                    throw new Exception("Expected two words in line!");

                var builder = new StringBuilder("public ");

                if (words[0].EndsWith("?"))
                {
                    string nw = words[0].Remove(words[0].Length - 1);
                    if (GetNativeTypeIndex(nw) == -1)
                        words[0] = nw;
                }

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
            var builder =
                new StringBuilder(
                    "public override void Parse(List<byte> bytes, FrameParserHelper.EndianType endianes)\n");

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
                                       "field " + line[1] + " must be enabled! It's not " +
                                       "an optional value!\");");
            }

            builder.AppendLine("\t\t}");

            return builder.ToString();
        }

        private int GetNativeTypeIndex(string type)
        {
            return Array.FindIndex(nativeTypes,
                t => t.IndexOf(type, StringComparison.InvariantCultureIgnoreCase) != -1);
        }

        private string GetConversionMethod(string type, string name)
        {
            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = GetNativeTypeIndex(type);

            if (index == 0)
                return "data[iterator + 2]";
            if (index != -1)
                return "BitConverter.To" + nativeTypes[index] + "(data, iterator + 2)";

            return "new " + type + "(); " + name +
                   ".Parse(data.ToList().GetRange(iterator + 2, data.Length - iterator - 2), endianes)";
        }

        private string GetSizeMethod(string type, string name)
        {
            if (string.IsNullOrEmpty(type))
                throw new Exception("Invalid empty type name!");

            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = Array.FindIndex(nativeTypes,
                t => t.IndexOf(type, StringComparison.InvariantCultureIgnoreCase) != -1);

            if (index != -1)
                return "sizeof(" + nativeTypes[index] + ")";

            return name + ".CurrentSize";
        }

        private string GetSerializeMethod(string type, string name, int number)
        {
            if (string.IsNullOrEmpty(type))
                throw new Exception("Invalid empty type name!");

            var builder = new StringBuilder();
            bool optional = false;
            if (type.EndsWith("?"))
            {
                builder.Append("\tif (" + name + " != null) { ");
                optional = true;
                type = type.Remove(type.Length - 1);
            }

            builder.Append("mustBe = (ushort)(mustBe | (1 << " + number + "));");
            builder.Append("bytes.AddRange (");
            int index = Array.FindIndex(nativeTypes,
                t => t.IndexOf(type, StringComparison.InvariantCultureIgnoreCase) != -1);

            if (index != -1)
            {
                if (type != "byte")
                    builder.Append("BitConverter.GetBytes(" + name + (optional ? ".GetValueOrDefault()" : "") + ")");
                else
                    builder.Append("new byte[]{" + name + (optional ? ".GetValueOrDefault()" : "") + "}");
            }
            else
                builder.Append(name + ".Serialize()");
            builder.Append(");\n");

            if (optional)
                builder.Append("}");

            return builder.ToString();
        }
    }
}