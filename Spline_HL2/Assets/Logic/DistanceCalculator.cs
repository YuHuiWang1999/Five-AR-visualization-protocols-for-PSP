using UnityEngine;
using System.IO;

public class DistanceCalculator : MonoBehaviour
{
    public Transform[] point1; // ��һ�����Transform���
    public Transform[] point2; // CP
    public Transform[] point3; // DHX
    public Transform[] point4; // GYH
    public Transform[] point5; // WHL
    public Transform[] point6; // WXY

    public Transform[] pointExport_1; // ��һ�����Transform���
    public Transform[] pointExport_2; // CP
    public Transform[] pointExport_3; // DHX
    public Transform[] pointExport_4; // GYH
    public Transform[] pointExport_5; // WHL
    public Transform[] pointExport_6; // WXY
    void Start()
    {
        // ��������֮��ľ���
        // ��ȡ����·�����ļ�·��
        string desktopPath = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop);
        string filePath = desktopPath + "/distance_result.txt";
        using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
        {
            writer.WriteLine("CP");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point2[i].position)*1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_2[i].position) * 1000f;
            // ��ȡ����A��B�ķ�������
            Vector3 directionA = point1[i].up; // ����A��y�᷽��
            Vector3 directionB = point2[i].up; // ����B��y�᷽��
            float angle = Vector3.Angle(directionA, directionB);
            // ���ļ���׷�Ӽ�����
            using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }  
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
        {
            writer.WriteLine("DHX");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point3[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_3[i].position) * 1000f;
            // ��ȡ����A��B�ķ�������
            Vector3 directionA = point1[i].up; // ����A��y�᷽��
            Vector3 directionB = point3[i].up; // ����B��y�᷽��
            float angle = Vector3.Angle(directionA, directionB);
            // ���ļ���׷�Ӽ�����
            using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));

            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
        {
            writer.WriteLine("GYH");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point4[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_4[i].position) * 1000f;
            // ��ȡ����A��B�ķ�������
            Vector3 directionA = point1[i].up; // ����A��y�᷽��
            Vector3 directionB = point4[i].up; // ����B��y�᷽��
            float angle = Vector3.Angle(directionA, directionB);
            // ���ļ���׷�Ӽ�����
            using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
        {
            writer.WriteLine("WHL");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point5[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_5[i].position) * 1000f;
            // ��ȡ����A��B�ķ�������
            Vector3 directionA = point1[i].up; // ����A��y�᷽��
            Vector3 directionB = point5[i].up; // ����B��y�᷽��
            float angle = Vector3.Angle(directionA, directionB);
            // ���ļ���׷�Ӽ�����
            using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));

            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
        {
            writer.WriteLine("WXY");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point6[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_6[i].position) * 1000f;
            // ��ȡ����A��B�ķ�������
            Vector3 directionA = point1[i].up; // ����A��y�᷽��
            Vector3 directionB = point6[i].up; // ����B��y�᷽��
            float angle = Vector3.Angle(directionA, directionB);
            // ���ļ���׷�Ӽ�����
            using (StreamWriter writer = new StreamWriter(filePath, true)) // �ڶ�������true��ʾ׷��ģʽ
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }
        }
    }
}
