using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using HighFlyers.Protocol.Generator.Types;

namespace HighFlyers.Protocol.Generator
{
    internal class CodeGenerator
    {
        enum CurrentType
        {
            None,
            Structure,
            Enumeration
        }

        readonly List<ObjectType> objectsTypes = new List<ObjectType>();
        private string[] data;
        private readonly List<string[]> currentCollector = new List<string[]>();
        private readonly string outputFileName;
        private readonly string inputFileName;
        private CurrentType currType = CurrentType.None;
        private bool wasStartBracket;
        readonly StringBuilder builder = new StringBuilder();
        private string currentName;

        public CodeGenerator(string inputFileName, string outputFileName)
        {
            this.inputFileName = inputFileName;
            this.outputFileName = outputFileName;
        }

        public void Generate()
        {
            builder.Clear();

            ReadFromFile();
            PrepareData();

            GenerateHeaders();
            GenerateObjectTypes();
            GenerateBottom();

            SaveToFile();
        }

        private void ReadFromFile()
        {
            data = System.IO.File.ReadAllLines(inputFileName);
        }

        private void SaveToFile()
        {
            var file = new System.IO.StreamWriter(outputFileName);
            file.WriteLine(builder.ToString());
            file.Close();
        }

        private void GenerateHeaders()
        {
            builder.AppendLine("namespace HighFlyers.Protocol.Frames");
            builder.AppendLine("{");
        }

        private void GenerateBottom()
        {
            builder.AppendLine("}");
        }

        private void GenerateObjectTypes()
        {
            foreach (var line in objectsTypes.SelectMany(objType => objType.GenerateClass()))
            {
                builder.Append("\t");
                builder.AppendLine(line);
            }
        }

        private void PrepareData()
        {
            foreach (
                string[] words in
                    data.Select(
                        line => System.Text.RegularExpressions.Regex.Replace(line.Trim(), @"\s+", " ").Split(' '))
                        .Where(words => words.Length != 0))
            {
                if (words.Length == 2 && (words[0] == "struct" || words[0] == "enum"))
                {
                    if (currType != CurrentType.None)
                        throw new Exception("Unexpected keyword 'struct' usage!");

                    currentName = words[1];
                    currentCollector.Clear();
                    currType = (words[0] == "struct" ? CurrentType.Structure : CurrentType.Enumeration);
                }
                else if (words.Length == 1 && words[0] == "{")
                {
                    if (wasStartBracket)
                        throw new Exception("Unexpected '{' token");

                    wasStartBracket = true;
                }
                else if (words.Length == 1 && words[0] == "}")
                {
                    if (!wasStartBracket || currType == CurrentType.None)
                        throw new Exception("Unexpected '}' token");

                    switch (currType)
                    {
                        case CurrentType.Structure:
                            objectsTypes.Add(new Structure(currentName, currentCollector.ToArray()));
                            break;
                        case CurrentType.Enumeration:
                            objectsTypes.Add(new Enumeration(currentName, currentCollector.ToArray()));
                            break;
                    }

                    wasStartBracket = false;
                    currType = CurrentType.None;
                }
                else if (!wasStartBracket)
                    throw new Exception("Unexpected value " + words[0]);
                else
                    currentCollector.Add(words);
            }
        }
    }
}
