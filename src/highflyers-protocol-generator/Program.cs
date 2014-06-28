using System;
using System.Diagnostics;

namespace HighFlyers.Protocol.Generator
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("Usage: " + Process.GetCurrentProcess().ProcessName +
                                  " <input hfproto file> <output frames file> <output frame builder file> --language=C/C#");
                return;
            }

            string language = "C#";
            if (args.Length == 4) language = args[3].Replace("--language=", "");

            try
            {
                var generator = new CodeGenerator(args[0], args[1], args[2], language.ToLower());
                generator.Generate();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Cannot generate files: " + ex.Message);
            }

            Console.WriteLine("Press [Enter] ...");
            Console.ReadLine();
        }
    }
}