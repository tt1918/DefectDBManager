using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace Coss.Forms
{
	public class CustomComboBox: ComboBox
	{
		public CustomComboBox ( ) : base()
		{
			m_size = new Size(0, 0);
			m_sizeDropDown = new Size(121, 120);
			this.DropDownHeight = 1;
        }
        public Size ComboSize { set { m_size = value; } get { return m_size; } }

		public Size DropDownSize { set { m_sizeDropDown = value; } get { return m_sizeDropDown; } }

		public const uint WM_LBUTTONDOWN = 0x201;
		[System.Runtime.ExceptionServices.HandleProcessCorruptedStateExceptions]
		protected override void WndProc ( ref Message m )
		{
			try
			{
				base.WndProc(ref m);
				if ( m.Msg == WM_LBUTTONDOWN )
				{
					if ( this.DropDownStyle != ComboBoxStyle.Simple )
					{
                        this.Size = DropDownSize;
                        this.DropDownStyle = ComboBoxStyle.Simple;
                    }
                }
			}
			catch(Exception /*e*/)
			{
            }
			
		}

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            //컨트롤폭을 초기화 (처음만호출)
            if (this.Visible && m_size.Width == 0)
            {
                this.DropDownStyle = ComboBoxStyle.DropDown;
                m_size.Width = Size.Width;
                m_sizeDropDown.Width = Size.Width;
            }
        }
        protected override void OnSelectedValueChanged ( EventArgs e )
		{
			if ( this.DropDownStyle != ComboBoxStyle.DropDown )
			{
                this.DropDownStyle = ComboBoxStyle.DropDown;
				this.Size = ComboSize;
            }

			base.OnSelectedValueChanged(e);
		}


		protected override void OnMouseLeave ( EventArgs e )
		{
			if ( this.DropDownStyle != ComboBoxStyle.DropDown )
			{
                this.DropDownStyle = ComboBoxStyle.DropDown;
                this.Size = ComboSize;
            }

			base.OnMouseLeave(e);
		}


		Size m_size;
		Size m_sizeDropDown;
	}
}
