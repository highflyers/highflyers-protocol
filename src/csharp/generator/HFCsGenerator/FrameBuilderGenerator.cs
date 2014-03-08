using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.Types;

namespace HighFlyers.Protocol.Generator
{
    internal class FrameBuilderGenerator
    {
        private StringBuilder lines;
        List<ObjectType> types;
        private readonly string fileName;

        public FrameBuilderGenerator(string fileName)
        {
            this.fileName = fileName;
        }

        public void GenerateBuilder(List<ObjectType> definedTypes)
        {
            lines = new StringBuilder();
            types = definedTypes;

            GenerateHeader();
            GenerateBuildMethod();
            GenerateBottom();

            SaveToFile();
        }

        private void GenerateHeader()
        {
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

        private void SaveToFile()
        {
            var file = new StreamWriter(fileName);
            file.WriteLine(lines.ToString());
            file.Close();
        }
    }
}

