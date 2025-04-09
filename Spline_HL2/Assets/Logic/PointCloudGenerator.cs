using System;
using System.Collections.Generic;
using System.Drawing;
using UnityEngine;
using UnityEngine.XR.ARSubsystems;
using Unity.Mathematics;
using MathNet.Numerics.LinearAlgebra;


public class PointCloudGenerator : MonoBehaviour
{
    // 模型 A
    public GameObject modelA;
    public GameObject ballA;
    public GameObject pointparent;
    private Vector3[] pointCloudData;
    private Vector3[] pointCloud;
    private Transform[] pointposition;
    // 最大迭代次数
    public int maxIterations = 100;

    // 迭代停止条件：阈值
    public float threshold = 0.001f;
    // 点云的数量
    public int pointCount = 1000;//模型点云数量
    public float noiseThreshold = 0.1f; // 采集点云-噪声阈值
    public int sampleRate = 2; // 采集点云-采样率
    void Start()
    {
        pointCloud = new Vector3[pointCount];
        pointposition = new Transform[pointCount];
        // 获取物体A下所有子物体
        Transform[] children = pointparent.GetComponentsInChildren<Transform>();
        pointCloudData = new Vector3[children.Length];
        for (int i = 0; i < children.Length; i++) 
        {
            pointCloudData[i] = children[i].position;
        }
        // 调用配准方法
        AlignModelToPointCloud();
    }

    // 生成点云
    public Vector3[] GeneratePointCloud()
    {

        // 获取模型A的MeshFilter组件
        MeshFilter meshFilter = modelA.GetComponent<MeshFilter>();

        // 检查MeshFilter组件是否存在
        if (meshFilter != null && meshFilter.mesh != null)
        {
            // 设置网格可读写
            meshFilter.mesh.MarkDynamic();
        }
        else
        {
            Debug.LogError("Mesh or MeshFilter not found on the provided GameObject.");
        }

        // 获取模型 A 的Mesh
        Mesh mesh = modelA.GetComponent<MeshFilter>().mesh;
        Debug.Log("开始");
        // 从模型 A 的顶点中随机选择点
        //for (int i = 0; i < pointCount; i++)
        //{
        //    //int randomIndex = Random.Range(0, mesh.vertices.Length);
        //    pointCloud[i] = modelA.transform.TransformPoint(mesh.vertices[randomIndex]);
        //}
        return pointCloud;
    }
    //噪声过滤
    void FilterNoise(Vector3[] pointCloud)
    {
        // 创建噪声过滤后的点云数组
        Vector3[] filteredPointCloud = new Vector3[pointCloud.Length];
        int filteredIndex = 0;

        // 迭代点云，保留不受噪声影响的点
        for (int i = 0; i < pointCloud.Length; i++)
        {
            if (pointCloud[i].magnitude > noiseThreshold)
            {
                filteredPointCloud[filteredIndex] = pointCloud[i];
                filteredIndex++;
            }
        }

        // 调整数组大小以匹配过滤后的点的数量
        System.Array.Resize(ref filteredPointCloud, filteredIndex);

        // 更新点云数据
        pointCloud = filteredPointCloud;
    }
    // 点云采样插值
    void SamplePointCloud(Vector3[] pointCloud)
    {
        // 创建采样后的点云数组
        Vector3[] sampledPointCloud = new Vector3[pointCloud.Length / sampleRate];
        int sampledIndex = 0;

        // 按照采样率从点云中抽取点
        for (int i = 0; i < pointCloud.Length; i += sampleRate)
        {
            sampledPointCloud[sampledIndex] = pointCloud[i];
            sampledIndex++;
        }

        // 更新点云数据
        pointCloud = sampledPointCloud;
    }
    //可视化点云
    void VisualizePointCloud(Vector3[] points)
    {
        for (int i = 0; i < points.Length; i++)
        {
            Debug.DrawRay(points[i], Vector3.up * 0.1f, UnityEngine.Color.red,1f); // 可视化点云为红色射线
            pointposition[i] = Instantiate(ballA, points[i], Quaternion.identity).transform;
        }
    }
    void AlignModelToPointCloud()
    {
        // 拷贝模型A的原始位置
        Vector3 originalPosition = modelA.transform.position;
        Quaternion originalRotation = modelA.transform.rotation;
        Vector3 originalScale = modelA.transform.localScale;

        // 将点云数据转换为List，以便快速查找
        List<Vector3> pointCloudList = new List<Vector3>(pointCloudData);

        // 迭代
        for (int i = 0; i < maxIterations; i++)
        {
            // 计算模型A与点云的变换
            Matrix4x4 transformation = CalculateTransformation(pointCloudList);

            // 应用变换到模型A
            ApplyTransformation(transformation);

            // 检查是否满足停止条件
            if (CheckStopCondition(transformation))
            {
                Debug.Log("Alignment converged after " + i + " iterations.");
                break;
            }
        }
    }

    Matrix4x4 CalculateTransformation(List<Vector3> pointCloudList)
    {
        // 将模型A的顶点与点云中最近的点进行配对
        List<Vector3> modelPoints = GetModelPoints(modelA);
        List<Vector3> closestPoints = FindClosestPoints(modelPoints, pointCloudList);

        // 计算变换矩阵
        Matrix4x4 transformation = ICP(modelPoints, closestPoints);

        return transformation;
    }

