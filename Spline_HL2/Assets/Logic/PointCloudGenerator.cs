using System;
using System.Collections.Generic;
using System.Drawing;
using UnityEngine;
using UnityEngine.XR.ARSubsystems;
using Unity.Mathematics;
using MathNet.Numerics.LinearAlgebra;


public class PointCloudGenerator : MonoBehaviour
{
    // ģ�� A
    public GameObject modelA;
    public GameObject ballA;
    public GameObject pointparent;
    private Vector3[] pointCloudData;
    private Vector3[] pointCloud;
    private Transform[] pointposition;
    // ����������
    public int maxIterations = 100;

    // ����ֹͣ��������ֵ
    public float threshold = 0.001f;
    // ���Ƶ�����
    public int pointCount = 1000;//ģ�͵�������
    public float noiseThreshold = 0.1f; // �ɼ�����-������ֵ
    public int sampleRate = 2; // �ɼ�����-������
    void Start()
    {
        pointCloud = new Vector3[pointCount];
        pointposition = new Transform[pointCount];
        // ��ȡ����A������������
        Transform[] children = pointparent.GetComponentsInChildren<Transform>();
        pointCloudData = new Vector3[children.Length];
        for (int i = 0; i < children.Length; i++) 
        {
            pointCloudData[i] = children[i].position;
        }
        // ������׼����
        AlignModelToPointCloud();
    }

    // ���ɵ���
    public Vector3[] GeneratePointCloud()
    {

        // ��ȡģ��A��MeshFilter���
        MeshFilter meshFilter = modelA.GetComponent<MeshFilter>();

        // ���MeshFilter����Ƿ����
        if (meshFilter != null && meshFilter.mesh != null)
        {
            // ��������ɶ�д
            meshFilter.mesh.MarkDynamic();
        }
        else
        {
            Debug.LogError("Mesh or MeshFilter not found on the provided GameObject.");
        }

        // ��ȡģ�� A ��Mesh
        Mesh mesh = modelA.GetComponent<MeshFilter>().mesh;
        Debug.Log("��ʼ");
        // ��ģ�� A �Ķ��������ѡ���
        //for (int i = 0; i < pointCount; i++)
        //{
        //    //int randomIndex = Random.Range(0, mesh.vertices.Length);
        //    pointCloud[i] = modelA.transform.TransformPoint(mesh.vertices[randomIndex]);
        //}
        return pointCloud;
    }
    //��������
    void FilterNoise(Vector3[] pointCloud)
    {
        // �����������˺�ĵ�������
        Vector3[] filteredPointCloud = new Vector3[pointCloud.Length];
        int filteredIndex = 0;

        // �������ƣ�������������Ӱ��ĵ�
        for (int i = 0; i < pointCloud.Length; i++)
        {
            if (pointCloud[i].magnitude > noiseThreshold)
            {
                filteredPointCloud[filteredIndex] = pointCloud[i];
                filteredIndex++;
            }
        }

        // ���������С��ƥ����˺�ĵ������
        System.Array.Resize(ref filteredPointCloud, filteredIndex);

        // ���µ�������
        pointCloud = filteredPointCloud;
    }
    // ���Ʋ�����ֵ
    void SamplePointCloud(Vector3[] pointCloud)
    {
        // ����������ĵ�������
        Vector3[] sampledPointCloud = new Vector3[pointCloud.Length / sampleRate];
        int sampledIndex = 0;

        // ���ղ����ʴӵ����г�ȡ��
        for (int i = 0; i < pointCloud.Length; i += sampleRate)
        {
            sampledPointCloud[sampledIndex] = pointCloud[i];
            sampledIndex++;
        }

        // ���µ�������
        pointCloud = sampledPointCloud;
    }
    //���ӻ�����
    void VisualizePointCloud(Vector3[] points)
    {
        for (int i = 0; i < points.Length; i++)
        {
            Debug.DrawRay(points[i], Vector3.up * 0.1f, UnityEngine.Color.red,1f); // ���ӻ�����Ϊ��ɫ����
            pointposition[i] = Instantiate(ballA, points[i], Quaternion.identity).transform;
        }
    }
    void AlignModelToPointCloud()
    {
        // ����ģ��A��ԭʼλ��
        Vector3 originalPosition = modelA.transform.position;
        Quaternion originalRotation = modelA.transform.rotation;
        Vector3 originalScale = modelA.transform.localScale;

        // ����������ת��ΪList���Ա���ٲ���
        List<Vector3> pointCloudList = new List<Vector3>(pointCloudData);

        // ����
        for (int i = 0; i < maxIterations; i++)
        {
            // ����ģ��A����Ƶı任
            Matrix4x4 transformation = CalculateTransformation(pointCloudList);

            // Ӧ�ñ任��ģ��A
            ApplyTransformation(transformation);

            // ����Ƿ�����ֹͣ����
            if (CheckStopCondition(transformation))
            {
                Debug.Log("Alignment converged after " + i + " iterations.");
                break;
            }
        }
    }

