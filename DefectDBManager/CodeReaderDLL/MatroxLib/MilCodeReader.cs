using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Matrox.MatroxImagingLibrary;
using OpenCvSharp;

namespace CodeReaderDLL.MatroxLib
{
    internal class MilCodeReader : IDisposable
    {
        private string mcoFilePath = @"C:\Coss\distortion.mco";
        private bool disposing = false;
        private MIL_ID milApplication = MIL.M_NULL;
        private MIL_ID milSystem = MIL.M_NULL;
        private MIL_ID milCodeContextIdPtr = MIL.M_NULL;
        private MIL_ID milCodeModel = MIL.M_NULL;
        private MIL_ID milImage = MIL.M_NULL;
        private MIL_ID milBuffer = MIL.M_NULL;
        private IntPtr milBufferPtr = IntPtr.Zero;
        private MIL_ID milCodeResult = MIL.M_NULL;
        private MIL_INT numberOfCodes = MIL.M_ZERO;
        private int sizeX;
        private int sizeY;

        public bool InitComplete { get; private set; } = false;
        public MIL_ID MilApplication { get { return milApplication; } }
        public MIL_ID MilSystem { get { return milSystem; } }
        public MIL_ID MilCodeContextIdPtr { get { return milCodeContextIdPtr; } }
        public MIL_ID MilCodeModel { get { return milCodeModel; } }
        public string ReadMatrixCode { get; private set; } = string.Empty;

        public MilCodeReader(int sizex, int sizey)
        {
            this.sizeX = sizex;
            this.sizeY = sizey;
            MIL.MappControl(MIL.M_ERROR, MIL.M_PRINT_DISABLE);
            if (MIL.MappAlloc(MIL.M_DEFAULT, ref milApplication) == 0) return;
            if (MIL.MsysAlloc(MIL.M_DEFAULT, MIL.M_SYSTEM_HOST, MIL.M_DEFAULT, MIL.M_DEFAULT, ref milSystem) == 0) return;
            if (MIL.McodeAlloc(MilSystem, MIL.M_DEFAULT, MIL.M_DEFAULT, ref milCodeContextIdPtr) == 0) return;
            if (MIL.McodeModel(MilCodeContextIdPtr, MIL.M_ADD, MIL.M_DATAMATRIX, MIL.M_NULL, MIL.M_DEFAULT, ref milCodeModel) == 0) return;
            if (MIL.MbufAlloc2d(MilSystem, sizex, sizey, 8 + MIL.M_UNSIGNED, MIL.M_IMAGE, ref milBuffer) == 0) return;

            // CodeModel Option Change
            if (File.Exists(mcoFilePath))
            {
                if (milCodeContextIdPtr != MIL.M_NULL)
                    MIL.McodeRestore(mcoFilePath, MilSystem, MIL.M_DEFAULT, ref milCodeContextIdPtr);
            }
            else
            {
                if (milCodeContextIdPtr != MIL.M_NULL)
                {
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_ENCODING, MIL.M_ENC_ASCII);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_ERROR_CORRECTION, MIL.M_ECC_200);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_FOREGROUND_VALUE, MIL.M_FOREGROUND_BLACK);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_THRESHOLD, MIL.M_ADAPTIVE);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_CELL_NUMBER_X, 12);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_CELL_NUMBER_Y, 26);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_SPEED, MIL.M_MEDIUM);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_SEARCH_ANGLE_DELTA_NEG, 5);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_SEARCH_ANGLE_DELTA_POS, 5);
                    MIL.McodeControl(MilCodeContextIdPtr, MIL.M_TIMEOUT, 50);
                }
            }

            milBufferPtr = MIL.MbufInquire(milBuffer, MIL.M_HOST_ADDRESS, MIL.M_NULL);
            MIL.MbufClear(milBuffer, 0xff);
            MIL.McodeControl(MilCodeModel, MIL.M_NUMBER, 1);
            InitComplete = true;
        }

        public bool PutMilImageBuffer(Mat image)
        {
            byte[] buffer = image.ToBytes();
            MIL.MbufPut2d(milImage, 0, 0, this.sizeX, this.sizeY, buffer);

            return true;
        }

        public bool CodeRead()
        {
            MIL.McodeRead(MilCodeContextIdPtr, milImage, milCodeResult);
            MIL.McodeGetResult(milCodeResult, MIL.M_NUMBER + MIL.M_TYPE_MIL_INT, ref numberOfCodes);

            MIL_INT codeIndex = 0;
            for (codeIndex = 0; codeIndex < numberOfCodes; codeIndex++)
            {
                double posX = 0, posY = 0, sizeX = 0,  sizeY = 0;
                StringBuilder decodedString = new StringBuilder();
                MIL.McodeGetResultSingle(milCodeResult, codeIndex, MIL.M_POSITION_X, ref posX);
                MIL.McodeGetResultSingle(milCodeResult, codeIndex, MIL.M_POSITION_Y, ref posY);
                MIL.McodeGetResultSingle(milCodeResult, codeIndex, MIL.M_SIZE_X, ref sizeX);
                MIL.McodeGetResultSingle(milCodeResult, codeIndex, MIL.M_SIZE_Y, ref sizeY);
                MIL.McodeGetResultSingle(milCodeResult, codeIndex, MIL.M_STRING, decodedString);
                ReadMatrixCode = decodedString.ToString();
            }

            if (numberOfCodes > 0)
                return true;
            else
                return false;
        }


        #region Disposable
        public void Dispose()
        {
           if (!disposing)
            {
                disposing = true;
                Dispose(disposing);
            }
        }

        private void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (milCodeContextIdPtr != MIL.M_NULL) MIL.McodeFree(milCodeContextIdPtr);
                if (milImage != MIL.M_NULL) MIL.MbufFree(milImage);
                if (milSystem != MIL.M_NULL) MIL.MsysFree(milSystem);
                if (milApplication != MIL.M_NULL) MIL.MappFree(milApplication);
            }
        }
        #endregion
    }
}
