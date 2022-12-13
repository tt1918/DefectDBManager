using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using OpenCvSharp;
using CodeReaderDLL.MatroxLib;
using CodeReaderDLL.CognexLib;
using System.Threading;
using System.Drawing.Imaging;
using System.Drawing;
using System.Diagnostics;

namespace CodeReaderDLL
{
    [Guid("399A4D99-2241-405C-BC96-3297FAB63404")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ICallCodeReader
    {
        bool Initialize(bool isMIL, int sizeX, int sizeY, string path);
        bool Terminate();
        string CodeRead(IntPtr img, int width, int height, bool usePreprocess);
        IntPtr GetCodePosition();
    }

    [Guid("ED558B56-012F-494D-AA98-588D0775327A")]
    public class CodeReader : ICallCodeReader
    {
        private bool isMil = false;
        private MilCodeReader milCodeReader = null; // dot spacing 0
        private CognexCodeReader cognexCodeReader = null;

        public string CodeRead(IntPtr img, int width, int height, bool usePreprocess)
        {
            string strReturn = string.Empty;
            if (isMil)
            {
                using (Mat bcrImg = new Mat(height, width, MatType.CV_8UC1, img))
                {
                    if (milCodeReader.InitComplete)
                    {
                        milCodeReader.PutMilImageBuffer(bcrImg);
                        if (milCodeReader.CodeRead())
                        {
                            strReturn = milCodeReader.ReadMatrixCode;
                        }
                    }
                }
            }
            else
            {
                using (Mat bcrImg = new Mat(height, width, MatType.CV_8UC1, img))
                {
                    if (cognexCodeReader.PutCogImage8Grey(bcrImg))
                    {
                        if (cognexCodeReader.CodeRead(usePreprocess))
                        {
                            strReturn = cognexCodeReader.ReadMatrixCode;
                        }
                    }
                }
            }

            return strReturn;
        }

        public IntPtr GetCodePosition()
        {
            if (isMil)
            {
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(milCodeReader.LastCodeRect));
                Marshal.StructureToPtr(milCodeReader.LastCodeRect, ptr, false);
                return ptr;
            }
            else
            {
                var ptr = Marshal.AllocHGlobal(Marshal.SizeOf(cognexCodeReader.LastCodeRect));
                Marshal.StructureToPtr(cognexCodeReader.LastCodeRect, ptr, false);
                return ptr;
            }
        }

        public bool Initialize(bool isMIL, int sizeX, int sizeY, string path)
        {
            isMil = isMIL;
            if (isMil) // MIL Code Reader 사용
            {
                milCodeReader = new MilCodeReader(sizeX, sizeY, path, 0);
                if (milCodeReader.InitComplete)
                    return true;
                else
                    return false;
            }
            else // Cognex vision pro Code Reader 사용    
            {
                cognexCodeReader = new CognexCodeReader(path);
                return cognexCodeReader.InitComplete;
            }
        }

        public bool Terminate()
        {
            if (isMil)
            {
                milCodeReader?.Dispose();
            }
            else
            {
                cognexCodeReader?.Dispose();
            }
            return true;
        }
    }
}
