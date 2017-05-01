using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public abstract class PersonAction : IAction
    {
        protected SocialNumber ssn;
        public abstract void Execute(Program program);
    }
}