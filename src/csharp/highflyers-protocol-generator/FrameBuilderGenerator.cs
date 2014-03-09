using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.Types;

namespace HighFlyers.Protocol.Generator
{
    internal class FrameBuilderGenerator
    {
        private readonly StringBuilder lines = new StringBuilder();
        List<ObjectType> types;
        
        public string GenerateCode(List<ObjectType> definedTypes)
        {
            lines.Clear();
            types = definedTypes;

            GenerateHeader();
            GenerateBuildMethod();
            GenerateBottom();

            return lines.ToString();
        }

        private void GenerateHeader()
        {
            lines.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            lines.AppendLine("using System.Collections.Generic;");
            lines.AppendLine("using System.IO;");
            lines.AppendLine("using HighFlyers.Protocol.Frames;");
            lines.AppendLine("namespace HighFlyers.Protocol");
            lines.AppendLine("{");
            lines.AppendLine("\tpartial class FrameBuilder");
            lines.AppendLine("\t{");
        }

        private void GenerateBottom()
        {
            lines.AppendLine("\t}");
            lines.AppendLine("}");
        }

        private void GenerateBuildMethod()
        {
            lines.AppendLine("\t\tpublic static Frame BuildFrame(List<byte> bytes)");
            lines.AppendLine("\t\t{");
            lines.AppendLine("\t\t\tCheckBytes(bytes);");
            lines.AppendLine("\t\t\tFrame frame;");
            lines.AppendLine("\t\t\tswitch ((FrameTypes) bytes[0])");
            lines.AppendLine("\t\t\t{");
            foreach (Structure type in types.OfType<Structure>())
            {
                lines.AppendLine("\t\t\t\tcase FrameTypes." + type.Name + ":");
                lines.AppendLine("\t\t\t\t\tframe = new " + type.Name + "(); break;");
            }
            lines.AppendLine("\t\t\t\tdefault: throw new InvalidDataException(\"Unexpected frame type\");");
            lines.AppendLine("\t\t\t}");
            lines.AppendLine("\t\t\tframe.Parse(bytes.GetRange(3, bytes.Count - 3));");
            lines.AppendLine("\t\t\treturn frame;");
            lines.AppendLine("\t\t}");
        }
    }
}

