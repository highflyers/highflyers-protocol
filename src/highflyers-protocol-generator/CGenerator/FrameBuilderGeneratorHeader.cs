using System.Collections.Generic;
using System.Text;
using HighFlyers.Protocol.Generator.CGenerator.Types;

namespace HighFlyers.Protocol.Generator.CGenerator
{
    internal class FrameBuilderGeneratorHeader
    {
        private readonly StringBuilder lines = new StringBuilder();

        public string GenerateCode(List<ObjectType> definedTypes)
        {
            lines.Clear();

            GenerateHeader();
            GenerateBuildMethod();
            GenerateBottom();

            return lines.ToString();
        }

        private void GenerateHeader()
        {
            lines.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            lines.AppendLine("#ifndef HIGHFLYERS_PROTOCOL_FRAME_BUILDER_H");
            lines.AppendLine("#define HIGHFLYERS_PROTOCOL_FRAME_BUILDER_H");
            lines.AppendLine("#include \"parser.h\"");
        }

        private void GenerateBottom()
        {
            lines.AppendLine("#endif");
        }

        private void GenerateBuildMethod()
        {
            lines.AppendLine("FrameProxy frame_builder_build_frame (byte* bytes, int size);");
        }
    }
}