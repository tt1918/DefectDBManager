using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{

    public class GroupUnit
    {
        public string name;
        public string item;
    }

    public class CodeUnit : BaseUnit
    {
        public const int MaxGroup = 20;

        public int index;
        public int count;

        public List<GroupUnit> group;

        public CodeUnit()
        {
            group = new List<GroupUnit>();

            Reset();
        }

        ~CodeUnit()
        {
            group.Clear();
        }

        public void Reset()
        {
            Title = "";
            count = 0;
            index = 0;
            group.Clear();
        }

        public void CopyData(CodeUnit data)
        {
            Reset();

            this.Title = data.Title;
            this.index = data.index;
            this.count = data.count;

            for (int i = 0; i < data.group.Count; i++)
            {
                group.Add(data.group[i]);
            }
        }

        private bool IsGroupMember(string group, string member)
        {
            bool isFind = false;

            Debug.Assert(member.Length == 2);
            if (member.Length < 2) return false;

            string upGroup = group.ToUpper();
            string upMember = member.ToUpper();

            upGroup = upGroup.Trim();
            upMember = upMember.Trim();

            string[] groupName = upGroup.Split(',');

            for (int i = 0; i < groupName.Length; i++)
            {
                string name = groupName[i];
                int nFind = name.IndexOf('~');
                if (name.Length == 5 && nFind == 2)
                {
                    string stt = name.Substring(0, nFind);
                    string end = name.Substring(nFind + 1);

                    if (stt.ElementAt(0) == end.ElementAt(0) && member.ElementAt(0) == stt.ElementAt(0) &&
                       Char.IsDigit(stt.ElementAt(0)) == true && Char.IsDigit(end.ElementAt(0)) == true &&
                       Char.IsDigit(member.ElementAt(0)) == true)
                    {
                        int nStt = Int32.Parse(stt.Substring(1));
                        int nEnd = Int32.Parse(end.Substring(1));
                        int nKey = Int32.Parse(member.Substring(1));
                        if (nStt <= nKey && nEnd <= nKey)
                        {
                            isFind = true;
                            break;
                        }
                    }
                }
                else if (nFind == -1)
                {
                    if (name == member)
                    {
                        isFind = true;
                        break;
                    }
                }
                //else
                //{
                //    break;
                //}
            }

            return isFind;
        }

        public string FindGroupName(string key)
        {
            Debug.Assert(count <= MaxGroup);

            string name = "";

            count = Math.Min(count, MaxGroup);
            for (int i = 0; i < count; i++)
            {
                if (IsGroupMember(group[i].item, key))
                {
                    name = group[i].name;
                    break;
                }
            }

            return name;
        }

        public int FindGroupIndex(string key)
        {
            Debug.Assert(count <= MaxGroup);
            
            int index = -1;
            count = Math.Min(count, MaxGroup);
            for (int i = 0; i < count; i++)
            {
                if (IsGroupMember(group[i].item, key))
                {
                    index = i;
                    break;
                }
            }
            return index;
        }

        public string GetGroupName(string item)
        {
            string name = "";
            for(int i=0; i<group.Count; i++)
            {
                if (group[i].item == item)
                {
                    name = group[i].name;
                    break;
                }    
            }

            return name;
        }

        public string GetGroupItem(string name)
        {
            string item = "";
            for (int i = 0; i < group.Count; i++)
            {
                if (group[i].name == name)
                {
                    item = group[i].item;
                    break;
                }
            }
            return item;
        }

        public string GetGroupName(int index)
        {
            Debug.Assert(count <= MaxGroup);
            
            return group[index].name;
        }

        public string GetGroupItem(int index)
        {
            Debug.Assert(count <= MaxGroup);

            return group[index].item;
        }
    }

    public class CodeConfig
    {
        public Dictionary<string, CodeUnit> DicCode;
        public double NGLengMin
        {
            get { return ngLengMin; }
        }
        public double NGLengMax
        {
            get { return ngLengMax; }
        }
        private double ngLengMin;
        private double ngLengMax;

        public CodeConfig()
        {
            DicCode = new Dictionary<string, CodeUnit>();
            ngLengMin = 0.0;
            ngLengMax = 0.0;
        }

        ~CodeConfig()
        {
            Reset();
        }

        public void Reset()
        {
            ngLengMin = 0.0;
            ngLengMax = 0.0;
            DicCode.Clear();
        }

        public int Read(string path)
        {

            return 0;
        }

        public void Write(string path)
        {

        }

        protected void UpdateIni(string fileName, bool bLoad, int keyIndex, CodeUnit c)
        {

        }

        protected void SetData(string title, CodeUnit p)
        {
            // 이부분 인덱스에 추가하는건지 새로 추가하는건지확인이 필요함. 

            DicCode.Add(title, p);
            // or
            //bool isFind = mapCode.ContainsKey(title);
            //if (isFind == true)
            //    mapCode[title] = p;
        }

        public int GetSize()
        {
            return DicCode.Count;
        }

        public bool GetData(int index, ref CodeUnit p)
        {
            bool bRet = false;
            foreach( KeyValuePair<string, CodeUnit> pair in DicCode)
            {
                if (pair.Value.index == index)
                {
                    p = pair.Value;
                    bRet = true;
                    break;
                }
            }

            return bRet;
        }

        public bool GetData(string title, ref CodeUnit p)
        {
            bool isFind = false;

            isFind = DicCode.ContainsKey(title);
            if (isFind == true)
                p = DicCode[title];
            return isFind;
        }

        public int GetIndex(string title)
        {
            CodeUnit unit = new CodeUnit();
            if (GetData(title, ref unit) == true)
                return unit.index;
            else
                return -1;
        }
    }
}
