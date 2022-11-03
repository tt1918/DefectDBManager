using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class DbProgress
    {
        public bool IsSkip 
        {
            get { return isSkip; } 
            set
            {
                isSkip = value;
                if (isSkip == true)
                    State = eProcessState.Complete;
            }
        }
        private bool isSkip;

        public eProcessState State { get; private set; }
        
        public DbProgress()
        {
            Reset();
        }

        public void Reset()
        {
            IsSkip = false;
            State = eProcessState.Ready;
        }

        public void Set()
        {
            State = eProcessState.Run;
        }

        public void Error()
        {
            State = eProcessState.Error;
        }

        public void Complete()
        {
            State = eProcessState.Complete;
        }

        public bool IsComplete()
        {
            if (IsSkip == true) return true;
            if(State == eProcessState.Complete) return true;
            return false;
        }
    }

    public class NittoDBProgress
    {
        public DbProgress[] _Progress;

        public bool IsComplete
        {
            get
            {
                return checkComplete();
            }

            private set
            {
                isComplete = value;
            }
        }
        private bool isComplete;

        public bool IsError { get; private set; }

        public eNittoDBProgress ErrorStep { get; private set; }

        public NittoDBProgress()
        {
            int count = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;

            _Progress = new DbProgress[count];

            for (int i = 0; i < count; i++)
            {
                _Progress[i] = new DbProgress();
            }

            ResetAll();
        }

        ~NittoDBProgress()
        {

        }

        public void ResetAll()
        {
            for (int i = 0; i < _Progress.Length; i++)
                _Progress[i].Reset();

            IsComplete = false;
            IsError = false;
            ErrorStep = eNittoDBProgress.PTRYLP;
        }

        public void Reset(eNittoDBProgress index)
        {
            _Progress[(int)index].Reset();
        }

        public void Set(eNittoDBProgress index)
        {
            _Progress[(int)index].Set();
        }

        public void Complete(eNittoDBProgress index)
        {
            _Progress[(int)index].Complete();
        }

        public void SetSkip(eNittoDBProgress index)
        {
            _Progress[(int)index].IsSkip = true;
        }

        public void SetError(eNittoDBProgress errStep)
        {
            IsComplete = true;
            IsError = true;
            ErrorStep = errStep;
            _Progress[(int)errStep].Error();
        }

        public bool CheckComplete(eNittoDBProgress index)
        {
            return _Progress[(int)index].IsComplete();
        }
        private bool checkComplete()
        {
            isComplete = true;
            for (int i = 0; i < _Progress.Length; i++)
                isComplete &= _Progress[i].IsComplete();

            return isComplete;
        }
    }
}
