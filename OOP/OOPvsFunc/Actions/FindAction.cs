using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOPvsFunc
{

    public class FindAction : IAction
    {
        string substr;
        public FindAction(string substr)
        {
            this.substr = substr;
        }
        public void Execute(Program program)
        {
            program.Gui.ShowPeople(program.Register.FindPerson(substr).AsReadOnly());
        }
    }

}
