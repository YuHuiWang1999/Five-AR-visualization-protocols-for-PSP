using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HololensDate : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        LogEventTime();
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    void LogEventTime()
    {
        // ��ȡ��ǰʱ��
        string currentTime = System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

        // �����洢·�����ڳ־�����·���´���һ����ΪLogs���ļ��У��������д洢��־�ļ���
        string folderPath = Application.persistentDataPath + "/Logs";
        string filePath = folderPath + "/event_log.txt";

        // �����ļ��У���������ڣ�
        if (!System.IO.Directory.Exists(folderPath))
        {
            System.IO.Directory.CreateDirectory(folderPath);
        }

        // д��ʱ�䵽�ļ�
        System.IO.File.AppendAllText(filePath, "Event Time: " + currentTime + "\n");

        // �ڿ���̨������Ա����
        Debug.Log("Event Time recorded: " + currentTime);
        Debug.Log("�洢·��: " + filePath);
    }
}
