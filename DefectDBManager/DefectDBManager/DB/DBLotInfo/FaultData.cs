using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    // 이 부분 다시 정리해야함

    #region FaultDatum
    /// <summary>
    ///  쿼리 탐색 후 결과 데이터 저장
    /// </summary>
    public class FaultDatum
    {
        public string LOTNO;
        public string TLOTNO;
        public string TBCNO;

        public string TCTLNO;
        public string FLTNO;
        public double OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float SIZE_X;
        public float SIZE_Y;
        public float SIZE_AREA;     // csv에서 읽어 온 fault area

        public string FLTID;
        public string RANK;
        public string KND;
        public string JIGCD;
        public string MACNO;
        public int CAM_NO;

        //public string CTLNO_FAULT;   <- 마킹 불량 데이터 쪽으로 가는게 맞아보임 
        //public float XOFFSET_ALARM;  <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public float SIZE_FAULT;    // DB에서 받은 fault area <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public string FAULTNO_FAULT; <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public string MNTTID_FAULT; <- 마킹 불량 데이터 쪽으로 가는게 맞아보임

        public FaultDatum Clone()
        {
            FaultDatum data = new FaultDatum();

            data.LOTNO = LOTNO;
            data.TLOTNO = TLOTNO;
            data.TBCNO = TBCNO;

            data.TCTLNO = TCTLNO;
            data.FLTID = FLTNO;
            data.OFFSET = OFFSET;
            data.YPOS_M = YPOS_M;
            data.XPOS_M = XPOS_M;
            data.SIZE_X = SIZE_X;
            data.SIZE_Y = SIZE_Y;
            data.SIZE_AREA = SIZE_AREA;

            data.FLTID = FLTID;
            data.RANK = RANK;
            data.KND = KND;
            data.JIGCD = JIGCD;
            data.MACNO = MACNO;
            data.CAM_NO = CAM_NO;

            return data;
        }

        public void SetData(string bcno, DB.FLTDATA_DailyData data)
        {
            this.TBCNO = bcno;
            this.FLTNO = data.FLTNO;
            this.OFFSET = data.OFFSET;
            this.YPOS_M = data.YPOS_M;
            this.XPOS_M = data.XPOS_M;

            // fault data 추가
            this.RANK = data.RANK;
            this.KND = data.KND;
            this.JIGCD = data.JIGCD;
            this.MACNO = data.MACNO;
        }

        public void SetData(string bcno, DB.FLTDATAData data)
        {

            this.TBCNO = bcno;
            this.FLTNO = data.FLTNO;
            this.OFFSET = data.OFFSET;
            this.YPOS_M = data.YPOS_M;
            this.XPOS_M = data.XPOS_M;

            // fault data 추가
            this.RANK = data.RANK;
            this.KND = data.KND;
            this.JIGCD = data.JIGCD;
            this.MACNO = data.MACNO;

        }
    }

    public class FltDatumList : ItemList<FaultDatum>
    {
        public void Copy(List<FaultDatum> input)
        {
            _data.Clear();
            foreach (FaultDatum lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(FltDatumList input)
        {
            _data.Clear();
            foreach (FaultDatum lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion FaultDatum

    public class MarkingFaultDatum
    {
        //////////////////////////////////////////////////////////
        // 데이터 전송용
        public string BCNO;
        public string FLTNO;        // DB 탐색에서 얻어오는 불량 순번
        public string FAULTID;      // 불량 항목 같음
        public double OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float XOFFSET;
        public bool UseCSVResult;
        public int DefectLine;
        public int CAM_NO;
        // 데이터 전송용
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // 마킹 시 파라미터 확인용
        public float XOFFSET_ALARM;
        // 표시용 데이터
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // 표시용 데이터
        public string CTLNO;
        public string MACNO;
        public string MNTTID;
        public float SIZE;
        public float SIZE_X;
        public float SIZE_Y;
        // 표시용 데이터
        //////////////////////////////////////////////////////////

        public MarkingFaultDatum Clone()
        {
            MarkingFaultDatum data = new MarkingFaultDatum();

            data.BCNO = BCNO;
            data.FLTNO = FLTNO;
            data.FAULTID = FAULTID;
            data.OFFSET = OFFSET;
            data.YPOS_M = YPOS_M;
            data.XPOS_M = XPOS_M;
            data.XOFFSET = XOFFSET;
            data.UseCSVResult = UseCSVResult;
            data.DefectLine = DefectLine;
            data.CAM_NO = CAM_NO;

            data.XOFFSET_ALARM = XOFFSET_ALARM;

            data.CTLNO = CTLNO;
            data.MACNO = MACNO;
            data.MNTTID = MNTTID;
            data.SIZE = SIZE;
            data.SIZE_X = SIZE_X;
            data.SIZE_Y = SIZE_Y;

            return data;
        }

        public void SetFaultData(eFCD fcd, eCSV_TYPE csvType, string bcno, float offsetX, bool csvRes, FaultDatum fltDat, DB.FLTDATAData data, Option option, ref Param param)
        {
            this.BCNO = bcno;
            this.FLTNO = data.FLTNO;
            this.FAULTID = data.FLTID;
            this.OFFSET = fltDat.OFFSET;
            this.YPOS_M = fltDat.YPOS_M;
            this.XPOS_M = fltDat.XPOS_M;
            this.XOFFSET = offsetX;
            this.UseCSVResult = false;
            this.CAM_NO = data.CAMNO;
            this.CTLNO = data.CTLNO;
            this.SIZE = data.AREA_M;
            this.MNTTID = data.MNTTAN;
            this.MACNO = data.MACNO;

            if (data.CAMNO != 9) this.XOFFSET_ALARM = offsetX;
            else this.XOFFSET_ALARM = float.MaxValue;

            if (csvType == eCSV_TYPE.NITTO)
            {
                if (fcd == eFCD.TG) this.DefectLine = 9; // 점착
                else this.DefectLine = 8; // 그외
            }
            else if (csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.KORENO_RK_IJP)
            {
                if (fcd == eFCD.TG) this.DefectLine = 9; //점착 
                else if (fcd == eFCD.ES) this.DefectLine = 8; // 연신 - 기타
                else this.DefectLine = 7; // 그외
            }
            else
            {
                if (fcd == eFCD.TG && option.useKT == true) // 점착
                {
                    int fldID = Int32.Parse(data.FLTID.Substring(data.FLTID.Length - 2));
                    this.DefectLine = FalutFunction.GetLineFromFLTID(fldID);
                    if (this.DefectLine != 13) param.DBFaultCount[fldID]++;
                }
                else if ((fcd == eFCD.ES && option.checkES == true) ||
                    (fcd == eFCD.ETC && option.checkETC == true))
                {
                    this.DefectLine = 0;
                    param.ESFalutCount++; // 연신 결점 데이터 카운트 처리
                }
            }

            // User Defect Class에 등록된 FLTID는 별도 클래스로 구분
            int defectLine = this.DefectLine;

            if (param._UserDefectClass.UpdateDefectLine(FAULTID.ToUpper(), ref defectLine) == true)
                this.DefectLine = defectLine;

            //RK는 CAMNO별로 Defect Class 를 구분
            if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.KORENO_RK_IJP)
                this.DefectLine += Global.MaxDefectLine * data.CAMNO;
        }


        public void SetFaultData(eFCD fcd, eCSV_TYPE csvType, string bcno, float offsetX, bool csvRes, FaultDatum fltDat, DB.FLTDATA_DailyData data, bool useKT)
        {
            this.BCNO = bcno;
            this.FLTNO = data.FLTNO;
            this.FAULTID = data.FLTID;
            this.OFFSET = fltDat.OFFSET;
            this.YPOS_M = fltDat.YPOS_M;
            this.XPOS_M = fltDat.XPOS_M;
            this.XOFFSET = offsetX;
            this.UseCSVResult = false;
            this.CAM_NO = data.CAMNO;
            this.CTLNO = data.CTLNO;
            this.SIZE = data.AREA_M;
            this.MNTTID = data.MNTTAN;
            this.MACNO = data.MACNO;

            if (data.CAMNO != 9) this.XOFFSET_ALARM = offsetX;
            else this.XOFFSET_ALARM = float.MaxValue;

            if (csvType == eCSV_TYPE.NITTO)
            {
                if (fcd == eFCD.TG) this.DefectLine = 9; // 점착
                else this.DefectLine = 8; // 그외
            }
            else if (csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.KORENO_RK_IJP)
            {
                if (fcd == eFCD.TG) this.DefectLine = 9; //점착 
                else if (fcd == eFCD.ES) this.DefectLine = 8; // 연신 - 기타
                else this.DefectLine = 7; // 그외
            }
            else
            {
                if (fcd == eFCD.TG && useKT == true) // 점착
                {
                    int fldID = Int32.Parse(data.FLTID.Substring(data.FLTID.Length - 2));
                    this.DefectLine = FalutFunction.GetLineFromFLTID(fldID);
                }
                else if (fcd == eFCD.ES || fcd == eFCD.ETC)
                    this.DefectLine = 0;
            }

            // User Defect Class에 등록된 FLTID는 별도 클래스로 구분
            int defectLine = this.DefectLine;

            //RK는 CAMNO별로 Defect Class 를 구분
            if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.KORENO_RK_IJP)
                this.DefectLine += Global.MaxDefectLine * data.CAMNO;
        }
    }

    public class MkFltDatumList : ItemList<MarkingFaultDatum>
    {
        public void Copy(List<MarkingFaultDatum> input)
        {
            _data.Clear();
            foreach (MarkingFaultDatum lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(MkFltDatumList input)
        {
            _data.Clear();
            foreach (MarkingFaultDatum lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }

    #region MrkFltDat
    public class MrkFltDat
    {
        /// <summary>
        /// 10M 기준으로 구분하여 데이터 입력함
        /// </summary>
        public MkFltDatumList Data = null;
        public Dictionary<int, MkFltDatumList> Dic = null;
        public float MinXPos;
        public float MaxXPos;
        public float MinSize;


        public MrkFltDat()
        {
            Data = new MkFltDatumList();
            Dic = new Dictionary<int, MkFltDatumList>();
        }

        public void Reset()
        {
            Data.Clear();
            Dic.Clear();
            MinXPos = 0.0f;
            MaxXPos = 0.0f;
            MinSize = 0.0f;
        }

        public void Add(MarkingFaultDatum val)
        {
            Data.Add(val);
            int key = (int)(val.OFFSET / 10000.0);
            if (Dic.ContainsKey(key) == true)
                Dic[key].Add(val);
            else
            {
                MkFltDatumList list = new MkFltDatumList();
                list.Add(val);
                Dic.Add(key, list);
            }
        }

        public void GetData(string bcno, double start, double end, ref List<MarkingData> data)
        {
            int key1 = (int)(start / 10000.0);
            int key2 = (int)(end / 10000.0);

            for (int i = key1; i <= key2; i++)
            {
                if (Dic.ContainsKey(i) == true)
                {
                    foreach (MarkingFaultDatum datum in Dic[i].Data)
                    {
                        if (datum.OFFSET >= start && datum.OFFSET <= end && datum.BCNO == bcno)
                        {
                            MarkingData item = new MarkingData();
                            item.DefectLine = datum.DefectLine;
                            item.BCNO = datum.BCNO;
                            item.CAM_NO = datum.CAM_NO;
                            item.FAULTID = datum.FAULTID;
                            item.FLTNO = datum.FLTNO;
                            item.OFFSET = datum.OFFSET;
                            item.UseCSVResult = datum.UseCSVResult;
                            item.XOFFSET = datum.XOFFSET;
                            item.XPOS_M = datum.XPOS_M;
                            item.YPOS_M = datum.YPOS_M;
                            item.SIZE_X = datum.SIZE_X;
                            item.SIZE_Y = datum.SIZE_Y;
                            data.Add(item);
                        }
                    }
                }
            }
        }

        public void GetData(string bcno, double start, double end, ref MkFltDatumList data)
        {
            int key1 = (int)(start / 10000.0);
            int key2 = (int)(end / 10000.0);

            for (int i = key1; i <= key2; i++)
            {
                if (Dic.ContainsKey(i) == true)
                {
                    foreach (MarkingFaultDatum datum in Dic[i].Data)
                    {
                        if (datum.OFFSET >= start && datum.OFFSET <= end && datum.BCNO == bcno)
                            data.Add(datum);
                    }
                }
            }
        }

        public void GetData(string bcno, double start, double end, ref List<MarkingFaultDatum> data)
        {
            int key1 = (int)(start / 10000.0);
            int key2 = (int)(end / 10000.0);

            for (int i = key1; i <= key2; i++)
            {
                if (Dic.ContainsKey(i) == true)
                {
                    foreach (MarkingFaultDatum datum in Dic[i].Data)
                    {
                        if (datum.OFFSET >= start && datum.OFFSET <= end && datum.BCNO == bcno)
                            data.Add(datum);
                    }
                }
            }
        }

    }

    #endregion MrkFltDat

    public class PreprocMrkDat
    {
        public string LNCD { get; set; } = "";
        public List<MarkingFaultDatum> Data = null;

        public MarkingFaultDatum this[int idx]
        {
            get { return Data[idx]; }
        }

        public PreprocMrkDat()
        {
            Data = new List<MarkingFaultDatum>();
        }

        public void Reset()
        {
            LNCD = "";
            Data.Clear();
        }

        public PreprocMrkDat Clone()
        {
            PreprocMrkDat data = new PreprocMrkDat();

            data.LNCD = LNCD;

            foreach(var item in Data)
                data.Data.Add(item);

            return data;
        }
    }

    public class ResultData
    {
        public FltDatumList Data;
        public MrkFltDat MarkFault;

        public ResultData()
        {
            Data = new FltDatumList();
            MarkFault = new MrkFltDat();
        }

        ~ResultData()
        {
            ResetAll();
        }

        public void ResetAll()
        {
            Data.Clear();
            MarkFault.Reset();
        }
    }

    public class PreProcResultData
    {
        /// <summary>
        /// FAULTData 저장
        /// 데이터는 각 공정 및 LNCD 기준으로 처리하도록 한다. 
        /// </summary>
        public PreProcDftList[] FLTDAT
        {
            get { return _fltdat; }
            private set { _fltdat = value; }
        }
        private PreProcDftList[] _fltdat;

        /// <summary>
        /// 미자막 공정의 상위 보고용 데이터
        /// </summary>
        public MrkFltDat MarkData
        {
            get { return _markData; }
            private set { _markData = value; }
        }
        private MrkFltDat _markData;

        /// <summary>
        /// 이전 공정 비교용 결점 데이터 
        /// </summary>
        public List<PreprocMrkDat>[] PreMarkData
        {
            get { return _preMarkData; }
            private set { _preMarkData = value; }
        }
        private List<PreprocMrkDat>[] _preMarkData;

        // 현재 생산하고 있는 BCNO
        public string BCNO
        {
            get { return _bcno; }
            set { _bcno = value; }
        }
        private string _bcno;

        public bool IsPreProc = false;

        public PreProcResultData()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            _fltdat = new PreProcDftList[count];
            for (int i = 0; i < count; i++)
                _fltdat[i] = new PreProcDftList();

            _markData = new MrkFltDat();
        }

        ~PreProcResultData()
        {
            ResetAll();
        }

        public void ResetAll()
        {            
            // 실시간 불량 전송용 데이터 
            _markData.Reset();

            for (int i = 0; i < _fltdat.Length; i++)
            {
                // 내부 데이터 삭제
                for (int j = 0; j < _fltdat[i].Count; j++)
                    _fltdat[i][j].ResetAll();

                // 공정 별 데이터 리스트 삭제
                _fltdat[i].Clear();
            }

            for (int i = 0; i < _preMarkData.Length; i++)
            {
                // 내부 데이터 삭제
                for (int j = 0; j < _preMarkData[i].Count; j++)
                    _preMarkData[i][j].Reset();

                // 공정 별 데이터 리스트 삭제
                _preMarkData[i].Clear();
            }
        }

        public void Add(MarkingFaultDatum data)
        {
            // 10M 단위로 데이터 자름
            _markData.Add(data);
        }

        /// <summary>
        /// 현재 생산중인 랏 영역의 불량 데이터 탐색 처리
        /// </summary>
        /// <param name="bcno"></param>
        /// <param name="startY"></param>
        /// <param name="endY"></param>
        /// <returns></returns>
        public List<MarkingFaultDatum> GetDefectPts(string bcno, float startY, float endY)
        {
            List<MarkingFaultDatum> pts = new List<MarkingFaultDatum>();

            _markData.GetData(bcno, startY, endY, ref pts);

            return pts;
        }

        public PreProcResultData Copy()
        {
            PreProcResultData data = new PreProcResultData();

            data.BCNO = this.BCNO;
            data.IsPreProc = this.IsPreProc;

            // 전체 결점 데이터
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for(int i=0; i<count; i++)
                data.FLTDAT[i].Copy(FLTDAT[i]);

            // 마킹 대상 결점 데이터
            foreach (var mrks in MarkData.Data.Data)
                data.Add(mrks.Clone());

            for(int i=0; i<count; i++)
            {
                foreach (var marks in PreMarkData[i])
                    data.PreMarkData[i].Add(marks.Clone());
            }
            
            return data;
        }
    }

    #region PreProcDefect
    // DB에서 탐색한 공정의 결점 데이터를 저장한다.
    public class PreProcDefect
    {
        // 이전 공정 라인 코드
        public string LNCD { get; set; } = "";

        // 결점 데이터
        public FltDatumList Data { get; set; } = null;

        public PreProcDefect()
        {
            Data = new FltDatumList();
        }

        ~PreProcDefect()
        {
            ResetAll();
        }

        public void ResetAll()
        {
            Data.Clear();
        }

        public PreProcDefect Clone()
        {
            PreProcDefect defect = new PreProcDefect();

            defect.LNCD = LNCD;

            foreach(var data in Data.Data)
                defect.Data.Add(data.Clone());

            return defect;
        }

        public void Add(FaultDatum item)
        {
            Data.Add(item);
        }
    }

    public class PreProcDftList : ItemList<PreProcDefect>
    {
        public void Copy(List<PreProcDefect> input)
        {
            _data.Clear();
            foreach (PreProcDefect lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(PreProcDftList input)
        {
            _data.Clear();
            foreach (PreProcDefect lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion PreProcDefect

    #region FalutFunction
    public static class FalutFunction
    {
        public static int GetLineFromFLTID(int id)
        {
            int defectLine = 13;

            switch ((eFLTID)id)
            {
                case eFLTID.JT_DOT_01: defectLine = 14; break;
                case eFLTID.GB_GIPPO_02: defectLine = 12; break;
                case eFLTID.PERIOD_GIPPO_03: defectLine = 18; break;
                case eFLTID.SAME_04: defectLine = 19; break;
                case eFLTID.SRKZ_05: defectLine = 17; break;
                case eFLTID.JT_LINE_06: defectLine = 15; break;
                case eFLTID.CROSS_07: defectLine = 11; break;
                case eFLTID.SCRATCH_09: defectLine = 10; break;

                default:
                    if ((id >= (int)eFLTID.JH_DOT_W_31 && id <= (int)eFLTID.JH_GROUP_37) ||
                        (id >= (int)eFLTID.MH_DOT_W_51 && id <= (int)eFLTID.MH_GROUP_57) ||
                        (id >= (int)eFLTID.NEL7_HJK_5_61 && id <= (int)eFLTID.NEL7_HJK_1_65) ||
                        (id >= (int)eFLTID.NEL8_W_71 && id <= (int)eFLTID.NEL8_WB_73) ||
                        (id >= (int)eFLTID.SAME2_81 && id <= (int)eFLTID.SAME8_87))
                    {
                        defectLine = id;
                    }
                    break;
            }

            return defectLine;
        }

        public static bool IsDefectExist(eCSV_TYPE csvType, int[] defectCnt)
        {
            bool isSuccess = true;
            if (csvType == eCSV_TYPE.KORENO || csvType == eCSV_TYPE.KORENO_RK || csvType == eCSV_TYPE.KORENO_RK_IJP)
            {
                // 하나라도 검색이 되었으면 OK
                isSuccess = false;
                for (int i = 0; i < defectCnt.Length; i++)
                {
                    // 갯수 확인 못했으면
                    if (defectCnt[i] != -1 && defectCnt[i] == 0) isSuccess = false;
                }
            }
            else
            {
                // 전체가 다 불량이 있어야 OK
                for (int i = 0; i < defectCnt.Length; i++)
                {
                    // 갯수 확인 못했으면
                    if (defectCnt[i] != -1 && defectCnt[i] == 0) isSuccess &= false;
                }
            }

            return isSuccess;
        }
    }
    #endregion FalutFunction

}
