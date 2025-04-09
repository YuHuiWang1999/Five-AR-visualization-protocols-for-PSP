using UnityEngine;
using System.IO;

public class DistanceCalculator : MonoBehaviour
{
    public Transform[] point1; // 第一个点的Transform组件
    public Transform[] point2; // CP
    public Transform[] point3; // DHX
    public Transform[] point4; // GYH
    public Transform[] point5; // WHL
    public Transform[] point6; // WXY

    public Transform[] pointExport_1; // 第一个点的Transform组件
    public Transform[] pointExport_2; // CP
    public Transform[] pointExport_3; // DHX
    public Transform[] pointExport_4; // GYH
    public Transform[] pointExport_5; // WHL
    public Transform[] pointExport_6; // WXY
    void Start()
    {
        // 计算两点之间的距离
        // 获取桌面路径和文件路径
        string desktopPath = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop);
        string filePath = desktopPath + "/distance_result.txt";
        using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
        {
            writer.WriteLine("CP");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point2[i].position)*1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_2[i].position) * 1000f;
            // 获取物体A和B的方向向量
            Vector3 directionA = point1[i].up; // 物体A的y轴方向
            Vector3 directionB = point2[i].up; // 物体B的y轴方向
            float angle = Vector3.Angle(directionA, directionB);
            // 打开文件并追加计算结果
            using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }  
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
        {
            writer.WriteLine("DHX");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point3[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_3[i].position) * 1000f;
            // 获取物体A和B的方向向量
            Vector3 directionA = point1[i].up; // 物体A的y轴方向
            Vector3 directionB = point3[i].up; // 物体B的y轴方向
            float angle = Vector3.Angle(directionA, directionB);
            // 打开文件并追加计算结果
            using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));

            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
        {
            writer.WriteLine("GYH");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point4[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_4[i].position) * 1000f;
            // 获取物体A和B的方向向量
            Vector3 directionA = point1[i].up; // 物体A的y轴方向
            Vector3 directionB = point4[i].up; // 物体B的y轴方向
            float angle = Vector3.Angle(directionA, directionB);
            // 打开文件并追加计算结果
            using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
        {
            writer.WriteLine("WHL");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point5[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_5[i].position) * 1000f;
            // 获取物体A和B的方向向量
            Vector3 directionA = point1[i].up; // 物体A的y轴方向
            Vector3 directionB = point5[i].up; // 物体B的y轴方向
            float angle = Vector3.Angle(directionA, directionB);
            // 打开文件并追加计算结果
            using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));

            }
        }
        using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
        {
            writer.WriteLine("WXY");

        }
        for (int i = 0; i < point1.Length; i++)
        {
            float distance = Vector3.Distance(point1[i].position, point6[i].position) * 1000f;
            float distance2 = Vector3.Distance(pointExport_1[i].position, pointExport_6[i].position) * 1000f;
            // 获取物体A和B的方向向量
            Vector3 directionA = point1[i].up; // 物体A的y轴方向
            Vector3 directionB = point6[i].up; // 物体B的y轴方向
            float angle = Vector3.Angle(directionA, directionB);
            // 打开文件并追加计算结果
            using (StreamWriter writer = new StreamWriter(filePath, true)) // 第二个参数true表示追加模式
            {
                writer.WriteLine("    " + distance.ToString("F4") + "    " + distance2.ToString("F4") + "    " + angle.ToString("F4"));
            }
        }
    }
}
