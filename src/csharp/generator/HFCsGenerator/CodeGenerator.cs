using System;
using System.Collections.Generic;
using System.Linq;
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
        private readonly string framesFileName;
        private readonly string inputFileName;
        private readonly string builderFileName;
        private CurrentType currType = CurrentType.None;
        private bool wasStartBracket;
        
        private string currentName;

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
            data = System.IO.File.ReadAllLines(inputFileName);
        }

        private void SaveToFile(string fileName, string content)
        {
            var file = new System.IO.StreamWriter(fileName);
            file.WriteLine(content);
            file.Close();
        }

        private void PrepareData()
        {
            foreach (
                string[] words in
                    data.Select(
                        line => System.Text.RegularExpressions.Regex.Replace(line.Trim(), @"\s+", " ").Split(' '))
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
                    AddNewObjectType();
                }
                else if (!wasStartBracket)
                    throw new Exception("Unexpected value " + words[0]);
                else
                    currentCollector.Add(words);
            }
        }

        void AddNewObjectType()
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
    }
}
