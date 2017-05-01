using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class Person
    {
        public string FirstName { get; }
        public string LastName { get; }
        public int Age { get; }
        public SocialNumber Ssn { get; }

        public Person(string firstname, string lastname, int age, SocialNumber ssn)
        {
            FirstName = firstname;
            LastName = lastname;
            Age = age;
            Ssn = ssn;
        }
    }
}