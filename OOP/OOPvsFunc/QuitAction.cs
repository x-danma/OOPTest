using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class QuitAction : IAction
    {
        public void Execute(Program program)
        {
            program.Quit();
        }
    }
}