using System;
using System.Diagnostics;

namespace HighFlyers.Protocol.Generator
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                Console.WriteLine("Usage: " + Process.GetCurrentProcess().ProcessName + " <input hfproto file> <output cs file>");
                return;
            }

            try
            {
                var generator = new CodeGenerator(args[0], args[1]);
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
