using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Unity.Collections;
using System.IO;
using Unity.Jobs;
using System.IO.Pipes;
using UnityEngine.UIElements;
using Unity.XR.CoreUtils;
using System.Linq;


public class readdata : MonoBehaviour
{

    public GameObject testBallPrefab;
    public LineRenderer lineRenderer; // 用于可视化拟合的直线
    public GameObject testball1;
    public GameObject testball2parent;
    public GameObject testball3R;
    public GameObject RealtimePointA;
    public GameObject RealtimePointB;
    public GameObject BuildingProbe;
    private Plane GuidePlaneA;                     //转换平面A
    private Plane GuidePlaneB;                     //转换平面A
    private Ray BuildingRay;        //预设射线
    private bool DriectionBuliding;
    private bool PositionBuliding;


    // 拟合后的圆心和半径
    public Transform[] dataPoints; // 这里使用Transform数组存储点
    private Vector3 circleCenter;
    private float circleRadius;
    // RANSAC算法的迭代次数和阈值
    public float fittingThreshold = 0.1f; // 拟合的阈值
    public int maxIterations = 1000; // 最大迭代次数
    private List<Vector3> points; // 存储点的三维坐标

    // 圆弧拟合结果
    private Vector3 arcCenter;
    private float arcRadius;
    private float arcStartAngle;
    private float arcEndAngle;

