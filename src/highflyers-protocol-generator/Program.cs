using System;
using System.Diagnostics;

namespace HighFlyers.Protocol.Generator
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Usage: " + Process.GetCurrentProcess().ProcessName +
                                  " <input hfproto file> <output cs file> <output builder file>");
                return;
            }

            try
            {
                var generator = new CodeGenerator(args[0], args[1], args[2]);
                generator.Generate();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Cannot generate cs file: " + ex.Message);
            }

            Console.WriteLine("Press [Enter] ...");
            Console.ReadLine();
        }
    }
}