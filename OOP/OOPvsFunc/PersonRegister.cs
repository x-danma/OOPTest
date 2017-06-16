using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class PersonRegister
    {
        private List<Person> people = new List<Person>();

        public ReadOnlyCollection<Person> People
        {
            get
            {
                return people.AsReadOnly();
            }


        }

        internal void DeletePerson(SocialNumber ssn)
        {
            people.RemoveAt(people.FindIndex(x => x.Ssn.Equals(ssn)));
        }

        public void AddPerson(Person newPerson)
        {
            people.Add(newPerson);
        }

        public List<Person> FindPerson(string substr)
        {
            return people.FindAll(x => x.FirstName.IndexOf(substr) != -1);
        }
    }
}