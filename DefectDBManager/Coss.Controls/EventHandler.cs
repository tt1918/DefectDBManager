using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace Coss.Controls
{
    #region Rollmap
    /// <summary>
    /// 마우스 클릭 이벤트 델리게이트
    /// <param name="real">전체맵에서의 클릭된 x,y좌표</param>
    /// <param name="gdi">GUI맵에서의 클릭된 x,y좌표</param>
    /// <param name="btn">MouseButtons</param>
    /// </summary>
    public delegate void RollmapMouseClickedHandler(Point real, Point gdi, MouseButtons btn);

    /// <summary>
    /// 결점 클릭 이벤트 델리게이트
    /// <param name="real">전체맵에서의 클릭된 x,y좌표</param>
    /// <param name="gdi">GUI맵에서의 클릭된 x,y좌표</param>
    /// <param name="optic">결점의 opticKey. -1이면 사용안함(기존버전),0이상이면 결점의 opticKey </param>
    /// <param name="defect">GUI맵에서의 클릭된 x,y좌표에 가장가까운 결점</param>
    /// </summary>
    public delegate void RollmapDefectClickedHandler(Point real, Point gdi, int optic, RollmapDefect defect);

    /// <summary>
    /// 결점 클릭 이벤트 델리게이트
    /// <param name="real">전체맵에서의 클릭된 x,y좌표</param>
    /// <param name="gdi">GUI맵에서의 클릭된 x,y좌표</param>
    /// <param name="optic">결점의 opticKey. -1이면 사용안함(기존버전),0이상이면 결점의 opticKey </param>
    /// <param name="defect">GUI맵에서의 클릭된 x,y좌표에 가장가까운 결점</param>
    /// </summary>
    public delegate void RollmapDefectsClickedHandler(Point real, Point gdi, Dictionary<int, RollmapDefect> defect);

    /// <summary>
    /// 이미지맵에서 마우스 클릭 이벤트 델리게이트
    /// </summary>
    /// <param name="real">전체맵에서의 클릭된 좌표</param>
    /// <param name="image">영상에서의 클릭된 좌표</param>
    /// <param name="gdi">GUI맵에서의 클릭된 좌표</param>
    /// <param name="extInfo">추가정보</param>
    public delegate void ImagemapMouseClickedHandler(Point real, Point image, Point gdi, string extInfo);


    public delegate void RollmapViewAreaChangedHandler(object sender, MapViewOptionArgs viewOption, Rectangle rect);

    public delegate void RollmapAutoViewHandler(object sender, bool value);
    /// <summary>
    /// 롤맵 이동 이벤트
    /// </summary>
    public delegate void RollmapUpDnHandler(bool up);

    public delegate void RollmapMouseUpHandler(Rectangle rect);

    public delegate void SyncRollmapEtcHandler(bool autoScroll, MapViewOptionArgs.tagMapViewRange viewRange, int x = 0, int y = 0, float zoom = 1.0f, bool forceUpdate = false);

    #endregion


    #region LedAlarm
    public enum tagAlarmStatus
    {
        Hide,
        Disabled,
        Noraml,
        Warning,
        Error,
    }
    public delegate void LedAlarmDoubleClickedHander(object sender, EventArgs e);

    //public class LedAlarmEventArgs : EventArgs
    //{
    //    public tagAlarmStatus AlarmStatus { get; }
    //    public LedAlarmEventArgs(tagAlarmStatus alarmStatus)
    //    {
    //        AlarmStatus = alarmStatus;
    //    }
    //}
    //public delegate void LedAlarmEventHandler(object sender, LedAlarmEventArgs e);
    #endregion

    #region Jog
    public enum tagJobType
    {
        LL = 0,
        LR,
        RL,
        RR
    }
    public class JogEventArgs : EventArgs
    {
        public string Key { get; }
        public tagJobType JogType { get; }

        public JogEventArgs(string key, tagJobType jogType)
        {
            Key = key;
            JogType = jogType;
        }
    }
    public delegate void JogEventHandler(object sender, JogEventArgs e);

    #endregion

}
