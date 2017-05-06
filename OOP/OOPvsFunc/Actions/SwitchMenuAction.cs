using System;

namespace OOPvsFunc
{
    internal class SwitchMenuAction : IAction
    {
        public void Execute(Program program)
        {
            if (program.Gui is PrettyTerminalGUI)
                program.Gui = new TerminalGUI();
            else
                program.Gui = new PrettyTerminalGUI();
        }
    }
}