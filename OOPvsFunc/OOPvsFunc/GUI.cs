using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class GUI
    {
        public void ShowPeople()
        {
            throw new System.NotImplementedException();
        }

        public void ShowMenu()
        {
            Console.WriteLine(@"Hello welcome to the personprogram.
            Enter q to quit
            Enter p to print persons
            Enter a to add person
            Enter d to delete a person by ssn"
            );
        }

        public IAction GetAction()
        {
           string input = Console.ReadLine();

            switch (input)
            {
                case "q": return new QuitAction();
                case "p": return new PrintAction();
                case "a": return new AddAction();
                case "d": return new DeleteAction();
                default:
                    break;
            }
            throw new FormatException();

        }
    }
}