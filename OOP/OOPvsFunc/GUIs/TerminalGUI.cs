using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class TerminalGUI : IGUI
    {
        protected virtual string ShowPeoplePrefix { get; set; } = " - ";
        public void ShowPeople(ReadOnlyCollection<Person> people)
        {
            Console.WriteLine("Your current people in your register are:");
            foreach (var person in people)
            {
                Console.WriteLine($"{ShowPeoplePrefix}{person.FirstName} {person.LastName} {person.Age} {person.Ssn}");
            }
        }

        public virtual void ShowMenu()
        {
            Console.WriteLine(@"Hello welcome to the personprogram.
            Enter q to quit
            Enter p to print persons
            Enter a to add person
            Enter d to delete a person by ssn
            Enter f to read people from file
            Enter m to switch menu  
            Enter s to search for people"
            );
        }

        public virtual IAction GetAction()
        {

            string input = Console.ReadLine();

            switch (input)
            {
                case "q": return new QuitAction();
                case "p": return new PrintAction();
                case "a":
                    return new AddAction(new Person(GetStringInput("Enter firstname"), GetStringInput("Enter last name"), GetIntInput("Enter age"), new SocialNumber(GetStringInput("Enter Social security number"))));
                case "d":
                    return new DeleteAction(new SocialNumber(GetStringInput("Enter social security number")));
                case "m":
                    return new SwitchMenuAction(new SwitchMenuGUI());
                case "f":
                    return new ReadFromFileAction(GetStringInput("Enter filename"));
                case "s":
                    return new FindAction(GetStringInput("Please enter firstname"));
                default:
                    Console.WriteLine("Please enter a correct input\n----------------------\n");
                    throw new FormatException();
            }
        }

        private string GetStringInput(string v)
        {
            Console.WriteLine(v);
            return Console.ReadLine();
        }

        private int GetIntInput(string v)
        {
            int output;
            while (true)
            {
                Console.WriteLine(v);
                if (Int32.TryParse(Console.ReadLine(), out output))
                    return output;
                Console.WriteLine("Please enter a number\n----------------------\n");
            }
        }
    }
}