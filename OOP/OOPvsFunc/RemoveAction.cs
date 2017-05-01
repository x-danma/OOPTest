using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class DeleteAction : PersonAction
    {
        public DeleteAction(SocialNumber socialNumber)
        {
            this.ssn = socialNumber;
        }
        public override void Execute(Program program)
        {
            program.Register.DeletePerson(ssn);
        }
    }
}