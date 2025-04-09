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
        // 获取当前时间
        string currentTime = System.DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");

        // 构建存储路径（在持久数据路径下创建一个名为Logs的文件夹，并在其中存储日志文件）
        string folderPath = Application.persistentDataPath + "/Logs";
        string filePath = folderPath + "/event_log.txt";

        // 创建文件夹（如果不存在）
        if (!System.IO.Directory.Exists(folderPath))
        {
            System.IO.Directory.CreateDirectory(folderPath);
        }

        // 写入时间到文件
        System.IO.File.AppendAllText(filePath, "Event Time: " + currentTime + "\n");

        // 在控制台输出，以便调试
        Debug.Log("Event Time recorded: " + currentTime);
        Debug.Log("存储路径: " + filePath);
    }
}
