using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Coss.Controls
{
    public class ProgressEx : ProgressBar
    {
        [DllImport("uxtheme.dll", SetLastError = true, ExactSpelling = true, CharSet = CharSet.Unicode)]
        public static extern int SetWindowTheme(IntPtr hWnd, string pszSubAppName, string pszSubIdList);
        Font customFont;
        public ProgressEx()
        {
            SetWindowTheme(this.Handle, "", "");
            typeof(ProgressBar).InvokeMember("DoubleBuffered",
                                         System.Reflection.BindingFlags.SetProperty |
                                            System.Reflection.BindingFlags.Instance |
                                            System.Reflection.BindingFlags.NonPublic,
                                         null,
                                         this,
                                         new object[] { true });
        }

        public int CustomValue
        {
            get
            {
                return Value;
            }
            set
            {
                Value = value;
            }
        }

        public Font CustomFont 
        {
            get { return customFont; }
            set { customFont = value; }
        }

        protected override void DefWndProc(ref Message m)
        {
            base.DefWndProc(ref m);

            switch (m.Msg)
            {
                case 0x000f:
                    using (var g = Graphics.FromHwnd(Handle))
                    {
                        StringFormat sf = new StringFormat();
                        sf.LineAlignment = StringAlignment.Center;
                        sf.Alignment = StringAlignment.Center;
                        g.DrawString($"{CustomValue} %", CustomFont, SystemBrushes.ControlText, ClientRectangle, sf);
                    }
                    break;
            }
        }
    }
}
