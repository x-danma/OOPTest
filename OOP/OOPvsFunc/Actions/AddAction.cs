using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public sealed class AddAction : PersonAction
    {
        private Person person;

        public AddAction(Person person)
        {
            this.person = person;
        }

        public override void Execute(Program program)
        {
            program.Register.AddPerson(person);
        }
    }
}