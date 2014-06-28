using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.CGenerator.Types;

namespace HighFlyers.Protocol.Generator.CGenerator
{
    internal class FramesGeneratorHeader
    {
        private readonly StringBuilder builder = new StringBuilder();
        private List<ObjectType> objectsTypes;

        public string GenerateCode(List<ObjectType> types)
        {
            builder.Clear();
            objectsTypes = types;

            GenerateHeaders();
            GenerateObjectTypes();
            GenerateBottom();

            return builder.ToString();
        }

        private void GenerateHeaders()
        {
            builder.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            builder.AppendLine("#ifndef HIGHFLYERS_PROTOCOL_FRAMES_H");
            builder.AppendLine("#define HIGHFLYERS_PROTOCOL_FRAMES_H");

            builder.AppendLine("#include \"types.h\"");

            builder.AppendLine("void frame_preparse_data (const byte* data, bool* output, int field_count);");
            builder.AppendLine("void frame_finalise(const byte* data, int size, bool* output);");
        }

        private void GenerateBottom()
        {
            builder.AppendLine("#endif");
        }

        private void GenerateObjectTypes()
        {
            builder.AppendLine("typedef enum");
            builder.AppendLine("{");

            Structure[] structures = objectsTypes.OfType<Structure>().ToArray();

            for (int i = 0; i < structures.Count(); i++)
            {
                builder.Append("\tT_" + structures.ElementAt(i).Name);
                if (i < (structures.Count() - 1))
                    builder.Append(",");
                builder.AppendLine();
            }

            builder.AppendLine("} FrameTypes;");

            foreach (Structure type in objectsTypes.OfType<Structure>()) builder.AppendLine(type.GenerateHeader());
        }
    }
}