    Matrix4x4 CalculateTransformation(List<Vector3> pointCloudList)
    {
        // ��ģ��A�Ķ��������������ĵ�������
        List<Vector3> modelPoints = GetModelPoints(modelA);
        List<Vector3> closestPoints = FindClosestPoints(modelPoints, pointCloudList);

        // ����任����
        Matrix4x4 transformation = ICP(modelPoints, closestPoints);

        return transformation;
    }

    List<Vector3> GetModelPoints(GameObject obj)
    {
        // ��ȡģ��A�Ķ���
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
        // ��ģ��A�ϵ�ÿ���㣬�ҵ������о�������ĵ�
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
        // �������ĵ�
        Vector3 modelCenter = CalculateCenter(modelPoints);
        Vector3 closestCenter = CalculateCenter(closestPoints);

        // ����ȥ���Ļ�������
        List<Vector3> modelNormalized = NormalizePoints(modelPoints, modelCenter);
        List<Vector3> closestNormalized = NormalizePoints(closestPoints, closestCenter);

        // ������ת�����ƽ������
        Matrix4x4 rotationMatrix = CalculateRotationMatrix(modelNormalized, closestNormalized);
        Vector3 translationVector = closestCenter - modelCenter;

        // �����任����
        Matrix4x4 transformation = Matrix4x4.Translate(translationVector) * rotationMatrix;

        return transformation;
    }

    Vector3 CalculateCenter(List<Vector3> points)
    {
        // ����㼯�����ĵ�
        Vector3 center = Vector3.zero;
        foreach (Vector3 point in points)
        {
            center += point;
        }
        return center / points.Count;
    }

    List<Vector3> NormalizePoints(List<Vector3> points, Vector3 center)
    {
        // ȥ���Ļ�
        List<Vector3> normalizedPoints = new List<Vector3>();
        foreach (Vector3 point in points)
        {
            normalizedPoints.Add(point - center);
        }
        return normalizedPoints;
    }

    Matrix4x4 CalculateRotationMatrix(List<Vector3> modelPoints, List<Vector3> closestPoints)
    {
        // ת��ΪMathNet Numerics��������
        Vector<double>[] modelVectors = modelPoints.ConvertAll(p => Vector<double>.Build.DenseOfArray(new double[] { p.x, p.y, p.z })).ToArray();
        Vector<double>[] closestVectors = closestPoints.ConvertAll(p => Vector<double>.Build.DenseOfArray(new double[] { p.x, p.y, p.z })).ToArray();

        // ������ת����
        Matrix<double> modelMatrix = Matrix<double>.Build.DenseOfColumnVectors(modelVectors);
        Matrix<double> closestMatrix = Matrix<double>.Build.DenseOfColumnVectors(closestVectors);

        var svd = modelMatrix.Svd();
        Matrix<double> rotationMatrix = svd.U * svd.VT;

        // ת��ΪUnity��Matrix4x4
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
        // ���任����Ӧ�õ�ģ��A��
        modelA.transform.position = transformation.GetColumn(3);
        modelA.transform.rotation = Quaternion.LookRotation(transformation.GetColumn(2), transformation.GetColumn(1));
        modelA.transform.localScale = transformation.lossyScale;
    }

    bool CheckStopCondition(Matrix4x4 transformation)
    {
        // ����ģ��A�ϵ����ж���������������ľ���
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

        // ����ƽ������
        float averageDistance = totalDistance / count;

        // ����Ƿ�����ֹͣ����
        return averageDistance < threshold;
    }

}
