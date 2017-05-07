using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class SimpleMenuGUI : TerminalGUI
    {
        public override void ShowMenu()
        {
            Console.WriteLine(@"
            Enter m to switch menu"
            );
        }
        public override IAction GetAction()
        {
            string input = Console.ReadLine();

            switch (input)
            {
                case "m":
                    return new SwitchMenuAction(new SwitchMenuGUI());
                default:
                    Console.WriteLine("Please enter a correct input\n----------------------\n");
                    throw new FormatException();
            }
        }
    }
}