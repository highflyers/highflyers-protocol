using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.CGenerator.Types;

namespace HighFlyers.Protocol.Generator.CGenerator
{
    internal class FrameBuilderGenerator
    {
        private readonly StringBuilder lines = new StringBuilder();
        private List<ObjectType> types;

        public string GenerateCode(List<ObjectType> definedTypes)
        {
            lines.Clear();
            types = definedTypes;

            GenerateHeader();
            GenerateBuildMethod();

            return lines.ToString();
        }

        private void GenerateHeader()
        {
            lines.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            lines.AppendLine("#include \"frame_builder.h\"");
        }

        private void GenerateBuildMethod()
        {
            lines.AppendLine("FrameProxy frame_builder_build_frame (byte* bytes, int size)");
            lines.AppendLine("{");
            lines.AppendLine("\tFrameProxy proxy;");
            lines.AppendLine("\tframe_proxy_initialize(&proxy);");

            lines.AppendLine("\tif (!frame_parser_helper_check_bytes (bytes, size))");
            lines.AppendLine("\t\treturn proxy;");

            lines.AppendLine("\tbyte endianes = bytes [0] & 128;");
            lines.AppendLine("\tFrameTypes frame_type = (FrameTypes)(bytes [0] & 127);");
            lines.AppendLine("\tvoid* frame;");

            lines.AppendLine("\tproxy.type = frame_type;");
            lines.AppendLine("\tswitch (frame_type)");
            lines.AppendLine("\t{");

            byte i = 0;
            foreach (Structure type in types.OfType<Structure>())
            {
                type.ID = i;
                lines.AppendLine("\tcase T_" + type.Name + ":");
                lines.AppendLine("\t\tproxy.pointer = (void*)" + type.Name + "_parse (bytes + 1);");
                lines.AppendLine("\t\tbreak;");
                i++;
            }
            lines.AppendLine("\tdefault:");
            lines.AppendLine("\t\tbreak;");
            lines.AppendLine("\t}");

            lines.AppendLine("\treturn proxy;");
            lines.AppendLine("}");
        }
    }
}