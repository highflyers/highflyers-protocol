using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.Types;

namespace HighFlyers.Protocol.Generator
{
    class FramesGenerator
    {
        private readonly StringBuilder builder = new StringBuilder();
        private List<ObjectType> objectsTypes;

        public string GenerateCode(List<ObjectType> types)
        {
            builder.Clear();
            objectsTypes = types;

            GenerateHeaders();
            GenerateFrameTypesEnum();
            GenerateObjectTypes();
            GenerateBottom();

            return builder.ToString();
        }

        private void GenerateHeaders()
        {
            builder.AppendLine("// GENERATED CODE! DON'T MODIFY IT!");
            builder.AppendLine("using System;");
            builder.AppendLine("using System.Collections.Generic;");
            builder.AppendLine("using System.Linq;");
            builder.AppendLine("namespace HighFlyers.Protocol.Frames");
            builder.AppendLine("{");
        }

        private void GenerateBottom()
        {
            builder.AppendLine("}");
        }

        private void GenerateFrameTypesEnum()
        {
            builder.AppendLine("\tenum FrameTypes");
            builder.AppendLine("\t{");

            foreach (Structure objType in objectsTypes.OfType<Structure>())
                builder.AppendLine("\t\t" + objType.Name + ",");

            builder.AppendLine("\t}");
        }

        private void GenerateObjectTypes()
        {
            foreach (var line in objectsTypes.SelectMany(objType => objType.GenerateClass()))
            {
                builder.Append("\t");
                builder.AppendLine(line);
            }
        }
    }
}
