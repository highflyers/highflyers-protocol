using System;
using System.Collections.Generic;
using System.Text;

namespace HighFlyers.Protocol.Generator.CGenerator.Types
{
    internal class Structure : ObjectType
    {
        private readonly string[,] nativeTypes =
        {
            {
                "byte",
                "byte"
            },
            {
                "UInt16",
                "uint16"
            },
            {
                "UInt32",
                "uint32"
            },
            {
                "UInt64",
                "-"
            },
            {
                "Int16",
                "-"
            },
            {
                "Int32",
                "int32"
            },
            {
                "Int64",
                "-"
            }
            ,
            {
                "Double",
                "double"
            }
            ,
            {
                "Single",
                "-"
            }
        };

        public Structure(string name, string[][] input, byte id)
            : base(name, input)
        {
            ID = id;
        }

        public byte ID { get; set; }

        private IEnumerable<string> GenerateCurrentSize()
        {
            yield return "int " + Name + "_current_size (const " + Name + "* value)";
            yield return "{";
            yield return "\tint size = 2; //enable flags";

            foreach (var words in Input)
            {
                string line = "\t";
                if (words[0].EndsWith("?"))
                    line += "if (value->" + words[1] + "_enabled) ";
                line += "size += " + GetSizeMethod(words[0], words[1]) + ";";
                yield return line;
            }

            yield return "\treturn size;";
            yield return "}";
        }

        private IEnumerable<string> GenerateSerialize()
        {
            yield return "void " + Name + "_serialize (const " + Name + "* value, byte* output)";
            yield return "{";
            yield return "\tint frame_size = 1 + " + Name + "_current_size (value) + sizeof(uint32); // struct + crc32";

            yield return "\tbyte* data = (byte*)malloc (frame_size);";

            yield return "\tdata[0] = " + ID + ";";

            yield return "\tint iterator = " + Name + "_preserialize(value, data + 1);";

            yield return "\tuint32 crc = frame_parser_helper_calculate_crc (data, iterator + 1);";

            yield return "\tframe_parser_helper_set_uint32 (data + 1 + iterator, crc);";

            yield return "\tframe_finalise(data, frame_size, output);";
            yield return "\tfree(data);";
            yield return "}";
        }

        private IEnumerable<string> GeneratePreSerialize()
        {
            yield return "int " + Name + "_preserialize (const " + Name + "* value, byte* output)";
            yield return "{";
            yield return "\tuint16 must_be = 0;";
            yield return "\tint iterator = 2;";

            int i = 0;
            foreach (var words in Input)
            {
                if (words[0].EndsWith("?"))
                {
                    yield return "\tif (value->" + words[1] + "_enabled)";
                    yield return "\t{";
                }

                yield return "\tmust_be = (uint16)(must_be | (1 << " + i + "));";
                string type = words[0];
                if (type.EndsWith("?"))
                    type = type.Remove(type.Length - 1);

                int index = GetNativeTypeIndex(type);
                if (index == -1)
                {
                    yield return "{";
                    yield return
                        "\tbyte* output_f = (byte*)malloc (" + type + "_current_size (&value->" + words[1] + "));";
                    yield return "\tint size_f = " + type + "_preserialize(&value->" + words[1] + ", output_f);";
                    yield return "\tmemcpy(output + iterator, output_f, size_f);";
                    yield return "\tfree(output_f);";
                    yield return "\titerator += size_f;";
                    yield return "}";
                }
                else
                {
                    yield return
                        "frame_parser_helper_set_" + nativeTypes[index, 1] + "(output + iterator, value->" + words[1] +
                        ");";
                    yield return "\titerator += " + GetSizeMethod(words[0], words[1]) + ";";
                }

                if (words[0].EndsWith("?")) yield return "\t}";
                i++;
            }

            yield return "\tframe_parser_helper_set_uint16 (output, must_be);";

            yield return "\treturn iterator;";
            yield return "}";
        }

        protected override IEnumerable<string> GenerateBody()
        {
            yield return GenerateParser();

            foreach (string l in GenerateCurrentSize())
                yield return l;

            foreach (string l in GeneratePreSerialize())
                yield return l;

            foreach (string l in GenerateSerialize())
                yield return l;
        }

