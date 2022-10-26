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
        public long Current { get; set; }
        public long Total { get; set; }
        public bool IsSkip { get; set; }
        public int Step { get; set; }
        public int MaxStep { get; set; }
        public double Progress 
        { 
            get
            {
                return ((double)Current / (double)Total) * 100.0;
            } 
        }

        public DbProgress()
        {
            Reset();
        }

        public void Reset()
        {
            Current = 0;
            Total = 0;
            IsSkip = false;
            Step = 0;
            MaxStep = 1;
        }

        public bool IsComplete()
        {
            if (IsSkip == true) return true;
            if (Total == 0) return false;
            //if (Total == Current) return true;  //PTRYLP에서 reader.RowSize가 실제 읽는 row개수와 다름.. 우선은 막고 테스트

            return true;
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

        public eNittoDBProgress ErrorStep { get; private set;  }

        public NittoDBProgress()
        {
            int count = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;

            _Progress = new DbProgress[count];

            for(int i=0; i<count; i++)
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

        public void SetMatStep(eNittoDBProgress index, int val)
        {
            _Progress[(int)index].MaxStep = val;
        }

        public void SetTotal(eNittoDBProgress index, long total, int step=1)
        {
            _Progress[(int)index].Step = step;
            _Progress[(int)index].Total = total;
        }
        public void AddCount(eNittoDBProgress index)
        {
            _Progress[(int)index].Current++;
        }
        public void SetCount(eNittoDBProgress index, long count)
        {
            _Progress[(int)index].Current = count;
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
        }

        public bool IsCompelete(eNittoDBProgress index)
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
