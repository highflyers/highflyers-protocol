using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace HighFlyers.Protocol.Generator
{
    internal class CodeGenerator
    {
        private readonly string builderFileName;
        private readonly List<CGenerator.Types.ObjectType> cObjectsTypes;
        private readonly List<CSharpGenerator.Types.ObjectType> cSharpObjectsTypes;
        private readonly List<string[]> currentCollector = new List<string[]>();
        private readonly string framesFileName;
        private readonly string inputFileName;
        private readonly Languages selectedLanguage;
        private CurrentType currType = CurrentType.None;

        private string currentName;
        private string[] data;
        private byte idCounter;
        private bool wasStartBracket;

        public CodeGenerator(string inputFileName, string framesFileName, string builderFileName, string language)
        {
            this.inputFileName = inputFileName;
            this.framesFileName = framesFileName;
            this.builderFileName = builderFileName;

            switch (language)
            {
                case "c":
                    selectedLanguage = Languages.C;
                    cObjectsTypes = new List<CGenerator.Types.ObjectType>();
                    break;
                case "c#":
                    selectedLanguage = Languages.CSharp;
                    cSharpObjectsTypes = new List<CSharpGenerator.Types.ObjectType>();
                    break;
                default:
                    throw new Exception("Unsupported language");
            }
        }

        public void Generate()
        {
            ReadFromFile();
            PrepareData();

            switch (selectedLanguage)
            {
                case Languages.CSharp:
                    SaveToFile(builderFileName, new CSharpGenerator.FrameBuilderGenerator().GenerateCode(cSharpObjectsTypes));
                    SaveToFile(framesFileName, new CSharpGenerator.FramesGenerator().GenerateCode(cSharpObjectsTypes));
                    break;
                case Languages.C:
                    SaveToFile(builderFileName + ".h", new CGenerator.FrameBuilderGeneratorHeader().GenerateCode(cObjectsTypes));
                    SaveToFile(framesFileName + ".h", new CGenerator.FramesGeneratorHeader().GenerateCode(cObjectsTypes));
                    SaveToFile(builderFileName + ".c",
                        new CGenerator.FrameBuilderGenerator().GenerateCode(cObjectsTypes));
                    SaveToFile(framesFileName + ".c", new CGenerator.FramesGenerator().GenerateCode(cObjectsTypes));
                    break;
            }
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

            switch (selectedLanguage)
            {
                case Languages.CSharp:
                    switch (currType)
                    {
                        case CurrentType.Structure:
                            cSharpObjectsTypes.Add(new CSharpGenerator.Types.Structure(currentName,
                                currentCollector.ToArray(), id));
                            break;
                        case CurrentType.Enumeration:
                            cSharpObjectsTypes.Add(new CSharpGenerator.Types.Enumeration(currentName,
                                currentCollector.ToArray()));
                            break;
                    }
                    break;
                case Languages.C:
                    switch (currType)
                    {
                        case CurrentType.Structure:
                            cObjectsTypes.Add(new CGenerator.Types.Structure(currentName,
                                currentCollector.ToArray(), id));
                            break;
                        case CurrentType.Enumeration:
                            cObjectsTypes.Add(new CGenerator.Types.Enumeration(currentName,
                                currentCollector.ToArray()));
                            break;
                    }
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

        private enum Languages
        {
            C,
            CSharp
        }
    }
}