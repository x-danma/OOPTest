using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOPvsFunc
{
    public class Program
    {

        private IGUI _gui = new TerminalGUI();
        private PersonRegister _register = new PersonRegister();
        bool keepAlive = true;

        public PersonRegister Register
        {
            get
            {
                return _register;
            }

            set
            {
                _register = value;
            }
        }

        public IGUI Gui
        {
            get
            {
                return _gui;
            }

            set
            {
                _gui = value;
            }
        }

        static void Main(string[] args)
        {
            Program currentProgram = new Program();
            currentProgram.Run();


            //Vad göra? ett program som kanske tittar på en lista av objekt och visar upp dem i en tabell? CRUD på det?
            // Eller kanske ett kortspel? med randomisering för Dice-throws?

            /*Programmet ska hantera ett personregister
             * programmet ska en meny för 
             *  - Visa listan
             *  - lägga till person i listan
             *  - ta bort person i listan
             *  - avsluta programmet 
             * 
             * För listan kan man 
             *  - Lägga till
             *  - Ta bort
             * en person
             * 
             * En person har
             *  förnamn
             *  efternamn
             *  ålder
             *  ssn
             * 
             * 
             * 
             * */

        }

        public void Run()
        {
            while (keepAlive)
            {
                Gui.ShowMenu();
                try
                {
                    IAction action = Gui.GetAction();
                    action.Execute(this);
                }
                catch (Exception)
                {
                }
            }
        }

        public void Quit()
        {
            keepAlive = false;
        }
    }
}