    List<Vector3> GetModelPoints(GameObject obj)
    {
        // 获取模型A的顶点
        List<Vector3> modelPoints = new List<Vector3>();
        MeshFilter meshFilter = obj.GetComponent<MeshFilter>();
        foreach (Vector3 vertex in meshFilter.mesh.vertices)
        {
            modelPoints.Add(obj.transform.TransformPoint(vertex));
        }
        return modelPoints;
    }

    List<Vector3> FindClosestPoints(List<Vector3> modelPoints, List<Vector3> pointCloudPoints)
    {
        // 对模型A上的每个点，找到点云中距离最近的点
        List<Vector3> closestPoints = new List<Vector3>();
        foreach (Vector3 modelPoint in modelPoints)
        {
            float minDistance = float.MaxValue;
            Vector3 closestPoint = Vector3.zero;
            foreach (Vector3 pointCloudPoint in pointCloudPoints)
            {
                float distance = Vector3.Distance(modelPoint, pointCloudPoint);
                if (distance < minDistance)
                {
                    minDistance = distance;
                    closestPoint = pointCloudPoint;
                }
            }
            closestPoints.Add(closestPoint);
        }
        return closestPoints;
    }

    Matrix4x4 ICP(List<Vector3> modelPoints, List<Vector3> closestPoints)
    {
        // 计算中心点
        Vector3 modelCenter = CalculateCenter(modelPoints);
        Vector3 closestCenter = CalculateCenter(closestPoints);

        // 计算去中心化的坐标
        List<Vector3> modelNormalized = NormalizePoints(modelPoints, modelCenter);
        List<Vector3> closestNormalized = NormalizePoints(closestPoints, closestCenter);

        // 计算旋转矩阵和平移向量
        Matrix4x4 rotationMatrix = CalculateRotationMatrix(modelNormalized, closestNormalized);
        Vector3 translationVector = closestCenter - modelCenter;

        // 构建变换矩阵
        Matrix4x4 transformation = Matrix4x4.Translate(translationVector) * rotationMatrix;

        return transformation;
    }

    Vector3 CalculateCenter(List<Vector3> points)
    {
        // 计算点集的中心点
        Vector3 center = Vector3.zero;
        foreach (Vector3 point in points)
        {
            center += point;
        }
        return center / points.Count;
    }

    List<Vector3> NormalizePoints(List<Vector3> points, Vector3 center)
    {
        // 去中心化
        List<Vector3> normalizedPoints = new List<Vector3>();
        foreach (Vector3 point in points)
        {
            normalizedPoints.Add(point - center);
        }
        return normalizedPoints;
    }

    Matrix4x4 CalculateRotationMatrix(List<Vector3> modelPoints, List<Vector3> closestPoints)
    {
        // 转换为MathNet Numerics向量类型
        Vector<double>[] modelVectors = modelPoints.ConvertAll(p => Vector<double>.Build.DenseOfArray(new double[] { p.x, p.y, p.z })).ToArray();
        Vector<double>[] closestVectors = closestPoints.ConvertAll(p => Vector<double>.Build.DenseOfArray(new double[] { p.x, p.y, p.z })).ToArray();

        // 计算旋转矩阵
        Matrix<double> modelMatrix = Matrix<double>.Build.DenseOfColumnVectors(modelVectors);
        Matrix<double> closestMatrix = Matrix<double>.Build.DenseOfColumnVectors(closestVectors);

        var svd = modelMatrix.Svd();
        Matrix<double> rotationMatrix = svd.U * svd.VT;

        // 转换为Unity的Matrix4x4
        Matrix4x4 unityRotationMatrix = new Matrix4x4(
            new Vector4((float)rotationMatrix[0, 0], (float)rotationMatrix[0, 1], (float)rotationMatrix[0, 2], 0),
            new Vector4((float)rotationMatrix[1, 0], (float)rotationMatrix[1, 1], (float)rotationMatrix[1, 2], 0),
            new Vector4((float)rotationMatrix[2, 0], (float)rotationMatrix[2, 1], (float)rotationMatrix[2, 2], 0),
            new Vector4(0, 0, 0, 1)
        );

        return unityRotationMatrix;
    }

    void ApplyTransformation(Matrix4x4 transformation)
    {
        // 将变换矩阵应用到模型A上
        modelA.transform.position = transformation.GetColumn(3);
        modelA.transform.rotation = Quaternion.LookRotation(transformation.GetColumn(2), transformation.GetColumn(1));
        modelA.transform.localScale = transformation.lossyScale;
    }

    bool CheckStopCondition(Matrix4x4 transformation)
    {
        // 计算模型A上的所有顶点与点云上最近点的距离
        float totalDistance = 0;
        int count = 0;
        List<Vector3> modelPoints = GetModelPoints(modelA);
        foreach (Vector3 modelPoint in modelPoints)
        {
            float minDistance = float.MaxValue;
            foreach (Vector3 pointCloudPoint in pointCloudData)
            {
                float distance = Vector3.Distance(modelPoint, pointCloudPoint);
                if (distance < minDistance)
                {
                    minDistance = distance;
                }
            }
            totalDistance += minDistance;
            count++;
        }

        // 计算平均距离
        float averageDistance = totalDistance / count;

        // 检查是否满足停止条件
        return averageDistance < threshold;
    }

}
