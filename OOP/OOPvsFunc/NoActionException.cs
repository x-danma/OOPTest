using System;
using System.Runtime.Serialization;

namespace OOPvsFunc
{
    [Serializable]
    internal class NoActionException : Exception
    {
        public NoActionException()
        {
        }

        public NoActionException(string message) : base(message)
        {
        }

        public NoActionException(string message, Exception innerException) : base(message, innerException)
        {
        }

        protected NoActionException(SerializationInfo info, StreamingContext context) : base(info, context)
        {
        }
    }
}