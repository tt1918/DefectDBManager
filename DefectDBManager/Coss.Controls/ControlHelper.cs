using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Windows.Forms;

namespace Coss.Controls
{
   public static class ControlHelper
    {
        /// <summary>
        /// 컨트롤의 DoubleBuffered 속성을 변경
        /// </summary>
        /// <param name="contorl"></param>
        /// <param name="setting"></param>
        public static void SetDoubleBuffered(this Control contorl, bool setting)
        {
            Type dgvType = contorl.GetType();
            PropertyInfo pi = dgvType.GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);
            pi.SetValue(contorl, setting, null);
        }

        /// <summary>
        /// Form 내에 동일한 Control 찾기
        /// </summary>
        static public Control[] GetAllTagetControl(Control control, Type type)
        {
            List<Control> allControl = new List<Control>();

            foreach (Control c in control.Controls)
            {
                if (c.GetType() == type)
                    allControl.Add((Control)c);
                if (c.Controls.Count > 0)
                    allControl.AddRange(GetAllTagetControl(c, type));
            }

            return allControl.ToArray();
        }
    }
}
