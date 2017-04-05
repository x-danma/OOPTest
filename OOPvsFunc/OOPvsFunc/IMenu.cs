using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public interface IMenu
    {
        OOPvsFunc.Action GetAction();
        void Show();
    }
}