﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class PrintAction : IAction
    {
        public void Execute(Program program)
        {
            program.Gui.ShowPeople(program.Register.People);
        }
    }

   
}