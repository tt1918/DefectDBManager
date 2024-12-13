using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Coss.Controls
{
    public class LRValue : IDisposable
    {
        private IntPtr handle;
        bool disposed = false;

        public LRValue()
        {
            handle = CreateDKLine();
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!this.disposed)
            {
                if (disposing)
                {
                }
                DeleteDKLine(handle);
                this.disposed = true;
            }
        }
        ~LRValue()
        {
            this.Dispose(false);
        }

        public void Find(int nData, int nSlope, IntPtr fmMap, int pitch, int nMaxY, int nFrame, int nX1, 
                            int nX2, ref int pValue, ref int pValueRel, ref int pPos)
        {
            CDKLine_Find(handle, nData, nSlope, fmMap, pitch, nMaxY, nFrame, nX1,
                            nX2, ref pValue, ref pValueRel, ref pPos);
        }

        [DllImport("LRValue.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static IntPtr CreateDKLine();

        [DllImport("LRValue.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static void DeleteDKLine(IntPtr handle);

        [DllImport("LRValue.dll", CallingConvention = CallingConvention.Cdecl)]
        extern private static void CDKLine_Find(IntPtr handle, int nData, int nSlope, IntPtr fmMap, int pitch, 
            int nMaxY, int nFrame, int nX1, int nX2, ref int pValue, ref int pValueRel, ref int pPos);
    }
}
