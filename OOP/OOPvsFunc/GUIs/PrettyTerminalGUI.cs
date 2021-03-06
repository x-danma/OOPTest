﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class PrettyTerminalGUI : TerminalGUI
    {
        protected override string ShowPeoplePrefix { get; set; } = " <3 ";
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

                <3     Enter f to read file           <3

                <3    Enter s to search for people    <3

                <3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3<3"
            );
        }
    }
}