    void Start()
    {
        // 指定文件路径
        string filePathOfD = Application.dataPath + "/positionrecord.txt";
        //string filePathOfP = Application.dataPath + "/testP.txt";
        //string filePathOfR = Application.dataPath + "/testR.txt";
        // 检查方向文件是否存在
        if (File.Exists(filePathOfD))
        {
            // 读取文件的所有行
            string[] lines = File.ReadAllLines(filePathOfD);

            // 遍历每一行数据
            List<Transform> ballPositions_1 = new List<Transform>();
            List<Transform> ballPositions_2 = new List<Transform>();

            foreach (string line in lines)
            {
                // 按空格分割每行的数据
                string[] parts = line.Split(' ');

                // 确保至少有三个部分（x、y、z）
                if (parts.Length >= 9)
                {
                    // 解析x、y、z值
                    float rw = float.Parse(parts[0]);
                    float rx = float.Parse(parts[1]);
                    float ry = float.Parse(parts[2]);
                    float rz = float.Parse(parts[3]);
                    float px = float.Parse(parts[4]);
                    float py = float.Parse(parts[5]);
                    float pz = float.Parse(parts[6]);
                    float x = float.Parse(parts[7]);
                    float y = float.Parse(parts[8]);
                    float z = float.Parse(parts[9]);
                    // 实例化新的GameObject
                    GameObject newBall1 = Instantiate(testBallPrefab, new Vector3(px, py, pz), new Quaternion (rw, rx, ry, rz));
                    GameObject newBall2 = Instantiate(testBallPrefab, new Vector3(x, y, z), Quaternion.identity);
                    newBall2.transform.SetParent(newBall1.transform);
                    newBall1.SetActive(true); newBall2.SetActive(true);
                    //// 添加小球坐标数据到列表中
                    ballPositions_1.Add(newBall1.transform);
                    ballPositions_2.Add(newBall2.transform);
                }
                else
                {
                    Debug.LogError("无效的数据行：" + line);
                }
            }
            //for (int i =1;i< ballPositions_1.Count; i++ )
            //{
            //    ballPositions_1[i].parent = ballPositions_1[0];
            //    ballPositions_1[i].localPosition = new Vector3(0,0,0);
            //    ballPositions_1[i].localEulerAngles = new Vector3(0, 0, 0);
            //    ballPositions_1[i].parent = ballPositions_1[0].parent;
            //}

            float averageposition_1_x = ballPositions_2[0].position.x + ballPositions_2[1].position.x + ballPositions_2[2].position.x + ballPositions_2[3].position.x;
            float averageposition_1_y = ballPositions_2[0].position.y + ballPositions_2[1].position.y + ballPositions_2[2].position.y + ballPositions_2[3].position.y;
            float averageposition_1_z = ballPositions_2[0].position.z + ballPositions_2[1].position.z + ballPositions_2[2].position.z + ballPositions_2[3].position.z;
            GameObject Ball1 = Instantiate(testBallPrefab, new Vector3(averageposition_1_x/4, averageposition_1_y/4, averageposition_1_z / 4), Quaternion.identity);
            float averageposition_2_x = ballPositions_2[4].position.x + ballPositions_2[5].position.x + ballPositions_2[6].position.x + ballPositions_2[7].position.x;
            float averageposition_2_y = ballPositions_2[4].position.y + ballPositions_2[5].position.y + ballPositions_2[6].position.y + ballPositions_2[7].position.y;
            float averageposition_2_z = ballPositions_2[4].position.z + ballPositions_2[5].position.z + ballPositions_2[6].position.z + ballPositions_2[7].position.z;
            GameObject Ball2 = Instantiate(testBallPrefab, new Vector3(averageposition_2_x / 4, averageposition_2_y / 4, averageposition_2_z / 4), Quaternion.identity);
            Ball1.SetActive(true); Ball2.SetActive(true);
            //testball1.transform.position = ballPositions[ballPositions.Count - 1];

            //// 方向点拟合
            //if (ballPositions.Count >= 2)
            //{
            //    // 计算点的平均位置
            //    Vector3 meanPosition = Vector3.zero;
            //    foreach (Vector3 point in ballPositions)
            //    {
            //        meanPosition += point;
            //    }
            //    meanPosition /= ballPositions.Count;
            //    Debug.Log("均值：" + meanPosition);

            //    // 计算协方差矩阵
            //    Matrix4x4 covarianceMatrix = new Matrix4x4();
            //    foreach (Vector3 point in ballPositions)
            //    {
            //        Vector3 deviation = point - meanPosition;
            //        covarianceMatrix.m00 += deviation.x * deviation.x;
            //        covarianceMatrix.m01 += deviation.x * deviation.y;
            //        covarianceMatrix.m02 += deviation.x * deviation.z;
            //        covarianceMatrix.m11 += deviation.y * deviation.y;
            //        covarianceMatrix.m12 += deviation.y * deviation.z;
            //        covarianceMatrix.m22 += deviation.z * deviation.z;
            //    }
            //    Debug.Log("协方差矩阵：" + covarianceMatrix);

            //    // 使用PCA计算主成分（特征向量）
            //    Vector3 lineDirection = CalculatePrincipalComponent(covarianceMatrix);

            //    // 现在，lineDirection 就是估算的线段方向
            //    Debug.Log("线段方向：" + lineDirection);

            //    Vector3 axisoftestball = testball1.transform.forward;
            //    Quaternion V3DFParent3DXrotation = Quaternion.FromToRotation(axisoftestball, lineDirection);
            //    testball1.transform.rotation = V3DFParent3DXrotation * testball1.transform.rotation;     // 应用旋转

            //    lineRenderer.SetPosition(0, testball1.transform.position);
            //    lineRenderer.SetPosition(1, testball1.transform.position + lineDirection * 0.1f);
            //    lineRenderer.widthMultiplier = 0.05f;

            //}
            //else
            //{
            //    Debug.LogError("需要至少两个点来估算线段方向。");
            //}
            //Debug.Log("X坐标为：" + testball1.transform.localEulerAngles.x + " Y坐标为：" + testball1.transform.localEulerAngles.y + " Z坐标为：" + testball1.transform.localEulerAngles.z);
        }
        // 检查位置文件是否存在
        //testball1.transform.position = new Vector3(-0.148184159887502f, 0.264902368239068f, -0.078741982684277f);
        //testball1.transform.up = new Vector3(0.654905565831911f, 4.048152946140192f, -0.313445608182285f);
    //    List<Vector3> positions = new List<Vector3>();
    //    if (File.Exists(filePathOfP))
    //    {
    //        // 读取文件的所有行
    //        string[] lines = File.ReadAllLines(filePathOfP);
    //        List<Transform> origntransform = new List<Transform>();

    //        //Transform[] positions;
    //        foreach (string line in lines)
    //        {
    //            // 按空格分割每行的数据
    //            string[] parts = line.Split(' ');
    //            // 确保至少有三个部分（x、y、z）
    //            if (parts.Length >= 3)
    //            {
    //                float x1 = float.Parse(parts[4]);
    //                float y2 = float.Parse(parts[5]);
    //                float z3 = float.Parse(parts[6]);
    //                float w1 = float.Parse(parts[0]);
    //                float w2 = float.Parse(parts[1]);
    //                float w3 = float.Parse(parts[2]);
    //                float w4 = float.Parse(parts[3]);

    //                Quaternion orignroation = new Quaternion(w1, w2, w3, w4);
    //                GameObject neworign = Instantiate(testBallPrefab, new Vector3(x1, y2, z3), orignroation);
    //                // 添加原点坐标数据到列表中
    //                origntransform.Add(neworign.transform);
    //                positions.Add(neworign.transform.position);
    //            }
    //            else
    //            {
    //                Debug.LogError("无效的数据行：" + line);
    //            }
    //            dataPoints = new Transform[origntransform.Count]; 
    //            for (int i = 0; i < origntransform.Count; i++)
    //            {
    //                dataPoints[i] = origntransform[i];
    //            }
    //        }
    //        // 初始化数据点列表
    //        points = new List<Vector3>();

    //        // 从Transform组件中提取点的坐标
    //        foreach (Transform point in dataPoints)
    //        {
    //            points.Add(point.position);
    //        }
    //        testball2parent.transform.position = origntransform[origntransform.Count / 2].position;
    //        testball2parent.transform.rotation = origntransform[origntransform.Count / 2].rotation;
    //        testball1.transform.parent = testball2parent.transform;
    //    }

    //    List<Vector3> Rpositions = new List<Vector3>();
    //    if (File.Exists(filePathOfR))
    //    {
    //        // 读取文件的所有行
    //        string[] lines = File.ReadAllLines(filePathOfR);
    //        List<Transform> origntransform = new List<Transform>();

    //        //Transform[] positions;
    //        foreach (string line in lines)
    //        {
    //            // 按空格分割每行的数据
    //            string[] parts = line.Split(' ');
    //            // 确保至少有三个部分（x、y、z）
    //            if (parts.Length >= 3)
    //            {
    //                float x1 = float.Parse(parts[4]);
    //                float y2 = float.Parse(parts[5]);
    //                float z3 = float.Parse(parts[6]);
    //                float w1 = float.Parse(parts[0]);
    //                float w2 = float.Parse(parts[1]);
    //                float w3 = float.Parse(parts[2]);
    //                float w4 = float.Parse(parts[3]);

    //                Quaternion orignroation = new Quaternion(w1, w2, w3, w4);
    //                GameObject neworign = Instantiate(testBallPrefab, new Vector3(x1, y2, z3), orignroation);
    //                // 添加原点坐标数据到列表中
    //                origntransform.Add(neworign.transform);
    //                Rpositions.Add(neworign.transform.position);
    //            }
    //            else
    //            {
    //                Debug.LogError("无效的数据行：" + line);
    //            }
    //        }
    //        testball3R.transform.position = new Vector3(-0.148184159887502f, 0.264902368239068f, -0.078741982684277f);
    //        testball3R.transform.up = new Vector3(0.654905565831911f, 4.048152946140192f, -0.313445608182285f);
            
    //    }

    //    // 使用PCA计算主成分（特征向量）能用
    //    Vector3 CalculatePrincipalComponent(Matrix4x4 covarianceMatrix)
    //{
    //    Vector3[] eigenVectors = new Vector3[3];
    //    float[] eigenValues = new float[3];

    //    // 计算特征值和特征向量
    //    for (int i = 0; i < 3; i++)
    //    {
    //        eigenVectors[i] = covarianceMatrix.GetRow(i).normalized; // 使用GetRow获取行
    //        eigenValues[i] = covarianceMatrix.GetRow(i).magnitude;
    //    }

    //    // 找到最大特征值对应的特征向量（主成分）
    //    int maxEigenValueIndex = 0;
    //    for (int i = 1; i < 3; i++)
    //    {
    //        if (eigenValues[i] > eigenValues[maxEigenValueIndex])
    //        {
    //            maxEigenValueIndex = i;
    //        }
    //    }
    //    return eigenVectors[maxEigenValueIndex];
    //}

}
    void Update()
    {
        if (DriectionBuliding) 
        {
            BuildingRay.origin = BuildingProbe.transform.position;//B点作为射出原点
            BuildingRay.direction = BuildingProbe.transform.up;//转换方向从B-A

            //创建平面，平面A：经过实时点A与射线垂直平面，平面B：经过实时点B与射线垂直平面
            GuidePlaneA = new Plane(RealtimePointA.transform.up, RealtimePointA.transform.position);
            float rayDistanceofA;//返回A平面与射线交点
            if (GuidePlaneA.Raycast(BuildingRay, out rayDistanceofA))
            {
                Vector3 intersectionPointofA = BuildingRay.GetPoint(rayDistanceofA);
                RealtimePointA.transform.position = intersectionPointofA;
                Vector3 RealtimePointAaxisUP = RealtimePointA.transform.up;
                Quaternion Buildeingrotation = Quaternion.FromToRotation(RealtimePointAaxisUP, -BuildingProbe.transform.up);
                RealtimePointA.transform.rotation = Buildeingrotation * RealtimePointA.transform.rotation;     // 应用旋转
                DriectionBuliding = false;
            }
            //BuildingRay.origin = testball3R.transform.position;//B点作为射出原点
            //BuildingRay.direction = -testball3R.transform.up;//转换方向从B-A

            ////创建平面，平面A：经过实时点A与射线垂直平面，平面B：经过实时点B与射线垂直平面
            //GuidePlaneA = new Plane(BuildingProbe.transform.up, BuildingProbe.transform.position);
            //float rayDistanceofA;//返回A平面与射线交点
            //if (GuidePlaneA.Raycast(BuildingRay, out rayDistanceofA))
            //{
            //    Vector3 intersectionPointofA = BuildingRay.GetPoint(rayDistanceofA);
            //    BuildingProbe.transform.position = intersectionPointofA;
            //    Vector3 RealtimePointAaxisUP = RealtimePointA.transform.up;
            //    Quaternion Buildeingrotation = Quaternion.FromToRotation(RealtimePointAaxisUP, testball3R.transform.up);
            //    BuildingProbe.transform.rotation = Buildeingrotation * BuildingProbe.transform.rotation;     // 应用旋转
            //    DriectionBuliding = false;
            //}
        }
        if (PositionBuliding)
        {
            RealtimePointA.transform.position =new Vector3(RealtimePointA.transform.position.x, BuildingProbe.transform.position.y, RealtimePointA.transform.position.z);
            PositionBuliding = false;
        }
    }
   public void DriectionBuilding ()
    {
        DriectionBuliding =true;
    }
    public void PositionBuilding()
    {
        PositionBuliding = true;
    }
}