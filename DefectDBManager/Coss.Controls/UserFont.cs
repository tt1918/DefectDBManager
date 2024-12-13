using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Text;
using System.Runtime.InteropServices;

namespace Coss.Controls
{
    public class UserFont
    {

        public UserFont()
        {
            AddFontFromMemory();
        }

        public static FontFamily Light
        {
            get
            {
                return inst.privateFont.Families[0];
            }
        }
        public static FontFamily Medium
        {
            get
            {
                return inst.privateFont.Families[2];
            }
        }

        public static FontFamily Bold
        {
            get
            {
                return inst.privateFont.Families[1];
            }
        }

        private static UserFont inst = new UserFont();
        private PrivateFontCollection privateFont = new PrivateFontCollection();

        private void AddFontFromMemory()
        {
            List<byte[]> fonts = new List<byte[]>();
            fonts.Add(Properties.Resources.segoeui);
            fonts.Add(Properties.Resources.seguisb);
            //fonts.Add(Properties.Resources.segoeuib);
            fonts.Add(Properties.Resources.seguibl);

            foreach (byte[] font in fonts)
            {
                IntPtr fontBuffer = Marshal.AllocCoTaskMem(font.Length);
                Marshal.Copy(font, 0, fontBuffer, font.Length);
                privateFont.AddMemoryFont(fontBuffer, font.Length);
            }
        }

    }
}
