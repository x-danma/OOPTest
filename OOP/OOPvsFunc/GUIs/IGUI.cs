using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public interface IGUI
    {
        void ShowMenu();
        IAction GetAction();
        void ShowPeople(ReadOnlyCollection<Person> people);
    }
}
