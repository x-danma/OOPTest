using System;

namespace OOPvsFunc
{
    internal class SwitchMenuAction : IAction
    {
        public IGUI Gui { get; private set; }

        public void Execute(Program program)
        {
            program.Gui = this.Gui;
        }
        public SwitchMenuAction(IGUI gui)
        {
            this.Gui = gui;
        }
    }
}