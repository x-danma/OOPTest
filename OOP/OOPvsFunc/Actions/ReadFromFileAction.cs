namespace OOPvsFunc
{
    public sealed class ReadFromFileAction : Action
    {
        public ReadFromFileAction(String filename)
        {
            this.person = person;
        }

        public override void Execute(Program program)
        {
            program.Register.AddPerson(person);
        }
    }
}