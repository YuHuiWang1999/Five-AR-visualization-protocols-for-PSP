using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class NetTest : MonoBehaviour
{
    public GameObject RandomPosition;
    public GameObject PointA;//旋转起点
    public GameObject DisplayPlane;
    public GameObject Display3DX;
    public GameObject Display3DZ;
    private GameObject[] CopyObjectList;//要复制的物体列表
    private List<GameObject> CopiesListX = new List<GameObject>();  private List<GameObject> CopiesListZ = new List<GameObject>();
    private bool V3DFcopyiscomplete = false;
    private bool V3DFistimetoupdate = false;
    private bool V3DFisactive = false;
    void Update() 
    {
        V3DFisactive = RandomPosition.GetComponent<RandomPosition>().tocopy3DF();
        if (V3DFisactive)
        {
            CopyObjectList = RandomPosition.GetComponent<RandomPosition>().Copyobjs();
            if (!V3DFcopyiscomplete)
            {
                V3DFcopyiscomplete = true;
                StartCoroutine(buttonchge(CopyObjectList));
            }
            if (V3DFistimetoupdate)
            {

                int[] index = new int[] {4,5,9,10};
                for (int i = 0;i< index.Length;i++) 
                {
                    (Vector3 newPositionX, Quaternion newRotationX) = TransformObjectsInList(PointA, Display3DX, CopyObjectList[index[i]], false);
                    CopiesListX[index[i]].transform.position = newPositionX;
                    CopiesListX[index[i]].transform.rotation = newRotationX;
                    (Vector3 newPositionZ, Quaternion newRotationZ) = TransformObjectsInList(PointA, Display3DZ, CopyObjectList[index[i]], true);
                    CopiesListZ[index[i]].transform.position = newPositionZ;
                    CopiesListZ[index[i]].transform.rotation = newRotationZ;
                }
            }
        }
        else 
        {
            V3DFcopyiscomplete = false;
            V3DFistimetoupdate = false;
        }

    }

    public IEnumerator buttonchge(GameObject[] PointC)//要转换的点
    {
        Debug.Log("执行了一次");
        yield return new WaitForSeconds(0.1f); // 延迟1秒
        GameObject[] copiesX = new GameObject[PointC.Length];
        GameObject[] copiesZ = new GameObject[PointC.Length];
        Display3DX.SetActive(true); Display3DZ.SetActive(true);
        Display3DX.transform.position = DisplayPlane.transform.position + DisplayPlane.transform.forward * 0.2f + DisplayPlane.transform.up * 0.1f - DisplayPlane.transform.right * 0.1f;
        Display3DZ.transform.position = DisplayPlane.transform.position + DisplayPlane.transform.forward * 0.2f + DisplayPlane.transform.up * 0.1f + DisplayPlane.transform.right * 0.1f;
        PointA.transform.rotation = DisplayPlane.transform.rotation;
        PointA.transform.position = PointC[0].transform.position;
        Display3DX.transform.rotation = PointA.transform.rotation;
        Display3DZ.transform.rotation = PointA.transform.rotation;
        for (int i = 0; i < PointC.Length; i++)
        {
            PointC[i].SetActive(true);
            copiesX[i] = Instantiate(PointC[i]);
            copiesX[i].transform.position = PointC[i].transform.position;
            copiesX[i].transform.rotation = PointC[i].transform.rotation;
            copiesX[i].transform.SetParent(Display3DX.transform);
            copiesX[i].transform.localScale = Display3DX.transform.localScale;
            CopiesListX.Add(copiesX[i]);
            // 调用函数并获取返回值
            bool needrote = false;
            (Vector3 newPosition, Quaternion newRotation) = TransformObjectsInList(PointA, Display3DX, PointC[i], needrote);
            CopiesListX[i].transform.position = newPosition;
            CopiesListX[i].transform.rotation = newRotation;
        }
        for (int i = 0; i < PointC.Length; i++)
        {
            PointC[i].SetActive(true);
            copiesZ[i] = Instantiate(PointC[i]);
            copiesZ[i].transform.position = PointC[i].transform.position;
            copiesZ[i].transform.rotation = PointC[i].transform.rotation;
            copiesZ[i].transform.SetParent(Display3DZ.transform);
            copiesZ[i].transform.localScale = Display3DZ.transform.localScale;
            CopiesListZ.Add(copiesZ[i]);
            // 调用函数并获取返回值
            bool needrote = true;
            (Vector3 newPosition, Quaternion newRotation) = TransformObjectsInList(PointA, Display3DZ, PointC[i], needrote);
            CopiesListZ[i].transform.position = newPosition;
            CopiesListZ[i].transform.rotation = newRotation;

        }
        V3DFistimetoupdate = true;

    }
    // 这个方法将C中的物体从A位置旋转和平移到B位置，并应用变换矩阵到这些物体
    public static (Vector3, Quaternion) TransformObjectsInList(GameObject A, GameObject B, GameObject C,bool needrote)
    {
        // 获取A和B的Transform组件
        Transform transformA = A.transform;
        Transform transformB = B.transform;
        Quaternion RotationBB1 = Quaternion.identity;
        if (needrote)
        {
            RotationBB1 = Quaternion.FromToRotation(transformB.forward, transformB.right);
        }
        else 
        {
            RotationBB1 = Quaternion.FromToRotation(transformB.forward, transformB.forward);
        }
        // 计算从A到B的变换矩阵
        Matrix4x4 matrix = transformB.worldToLocalMatrix * transformA.localToWorldMatrix;
        // 遍历列表C中的物体，并应用变换
        Quaternion RoationC = C.transform.rotation;
        // 应用变换矩阵
        //Vector3 newPosition = matrix.MultiplyPoint(PositionC);
        Vector3 newPosition1 = transformB.position + (C.transform.position - transformA.position);//相对位置
        Quaternion newRotation1 = matrix.rotation * RoationC;

        // 获取物体C相对于物体A的位置
        Vector3 relativePosition = newPosition1 - transformB.position;
        // 绕UP轴旋转90度
        relativePosition = RotationBB1 * relativePosition;
        // 更新物体C的位置
        Vector3 newPosition2 = transformB.position + relativePosition;
        // 更新物体C的旋转
        Quaternion newRotation2 = RotationBB1 * newRotation1;

        return (newPosition2, newRotation2);
    }

    public bool V3DFcopycomplete()
    {
        return V3DFistimetoupdate;//
    }
    public List<GameObject> ListX()
    {
        return CopiesListX;//
    }
    public List<GameObject> ListZ()
    {
        return CopiesListZ;//
    }
}


