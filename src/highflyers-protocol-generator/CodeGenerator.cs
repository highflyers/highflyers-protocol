using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using HighFlyers.Protocol.Generator.Types;

namespace HighFlyers.Protocol.Generator
{
    internal class CodeGenerator
    {
        private readonly string builderFileName;
        private readonly List<string[]> currentCollector = new List<string[]>();
        private readonly string framesFileName;
        private readonly string inputFileName;
        private readonly List<ObjectType> objectsTypes = new List<ObjectType>();
        private CurrentType currType = CurrentType.None;

        private string currentName;
        private string[] data;
        private byte idCounter;
        private bool wasStartBracket;

        public CodeGenerator(string inputFileName, string framesFileName, string builderFileName)
        {
            this.inputFileName = inputFileName;
            this.framesFileName = framesFileName;
            this.builderFileName = builderFileName;
        }

        public void Generate()
        {
            ReadFromFile();
            PrepareData();

            SaveToFile(builderFileName, new FrameBuilderGenerator().GenerateCode(objectsTypes));
            SaveToFile(framesFileName, new FramesGenerator().GenerateCode(objectsTypes));
        }

        private void ReadFromFile()
        {
            data = File.ReadAllLines(inputFileName);
        }

        private void SaveToFile(string fileName, string content)
        {
            var file = new StreamWriter(fileName);
            file.WriteLine(content);
            file.Close();
        }

        private void PrepareData()
        {
            foreach (
                var words in
                    data.Select(
                        line => Regex.Replace(line.Trim(), @"\s+", " ").Split(' '))
                        .Where(words => words.Length != 0 && !string.IsNullOrEmpty(words[0])))
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
                    AddNewObjectType(idCounter++);
                }
                else if (!wasStartBracket)
                    throw new Exception("Unexpected value " + words[0]);
                else
                    currentCollector.Add(words);
            }
        }

        private void AddNewObjectType(byte id)
        {
            if (!wasStartBracket || currType == CurrentType.None)
                throw new Exception("Unexpected '}' token");

            switch (currType)
            {
                case CurrentType.Structure:
                    objectsTypes.Add(new Structure(currentName, currentCollector.ToArray(), id));
                    break;
                case CurrentType.Enumeration:
                    objectsTypes.Add(new Enumeration(currentName, currentCollector.ToArray()));
                    break;
            }

            wasStartBracket = false;
            currType = CurrentType.None;
        }

        private enum CurrentType
        {
            None,
            Structure,
            Enumeration
        }
    }
}