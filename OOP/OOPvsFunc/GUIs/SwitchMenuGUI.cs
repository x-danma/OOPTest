using System;
using System.Collections.ObjectModel;

namespace OOPvsFunc
{
    public class SwitchMenuGUI : IGUI
    {
        public IAction GetAction()
        {
            string input = Console.ReadLine();

            switch (input)
            {
                case "n":
                    return new SwitchMenuAction(new TerminalGUI());
                case "p":
                    return new SwitchMenuAction(new PrettyTerminalGUI());
                case "s":
                    return new SwitchMenuAction(new SimpleMenuGUI());
                default:
                    Console.WriteLine("Please enter a correct input\n----------------------\n");
                    throw new FormatException();
            }
        }

        public void ShowMenu()
        {
            Console.WriteLine(@"Hello welcome to the personprogram.
            Enter n for Normal Menu
            Enter p for Pretty Menu
            Enter s for Simple Menu
            ");
        }

        public void ShowPeople(ReadOnlyCollection<Person> people)
        {
            throw new InvalidOperationException();
        }
    }
}