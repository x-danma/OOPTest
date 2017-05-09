using System;
using System.IO;



namespace OOPvsFunc
{
    internal class ReadFromFileAction : IAction
    {
        private string File { get; set; }

        public ReadFromFileAction(string file)
        {
            this.File = file;
        }

        public void Execute(Program program)
        {
            System.Diagnostics.Stopwatch myStopWatch = new System.Diagnostics.Stopwatch();
            myStopWatch.Start();
            try
            {
                using (StreamReader sr = new StreamReader(this.File))
                {
                    string line;
                    while ((line = sr.ReadLine()) != null)
                    {
                        string[] linearray = line.Split('\t');
                        Person newPerson = new OOPvsFunc.Person(linearray[0], linearray[1], Int32.Parse(linearray[2]), new SocialNumber(linearray[3]));
                        program.Register.AddPerson(newPerson);
                    }
                }
                myStopWatch.Stop();
                Console.WriteLine($"Time to read complete file: {myStopWatch.ElapsedMilliseconds} ms"); 
            }
            
            catch (Exception e)
            {
                Console.WriteLine($"Failed to read file, error = {e.Message}");
                throw e;
            }
        }
    }
}
