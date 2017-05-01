using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OOPvsFunc
{
    public class SocialNumber
    {
        public string Ssn { get; private set; }

        public SocialNumber(string ssn)
        {
            Ssn = ssn;
        }

        public override string ToString()
        {
            return Ssn;
        }

        public override bool Equals(object obj)
        {
            var other = obj as SocialNumber;
            return other.Ssn == this.Ssn;
        }
        public override int GetHashCode()
        {
            return this.Ssn.GetHashCode();
        }
    }
}