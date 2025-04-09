using UnityEngine;
using System.IO;
using System.Threading;
using System.Text;
using System;
using System.Collections;

public class HeadTrackingRecorder : MonoBehaviour
{
    private string filePath;
    private StreamWriter fileWriter;
    private bool isRecording = false;
    private int recordCount = 0;
    private bool timerecord = false;
    private Vector3 headPosition;
    private Quaternion headRotation;
    private string currentTime;
    public GameObject RandomPosition;
    private float recordingInterval = 0.1f; // 设置记录的时间间隔为100ms

    void Start()
    {
        filePath = Application.persistentDataPath + "/HeadTrackingData.txt";
        StartRecordingCoroutine();
        Debug.Log("Persistent Data Path: " + Application.persistentDataPath);
        currentTime = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
    }

    void Update()
    {
        lock (fileWriter)
        {
            isRecording = RandomPosition.GetComponent<RandomPosition>().headisrecord();
        }
        headPosition = Camera.main.transform.position;
        headRotation = Camera.main.transform.rotation;
        currentTime = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
    }

    void StartRecordingCoroutine()
    {
        StartCoroutine(RecordHeadTrackingDataInBackground());
    }

    IEnumerator RecordHeadTrackingDataInBackground()
    {
        fileWriter = new StreamWriter(filePath, true);
        float lastRecordTime = Time.time;

        while (true)
        {
            yield return null;

            float elapsedTimeSinceLastRecord = Time.time - lastRecordTime;

            if (elapsedTimeSinceLastRecord >= recordingInterval)
            {
                lastRecordTime = Time.time;

                lock (fileWriter)
                {
                    if (isRecording)
                    {
                        if (timerecord)
                        {
                            recordCount++;
                            fileWriter.WriteLine($"\n第{recordCount}次写入，写入时间为{currentTime}\n");
                            timerecord = false;
                        }

                        // Perform file I/O in a thread-safe manner
                        if (fileWriter != null)
                        {
                            // Check if the stream is still open for writing
                            if (fileWriter.BaseStream.CanWrite)
                            {
                                string data = $"{lastRecordTime}\t{headPosition.x}\t{headPosition.y}\t{headPosition.z}\t{headRotation.x}\t{headRotation.y}\t{headRotation.z}\t{headRotation.w}\n";
                                fileWriter.Write(data);
                                fileWriter.Flush();  // Flush the buffer to ensure data is written immediately
                            }
                            else
                            {
                                Debug.LogError("文件流无法写入！");
                            }
                        }
                    }
                    else
                    {
                        timerecord = true;
                    }
                }
            }
        }
    }


    void OnDisable()
    {
        lock (fileWriter)
        {
            if (fileWriter != null)
            {
                fileWriter.Close();
            }
        }
    }
}
