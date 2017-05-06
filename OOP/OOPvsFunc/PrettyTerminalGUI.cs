using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class PrettyTerminalGUI : TerminalGUI
    {
        public override void ShowMenu()
        {
            Console.WriteLine(@"
                <3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3

                <3 Hello welcome to the personprogram.<3

                <3          Enter q to quit           <3

                <3      Enter p to print persons      <3
            
                <3        Enter a to add person       <3

                <3  Enter d to delete a person by ssn <3

                <3     Enter m to switch menu         <3

                <3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3"
            );
        }
    }
}