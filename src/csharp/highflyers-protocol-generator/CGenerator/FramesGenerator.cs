using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.CGenerator.Types;

namespace HighFlyers.Protocol.Generator.CGenerator
{
    internal class FramesGenerator
    {
        private readonly StringBuilder builder = new StringBuilder();
        private List<ObjectType> objectsTypes;

        public string GenerateCode(List<ObjectType> types)
        {
            builder.Clear();
            objectsTypes = types;

            GenerateHeaders();
            GenerateGeneralFunctions();
            GenerateObjectTypes();

            return builder.ToString();
        }

        private void GenerateHeaders()
        {
            builder.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            builder.AppendLine("#include \"frames.h\"");
            builder.AppendLine("#include \"frame_parser_helper.h\"");
            builder.AppendLine("#include <stdlib.h>");
        }

        private void GenerateGeneralFunctions()
        {
            builder.AppendLine("void frame_preparse_data (const byte* data, bool* output, int field_count)");
            builder.AppendLine("{");
            builder.AppendLine("\tuint16 field_flags = frame_parser_helper_to_uint16 (data, 0);");
            builder.AppendLine("\tint i;");

            builder.AppendLine("\tfor (i = 0; i < field_count; i++)");
            builder.AppendLine("\t\toutput[i] = (field_flags & (1 << i)) != 0;");
            builder.AppendLine("}");

            builder.AppendLine("void frame_finalise(const byte* data, int size, bool* output)");
            builder.AppendLine("{");
            builder.AppendLine("\tint bytesAdded = 0;");
            builder.AppendLine("\tint i;");

            builder.AppendLine("\tfor (i = 0; i < size; i++)");
            builder.AppendLine("\t{");
            builder.AppendLine("\t\tif ((data[i] == FRAMEPARSER_HELPER_SENTINEL)");
            builder.AppendLine("\t\t\t\t|| (data[i] == FRAMEPARSER_HELPER_ENDFRAME))");
            builder.AppendLine("\t\t{");
            builder.AppendLine("\t\t\tframe_parser_helper_set_byte (output + i + bytesAdded,");
            builder.AppendLine("\t\t\tFRAMEPARSER_HELPER_SENTINEL);");
            builder.AppendLine("\t\t\tbytesAdded++;");
            builder.AppendLine("\t\t}");

            builder.AppendLine("\t\tframe_parser_helper_set_byte (output + i + bytesAdded, data[i]);");
            builder.AppendLine("\t}");

            builder.AppendLine("\tframe_parser_helper_set_byte (output + size + bytesAdded,");
            builder.AppendLine("\tFRAMEPARSER_HELPER_ENDFRAME);");
            builder.AppendLine("}");
        }

        private void GenerateObjectTypes()
        {
            foreach (string line in objectsTypes.SelectMany(objType => objType.GenerateClass()))
            {
                builder.AppendLine(line);
            }
        }
    }
}