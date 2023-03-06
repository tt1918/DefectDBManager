using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    #region FormDB
    #region BCNO List
    public enum eBCNOList
    {
        No = 0,
        LotNo,
        BadCount,
        NG_M2,
        Use,
        BCno,
    }

    //
    #endregion

    #region PTRYLP List
    public enum ePTRYLPList
    {
        TGLot,
        Lot,
        InsertM,
        CompleteM,
        Width
    }
    #endregion

    #region PTRYOP List
    public enum ePTRYOPList
    {
        Type,
        ESLot,
        LineCode,
        StartTime,
        EndTime,
    }
    #endregion

    #region MRKCTLMST List
    public enum eMRKCTLMSTList
    {
        LineCode,
        FaultID,
        Type,
        Size,
    }
    #endregion

    #region INSPDAT List
    public enum eINSPDATList
    {
        AdminNo,
        Type,
        LotNo,
        StrDT,
        StrTM,
        EndDT,
        EndTM,
        Width,
        Length,
    }
    #endregion

    #region FAULTDAT List
    public enum eFAULTDATList
    {
        Cnt,
        AdminNo,
        NgCnt,
        Offset,
        XPosM,
        YPosM,
        Type,
        FaultID,
        Size,
        Cam,
        Class,
        MClass,
        XOffset,
        MNTTID
    }
    #endregion

    #region XOFSMST List
    public enum eXOFSMSTList
    {
        KYCD,
        PPCD,
        LNCD,
        YLMZKN2,
        YLSZKN,
        X_OFFSET,
    }
    #endregion

    #region AREADEL List
    public enum eAREADELList
    {
        NO,
        KYCD,
        PPCD,
        LNCD,
        LOTNO,
        STR_WD,
        END_WD,
        STR_MD,
        END_MD,
    }
    #endregion
    #endregion FormDB

    #region Nitto Database

    public enum eNittoDBProgress
    {
        PTRYLP = 0,
        XOFSMST,
        AREADEL,
        PTRYOP,
        MRKCTLMST_ES,
        MRKCTLMST_TG,
        MRKCTLMST_ETC,
        INSPDAT_ES,
        INSPDAT_TG,
        INSPDAT_ETC,
        FAULTDAT_ES,
        FAULTDAT_TG,
        FAULTDAT_ETC,
    }
    public enum eCSV_TYPE
    {
        None = 0,
        KORENO,
        NITTO,
        NITTO_RTS,
        NITTO_RK,
        KORENO_RK,
        KORENO_RK_IJP,
        MARKING_VISION,
    }


    public enum eOpticClass
    {
        OpticClass1 = 0,   // 경계 반사
        OpticClass2,       // 정투과
        OpticClass3,       // 주기성 불량
        OpticClass4,       // 크로스 불량
        OpticClass5,       // 정반사
        OpticClass6,       // 미분반사
    }
    public enum eFCD
    {
        ES = 0,
        TG,
        ETC,
    }

    public enum eDbIdWhen
    {
        Now = 0,
        Next,
    }

    enum eFLTID
    {
        JT_DOT_01 = 1,               //정투과점
        GB_GIPPO_02,                 //경계반사 기포
        PERIOD_GIPPO_03,             //주기성 기포
        SAME_04,                     //동일결점
        SRKZ_05,                     //쓰리키즈
        JT_LINE_06,                  //정투과선
        CROSS_07,                    //크로스
        NONE_08,
        SCRATCH_09,

        DUMMY_11 = 11,

        DUMMY_21 = 21,

        JH_DOT_W_31 = 31,
        JH_DOT_B_32,
        JH_CIRCLE_W_33,
        JH_CIRCLE_B_34,
        JH_LINE_W_35,
        JH_LINE_B_36,
        JH_GROUP_37,

        DUMMY_41 = 41,

        MH_DOT_W_51 = 51,
        MH_DOT_B_52,
        MH_CIRCLE_W_53,
        MH_CIRCLE_B_54,
        MH_LINE_W_55,
        MH_LINE_B_56,
        MH_GROUP_57,

        NEL7_HJK_5_61 = 61,
        NEL7_HJK_4_62,
        NEL7_HJK_3_63,
        NEL7_HJK_2_64,
        NEL7_HJK_1_65,

        NEL8_W_71 = 71,
        NEL8_B_72,
        NEL8_WB_73,

        SAME2_81 = 81,
        SAME3_82,
        SAME4_83,
        SAME5_84,
        SAME6_85,
        SAME7_86,
        SAME8_87,
        MAX_PARAM
    };

    public enum eUserDefectClass
    {
        Class1 = 0,
        Class2,
        Class3,
        Class4,
        Class5,
        Class6,
        Class7,
        Class8,
        Class9,
        Class10,
    }

    public enum eESDefectClass
    {
        CROSS1 = 0,
        CROSS2,
        CROSS3,
        CROSS4,
        CROSS5,
        CROSS6,
        CROSS7,
        THRU1,
        THRU2,
        THRU3,
        THRU4,
        REF1,
        FREQ1,
        FREQ2,
        FREQ3,
        FREQ4,
        FREQ5,
        FREQ6,
        SAME1,
        SAME2,
        SAME3,
        SAME4,
        SAME5,
        ETC,
        MAX
    };


    #endregion

    #region 이벤트 보고용 
    public enum eEventReport
    {
        eUpdateDataNow = 1,
        eUpdateDataNext,
        eResetDataNow,
        eResetDataNext,
        eFinishedSearchLot,
        eFinishedSearchModel,
        eFinishedReadCSVFile,
        eFailedReadCSVFile,
        eBCR_FLTID_CheckError,
        eBCR_INSPMETER_CheckError,
    }
    #endregion

    #region 검색 결과 
    public enum eSearchProcessRes
    {
        DB_SearchIsBusy=-3,
        DB_NoExistES=-2,
        DB_Disconnected= -1,
        Process_None=0,
        DB_SearchDone=1,
    }
    #endregion

    #region Pross 상태 확인용
    public enum eProcessState
    {
        Ready = 0,
        Run,
        Complete,
        Error,
    }
    #endregion
}
