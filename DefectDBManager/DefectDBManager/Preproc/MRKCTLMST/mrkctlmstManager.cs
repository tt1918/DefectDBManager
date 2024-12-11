using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class MrkctlmstMaterial
    {
        #region 각 공정에 대한 품종 별 MRKCTLMST_MODEL

        public string Name { get; private set; }

        /// <summary>
        /// 각 공정별 MRKCTLMST 모델 데이터
        /// </summary>
        public Dictionary<string, MRKCTLMST_MODEL> MRKCTLMST
        {
            get { return _mrkctlmst; }
            private set { _mrkctlmst = value; }
        }
        Dictionary<string, MRKCTLMST_MODEL> _mrkctlmst = null;
        #endregion


        public MrkctlmstMaterial()
        {
            _mrkctlmst = new Dictionary<string, MRKCTLMST_MODEL>();
        }

        #region 모델 데이터 관리
        /// <summary>
        /// 
        /// </summary>
        /// <param name="lncd"></param>
        /// <param name="model"></param>
        public void Add(string lncd, MRKCTLMST_MODEL model)
        {
            if (MRKCTLMST.ContainsKey(lncd) == true)
                MRKCTLMST[lncd] = model;
            else
                MRKCTLMST.Add(lncd, model);
        }
        #endregion

        /// <summary>
        /// 품종의 각 공정별 mrkctlmst를 가지고 온다.
        /// </summary>
        /// <returns>정상적으로 처리되는 경우 true</returns>
        public bool Load()
        {
            string path = System.IO.Path.Combine(Define.MKCDModelPath, Name);

            if (Directory.Exists(path) == false)
                return false;

            System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(path);
            foreach (System.IO.FileInfo file in di.GetFiles())
            {
                string fileName = file.Name;
                string[] parse = fileName.Split('.');

                MRKCTLMST_MODEL model = new MRKCTLMST_MODEL();
                model.Name = parse[0];
                model.Load(path);
                MRKCTLMST.Add(parse[0], model);
            }
            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        public void Save()
        {
            string path = System.IO.Path.Combine(Define.MKCDModelPath, Name);

            Directory.CreateDirectory(path);

            foreach (string lncd in MRKCTLMST.Keys)
                MRKCTLMST[lncd].Save(path);
        }
    }
}
