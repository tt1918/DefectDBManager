using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Coss.Controls
{
    public class GridFrameSimple
    {
        public Color BgTopColor { get; set; }
        public Color BgBottomColor { get; set; }
        public Color BgGridColor { get; set; }
        public Font GridMeterFont { get; set; }
        public Brush GridMeterBrush { get; set; }
        public bool IsDirty { get; set; }
        public float BgGridPenWidth { get; set; }
        public int DrawSmallGapY { get; set; }
        public int DrawMediumGapY { get; set; }
        public int DrawLargeGapY { get; set; }
        public double ScaleRealX { get; set; }
        public double ScaleRealY { get; set; }
        public int SmallLength { get; set; } = 100000;
        public int MediumLength { get; set; } = 1000000;

        /// <summary>
        /// Frame 생성자
        /// </summary>
        /// <param name="smallGap">길이가 100M미만의 경우 보여질 M간격</param>
        /// <param name="mediumGap">길이가 100~1000M의 경우 보여질 M간격</param>
        /// <param name="largeGap">길이가 1000M이상의 경우 보여질 M간격</param>
        public GridFrameSimple(int smallGap, int mediumGap, int largeGap)
        {
            BgTopColor = Color.White;
            BgBottomColor = Color.White;
            BgGridColor = Color.LightGray;
            GridMeterFont = new System.Drawing.Font("굴림", 14F, System.Drawing.FontStyle.Regular,
            System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            GridMeterBrush = new SolidBrush(Color.LightGray);
            BgGridPenWidth = 4;
            DrawSmallGapY = smallGap;
            DrawMediumGapY = mediumGap;
            DrawLargeGapY = largeGap;
        }

        /// <summary>
        /// Map에 거리표시
        /// </summary>
        /// <param name="g">Graphics</param>
        /// <param name="srcRect">Real좌표 (예: 0 ~ 40,000mm)</param>
        /// <param name="width">Dest 폭</param>
        /// <param name="height">Dest 길이</param>
        /// <param name="offsetX">Dest OffsetX</param>
        /// <param name="offsetY">Dest OffsetY</param>
        public void Draw(Graphics g, Rectangle srcRect, int width, int height, int offsetX, int offsetY)
        {
            Rectangle rbgn = new Rectangle(offsetX, offsetY, width, height);
            ScaleRealX = (double)rbgn.Width / srcRect.Width;
            ScaleRealY = (double)rbgn.Height / srcRect.Height;

            using (Pen pen = new Pen(BgGridColor, BgGridPenWidth))
            {
                int drawGapY = DrawSmallGapY;
                if (SmallLength < srcRect.Height && srcRect.Height <= MediumLength)
                    drawGapY = DrawMediumGapY;
                else if (MediumLength < srcRect.Height)
                    drawGapY = DrawLargeGapY;
                var stt = (int)(srcRect.Top / drawGapY);
                var end = (int)(srcRect.Bottom / drawGapY);
                int marginX = 70;
                int marginY = 5; 
                for (int i = stt + 1; i < end + 1; i++)
                {
                    g.DrawLine(pen, rbgn.Right - 80, (int)((i * drawGapY - srcRect.Top) * ScaleRealY + rbgn.Top),
                        rbgn.Right, (int)((i * drawGapY - srcRect.Top) * ScaleRealY + rbgn.Top));
                }
                for (int i = stt; i < end + 1; i++)
                {
                    string txt = ((i * drawGapY) / 1000).ToString() + "M";
                    g.DrawString(txt, GridMeterFont, GridMeterBrush,
                        rbgn.Right - marginX, (int)((i * drawGapY - srcRect.Top) * ScaleRealY + rbgn.Top + marginY));
                }
            }
        }

    }
}
