using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

using Cognex.VisionPro; // Cognex.VisionPro.dll Cognex.VisionPro.Core.dll
using Cognex.VisionPro.ID; // Cognex.VisionPro.ID.dll
using Cognex.VisionPro.Implementation.Internal; // CogWaitCursor Cognex.VisionPro.Controls.dll
using Cognex.VisionPro.ImageProcessing;
using Cognex.VisionPro.ToolGroup;

using OpenCvSharp;
using OpenCvSharp.Extensions;
using System.Diagnostics;
using System.Windows.Forms;
using System.IO;

namespace CodeReaderDLL.CognexLib
{
    internal class CognexCodeReader : IDisposable
    {
        private CogIDTool idTool = null;
        private CogToolGroup toolGroup = null;
        private CogIPOneImageTool imageTool = null;
        private ICogImage cogImage = null;
        
        public bool InitComplete { get; private set; } = false;
        public string LastErrorMessage { get; private set; } = string.Empty;
        public string ReadMatrixCode { get; private set; } = string.Empty;
        public string CodeReadElapsedTime { get; private set; } = string.Empty;
        public CognexCodeReader(string vppFilePath)
        {
            idTool = new CogIDTool();
            idTool.RunParams.DisableAllCodes();
            idTool.RunParams.DataMatrix.Enabled = true;
            idTool.RunParams.DataMatrix.ProcessControlMetrics = CogIDDataMatrixProcessControlMetricsConstants.None;
            idTool.RunParams.NumToFind = 1;
            idTool.RunParams.Timeout = 50;
            idTool.RunParams.TimeoutEnabled = true;

            string groupFile = vppFilePath;
            if (groupFile != string.Empty)
            {
                if (File.Exists(groupFile))
                {
                    toolGroup = CogSerializer.LoadObjectFromFile(groupFile) as CogToolGroup;
                    imageTool = toolGroup.Tools["CogIPOneImageTool1"] as CogIPOneImageTool;

                    InitComplete = true;
                }
                else
                    InitComplete = false;
            }

            InitComplete = true;
        }

        public bool PutCogImage8Grey(Mat image)
        {
            var bitmap = BitmapConverter.ToBitmap(image);
            var aImage = new CogImage8Grey(bitmap);

            if (idTool != null)
            {
                cogImage = aImage;
                return true;
            }

            return false;
        }

        public bool CodeRead(bool isPreprocess)
        {
            Stopwatch sw = new Stopwatch();
            sw.Start();
            using (new CogWaitCursor())
            {
                if (isPreprocess)
                {
                    // image preprocess
                    imageTool.InputImage = cogImage;
                    imageTool.Run();
                    ICogRunStatus imageToolStatus = imageTool.RunStatus;
                    if (imageToolStatus.Result == CogToolResultConstants.Error)
                    {
                        LastErrorMessage = imageToolStatus.Message;
                        ReadMatrixCode = string.Empty;
                        sw.Stop();
                        CodeReadElapsedTime = sw.ElapsedMilliseconds.ToString();
                        return false;
                    }
                    idTool.InputImage = imageTool.OutputImage;
                }
                else
                {
                    idTool.InputImage = cogImage;
                }
                // Run the CogIDTool
                idTool.Run();

                ICogRunStatus runStatus = idTool.RunStatus;
                if (runStatus.Result == CogToolResultConstants.Error)
                {
                    LastErrorMessage = runStatus.Message;
                    ReadMatrixCode = string.Empty;
                    sw.Stop();
                    CodeReadElapsedTime = sw.ElapsedMilliseconds.ToString();
                    return false;
                }
                if (idTool.Results.Count < 1)
                {
                    LastErrorMessage = "Symbol not found.";
                    ReadMatrixCode= string.Empty;
                    sw.Stop();
                    CodeReadElapsedTime = sw.ElapsedMilliseconds.ToString();
                    return false;
                }

                CogIDResultDecoded rd = idTool.Results[0].DecodedData;
                if (rd == null)
                {
                    LastErrorMessage = "Symbol not decoded.";
                    ReadMatrixCode = string.Empty;
                    sw.Stop();
                    CodeReadElapsedTime = sw.ElapsedMilliseconds.ToString();
                    return false;
                }

                LastErrorMessage = String.Empty;
                ReadMatrixCode = rd.DecodedString;
                sw.Stop();
                CodeReadElapsedTime = sw.ElapsedMilliseconds.ToString();

                return true;
            }
        }
        #region Disposable
        private bool disposing = false;
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

            }
        }
        #endregion
    }
}
