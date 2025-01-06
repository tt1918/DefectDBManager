using Microsoft.SqlServer.Server;
using Newtonsoft.Json;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{

    #region DB Query Item List
    public class ItemList<T>
    {
        public T this[int index]
        {
            get { return _data[index]; }
            set { _data[index] = value; }
        }
        public List<T> Data
        {
            get { return _data; }
            protected set { _data = value; }
        }
        protected List<T> _data = null;

        [JsonIgnore]
        public int Count
        {
            get { return _data.Count; }
        }

        public ItemList()
        {
            _data = new List<T>();
        }

        public List<T> Clone()
        {
            List<T> data = new List<T>();
            foreach (var ypData in _data)
                data.Add(ypData);

            return data;
        }

        public void Add(T data) { _data.Add(data); }

        public void Clear() { _data.Clear(); }
    }

    #endregion DB Query Item List

}
