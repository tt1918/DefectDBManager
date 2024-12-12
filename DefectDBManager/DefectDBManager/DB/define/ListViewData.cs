using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class DBListViewBuf
    {
        public string[] items;
        private int size;
        public DBListViewBuf(int size)
        {
            this.size = size;
            items = new string[size];
            Reset();
        }

        public void Reset()
        {
            for (int i = 0; i < this.size; i++)
                items[i] = "";
        }
    }

    public class ListViewData
    {
        public List<DBListViewBuf> Data;

        public DBListViewBuf this[int index]
        {
            get { return Data[index]; }
            set { Data[index] = value; }
        }

        public ListViewData()
        {
            Data = new List<DBListViewBuf>();
        }

        public void Reset()
        {
            Data.Clear();
        }
    }

}
