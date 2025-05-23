using System;
using System.Windows.Forms;

namespace MarkCompare.Helper
{
    public static class UIHelper
    {
        /// <summary>
        /// UI 스레드에서 안전하게 Control을 업데이트합니다.
        /// </summary>
        public static void RunOnUIThread(Control control, Action action)
        {
            if (control == null || control.IsDisposed || action == null)
                return;

            if (control.InvokeRequired)
                control.BeginInvoke(action);
            else
                action();
        }

        /// <summary>
        /// 텍스트 설정 전용 헬퍼
        /// </summary>
        public static void SetText(Control control, string text)
        {
            RunOnUIThread(control, () => control.Text = text);
        }

        /// <summary>
        /// 컨트롤 활성화/비활성화
        /// </summary>
        public static void SetEnabled(Control control, bool enabled)
        {
            RunOnUIThread(control, () => control.Enabled = enabled);
        }

        /// <summary>
        /// 컨트롤 보이기/숨기기
        /// </summary>
        public static void SetVisible(Control control, bool visible)
        {
            RunOnUIThread(control, () => control.Visible = visible);
        }
    }
}