        public string GenerateHeader()
        {
            var builder = new StringBuilder();

            builder.AppendLine("typedef struct");
            builder.AppendLine("{");
            foreach (var line in Input)
            {
                bool optional = line[0].EndsWith("?");
                string type = line[0];
                if (optional)
                    type = line[0].Remove(line[0].Length - 1);

                int nativeIndex = GetNativeTypeIndex(type);
                if (nativeIndex != -1)
                    type = nativeTypes[nativeIndex, 1];

                builder.AppendLine("\t" + type + " " + line[1] + ";");
                if (optional)
                {
                    builder.AppendLine("\tbool " + line[1] + "_enabled;");
                }
            }

            builder.AppendLine("} " + Name + ";");


            builder.AppendLine(Name + "* " + Name + "_parse (const byte* data);");
            builder.AppendLine("int " + Name + "_current_size (const " + Name + "* value);");
            builder.AppendLine("int " + Name + "_preserialize (const " + Name + "* value, byte* output);");
            builder.AppendLine("void " + Name + "_serialize (const " + Name + "* value, byte* output);");

            return builder.ToString();
        }

        private string GenerateParser()
        {
            var builder = new StringBuilder();

            builder.AppendLine(Name + "* " + Name + "_parse (const byte* data)");
            builder.AppendLine("{");
            builder.AppendLine("\tbool fields[" + Input.Length + "];");
            builder.AppendLine("\tint iterator = 2;");
            builder.AppendLine("\t" + Name + "* value;");

            builder.AppendLine("\tvalue = (" + Name + "*)malloc(sizeof(" + Name + "));");
            builder.AppendLine("\tframe_preparse_data(data, fields, " + Input.Length + ");");


            int i = 0;
            foreach (var line in Input)
            {
                bool optional = line[0].EndsWith("?");
                string type = line[0];
                if (optional)
                    type = line[0].Remove(line[0].Length - 1);

                builder.AppendLine("\tif (fields[" + i++ + "])");
                builder.AppendLine("\t{");

                if (GetNativeTypeIndex(line[0]) == -1)
                {
                    builder.AppendLine("\t\t" + type + "* str = " + type + "_parse(data + iterator);");
                    builder.AppendLine("\t\tvalue->" + line[1] + " = *str;");
                    builder.AppendLine("\t\tfree(str);");
                }
                else
                    builder.AppendLine("\t\tvalue->" + line[1] + " = " + GetConversionMethod(line[0], line[1]) + ";");

                builder.AppendLine("\t\titerator += " + GetSizeMethod(line[0], line[1]) + ";");
                if (optional)
                {
                    builder.AppendLine("\t\tvalue->" + line[1] + "_enabled = true;");
                    builder.AppendLine("\t}");
                    builder.AppendLine("\telse value->" + line[1] + "_enabled = false;");
                }
                else
                {
                    builder.AppendLine("\t}");
                    builder.AppendLine("\telse");
                    builder.AppendLine("\t{");
                    builder.AppendLine("\t\tfree (value);");
                    builder.AppendLine("\t\treturn NULL;");
                    builder.AppendLine("\t}");
                }
            }

            builder.AppendLine("\treturn value;");
            builder.AppendLine("}");

            return builder.ToString();
        }

        private string GetConversionMethod(string type, string name)
        {
            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = GetNativeTypeIndex(type);

            if (index == 0)
                return "data[iterator]";

            return "frame_parser_helper_to_" + nativeTypes[index, 1] + "(data, iterator)";
        }

        private int GetNativeTypeIndex(string type)
        {
            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = -1;
            for (int i = 0; i < (nativeTypes.Length / nativeTypes.Rank); i++)
            {
                int result = StringComparer.InvariantCultureIgnoreCase.Compare(nativeTypes[i, 0], type);
                if (result == 0)
                {
                    index = i;
                    break;
                }
            }
            return index;
        }

        private string GetSizeMethod(string type, string name)
        {
            if (string.IsNullOrEmpty(type))
                throw new Exception("Invalid empty type name!");

            if (type.EndsWith("?"))
                type = type.Remove(type.Length - 1);

            int index = GetNativeTypeIndex(type);

            if (index != -1)
                return "sizeof(" + nativeTypes[index, 1] + ")";

            return type + "_current_size(&value->" + name + ")";
        }
    }
}