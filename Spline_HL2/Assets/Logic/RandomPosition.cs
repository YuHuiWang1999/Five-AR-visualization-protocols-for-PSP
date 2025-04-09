using Microsoft.MixedReality.Toolkit.Input;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem.HID;
using UnityEngine.UIElements;
using static UnityEngine.GraphicsBuffer;
using Image = UnityEngine.UI.Image;

public class RandomPosition : MonoBehaviour
{
    //----------------------------- 总控制 ----------------------------------------
    public Collider ArrivedBallA0;
    public Collider ArrivedBallB0;
    public Collider ArrivedBallC0;
    public Collider ArrivedBallA1;
    public Collider ArrivedBallB1;
    public Collider ArrivedBallC1;
    public Collider DrillGuidge;
    public GameObject Drillduidge;
    public GameObject TestBall3;//接收NID数据
    public GameObject TestBall4;//接收NID数据
    public GameObject TestBall1;//接收NID数据
    public GameObject TestBall2;//接收NID数据
    private List<int> FirstSelectedIndexes01 = new List<int>();
    private List<int> Indexes01 = new List<int>();
    private List<int> Indexes02 = new List<int>();
    private bool isindexout = false;
    private Dictionary<int, int> indexNumberMap = new Dictionary<int, int>();
    private int currentIndex;//选中物体在现列表中index
    private int originalIndex;//选出物体在源列表中index
    private bool[] VisualizationEnabled = new bool[10];//用于可视化方案切换后update中函数的启用切换
    private int LineobjectIndex;
    private bool listHasElements = true;
    private bool ControllerV3;
    private bool ControllerV4;
    private bool V3DFisactive; private bool V3DFcopyiscomplete; private bool V3DFcopyisupdate;
    private bool V3DYisactive;
    private bool isbegin;
    private bool isright;
    private bool ballisarrived;
    private bool needroate;
    private int  submitnumber;
    private bool submitisrote;
    private bool notneedroate;
    private bool isroate;
    private bool isactive;
    public GameObject communication;//接收NID数据
    private Transform mainCameraTransform;   //主相机
    private float PreviousNumber;
    private float Number;
    public GameObject whole;
    private Quaternion initialRotation;  // 初始旋转
    private GameObject ParentPbject;
    //------------发送的数据---------
    private float StartTimeStamp;
    private float EndTimeStamp;
    private Vector3 VP;            // 预设
    private Vector3 VR;            // 实时    
    private float MedianOffset;            // 实时    
    private float CoronalOffset;            // 实时    
    private float TransverseOffset;            // 实时    
    private float angleDegrees;
    private float ExportOffset;            
    private float EnterOffset;            
    private MeshCollider [] BoneSurface;            // 碰撞皮肤表面

    //------------ 总体预设 ----------------
    public Renderer [] expermentstateimages;             // 显示实验几的图像
    public Material[] expermentstatematerials;             // 显示实验几的图像
    public Material plane3material;             // 显示实验几的图像
    public Texture [] plane3background;             // 显示实验几的图像
    private int statenumber = 0;                    // 记录实验几的数据
    public Transform[] positions;                // 预设10个针位置列表
    public GameObject[] SpineModels;             // 显示脊骨模型
    public GameObject[] otherSpineModels;        // 脊骨模型
    private GameObject PresetPointA;             //当前预设射线点A头
    private GameObject PresetPointB;             //当前预设射线点B尾
    private Vector3 PresetLineDirection = new Vector3(0, 0, 0);//当前预设射线方向
    public float PresetLineLength;               // 预设线长度
    private LineRenderer PresetLine3D;           //当前预设渲染3D线段
    public GameObject presetnail;                // 预设钉物体
    private Vector3 Realtimeendnail;                // 预设钉物体
    public MeshCollider skinSurface;            // 碰撞皮肤表面
    private Vector3 CastPointYaxis;                    // 投影深度图像
    private Vector3 skinbeginPosition;                    // 投影深度图像
    private GameObject PresetCastPoint;             // 碰撞皮肤表面产生的点
    private GameObject PresetCastBone;             // 碰撞皮肤表面产生的点
    private GameObject RealtimeCastBone;             // 碰撞皮肤表面产生的点
    private GameObject LineObjects;
    private bool isHande;
    private float [] begindate;
    private GameObject RealtimeGuidePointA;              // 数字孪生2旋转后的点B
    private GameObject RealtimeGuidePointB;              // 数字孪生2旋转后的点B
    private GameObject RealtimeGuidePointC;              // 数字孪生2旋转后的点B
    private Vector3 PresetcastPosition; // 实时轨迹延长线段---of z平面 
    private Vector3 PresetcastNormal; // 实时轨迹延长线段---of z平面
    private float PresetInsertionDepth;
    private float RealtimeInsertionDepth;
    private float InsertionPercentage;
    public GameObject roteimage;
    public GameObject aciverote;
    public TextMeshPro rotetextmesh;
    private float rotationDirection = 1f;  // 旋转方向（正向：1，反向：-1）
    private bool pictureDriection = true;            // 孪生2旋转过的角度
    private int ABNumber0;                    // 
    private int ABNumber1;                    // 
    //------------ 总体实时 ----------------
    private LineRenderer RealtimeGuideLine3D;//实时渲染3D引导器线段
    private LineRenderer RealtimeNoRGuideLine3D1;//实时渲染非原位3D引导器线段1
    private LineRenderer RealtimeNoRGuideLine3D2;//实时渲染非原位3D引导器线段2
    public GameObject DrillPointA;//钻头实时A
    private Ray PresetRay;        //预设射线
    private Ray DrillRay;        //预设射线
    private Ray RealtimeRay2;        //预设射线
    private Ray RealtimeRay;        //预设射线
    private RaycastHit skinRealtimeRayHit;    // 定义一个 RaycastHit 对象，用于保存碰撞信息
    private float EntryEviation; //入口偏差
    private float ExportEviation; //出口偏差
    private float AngleEviation1; //出口偏差
    private float AngleEviation2; //出口偏差
    private float InsertmaxValue;  // 最大值
    private Vector3 X2DinitialPosition; //X初始位置
    private Vector3 Z2DinitialPosition; //Z初始位置

    public float rotationSpeed = 90f;  // 旋转速度（每秒旋转的角度）
    public float rotationDuration = 1f;  // 每个方向的旋转时间
    private Quaternion startRotation;  // 起始旋转
    private Quaternion targetRotation;  // 目标旋转
    private float rotationTimer;  // 旋转计时器

    //----------- 2D非原位显示 ----------
    public MeshRenderer[] planesofx;                       // 平面数组网格渲染，决定哪个x平面  被显示
    public MeshRenderer[] planesofz;                       // 平面数组网格渲染，决定哪个z平面被显示
    public MeshRenderer[] planesofy;                       // 平面数组网格渲染，决定哪个z平面被显示
    public GameObject CameraOfz;                           // z轴截面相机
    public GameObject CameraOfx;                           // x轴截面相机
    public GameObject[] DisplayPlane;                       // 2D非原X显示平面位置
    public GameObject[] V2DFImage;                    // 投影深度图像

    private Transform selectedPlaneofxTransform;          //得到被选中的这个平面同时它也是要被投影的平面的位置----x轴平面
    private Transform selectedPlaneofzTransform;          //得到被选中的这个平面同时它也是要被投影的平面的位置----z轴平面
    private LineRenderer RealtimeGuideLine2DX;//实时渲染X平面2D引导器线段
    private LineRenderer RealtimeGuideLine2DZ;//实时渲染Z平面2D引导器线段
    private LineRenderer PresetLine2DX;//预设渲染X平面2D线段
    private LineRenderer PresetLine2DZ;//预设渲染Z平面2D线段
    private int xindexofdisplayplane;//要显示X平面的索引
    private int zindexofdisplayplane;//要显示Z平面的索引
    private GameObject PresetPointA2DX; // 预设轨迹投影点A---of x轴
    private GameObject PresetPointB2DX; // 预设轨迹投影点B---of x轴
    private GameObject PresetCastX; // 预设轨迹投影点A---of x轴
    private GameObject PresetCastZ;// 预设轨迹投影点A---of x轴
    private GameObject PresetPointA2DZ; // 预设轨迹投影点A---of z轴
    private GameObject PresetPointB2DZ; // 预设轨迹投影点B---of z轴
    private GameObject PresetcopynailX; // 预设轨迹投影点B---of z轴
    private GameObject PresetcopynailZ; // 预设轨迹投影点B---of z轴
    private Vector3 RealtimePointA2DX; // 实时轨迹投影点A---of x轴
    private Vector3 RealtimePointB2DX; // 实时轨迹投影点B---of x轴
    private Vector3 RealtimePointExtend2DX; // 实时轨迹延长线段---of x平面 
    private Vector3 RealtimePointA2DZ; // 实时轨迹投影点A---of z轴
    private Vector3 RealtimePointB2DZ; // 实时轨迹投影点B---of z轴
    private Vector3 RealtimePointExtend2DZ; // 实时轨迹延长线段---of z平面 
    private GameObject RealtimecopynailX; // 预设轨迹投影点B---of z轴
    private GameObject RealtimecopynailZ; // 预设轨迹投影点B---of z轴

    //------------------ 3D非原位显示 -----------------
    public GameObject NetTest;
    public GameObject V3DParent;
    public Texture [] guidetexture;                   // 皮肤表面引导点
    public Material V3DMaterial;//材质

    private Plane planeAofcast;                     //转换平面A
    private Plane planeBofcast;                     //转换平面B
    private Plane planeCofcast;                     //转换平面B
    private Plane planeDofcast;                     //转换平面B
    private Plane planeofdisplay;                     //显示平面
    private LineRenderer PresetLineNooriginal3D1;//预设渲染非原位3D线段1
    private LineRenderer PresetLineNooriginal3D2;//预设渲染非原位3D线段1
    public Collider[] cubeCollider;            //碰撞体
    private Vector3 addvector;
    private GameObject selectedmesh;                 //要复制的物体
                                                     //要复制的物体列表1
    private GameObject[] CopyList;                 //要复制的物体列表
    private GameObject[] CopyObjectList1;                 //要复制的物体列表
    private List<GameObject> CopiesListX = new List<GameObject>();
    private GameObject inderct1;
    private List<Vector3> relativePositions1 = new List<Vector3>();
    private List<Vector3> relativeRotations1 = new List<Vector3>();
    //要复制的物体列表2
    private GameObject[] CopyObjectList2;                 //要复制的物体列表
    private GameObject inderct2;
    private List<GameObject> CopiesListZ = new List<GameObject>();
    private List<Vector3> relativePositions2 = new List<Vector3>();
    private List<Vector3> relativeRotations2 = new List<Vector3>();

    public GameObject Display3DX;                         // 镜像物体1的位置
    public GameObject Display3DZ;                         // 镜像物体2的位置
    public GameObject realtimenail;                          // 实时钉子位置

    public GameObject PointA;                  // 用来做临时父级1--会拿来旋转
    private GameObject Recordroate1;                          // 记录原先旋转位置
    public GameObject Drillmodel;                  // 用来做临时父级1--会拿来旋转

    private GameObject pointofrealtimetwine1A;              // 数字孪生1旋转后的点A
    private GameObject pointofrealtimetwine1B;              // 数字孪生1旋转后的点B
    private GameObject pointofrealtimePrecastPoint1;
    public GameObject replaceparentTarget2;                  // 用来做临时父级1--会拿来旋转
    private GameObject Recordroate2;                           // 记录原先旋转位置

    private GameObject pointofrealtimetwine2A;              // 数字孪生2旋转后的点As
    private GameObject pointofrealtimetwine2B;              // 数字孪生2旋转后的点B
    private GameObject pointofrealtimePrecastPoint2;

    private Vector3 rotationof1;            // 孪生1旋转过的角度
    private Vector3 rotationof2;            // 孪生2旋转过的角度

    //----------- 3D原位显示 ----------
    public GameObject[] RealtimeGuideList;             // 实时  引导器点
    public GameObject[] V3DYFImageObject;
    public Image[] V3DYFImage;
    public GameObject[] V3DYcopyobject;              // 开启隐藏与显示物体
    private Transform[] spawnedObjects1; // 已复制的物体数组
    private Transform[] spawnedObjects2; // 已复制的物体数组
    private float currentSpacing;
    public Material[] V3DYFmaterials;

    //----------- Shadow显示 ----------
    public float radius;        // 大圆的半径
    public Image CastPointUI;                   // 皮肤表面引导点
    public GameObject UIenbledControl;                   // 皮肤表面引导点
    private RaycastHit RealtimeRayHit;    // 定义一个 RaycastHit 对象，用于保存碰撞信息

    private List<GameObject> LineObjectList;//渲染线条物体列表
    private List<LineRenderer> LineRanderList;//渲染线条物体列表
    public Material PresetlineMaterial;//预设线材质
    public Material RealtimelineMaterial;//实时线材质
    public Material[] Linematerials;//实时线材质
    public Material[] BoneMaterials;//实时线材质
    public Material[] V3DYMaterials;//材质
    public GameObject[] V3DYIndicater;
    public Material RealtimeIndicaterBallA;
    public Material RealtimeIndicaterBallB;
    public Image[] VShadowImage;                    // 投影深度图像
    public GameObject[] VShadowRPPoints;                // 投影旋转图像
    public GameObject RealtimeCastPoint;                          // 指示环
    public TextMeshPro[] textMeshs;


    public Collider JudgeBall;            //碰撞体
    private bool A0isarrived;
    private bool B0isarrived;
    private bool C0isarrived;
    private bool ALL0isarrived;
    private bool A1isarrived;
    private bool B1isarrived;
    private bool C1isarrived;
    private bool ALL1isarrived;
    private bool Drillisarrived;            // 孪生2旋转过的角度
    private bool isinsert;            // 孪生2旋转过的角度
    private float[] ArrivedCount;
    private int IndexesCount=0;
    

    //2DY原位
    public Image[] V2DYImage;                    // 投影深度图像
    private Image[] V2DYcopyImage = new Image[20];                    // 投影深度图像
    private GameObject V2DYcopyImageParent;                    // 投影深度图像

    private Communication script_Communication;

    private Vector3 skinbeginlicalPosition;                    // 
    private GameObject presetnewposition;                          // 
    private bool isChangeTrack;
    private bool tempisChanged;
    private int count1number;
    private GameObject presetbeginposition;                    // 投影深度图像
    public GameObject Endtips;
    public bool isrecord = false;
    public GameObject ooooooooo;
    public GameObject AAAAAAAAAA;
    //----------- 2D 原位 -------------

    void Start()
    {
        startset();
        CastPointUI.material.color = Color.white;
        isChangeTrack = false;
    }
    void Update()
    {
        float angleDifference = Quaternion.Angle(initialRotation, skinSurface.transform.rotation);
        // 判断角度差是否超过150度

        // 计算当前旋转与初始旋转之间的角度差
        if (isChangeTrack)
        {
            imagerote();
            roteimage.transform.position = skinSurface.gameObject.transform.position + skinSurface.gameObject.transform.up * 0.05f;
            rotetextmesh.transform.position = roteimage.transform.position - skinSurface.gameObject.transform.right * 0.1f;
            rotetextmesh.transform.forward = -skinSurface.gameObject.transform.up;
            Quaternion rotetextmeshrotation = Quaternion.FromToRotation(rotetextmesh.transform.up, skinSurface.transform.right);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
            rotetextmesh.transform.rotation = rotetextmeshrotation * rotetextmesh.transform.rotation;     // 应用旋转
            for (int i = 0; i < 5; i++)
            {
                Renderer renderer1 = SpineModels[i].GetComponent<MeshRenderer>();
                renderer1.material = BoneMaterials[0];
                renderer1.enabled = true;
            }
            if (angleDifference >= 150f)
            {
                Debug.Log("物体发生了150度以上的旋转");
                rotetextmesh.enabled = false;
                roteimage.SetActive(false);
                aciverote.SetActive(true);
                // 调用延时出现函数
                Invoke("HideroteObject", 2.0f);
                isChangeTrack = false;
                tempisChanged = true;
                needroate = false;
                isroate = true;
            }
            roteimage.SetActive(true);
            rotetextmesh.enabled = true;
            DisplayPlane[0].SetActive(false);
            isbegin = false;
        }
        else
        {
            if (!isbegin)//执行一次
            {
                roteimage.SetActive(false);
                rotetextmesh.enabled = false;
                Debug.Log("已经设置好了");
                DisplayPlane[0].SetActive(true);
                if (tempisChanged)
                {
                    Debug.Log("反向旋转了");
                    DisplayPlane[0].transform.forward = skinSurface.transform.right;
                }
                else
                {
                    Debug.Log("没有进行反向旋转");
                    DisplayPlane[0].transform.forward = -skinSurface.transform.right;
                }

                if (VisualizationEnabled[0] || VisualizationEnabled[1])
                {
                    StVision3DY();
                }
                if (VisualizationEnabled[2] || VisualizationEnabled[3])
                {
                    StVision3DF();
                }
                if (VisualizationEnabled[4] || VisualizationEnabled[5])
                {
                    StVisionShadow();
                }
                if (VisualizationEnabled[6] || VisualizationEnabled[7])
                {
                    StVision2DF();
                }
                if (VisualizationEnabled[8] || VisualizationEnabled[9])
                {
                    StVision2DY();
                }
                isChangeTrack = false;
                isbegin = true;
            }
            if (isbegin)
            {
                ResetsPresetCollisionPosition();//设置A点是否进入到碰撞小球中若进入就重设cast点
                DisplayPlane[0].SetActive(true);
                if (tempisChanged)
                {
                    DisplayPlane[0].transform.up = skinSurface.transform.up;
                    Quaternion DisplayPlane0rotation = Quaternion.FromToRotation(DisplayPlane[0].transform.forward, -skinSurface.transform.right);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
                    DisplayPlane[0].transform.rotation = DisplayPlane0rotation * DisplayPlane[0].transform.rotation;     // 应用旋转
                }
                else
                {
                    DisplayPlane[0].transform.up = skinSurface.transform.up;
                    Quaternion DisplayPlane0rotation = Quaternion.FromToRotation(DisplayPlane[0].transform.forward, skinSurface.transform.right);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
                    DisplayPlane[0].transform.rotation = DisplayPlane0rotation * DisplayPlane[0].transform.rotation;     // 应用旋转
                }
                if (VisualizationEnabled[0])
                {
                    UpVision3DY();
                }//-------------------------------------- 3D原位 - 机器人 -----------------------------------------------

                if (VisualizationEnabled[1])
                {
                    UpVision3DY();
                }//-------------------------------------- 3D原位 - 手持 -------------------------------------------------

                if (VisualizationEnabled[2] & ControllerV3)
                {
                    UpVision3DF();
                }//-------------------------------------- 3D非原 - 机器人 -----------------------------------------------

                if (VisualizationEnabled[3] & ControllerV4)
                {
                    UpVision3DF();
                }//-------------------------------------- 3D非原 - 手持 -------------------------------------------------

                if (VisualizationEnabled[4])
                {
                    UpVisionShadow();
                }//-------------------------------------- 2D投影 - 机器人 -----------------------------------------------

                if (VisualizationEnabled[5])
                {
                    UpVisionShadow();
                }//-------------------------------------- 2D投影 - 手持 -------------------------------------------------

                if (VisualizationEnabled[6])
                {
                    UpVision2DF();
                }//-------------------------------------- 2D非原 - 机器人 -----------------------------------------------

                if (VisualizationEnabled[7])
                {
                    UpVision2DF();
                }//-------------------------------------- 2D非原 - 手持 -------------------------------------------------
                if (VisualizationEnabled[8])
                {
                    UpVision2DY();
                }//-------------------------------------- 2D原位 - 机器 -------------------------------------------------
                if (VisualizationEnabled[9])
                {
                    UpVision2DY();
                }//-------------------------------------- 2D原位 - 手持 ------------------------------------------------- 

            }

            if (isHande)
            {
                RealtimeRay.origin = RealtimeGuideList[2].transform.position;
                RealtimeRay.direction = RealtimeGuideList[1].transform.up;
                if (RealtimeGuideList[6].activeSelf)
                {
                    //Debug.Log("游戏对象处于激活状态");
                    RealtimeGuideLine3D.enabled = true;
                    RealtimeGuideLine2DX.enabled = true;
                    RealtimeGuideLine2DZ.enabled = true;
                }
                else
                {
                    //Debug.Log("游戏对象处于非激活状态");
                    RealtimeGuideLine3D.enabled = false;
                    RealtimeGuideLine2DX.enabled = false;
                    RealtimeGuideLine2DZ.enabled = false;

                }
            }
            else
            {
                RealtimeRay.origin = RealtimeGuideList[5].transform.position;
                RealtimeRay.direction = RealtimeGuideList[4].transform.up;
                if (RealtimeGuideList[7].activeSelf)
                {
                    //Debug.Log("游戏对象处于激活状态");
                    RealtimeGuideLine3D.enabled = true;
                    RealtimeGuideLine2DX.enabled = true;
                    RealtimeGuideLine2DZ.enabled = true;

                }
                else
                {
                    //Debug.Log("游戏对象处于非激活状态");
                    RealtimeGuideLine3D.enabled = false;
                    RealtimeGuideLine2DX.enabled = false;
                    RealtimeGuideLine2DZ.enabled = false;

                }
            }

        }
        //--------------------------------------------- 按键按下后10个位置中选出一个位置并决定可视化方案 -------------------------------------------------
        //------------先从偶数的5个数中选出顺序，然后执行，当执行完成5次后旋转换边进行第二次选择仍然是5个数中选择
        if (FirstSelectedIndexes01.Count == 0)
        {
            listHasElements = false;
        }
        RealtimeGuideLine3D.enabled = true;
        LineObjectList[5].SetActive(true);
        LineObjectList[5].transform.parent = AAAAAAAAAA.transform;
        if (skinSurface.Raycast(RealtimeRay, out skinRealtimeRayHit, float.MaxValue))
        {

            Vector3 intersectionPoint = skinRealtimeRayHit.point; // 获取交点位置
            Vector3 normal = skinRealtimeRayHit.normal; // 获取法线向量
            RealtimeCastPoint.SetActive(true);
            RealtimeCastPoint.transform.position = intersectionPoint;
            RealtimeCastPoint.transform.up = normal;
            RealtimeGuideLine3D.SetPosition(0, RealtimeCastPoint.transform.position);
            RealtimeGuideLine3D.SetPosition(1, ooooooooo.transform.position);
        }
        else
        {
            RealtimeCastPoint.SetActive(false);
            Vector3 driection = Vector3.Normalize(AAAAAAAAAA.transform.position - ooooooooo.transform.position);
            Vector3 pointC = AAAAAAAAAA.transform.position + driection * 0.5f;
            RealtimeGuideLine3D.SetPosition(0, pointC);
            RealtimeGuideLine3D.SetPosition(1, ooooooooo.transform.position);
            //Debug.Log("00000000000000000000000000000000000");
        }



    }

    void startset()
    {

        PresetPointA = new GameObject("PresetPointA"); PresetPointA.transform.SetParent(whole.transform);
        PresetPointB = new GameObject("PresetPointB"); PresetPointB.transform.SetParent(whole.transform);
        Recordroate1 = new GameObject("Recordroate1"); Recordroate1.transform.SetParent(whole.transform);
        Recordroate2 = new GameObject("Recordroate2"); Recordroate2.transform.SetParent(whole.transform);
        pointofrealtimetwine1A = new GameObject("pointofrealtimetwine1A"); pointofrealtimetwine1A.transform.SetParent(whole.transform);
        pointofrealtimetwine1B = new GameObject("pointofrealtimetwine1B"); pointofrealtimetwine1B.transform.SetParent(whole.transform);
        pointofrealtimePrecastPoint1 = new GameObject("pointofrealtimePrecastPoint1"); pointofrealtimePrecastPoint1.transform.SetParent(whole.transform);
        pointofrealtimetwine2A = new GameObject("pointofrealtimetwine2A"); pointofrealtimetwine2A.transform.SetParent(whole.transform);
        pointofrealtimetwine2B = new GameObject("pointofrealtimetwine2B"); pointofrealtimetwine2B.transform.SetParent(whole.transform);
        pointofrealtimePrecastPoint2 = new GameObject("pointofrealtimePrecastPoint2"); pointofrealtimePrecastPoint2.transform.SetParent(whole.transform);
        PresetPointA2DX = new GameObject("PresetPointA2DX"); PresetPointA2DX.transform.SetParent(whole.transform);
        PresetCastX = new GameObject("PresetCastX"); PresetCastX.transform.SetParent(whole.transform);
        PresetCastZ = new GameObject("PresetCastZ"); PresetCastZ.transform.SetParent(whole.transform);
        PresetPointB2DX = new GameObject("PresetPointB2DX"); PresetPointB2DX.transform.SetParent(whole.transform);
        PresetPointA2DZ = new GameObject("PresetPointA2DZ"); PresetPointA2DZ.transform.SetParent(whole.transform);
        PresetPointB2DZ = new GameObject("PresetPointB2DZ"); PresetPointB2DZ.transform.SetParent(whole.transform);
        PresetCastPoint = new GameObject("PresetCastPoint"); PresetCastPoint.transform.SetParent(whole.transform);
        PresetCastBone = new GameObject("PresetCastBone"); PresetCastBone.transform.SetParent(whole.transform);
        RealtimeCastBone = new GameObject("RealtimeCastBone"); RealtimeCastBone.transform.SetParent(whole.transform);
        ParentPbject = new GameObject("ParentPbject"); ParentPbject.transform.SetParent(whole.transform);
        RealtimeGuidePointA = new GameObject("RealtimeGuidePointA"); RealtimeGuidePointA.transform.SetParent(whole.transform);
        RealtimeGuidePointB = new GameObject("RealtimeGuidePointB"); RealtimeGuidePointB.transform.SetParent(whole.transform);
        RealtimeGuidePointC = new GameObject("RealtimeGuidePointC"); RealtimeGuidePointC.transform.SetParent(whole.transform);
        LineObjects = new GameObject("LineObjects"); LineObjects.transform.SetParent(whole.transform);
        V2DYcopyImageParent = new GameObject("V2DYcopyImageParent"); V2DYcopyImageParent.transform.SetParent(whole.transform);
        inderct1 = new GameObject("inderct1"); inderct2 = new GameObject("inderct2");
        presetbeginposition = new GameObject("presetbeginposition"); presetnewposition = new GameObject("presetnewposition");
        LineObjectList = new List<GameObject>();//初始化
        LineRanderList = new List<LineRenderer>();//初始化
        selectedPlaneofxTransform = PresetCastPoint.transform;
        textMeshs[0].text = "NO";        // 设置初始文本内容
        textMeshs[0].enabled = false; textMeshs[1].enabled = false;
        ControllerV3 = false;//初始布尔值
        ControllerV4 = false;//初始布尔值
        V3DFisactive = false;//初始布尔值
        V3DFcopyisupdate = false;
        V3DYisactive = false;//初始布尔值
        isHande = false;
        spawnedObjects1 = new Transform[11];
        spawnedObjects2 = new Transform[11];
        CastPointUI.enabled = false;//初始隐藏碰撞点
        V3DYIndicater[6].SetActive(false);
        DisplayPlane[1].SetActive(false);        //2DX显示平面隐藏
        Display3DX.SetActive(false); //3DX复制物体隐藏
        Display3DZ.SetActive(false); //3DZ复制物体隐藏
        realtimenail.SetActive(true);
        UIenbledControl.SetActive(false);
        LineObjects.SetActive(false);
        presetnail.SetActive(false);
        cubeCollider[0].gameObject.SetActive(false);
        cubeCollider[1].gameObject.SetActive(false);
        RealtimeCastPoint.SetActive(false);
        VShadowRPPoints[0].SetActive(false);
        VShadowRPPoints[1].SetActive(false);
        aciverote.SetActive(false);
        V2DYImage[0].enabled = false; V2DYImage[1].gameObject.SetActive(false); V2DYImage[4].enabled = false;
        V3DYFImageObject[0].SetActive(false); V3DYFImageObject[1].SetActive(false);
        inderct1 = Instantiate(V3DYcopyobject[2]); inderct2 = Instantiate(V3DYcopyobject[2]);
        RealtimeGuideList[6].SetActive(true);
        for (int i = 0; i < textMeshs.Length; i++)
        {
            textMeshs[i].transform.SetParent(DisplayPlane[5].transform);
        }
        for (LineobjectIndex = 0; LineobjectIndex < 10; LineobjectIndex++)// 循环创建10个线条物体
        {
            GameObject Lineobject = new GameObject("RanderLine" + LineobjectIndex); //new一个线条物体
            Lineobject.AddComponent<LineRenderer>();//给这个物体添加LineRenderer组件
            Lineobject.GetComponent<LineRenderer>().widthMultiplier = 0.0005f;
            Lineobject.GetComponent<LineRenderer>().positionCount = 2;
            Lineobject.transform.SetParent(LineObjects.transform);
            LineObjectList.Add(Lineobject); // 将创建的线条物体添加到渲染线条列表中
        }

        addvector = new Vector3(0, 0.1f, 0);
        PresetInsertionDepth = 0;
        RealtimeInsertionDepth = 0;
        InsertionPercentage = 0;
        A0isarrived = false; B0isarrived = false; A1isarrived = false; B1isarrived = false;
        ballisarrived = false;
        inderct1.SetActive(false); inderct2.SetActive(false);
        EntryEviation = 0;
        ExportEviation = 0;
        AngleEviation1 = 0;
        AngleEviation2 = 0;
        mainCameraTransform = Camera.main.transform;
        PreviousNumber = 0;
        Number = 0;
        begindate = new float[7];
        ExportOffset = 0;
        EnterOffset = 0;
        angleDegrees = 0;
        MedianOffset = 0;
        CoronalOffset = 0;
        TransverseOffset = 0;
        ArrivedCount = new float[6];
        V3DYFImageObject[0].SetActive(false);
        V3DYFImageObject[1].SetActive(false);
        V3DYFImageObject[2].SetActive(false);
        V3DYFImageObject[3].SetActive(false);

        // 获取起始旋转
        startRotation = roteimage.transform.rotation;

        // 设置目标旋转
        targetRotation = startRotation * Quaternion.Euler(0f, 90f, 0f);

        // 初始化旋转计时器
        rotationTimer = 0f;

        roteimage.SetActive(false);
        rotetextmesh.enabled = false;
        Drillisarrived = false;
        Renderer JudgeBallrander = JudgeBall.gameObject.GetComponent<Renderer>();
        JudgeBallrander.enabled = false;
        //初始展示材质替换
        for (int i = 0; i < 5; i++)
        {
            MeshRenderer render = SpineModels[i].GetComponent<MeshRenderer>();
            render.material = BoneMaterials[0];
        }
        for (int i = 0; i < 2; i++)
        {
            MeshRenderer render = otherSpineModels[i].GetComponent<MeshRenderer>();
            render.material = BoneMaterials[0];
        }
        for (int i = 0; i < planesofy.Length; i++)
        {
            planesofy[i].enabled = false; // 隐藏未选中的平面
        }

        for (int i = 0; i < V3DYIndicater.Length; i++)
        {
            V3DYIndicater[i].SetActive(false);        //3DY引导器隐藏初始化
        }
        for (int i = 0; i < textMeshs.Length; i++)
        {
            textMeshs[i].enabled = false;        //3DY引导器隐藏初始化
        }

        //--------------------------------------------- 选出8个位置并分配序号 -------------------------------------------------
        int[] numbersx = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };//位置
        //int[] numbersx = new int[] { 4 };//位置
        //for (int i = 0; i < 10; i++)
        for (int i = 0; i < numbersx.Length; i++)
            {
            //修改
            int index = UnityEngine.Random.Range(0, 10);
            while (FirstSelectedIndexes01.Contains(index))
            {
                index = UnityEngine.Random.Range(0, 10);
            }
            //修改
            //FirstSelectedIndexes01.Add(index);//位置
            FirstSelectedIndexes01.Add(numbersx[i]);
        }//存的可能为 2、8、6、3、5、4、0、1、7、9
         //修改
        int[] numbers = new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };//方案
        //int[] numbers = new int[] { 1 };//方案
        //修改
        //int[] numbers = new int[] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };//位置选择
        for (int i = 0; i < numbers.Length; i++)
        {
            indexNumberMap[FirstSelectedIndexes01[i]] = numbers[i];
        }//存的可能为2-1、8-2、6-3、3-4、5-5、4-6、0-7、1-8、7-9、9-10
        //indexNumberMap[0] = 2; indexNumberMap[1] = 3; indexNumberMap[2] = 1; indexNumberMap[3] = 1; indexNumberMap[4] = 1; indexNumberMap[5] = 1; indexNumberMap[6] = 1; indexNumberMap[7] = 8; indexNumberMap[8] = 8; indexNumberMap[9] = 8;

        //换为列表一：2-1、8-2、6-3、4-6、0-7 列表二：3-4、5-5、1-8、7-9、9-10
        for (int i = 0; i < numbersx.Length; i++)
        {
            bool isfirstValueEven = (FirstSelectedIndexes01[i] % 2) == 0;
            if (isfirstValueEven)
            {
                Indexes01.Add(FirstSelectedIndexes01[i]);//存0、2、4、6、8
            }
            else
            {
                Indexes02.Add(FirstSelectedIndexes01[i]);//存1、3、5、7、9
            }
        }
        //修改
        currentIndex = FirstSelectedIndexes01[UnityEngine.Random.Range(0, Indexes02.Count)];
        Debug.Log("Indexes01"+ Indexes01.Count+ "Indexes02"+ Indexes02.Count+"currentIndex" +currentIndex);
        IndexesCount = Indexes01.Count + Indexes02.Count;
        count1number = Indexes01.Count;

        //修改
        /*        currentIndex = FirstSelectedIndexes01[UnityEngine.Random.Range(0, Indexes01.Count)]; */       //--------------------------------------------- 选出8个位置并分配序号 -------------------------------------------------
                                                                                                                //随机取一个当做第一个索引
                                                                                                                //随机取列表一的一个当做第一个索引
                                                                                                                //--------------------------------------------- 循环创建10个渲染线条物体 -------------------------------------------------

        PresetLine3D = LineObjectList[0].GetComponent<LineRenderer>();
        PresetLine2DX = LineObjectList[1].GetComponent<LineRenderer>();
        PresetLine2DZ = LineObjectList[2].GetComponent<LineRenderer>();
        PresetLineNooriginal3D1 = LineObjectList[3].GetComponent<LineRenderer>();
        PresetLineNooriginal3D2 = LineObjectList[4].GetComponent<LineRenderer>();
        RealtimeGuideLine3D = LineObjectList[5].GetComponent<LineRenderer>();

        RealtimeGuideLine2DX = LineObjectList[6].GetComponent<LineRenderer>();
        RealtimeGuideLine2DZ = LineObjectList[7].GetComponent<LineRenderer>();
        RealtimeNoRGuideLine3D1 = LineObjectList[8].GetComponent<LineRenderer>();//非原位
        RealtimeNoRGuideLine3D2 = LineObjectList[9].GetComponent<LineRenderer>();//非原位


        for (int i = 0; i < 10; i++)
        {
            LineObjectList[i].SetActive(false);
        }
        for (int i = 0; i < V2DFImage.Length; i++)
        {
            V2DFImage[i].SetActive(false);
        }
        PresetLine3D.material = PresetlineMaterial;
        PresetLine2DX.material = PresetlineMaterial;
        PresetLine2DZ.material = PresetlineMaterial;
        PresetLineNooriginal3D1.material = PresetlineMaterial;
        PresetLineNooriginal3D2.material = PresetlineMaterial;
        RealtimeGuideLine3D.material = RealtimelineMaterial;
        RealtimeGuideLine2DX.material = RealtimelineMaterial;
        RealtimeGuideLine2DZ.material = RealtimelineMaterial;
        RealtimeNoRGuideLine3D1.material = RealtimelineMaterial;
        RealtimeNoRGuideLine3D2.material = RealtimelineMaterial;

        plane3material.mainTexture = plane3background[10];//开始的背景
        for (int i = 0; i < expermentstateimages.Length; i++) { expermentstateimages[i].enabled = false; }



        //--------------------------------------------- 设置初始值 -----------------------------------------------------
        PresetRay = new Ray(Vector3.zero, Vector3.forward);//射线初始值归零
        DrillRay = new Ray(Vector3.zero, Vector3.forward);//射线初始值归零
        RealtimeRay = new Ray(Vector3.zero, Vector3.forward);//射线初始值归零
        skinbeginPosition = skinSurface.transform.localPosition;
        BoneSurface = new MeshCollider[5];
        submitnumber = 0;
        submitisrote = false;
        for (int i = 0; i < SpineModels.Length; i++) { BoneSurface[i] = SpineModels[i].GetComponent<MeshCollider>(); }
        Realtimeendnail = new Vector3(0,0,0);
        script_Communication = FindObjectOfType<Communication>();
        presetbeginposition.transform.position = PresetCastPoint.transform.position;
        presetbeginposition.transform.parent = skinSurface.transform.parent;
        skinbeginlicalPosition = skinSurface.transform.localPosition;

    }
    public void ExpermentEnd()
    {
        //暂停可视化方案中实时更新
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = false;
        }
        //整体清理
        RealtimeCastPoint.SetActive(false);    //实时碰撞点
        LineObjects.SetActive(false);

        for (int i = 0; i < 10; i++)        //设置线段不可见
        {
            LineObjectList[i].SetActive(false);
        }
        for (int i = 0; i < planesofy.Length; i++)
        {
            planesofy[i].enabled = false; // 显示选中的平面       
        }
        for (int i = 0; i < V3DYcopyobject.Length; i++)
        {
            V3DYcopyobject[i].SetActive(false);
        }
        for (int i = 0; i < V3DYIndicater.Length; i++)
        {
            V3DYIndicater[i].SetActive(false);
        }
        //2D非清理
        DisplayPlane[1].SetActive(false);        //2DX显示平面隐藏
        Destroy(PresetcopynailX); Destroy(PresetcopynailZ); Destroy(RealtimecopynailX); Destroy(RealtimecopynailZ);
        //3D非清理
        Display3DX.SetActive(false);      //3DX复制物体隐藏
        Display3DZ.SetActive(false);      //3DZ复制物体隐藏
        cubeCollider[0].gameObject.SetActive(false);//3DX方框容器
        cubeCollider[1].gameObject.SetActive(false);//3DZ方框容器
        inderct1.transform.SetParent(DisplayPlane[0].transform); inderct2.transform.SetParent(DisplayPlane[0].transform);
        inderct1.SetActive(false); inderct2.SetActive(false);
        RealtimeGuideList[0].SetActive(false);
        RealtimeGuideList[3].SetActive(false);
        V3DYcopyobject[2].transform.SetParent(V3DYcopyobject[1].transform);
        for (int i = 0; i < textMeshs.Length; i++)
        {
            textMeshs[i].transform.SetParent(DisplayPlane[5].transform);
            textMeshs[i].enabled = false;
            textMeshs[i].color = Color.white;
            textMeshs[i].gameObject.layer = 0;
        }

        if (V3DFisactive)
        {
            for (int i = 0; i < 10; i++)
            {
                Destroy(CopiesListX[i]);
                Destroy(CopiesListZ[i]);
            }
            for (int i = 0; i < 11; i++)
            {
                Destroy(spawnedObjects1[i].gameObject);
                Destroy(spawnedObjects2[i].gameObject);
            }
        }
        if (V3DYisactive)
        {
            for (int i = 0; i < 11; i++)
            {
                Destroy(spawnedObjects1[i].gameObject);
            }
        }

        for (int i = 0; i < expermentstateimages.Length; i++)
        {
            // 根据状态值设置颜色
            if (i < statenumber)
            {
                // 数值小于state的图片设置为绿色
                expermentstateimages[i].material = expermentstatematerials[0];
            }
            else if (i == statenumber)
            {
                // 数值小于state的图片设置为绿色
                expermentstateimages[i].material = expermentstatematerials[3];
            }
            else
            {
                // 数值大于state的图片设置为灰色
                expermentstateimages[i].material = expermentstatematerials[2];
            }
        }



        CopiesListX.Clear();                        //3DX列表
        CopiesListZ.Clear();                        //3DZ列表
        V3DFisactive = false; V3DYisactive = false; V3DFcopyisupdate = false;
        Display3DX.SetActive(false); Display3DZ.SetActive(false);
        ControllerV3 = false;
        ControllerV4 = false;
        //2D原清理
        CastPointUI.enabled = false;
        UIenbledControl.SetActive(false);
        //3D原清理
        V3DYIndicater[6].SetActive(false);  //预设引导
        presetnail.SetActive(false);           //预设钉子
        RealtimeGuideLine3D.enabled = false;
        PresetLine3D.enabled = false;

        V2DYImage[0].enabled = false;
        V2DYImage[1].gameObject.SetActive(false);
        V3DYFImageObject[0].SetActive(false);
        V3DYFImageObject[1].SetActive(false);
        A0isarrived = false; B0isarrived = false; A1isarrived = false; B1isarrived = false;
        V2DYcopyImageParent.SetActive(false);
        //发送记录的所有数据
        EndTimeStamp = Time.time;
        float elapsedTime = (EndTimeStamp - StartTimeStamp) * 1000f;
        // 计算夹角（弧度）
        float angleRadians = Mathf.Acos(Vector3.Dot(VP.normalized, VR.normalized));
        // 将弧度转换为角度
        angleDegrees = angleRadians * Mathf.Rad2Deg;
        Debug.Log("相距角度为"+ angleDegrees+"任务时长为"+elapsedTime);
        V3DMaterial.mainTexture = guidetexture[0];
        C1isarrived = false;
        C0isarrived = false;
        ALL1isarrived = false;
        ALL0isarrived = false;
        isrecord = false;
        skinSurface.transform.localPosition = new Vector3(skinbeginlicalPosition.x, skinbeginlicalPosition.y, skinbeginlicalPosition.z);
    }//实验结束
    public void ExpermentNumber() 
    {
        //isindexout = true;
        Debug.Log("Indexes01" + Indexes01.Count);
        if (listHasElements)//更改输入设置FirstSelectedIndexes01列表里面有数了以后再执行
        {
            Debug.Log("Indexes01" + Indexes01);
            if (!isindexout)
            {
                int SecondIndex = Indexes01[UnityEngine.Random.Range(0, Indexes01.Count)];
                //int SecondIndex = Indexes01[0];
                //while (SecondIndex == currentIndex)
                //{
                //    SecondIndex = Indexes01[UnityEngine.Random.Range(0, Indexes01.Count)];
                //}
                Indexes01.Remove(SecondIndex);//
                currentIndex = SecondIndex;//当前的索引设为6
                if (Indexes01.Count == 0)
                {
                    isindexout = true;
                }
            }
            else
            {
                if (statenumber < IndexesCount)
                {
                    int SecondIndex = Indexes02[UnityEngine.Random.Range(0, Indexes02.Count)];
                    while (SecondIndex == currentIndex)
                    {
                        SecondIndex = Indexes02[UnityEngine.Random.Range(0, Indexes02.Count)];
                    }
                    Indexes02.Remove(SecondIndex);//
                    currentIndex = SecondIndex;//当前的索引设为6
                }
            }
        }
        Debug.Log("currentIndex" + currentIndex);
        plane3material.mainTexture = plane3background[indexNumberMap[currentIndex] - 1];//换背景
        Debug.Log("测试测试测试"+ indexNumberMap[currentIndex]);
        for (int i = 0; i < expermentstateimages.Length; i++) { expermentstateimages[i].enabled = true; }

    }
    public void ExpermentBegin()
    {
        skinSurface.transform.localPosition = new Vector3(skinbeginlicalPosition.x, skinbeginlicalPosition.y , skinbeginlicalPosition.z);
        RealtimeGuideList[0].SetActive(true);
        RealtimeGuideList[3].SetActive(true);
        for (int i = 0; i < 5; i++)
        {
            Renderer renderer1 = SpineModels[i].GetComponent<MeshRenderer>();
            renderer1.enabled = false;
        }
        Renderer meshrender = SpineModels[xindexofdisplayplane].GetComponent<MeshRenderer>();
        meshrender.enabled = true;
        meshrender.material = BoneMaterials[2];
        originalIndex = -1;
            for (int i = 0; i < positions.Length; i++)
            {
                if (positions[i] == positions[currentIndex])//第一次选出8个数中再选出一个数
                {
                    originalIndex = i;
                    positions[currentIndex].GetComponent<Renderer>().material.color = Color.green;//取到了索引几的骨头比如取到了6
                    switch (originalIndex)
                    {
                        case 0:
                            isright = false;
                            xindexofdisplayplane = 0;
                            zindexofdisplayplane = 0;
                            break;
                        case 1:
                            isright = true;
                            xindexofdisplayplane = 0;
                            zindexofdisplayplane = 0;
                            break;
                        case 2:
                            isright = false;
                            xindexofdisplayplane = 1;
                            zindexofdisplayplane = 0;
                            break;
                        case 3:
                            isright = true;
                            xindexofdisplayplane = 1;
                            zindexofdisplayplane = 0;
                            break;
                        case 4:
                            isright = false;
                            xindexofdisplayplane = 2;
                            zindexofdisplayplane = 1;
                            break;
                        case 5:
                            isright = true;
                            xindexofdisplayplane = 2;
                            zindexofdisplayplane = 1;
                            break;
                        case 6:
                            isright = false;
                            xindexofdisplayplane = 3;
                            zindexofdisplayplane = 1;
                            break;
                        case 7:
                            isright = true;
                            xindexofdisplayplane = 3;
                            zindexofdisplayplane = 1;
                            break;
                        case 8:
                            isright = false;
                            xindexofdisplayplane = 4;
                            zindexofdisplayplane = 1;
                            break;
                        case 9:
                            isright = true;
                            xindexofdisplayplane = 4;
                            zindexofdisplayplane = 1;
                            break;
                        default:
                            break;
                    }// 2D非原位 决定哪个X与Z平面显示

                    switch (indexNumberMap[currentIndex])//分配可视化方案取到了6-3就是3
                    {
                        case 1:
                            Visualization1();
                            break;
                        case 2:
                            Visualization2();
                            break;
                        case 3:
                            Visualization3();
                            break;
                        case 4:
                            Visualization4();
                            break;
                        case 5:
                            Visualization5();
                            break;
                        case 6:
                            Visualization6();
                            break;
                        case 7:
                            Visualization7();
                            break;
                        case 8:
                            Visualization8();
                            break;
                        case 9:
                            Visualization9();
                            break;
                        case 10:
                            Visualization10();
                            break;
                        default:
                            break;
                    }//分配可视化方案
                    break;
                }
                else
                {
                    positions[currentIndex].GetComponent<Renderer>().material.color = Color.red;
                }
            }
            ChangePresetPosition();//更改预设位置
            Debug.Log("当前脊柱位置位是" + isright + "侧");
            Debug.Log("当前的骨头为" + xindexofdisplayplane + "当前位置在原来的列表中的索引为：" + originalIndex + "，其可视化方案为：" + indexNumberMap[currentIndex] + "剩余未植入的脊骨数量为：" + FirstSelectedIndexes01.Count);

            bool iseven = (originalIndex % 2) == 0;
            if (iseven)
            {
                pictureDriection = true;
            }
            else {
                pictureDriection = false;
            }
            initialRotation = skinSurface.transform.rotation;  // 记录初始旋转
            PreviousNumber = originalIndex;
            Number++;
        
        isbegin = false;
        aciverote.SetActive(false);
        LineObjects.SetActive(true);
        VShadowRPPoints[0].SetActive(false);
        VShadowRPPoints[1].SetActive(false);
        RealtimeGuideLine3D.enabled = false;
        PresetLine3D.enabled = false;
        A0isarrived = false; B0isarrived = false; InsertmaxValue = 0f; ballisarrived = false;
        textMeshs[0].gameObject.transform.SetParent(PresetPointA.transform);
        textMeshs[1].gameObject.transform.SetParent(PresetPointB.transform);
        //设置选中脊骨的材质
        Renderer meshrender2 = SpineModels[xindexofdisplayplane].GetComponent<MeshRenderer>();
        meshrender2.enabled = true;
        meshrender2.material = BoneMaterials[2];
        for (int i = 0; i < textMeshs.Length; i++)
        {
            textMeshs[i].enabled = false;
            textMeshs[i].color = Color.white;
            textMeshs[i].transform.SetParent(DisplayPlane[5].transform);
        }

        for (int i = 0; i < expermentstateimages.Length; i++)
        {
            // 根据状态值设置颜色
            if (i < statenumber)
            {
                // 数值小于state的图片设置为绿色
                expermentstateimages[i].material = expermentstatematerials[0];
            }
            else if (i == statenumber)
            {
                // 数值等于state的图片设置为蓝色
                expermentstateimages[i].material = expermentstatematerials[1]; 
                Debug.Log("statenumber是" + statenumber + "i是" + i); 
            }
            else
            {
                // 数值大于state的图片设置为灰色
                expermentstateimages[i].material = expermentstatematerials[2];
            }
        }
        statenumber++;

        V3DYFImageObject[0].SetActive(false);
        V3DYFImageObject[1].SetActive(false);
        V3DYFImageObject[2].SetActive(false);
        V3DYFImageObject[3].SetActive(false);
        A0isarrived = false; B0isarrived = false; A1isarrived = false; B1isarrived = false;
        V2DYcopyImageParent.SetActive(false);

        begindate[0] = 0;
        begindate[1] = statenumber;//第几次植入
        begindate[2] = Number;//植入的位置
        begindate[3] = indexNumberMap[currentIndex];//方案几
        StartTimeStamp = Time.time;

        presetbeginposition.transform.position = PresetCastPoint.transform.position;
        presetbeginposition.transform.parent = skinSurface.transform.parent;
        V3DMaterial.mainTexture = guidetexture[0];
        C1isarrived = false;
        C0isarrived = false;
        ALL1isarrived = false;
        ALL0isarrived = false;
        isrecord = true;
    }//开始实验
    public float[] beginfloat()
    {
        return begindate;//手持或机器
    }
    void ChangePresetPosition()
    {
        presetnail.SetActive(true);
        LineObjects.SetActive(true);
        DisplayPlane[1].SetActive(false);//2DX显示
        V3DYIndicater[6].SetActive(true);
        //挑选预设位置
        PresetPointA.transform.position = positions[currentIndex].position;
        PresetPointA.transform.rotation = positions[currentIndex].rotation;
        PresetPointB.transform.position = PresetPointA.transform.position + PresetPointA.transform.up * PresetLineLength;
        PresetPointB.transform.rotation = positions[currentIndex].rotation;
        PresetLineDirection = PresetPointA.transform.position - PresetPointB.transform.position;
        PresetPointA.transform.SetParent(positions[currentIndex]);//A点放入
        PresetPointB.transform.SetParent(positions[currentIndex]);//B点放入
        presetnail.transform.SetParent(positions[currentIndex]);//钉子放入
        presetnail.transform.position = PresetPointA.transform.position;//钉子位置
        presetnail.transform.up = -PresetLineDirection;//钉子方向
        V3DYIndicater[6].transform.SetParent(positions[currentIndex]);//指示放入
        PresetRay.origin = PresetPointB.transform.position;//B点作为射出原点
        PresetRay.direction = -presetnail.transform.up;//转换方向从B-A
        JudgeBall.transform.SetParent(positions[currentIndex]);//碰撞小球放入
        Drillisarrived = false;
        //设置所有骨头mesh材质为未选中
        for (int i = 0; i < 5; i++)
        {
            Renderer renderer1 = SpineModels[i].GetComponent<MeshRenderer>();
            renderer1.material = BoneMaterials[1];
            renderer1.enabled = false;
        }
        for (int i = 0; i < 2; i++)
        {
            MeshRenderer render = otherSpineModels[i].GetComponent<MeshRenderer>();
            render.material = BoneMaterials[1];
            render.enabled = false;
        }

        for (int i = 0; i < LineObjectList.Count; i++)
        {
            LineObjectList[i].SetActive(false);
        }

        if (skinSurface.Raycast(PresetRay, out RaycastHit hit, float.MaxValue))
        {
            PresetCastPoint.transform.position = hit.point;
            Vector3 normal = hit.normal;
            PresetCastPoint.transform.up = normal;
            GameObject collidedObject = hit.collider.gameObject;
        }
        RaycastHit BonePresetRayHit;
        if (BoneSurface[xindexofdisplayplane].Raycast(PresetRay, out BonePresetRayHit, float.MaxValue))
        {
            Vector3 intersectionPoint = BonePresetRayHit.point; // 获取交点位置
            PresetCastBone.transform.position = intersectionPoint;
        }
        PresetCastPoint.transform.SetParent(positions[currentIndex]);
        JudgeBall.transform.position = PresetCastPoint.transform.position;//碰撞小球位置
        PresetcastPosition = PresetCastPoint.transform.position;
        PresetcastNormal = PresetCastPoint.transform.up;
        presetnail.SetActive(false);
        V3DYIndicater[6].SetActive(false);
        CastPointYaxis = PresetCastPoint.transform.position;
    }//更新预设点位置
    void setRealtimeIndicaterPosition() {

        if (isHande)
        {
            RealtimeGuideList[6].SetActive(true);
            RealtimeGuideList[0].SetActive(true);
            RealtimeGuideList[7].SetActive(false);
            RealtimeGuideList[3].SetActive(false);
            Debug.Log("手持引导器");
            RealtimeGuidePointA.transform.position = RealtimeGuideList[1].transform.position;
            RealtimeGuidePointA.transform.up = -RealtimeGuideList[1].transform.up;
            RealtimeGuidePointA.transform.SetParent(RealtimeGuideList[1].transform);
            RealtimeGuidePointB.transform.position = RealtimeGuideList[2].transform.position;
            RealtimeGuidePointB.transform.SetParent(RealtimeGuideList[2].transform);   
        }
        else
        {
            RealtimeGuideList[6].SetActive(false);
            RealtimeGuideList[0].SetActive(false);
            RealtimeGuideList[7].SetActive(true);
            RealtimeGuideList[3].SetActive(true);
            Debug.Log("机器引导器");
            RealtimeGuidePointA.transform.position = RealtimeGuideList[4].transform.position;
            RealtimeGuidePointA.transform.up = -RealtimeGuideList[4].transform.up;
            RealtimeGuidePointA.transform.SetParent(RealtimeGuideList[4].transform);
            RealtimeGuidePointB.transform.position = RealtimeGuideList[5].transform.position;
            RealtimeGuidePointB.transform.SetParent(RealtimeGuideList[5].transform);

        }


    }//设置是手持还是机器可见
    void ResetsPresetCollisionPosition()
    {

        PresetRay.origin = PresetPointB.transform.position;//B点作为射出原点
        PresetRay.direction = -presetnail.transform.up;//转换方向从B-A
        
        PresetInsertionDepth = Vector3.Distance(PresetCastPoint.transform.position, presetnail.transform.position);
        if (isHande)
        {
            RealtimeGuidePointC.transform.position = RealtimeGuidePointA.transform.position + RealtimeGuidePointA.transform.up * PresetInsertionDepth;
        }
        else
        {
            RealtimeGuidePointC.transform.position = RealtimeGuidePointA.transform.position - RealtimeGuidePointA.transform.up * PresetInsertionDepth;
        }

        RaycastHit skinPresetRayHit;
        if (skinSurface.Raycast(PresetRay, out skinPresetRayHit, float.MaxValue))
        {
            Vector3 intersectionPoint = skinPresetRayHit.point; // 获取交点位置
            PresetCastPoint.transform.position = intersectionPoint;
        }

        if (JudgeBall.bounds.Contains(RealtimeGuidePointA.transform.position))
        {
            ballisarrived = true;
            //创建2个平面，平面A：经过实时点A与射线垂直平面，平面B：经过实时点B与射线垂直平面
            planeAofcast = new Plane(PresetRay.direction, RealtimeGuidePointA.transform.position);
            planeBofcast = new Plane(PresetRay.direction, RealtimeGuidePointB.transform.position);
            planeCofcast = new Plane(PresetRay.direction, RealtimeGuidePointC.transform.position);
            //计算射线与平面的交点
            float rayDistanceofA;//返回A平面与射线交点
            float distanceofAwithAO = 10f;//A点跟平面射线交点间的距离
            float rayDistanceofB;//返回B平面与射线交点
            Vector3 intersectionPointofB = new Vector3(0, 0, 0);//存放B平面与射线的交点
            float rayDistanceofC;//返回B平面与射线交点
            Vector3 intersectionPointofC = new Vector3(0, 0, 0);//存放B平面与射线的交点
            EntryEviation = Vector3.Distance(RealtimeGuidePointA.transform.position, ArrivedBallA0.transform.position) * 1000f;
            ExportEviation = Vector3.Distance(RealtimeGuidePointC.transform.position, ArrivedBallC0.transform.position) * 1000f;


            PresetInsertionDepth = Vector3.Distance(PresetCastPoint.transform.position, presetnail.transform.position);
            RealtimeGuidePointC.transform.SetParent(RealtimeGuidePointA.transform);
            if (isHande)
            {
                RealtimeGuidePointC.transform.position = RealtimeGuidePointA.transform.position + RealtimeGuidePointA.transform.up * PresetInsertionDepth;
            }
            else
            {
                RealtimeGuidePointC.transform.position = RealtimeGuidePointA.transform.position - RealtimeGuidePointA.transform.up * PresetInsertionDepth;
            }

            //计算平面与射线交点位置
            if (planeAofcast.Raycast(PresetRay, out rayDistanceofA))
            {
                Vector3 intersectionPointofA = PresetRay.GetPoint(rayDistanceofA); 
                presetnewposition.transform.position = intersectionPointofA;
                presetnewposition.transform.parent = skinSurface.transform.parent;
                float incrementZ = presetnewposition.transform.localPosition.z - presetbeginposition.transform.localPosition.z;
                skinSurface.transform.localPosition = new Vector3(skinbeginlicalPosition.x , skinbeginlicalPosition.y , skinbeginlicalPosition.z + incrementZ);
                distanceofAwithAO = Vector3.Distance(RealtimeGuidePointA.transform.position, intersectionPointofA);
                ArrivedBallA0.transform.position = intersectionPointofA;
                ArrivedBallA0.transform.up = -PresetRay.direction;
                bool currentA1isarrived = !A1isarrived;
                bool currentA0isarrived = !A0isarrived;
                if (ArrivedBallA1.bounds.Contains(RealtimeGuidePointA.transform.position))
                {
                    A1isarrived = true;
                    //Debug.Log("A1到了");
                    if (ArrivedBallA0.bounds.Contains(RealtimeGuidePointA.transform.position))
                    {
                        A0isarrived = true;
                        //Debug.Log("A0到了，出口点偏差为" + EntryEviation);
                    }
                    else
                    {
                        A0isarrived = false;
                        //Debug.Log("A0还没有到");
                    }
                }
                else
                {
                    A1isarrived = false;
                    A0isarrived = false;
                    //Debug.Log("A1还没有到");
                }
                if (currentA1isarrived && A1isarrived)
                {
                    // A1isarrived变为true时记录一次
                    ArrivedCount[0]++;
                    Debug.Log("A1到了" + ArrivedCount[0]+"次");
                }
                if (currentA0isarrived && A0isarrived)
                {
                    // A1isarrived变为true时记录一次
                    ArrivedCount[1]++;
                    Debug.Log("A0到了" + ArrivedCount[1] + "次");
                }
            }
            if (planeBofcast.Raycast(PresetRay, out rayDistanceofB))
            {
                intersectionPointofB = PresetRay.GetPoint(rayDistanceofB);
                float distanceofBwithBO = Vector3.Distance(RealtimeGuidePointB.transform.position, intersectionPointofB);
                ArrivedBallB0.transform.position = intersectionPointofB;
                if (ArrivedBallB1.bounds.Contains(RealtimeGuidePointB.transform.position))
                {
                    B1isarrived = true;
                    //Debug.Log("B1到了");
                    if (ArrivedBallB0.bounds.Contains(RealtimeGuidePointB.transform.position))
                    {
                        B0isarrived = true;
                        //Debug.Log("B0到了，出口点偏差为" + ExportEviation);
                    }
                    else
                    {
                        B0isarrived = false;
                        //Debug.Log("B0还没有到");
                    }
                }
                else
                {
                    B1isarrived = false;
                    B0isarrived = false;
                    //Debug.Log("B1还没有到");
                }
            }
            bool currentC1isarrived = !C1isarrived;
            bool currentC0isarrived = !C0isarrived;
            if (planeCofcast.Raycast(PresetRay, out rayDistanceofC))
            {
                intersectionPointofC = PresetRay.GetPoint(rayDistanceofC);
                ArrivedBallC0.transform.position = intersectionPointofC;
                if (ArrivedBallC1.bounds.Contains(RealtimeGuidePointC.transform.position))
                {
                    C1isarrived = true;
                    //Debug.Log("C1到了");
                    if (ArrivedBallC0.bounds.Contains(RealtimeGuidePointC.transform.position))
                    {
                        C0isarrived = true;
                        //Debug.Log("C0到了");
                    }
                    else
                    {
                        C0isarrived = false;
                    }
                }
                else
                {
                    C1isarrived = false;
                    C0isarrived = false;
                }
                if (currentC1isarrived && C1isarrived)
                {
                    // A1isarrived变为true时记录一次
                    ArrivedCount[2]++;
                }
                if (currentC0isarrived && C0isarrived)
                {
                    // A1isarrived变为true时记录一次
                    ArrivedCount[3]++;
                }
            }
            bool currentALL1isarrived = !ALL1isarrived;
            bool currentALL0isarrived = !ALL0isarrived;
            if (C1isarrived && A1isarrived)
            {
                ALL1isarrived = true;
            }
            else {
                ALL1isarrived = false;
            }
            if (C0isarrived && A0isarrived)
            {
                ALL0isarrived = true;
            }
            else {
                ALL0isarrived = false;
            }


            if (currentALL1isarrived && ALL1isarrived)
            {
                ArrivedCount[4]++;
                Debug.Log("1都到了" + ArrivedCount[4] + "次");
            }
            if (currentALL0isarrived && ALL0isarrived)
            {
                ArrivedCount[5]++;
                Debug.Log("0都到了" + ArrivedCount[5] + "次");
            }

            if (JudgeBall.bounds.Contains(DrillPointA.transform.position) && A1isarrived)
            {
                Drillisarrived = true;
            }
            Drillduidge.transform.position = PresetCastPoint.transform.position;
            Drillduidge.transform.up = presetnail.transform.up;
            if (DrillGuidge.bounds.Contains(DrillPointA.transform.position)) 
            {
                if (Drillisarrived)
                {
                    DrillRay.origin = DrillPointA.transform.position;
                    DrillRay.direction = -DrillPointA.transform.up;
                    if (skinSurface.Raycast(DrillRay, out RaycastHit hit, float.MaxValue))
                    {
                        //Debug.Log("00000000000000");
                    }
                    else
                    {                        
                        RealtimeInsertionDepth = Vector3.Distance(PresetCastPoint.transform.position, DrillPointA.transform.position);
                        InsertionPercentage = (RealtimeInsertionDepth / PresetInsertionDepth) * 100f;
                        if (InsertionPercentage > InsertmaxValue)
                        {
                            InsertmaxValue = InsertionPercentage;  // 更新最大值
                            VR = realtimenail.transform.position - RealtimeGuidePointA.transform.position;
                            VP = presetnail.transform.position - PresetCastPoint.transform.position;
                            Realtimeendnail = realtimenail.transform.position;
                            if (InsertionPercentage > 10) 
                            {
                                RaycastHit BonePresetRayHit;
                                Ray RealtimeDrill = new Ray(Vector3.zero, Vector3.forward);
                                RealtimeDrill.origin = RealtimeGuidePointA.transform.position;
                                Vector3 normalizedDirection = Vector3.Normalize(VR);
                                RealtimeDrill.direction = normalizedDirection;

                                if (BoneSurface[xindexofdisplayplane].Raycast(PresetRay, out BonePresetRayHit, float.MaxValue))
                                {
                                    Vector3 intersectionPoint = BonePresetRayHit.point; // 获取交点位置
                                    PresetCastBone.transform.position = intersectionPoint;
                                }
                                if (BoneSurface[xindexofdisplayplane].Raycast(RealtimeDrill, out BonePresetRayHit, float.MaxValue))
                                {
                                    Vector3 intersectionPoint = BonePresetRayHit.point; // 获取交点位置
                                    RealtimeCastBone.transform.position = intersectionPoint;
                                    Debug.Log("骨头碰撞点为" + RealtimeCastBone.transform.position);
                                    ExportOffset = Vector3.Distance(realtimenail.transform.position, presetnail.transform.position) * 1000f;//出口偏移
                                    EnterOffset = Vector3.Distance(RealtimeCastBone.transform.position, PresetCastBone.transform.position) * 1000f;//入口偏移
                                }

                                Vector3 Preset_Entry_Median = ProjectPointToPlane(PresetCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.up);
                                Vector3 Preset_Entry_Coronal = ProjectPointToPlane(PresetCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.right);
                                Vector3 Preset_Entry_Transverse = ProjectPointToPlane(PresetCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward);
                                Vector3 Preset_Export_Median = ProjectPointToPlane(presetnail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.up);
                                Vector3 Preset_Export_Coronal = ProjectPointToPlane(presetnail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.right);
                                Vector3 Preset_Export_Transverse = ProjectPointToPlane(presetnail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward);

                                Vector3 Realtime_Entry_Median = ProjectPointToPlane(RealtimeCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.up);
                                Vector3 Realtime_Entry_Coronal = ProjectPointToPlane(RealtimeCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.right);
                                Vector3 Realtime_Entry_Transverse = ProjectPointToPlane(RealtimeCastBone.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward);
                                Vector3 Realtime_Export_Median = ProjectPointToPlane(realtimenail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.up);
                                Vector3 Realtime_Export_Coronal = ProjectPointToPlane(realtimenail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.right);
                                Vector3 Realtime_Export_Transverse = ProjectPointToPlane(realtimenail.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward);

                                Vector3 Preset_Line_Median = Preset_Export_Median - Preset_Entry_Median;
                                Vector3 Preset_Line_Coronal = Preset_Export_Coronal - Preset_Entry_Coronal;
                                Vector3 Preset_Line_Transverse = Preset_Export_Transverse - Preset_Entry_Transverse;

                                Vector3 Realtime_Line_Median = Realtime_Export_Median - Realtime_Entry_Median;                                                          
                                Vector3 Realtime_Line_Coronal = Realtime_Export_Coronal - Realtime_Entry_Coronal;
                                Vector3 Realtime_Line_Transverse = Realtime_Export_Transverse - Realtime_Entry_Transverse;
                                TestBall3.transform.position = PresetCastBone.transform.position;
                                TestBall4.transform.position = RealtimeCastBone.transform.position;
                                TestBall1.transform.position = presetnail.transform.position;
                                TestBall2.transform.position = realtimenail.transform.position;
                                int decimalPlaces = 2; // 限制小数点后的位数
                                float angle_Median = Vector3.Angle(Realtime_Line_Median, Preset_Line_Median); // 计算夹角
                                float angle_Coronal = Vector3.Angle(Realtime_Line_Coronal, Preset_Line_Coronal); // 计算夹角
                                float angle_Transverse = Vector3.Angle(Realtime_Line_Transverse, Preset_Line_Transverse); // 计算夹角
                                if (angle_Median > 90.0f)
                                {
                                    angle_Median = 180.0f - angle_Median;
                                }
                                if (angle_Coronal > 90.0f)
                                {
                                    angle_Coronal = 180.0f - angle_Coronal;
                                }
                                if (angle_Transverse > 90.0f)
                                {
                                    angle_Transverse = 180.0f - angle_Transverse;
                                }
                                MedianOffset = Mathf.Round(angle_Median * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                                CoronalOffset = Mathf.Round(angle_Coronal * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                                TransverseOffset = Mathf.Round(angle_Transverse * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                                //Debug.Log("角度偏移1" + MedianOffset+ "角度偏移2" + CoronalOffset + "角度偏移3" + TransverseOffset);
                            }
                        }
                        //Debug.Log("植入百分比" + InsertmaxValue + "%");
                        //Debug.Log("骨头入口偏移" + EnterOffset+"骨头出口偏移"+ ExportOffset);
                    }

                }
            }

            if (A0isarrived && A0isarrived) { ABNumber0 = 2; } else if (A0isarrived && !B0isarrived) { ABNumber0 = 1; } else { ABNumber0 = 0; }
            if (A1isarrived && B1isarrived) { ABNumber1 = 2; } else if (A1isarrived && !B1isarrived) { ABNumber1 = 1; } else { ABNumber1 = 0; }

        }
        else { ballisarrived = false; }
    }//A点到/N点到/钻头到
    void setLinePoint()
    {
        RealtimeGuideLine3D.enabled = true;
        PresetLine3D.enabled = true;
        Vector3 RealtimeDriect = RealtimeGuidePointA.transform.up; 
        Vector3 RealtimePointEnd = RealtimeGuidePointA.transform.position + RealtimeDriect * PresetInsertionDepth;
        //实时绘制两点位置——3D原位手持
        RealtimeGuideLine3D.SetPosition(0, RealtimeGuidePointB.transform.position);
        RealtimeGuideLine3D.SetPosition(1, RealtimeGuidePointC.transform.position);
        PresetLine3D.SetPosition(0, PresetPointA.transform.position);
        PresetLine3D.SetPosition(1, PresetPointB.transform.position);
    }
    // -------------显示2D非原位可视化图像------------
    void imagerote()
    {
        // 更新旋转时间
        rotationTimer += Time.deltaTime;

        // 获取起始旋转
        startRotation = roteimage.transform.rotation;

        float rotationProgress = Mathf.Clamp01(rotationTimer / rotationDuration);

        // 根据缓入缓出效果计算旋转插值权重
        float smoothedProgress = Mathf.SmoothStep(0f, 1f, rotationProgress);

        // 使用插值权重进行旋转
        roteimage.transform.rotation = Quaternion.Slerp(startRotation, targetRotation, smoothedProgress);

        Quaternion rotationToMatchUp = Quaternion.FromToRotation(roteimage.transform.up, skinSurface.gameObject.transform.up);

        // 应用旋转到roteimage
        roteimage.transform.rotation = rotationToMatchUp * roteimage.transform.rotation;

        // 检查旋转是否完成一个阶段
        if (rotationTimer >= rotationDuration)
        {
            Vector3 currentup = roteimage.transform.up;
            Vector3 currentright = roteimage.transform.right;

            // 计算旋转到forward轴的目标旋转
            targetRotation = Quaternion.LookRotation(currentright, currentup);
            rotationTimer = 0f;
        }
    }
    void HideroteObject()
    {
        aciverote.SetActive(false);
    }

    // ------------------------------------------------ 定义可视化方案 -------------------------------------------------
    void Visualization1()
    {
        isHande = false;

        setRealtimeIndicaterPosition();
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 0);
        }
    }//3D原位-机器人  3D original robot
    void Visualization2()
    {
        isHande = true;

        setRealtimeIndicaterPosition();
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 1);
        }
    }//3D原位-手持    3D original Hand
    void Visualization3()
    {
        isHande = false;

        setRealtimeIndicaterPosition();
        ControllerV3 = true;
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 2);
        }
    }//3D非原-机器人  3D nooriginal robot
    void Visualization4()
    {
        isHande = true;

        setRealtimeIndicaterPosition();
        ControllerV4 = true;
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 3);
        }
    }//3D非原-手  持  3D nooriginal Hand
    void Visualization5()
    {
        isHande = false;

        setRealtimeIndicaterPosition();
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 4);
        }
    }//2D原位-机器人  2D original robot
    void Visualization6()
    {
        isHande = true;

        setRealtimeIndicaterPosition();
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 5);
        }
    }//2D投影-手  持  2D original Hand
    void Visualization7()
    {
        isHande = false;

        setRealtimeIndicaterPosition();

        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 6);
        }
    }//2D投影-机器人  2D nooriginal robot
    void Visualization8()
    {
        isHande = true;

        setRealtimeIndicaterPosition();

        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 7);
        }
    }//2D非原-手  持  2D nooriginal Hand
    void Visualization9()
    {
        isHande = false;

        setRealtimeIndicaterPosition();

        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 8);
        }
    }//2D原位-手  持  2D nooriginal robot
    void Visualization10()
    {
        isHande = true;

        setRealtimeIndicaterPosition();
        for (int i = 0; i < VisualizationEnabled.Length; i++)
        {
            VisualizationEnabled[i] = (i == 9);
        }
    }//2D原位-机器人 2D nooriginal robot


    public void UpVision3DY()
    {
        RealtimeGuideList[0].SetActive(false); RealtimeGuideList[3].SetActive(false);
        V3DYIndicater[6].transform.position = PresetCastPoint.transform.position;//指示位置
        V3DYIndicater[6].transform.up = presetnail.transform.up;//指示旋转
        //朝向相机
        textMeshs[0].transform.LookAt(textMeshs[0].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
        textMeshs[1].transform.LookAt(textMeshs[1].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up); 
        int decimalPlaces = 2; // 限制小数点后的位数
        float roundeEntryEviation = Mathf.Round(EntryEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
        float roundeExportEviation = Mathf.Round(ExportEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
        Vector3 PresetGuideDriection = PresetPointA.transform.position - PresetPointB.transform.position;
        Ray PresetRayC = new Ray(Vector3.zero, Vector3.forward);
        Ray RealtimerayD = new Ray(Vector3.zero, Vector3.forward);
        //float rayDistanceofC;//返回平面与射线交点

        //指示B
        V3DYFImageObject[1].transform.position = V3DYIndicater[1].transform.position;
        V3DYFImageObject[1].transform.right = -V3DYIndicater[6].transform.up;
        Vector3 abDirectionB = RealtimeGuidePointB.transform.position - V3DYIndicater[1].transform.position;
        Vector3 planeDriectionB = V3DYIndicater[1].transform.position - V3DYIndicater[0].transform.position;
        Vector3 projectionDirectionB = castdriection(abDirectionB, planeDriectionB);
        Quaternion V3DYFImageObject0rotationB = Quaternion.FromToRotation(V3DYFImageObject[1].transform.up, projectionDirectionB);
        V3DYFImageObject[1].transform.rotation = V3DYFImageObject0rotationB * V3DYFImageObject[1].transform.rotation;


        //指示A
        V3DYFImageObject[0].transform.position = V3DYIndicater[0].transform.position;
        V3DYFImageObject[0].transform.right = -V3DYIndicater[6].transform.up;
        Vector3 abDirectionA = RealtimeGuidePointA.transform.position - V3DYIndicater[0].transform.position;
        Vector3 planeDriectionA = V3DYIndicater[0].transform.position - V3DYIndicater[1].transform.position;
        Vector3 projectionDirectionA = castdriection(abDirectionA, planeDriectionA);
        Quaternion V3DYFImageObject0rotationA = Quaternion.FromToRotation(V3DYFImageObject[0].transform.up, projectionDirectionA);
        V3DYFImageObject[0].transform.rotation = V3DYFImageObject0rotationA * V3DYFImageObject[0].transform.rotation;


        Debug.DrawLine(V3DYIndicater[0].transform.position, V3DYIndicater[0].transform.position + projectionDirectionA * 0.05F);
        //文字显示
        textMeshs[0].transform.SetParent(V3DYIndicater[0].transform);   textMeshs[0].text = roundeEntryEviation.ToString() + "mm";   textMeshs[0].transform.localPosition = new Vector3(-0.05f, 0, 0);
        textMeshs[1].transform.SetParent(V3DYIndicater[1].transform);   textMeshs[1].text = roundeExportEviation.ToString() + "mm";  textMeshs[1].transform.localPosition = new Vector3(-0.05f, 0, 0);
        if (A0isarrived && C0isarrived)
        {
            PresetLine3D.material = Linematerials[1];
            RealtimeGuideLine3D.material = Linematerials[1];
        }
        else if (A1isarrived || C1isarrived)
        {
            //预设线变白//实时线变黄
            PresetLine3D.material = Linematerials[0];
            RealtimeGuideLine3D.material = Linematerials[3];
        }
        else if (!A0isarrived && !C0isarrived)
        {
            PresetLine3D.material = Linematerials[0];
            RealtimeGuideLine3D.material = Linematerials[2];
        }

        if (!A1isarrived && !C1isarrived)
        {
            textMeshs[0].color = Color.white; textMeshs[1].color = Color.white;
            V3DMaterial.mainTexture = guidetexture[0];
            RealtimeIndicaterBallA.color = Color.magenta; RealtimeIndicaterBallB.color = Color.magenta;
            V3DYFImage[0].color = Color.magenta; V3DYFImage[1].color = Color.magenta;
        }
        if (A1isarrived && !C1isarrived)
        {
            if (A0isarrived)
            {
                V3DMaterial.mainTexture = guidetexture[2]; textMeshs[0].color = Color.green; RealtimeIndicaterBallA.color = Color.green; V3DYFImage[0].color = Color.green; 
            }
            else 
            { 
                V3DMaterial.mainTexture = guidetexture[1]; textMeshs[0].color = Color.yellow; RealtimeIndicaterBallA.color = Color.yellow; V3DYFImage[0].color = Color.yellow;
            }
            textMeshs[1].color = Color.white; RealtimeIndicaterBallB.color = Color.magenta; V3DYFImage[1].color = Color.magenta;
        }
        if (!A1isarrived && C1isarrived)
        {
            if (C0isarrived)
            {
                V3DMaterial.mainTexture = guidetexture[6]; textMeshs[1].color = Color.green; RealtimeIndicaterBallB.color = Color.green; V3DYFImage[1].color = Color.green;
            }
            else
            {
                V3DMaterial.mainTexture = guidetexture[3]; textMeshs[1].color = Color.yellow; RealtimeIndicaterBallB.color = Color.yellow; V3DYFImage[1].color = Color.yellow;
            }
            textMeshs[0].color = Color.white; RealtimeIndicaterBallA.color = Color.magenta; V3DYFImage[0].color = Color.magenta;
        }
        if (A1isarrived && C1isarrived)
        {
            if (A0isarrived && C0isarrived)
            {
                V3DMaterial.mainTexture = guidetexture[8]; textMeshs[0].color = Color.green; textMeshs[1].color = Color.green;
                RealtimeIndicaterBallA.color = Color.green; RealtimeIndicaterBallB.color = Color.green; V3DYFImage[0].color = Color.green; V3DYFImage[1].color = Color.green;
            }
            else 
            {
                if (A0isarrived && !C0isarrived) 
                {   V3DMaterial.mainTexture = guidetexture[5]; textMeshs[0].color = Color.green; textMeshs[1].color = Color.yellow;
                    RealtimeIndicaterBallA.color = Color.green; RealtimeIndicaterBallB.color = Color.yellow; V3DYFImage[0].color = Color.green; V3DYFImage[1].color = Color.yellow;}
                if (!A0isarrived && C0isarrived) { V3DMaterial.mainTexture = guidetexture[7]; textMeshs[0].color = Color.yellow; textMeshs[1].color = Color.green;
                    RealtimeIndicaterBallA.color = Color.yellow; RealtimeIndicaterBallB.color = Color.green; V3DYFImage[0].color = Color.yellow; V3DYFImage[1].color = Color.green;}
                if (!A0isarrived && !C0isarrived) { V3DMaterial.mainTexture = guidetexture[4]; textMeshs[0].color = Color.yellow; textMeshs[1].color = Color.yellow;
                    RealtimeIndicaterBallA.color = Color.yellow; RealtimeIndicaterBallB.color = Color.yellow; V3DYFImage[0].color = Color.yellow; V3DYFImage[1].color = Color.yellow;}
            }
        }

        //方向引导——————到达判断球后就执行
        if (ballisarrived)
        {
            V3DYFImageObject[0].SetActive(true);
            V3DYFImageObject[1].SetActive(true);
            textMeshs[3].enabled = false;
            textMeshs[0].enabled = true; textMeshs[1].enabled = true;

            if (A1isarrived && C1isarrived || InsertmaxValue > 0)
            {
                if (InsertmaxValue > 0.1)
                {
                    V3DYIndicater[0].SetActive(false);       V3DYIndicater[1].SetActive(false); V3DYIndicater[2].SetActive(false);                    
                }
                else 
                {
                    V3DYIndicater[0].SetActive(true);       V3DYIndicater[1].SetActive(true); V3DYIndicater[2].SetActive(true);                    
                }
                textMeshs[3].enabled = false;
                //深度植入提示信息
                V3DYcopyobject[1].SetActive(true);
                textMeshs[2].enabled = true;
                V3DYcopyobject[1].transform.SetParent(presetnail.transform);
                V3DYcopyobject[2].transform.position = spawnedObjects1[10].position;
                V3DYcopyobject[2].transform.SetParent(spawnedObjects1[10]);
                V3DYcopyobject[2].transform.rotation = spawnedObjects1[10].rotation;
                V3DYcopyobject[1].transform.up = presetnail.transform.up;
                currentSpacing = Vector3.Distance(PresetCastPoint.transform.position, presetnail.transform.position) / 11;
                Vector3 Position = presetnail.transform.position + presetnail.transform.right * 0.02f;
                for (int i = 0; i < 11; i++)
                {
                    spawnedObjects1[i].gameObject.SetActive(true);
                    Vector3 newPosition = Position + presetnail.transform.up * i * currentSpacing;
                    spawnedObjects1[i].position = newPosition;
                    spawnedObjects1[i].up = presetnail.transform.up;
                }
                //跟随相机旋转
                Plane cameraplaneA = new Plane(presetnail.transform.up, presetnail.transform.position);
                Ray camerarayA = new Ray(mainCameraTransform.position, -presetnail.transform.up);
                float rayDistanceofplaneA = 0f;
                if (cameraplaneA.Raycast(camerarayA, out rayDistanceofplaneA))
                {
                    Vector3 PointofA = camerarayA.GetPoint(rayDistanceofplaneA);
                    Vector3 DriectionA = PointofA - presetnail.transform.position;
                    Quaternion rotationA = Quaternion.FromToRotation(V3DYcopyobject[1].transform.forward, DriectionA);
                    V3DYcopyobject[1].transform.position = presetnail.transform.position;
                    V3DYcopyobject[1].transform.rotation = rotationA * V3DYcopyobject[1].transform.rotation;
                }
                //显示数字,并跟随，限制文字位置
                int decimalPlace0 = 0; // 限制小数点后的位数
                float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                float convernumber = currentSpacing * 10f / 0.1061943F;//初始长度除以现在长度换算后的scale总长
                if (InsertmaxValue >= 0 && InsertmaxValue <= 100)
                {
                    V3DYcopyobject[2].SetActive(true);
                    V3DYcopyobject[2].transform.localScale = new Vector3(1f, InsertmaxValue * convernumber * 0.01f, 1f);
                    Renderer cruberenderer = V3DYcopyobject[2].GetComponent<Renderer>();
                    cruberenderer.material = V3DYFmaterials[0];
                    float distance = currentSpacing * 10;
                    textMeshs[2].transform.SetParent(V3DYcopyobject[1].transform);
                    Vector3 initialPosition = spawnedObjects1[10].position + spawnedObjects1[10].right * 0.03f;
                    textMeshs[2].transform.position = initialPosition - presetnail.transform.up * distance * InsertmaxValue * 0.01f;
                }
                else if (InsertmaxValue > 100)
                {
                    V3DYcopyobject[2].transform.localScale = new Vector3(1f, convernumber, 1f);
                    Renderer cruberenderer = V3DYcopyobject[2].GetComponent<Renderer>();
                    cruberenderer.material = V3DYFmaterials[1];
                    textMeshs[2].color = Color.red;
                }
                textMeshs[2].transform.LookAt(textMeshs[2].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
            }
            else
            {
                for (int i = 0; i < V3DYcopyobject.Length; i++)
                {
                    V3DYcopyobject[i].SetActive(false);
                }
                for (int i = 0; i < 11; i++)
                {
                    spawnedObjects1[i].gameObject.SetActive(false);
                }
                textMeshs[2].enabled = false;
                textMeshs[0].transform.SetParent(V3DYIndicater[0].transform);
                textMeshs[1].transform.SetParent(V3DYIndicater[1].transform);
            }
        }
        else
        {
            textMeshs[0].enabled = false; textMeshs[1].enabled = false; textMeshs[3].enabled = true;
            V3DYFImageObject[0].SetActive(false); V3DYFImageObject[1].SetActive(false);
            textMeshs[3].transform.position = (V3DYIndicater[0].transform.position + V3DYIndicater[1].transform.position) / 2;
            textMeshs[3].transform.SetParent(V3DYIndicater[1].transform);
            textMeshs[3].text = "Move here"; 
            textMeshs[3].transform.localPosition = new Vector3(0, -0.05f, 0);
        }


        //实时绘制两点位置——3D原位机器人
        setLinePoint();
    }
    public void StVision3DY()
    {
        V3DYFImageObject[0].SetActive(false);V3DYFImageObject[1].SetActive(false); V3DYFImageObject[2].SetActive(false); V3DYFImageObject[3].SetActive(false);
        RealtimeGuideList[0].SetActive(false); RealtimeGuideList[3].SetActive(false);
        V3DMaterial.mainTexture = guidetexture[0];
        presetnail.SetActive(true);
        textMeshs[0].enabled = true; textMeshs[1].enabled = true;
        //设置引导器显示隐藏 
        V3DYIndicater[6].SetActive(true);//指导线环
        //line
        PresetLine3D.enabled = true; RealtimeGuideLine3D.enabled = true; LineObjectList[0].SetActive(true); LineObjectList[5].SetActive(true);
        V3DYIndicater[6].transform.SetParent(positions[currentIndex]);//放到植入位置下一起移动
        V3DYIndicater[6].transform.position = PresetcastPosition;//指示位置
        V3DYIndicater[6].transform.up = -PresetcastNormal;//指示旋转
        RealtimeGuidePointA.transform.localEulerAngles = new Vector3(0, 0, 0);
        V3DYIndicater[7].transform.position = RealtimeGuidePointA.transform.position;
        V3DYIndicater[7].transform.SetParent(RealtimeGuidePointA.transform);
        RealtimeGuidePointA.transform.localScale = new Vector3(1f, 1f, 1f);
        V3DYIndicater[7].transform.localScale = new Vector3(1f, 1f, 1f);
        if (isHande)
        {
            V3DYIndicater[7].transform.localEulerAngles = new Vector3(0, 0, 180);
        }
        else
        {
            V3DYIndicater[7].transform.localEulerAngles = new Vector3(0, 0, 0);
        }

        textMeshs[0].transform.SetParent(V3DYIndicater[6].transform);
        currentSpacing = Vector3.Distance(PresetcastPosition, presetnail.transform.position) / 11;
        V3DYcopyobject[1].SetActive(false);
        RealtimeIndicaterBallA.color = Color.magenta;
        RealtimeIndicaterBallB.color = Color.magenta;
        for (int i = 0; i < 11; i++)
        {
            Vector3 spawnPosition = PresetCastPoint.transform.position + PresetCastPoint.transform.up * i * currentSpacing;
            spawnedObjects1[i] = Instantiate(V3DYcopyobject[0], spawnPosition, Quaternion.identity).transform;
            spawnedObjects1[i].SetParent(DisplayPlane[0].transform);
        }
        for (int i = 0; i < 11; i++)
        {
            spawnedObjects1[i].SetParent(V3DYcopyobject[1].transform);
        }
        for (int i = 0; i < V3DYIndicater.Length; i++)
        {
            V3DYIndicater[i].SetActive(true);
        }
        V3DYisactive = true;
    }

    public void UpVision3DF()
    {
        V3DFcopyiscomplete = NetTest.GetComponent<NetTest>().V3DFcopycomplete();
        if (V3DFcopyiscomplete && !V3DFcopyisupdate)
        {

            CopiesListX = NetTest.GetComponent<NetTest>().ListX();
            CopiesListZ = NetTest.GetComponent<NetTest>().ListZ();
            CopiesListX[0].transform.localScale = new Vector3(10,10,10); CopiesListZ[0].transform.localScale = new Vector3(10, 10, 10);
            inderct1 = CopiesListX[8].transform.Find("inderct").gameObject;
            inderct2 = CopiesListZ[8].transform.Find("inderct").gameObject;
            CopiesListX[8].SetActive(false); CopiesListZ[8].SetActive(false);
            CopiesListX[7].transform.SetParent(CopiesListX[4].transform); CopiesListX[7].transform.position = CopiesListX[4].transform.position;
            CopiesListZ[7].transform.SetParent(CopiesListZ[4].transform); CopiesListZ[7].transform.position = CopiesListZ[4].transform.position;
            for (int i = 0; i < V3DYIndicater.Length; i++)
            {
                V3DYIndicater[i].SetActive(false);
            }
            for (int i = 0; i < 11; i++)
            {
                spawnedObjects1[i].SetParent(CopiesListX[8].transform);
                spawnedObjects2[i].SetParent(CopiesListZ[8].transform);
            }
            V3DFcopyisupdate = true;
        }
        if (V3DFcopyisupdate)
        {
            Vector3 Driection1 = CopiesListX[5].transform.position - CopiesListX[4].transform.position;
            CopiesListX[4].transform.up = -Driection1;
            CopiesListX[7].transform.up = Driection1;
            CopiesListX[6].transform.position = CopiesListX[9].transform.position;
            CopiesListX[6].transform.up = CopiesListX[1].transform.up;

            RealtimeNoRGuideLine3D1.SetPosition(0, CopiesListX[4].transform.position - Driection1 * 1.2f);
            RealtimeNoRGuideLine3D1.SetPosition(1, CopiesListX[4].transform.position + Driection1 * 0.7f);


            Vector3 Driection2 = CopiesListZ[5].transform.position - CopiesListZ[4].transform.position;
            CopiesListZ[4].transform.up = -Driection2;
            CopiesListZ[7].transform.up = Driection2;
            CopiesListZ[6].transform.position = CopiesListZ[9].transform.position;
            CopiesListZ[6].transform.up = CopiesListZ[1].transform.up;

            //8.设置线条渲染的两个端点
            RealtimeNoRGuideLine3D2.SetPosition(0, CopiesListZ[4].transform.position - Driection2 * 1.2f);
            RealtimeNoRGuideLine3D2.SetPosition(1, CopiesListZ[4].transform.position + Driection2 * 0.7f);

            //渲染线段--------------3D 预设 ----数字孪生1
            PresetLineNooriginal3D1.SetPosition(0, CopiesListX[1].transform.position);
            PresetLineNooriginal3D1.SetPosition(1, CopiesListX[2].transform.position);
            //渲染线段--------------3D 预设 ----数字孪生2
            PresetLineNooriginal3D2.SetPosition(0, CopiesListZ[1].transform.position);
            PresetLineNooriginal3D2.SetPosition(1, CopiesListZ[2].transform.position);

            if (cubeCollider[0].bounds.Contains(CopiesListX[4].transform.position))
            {
                RealtimeNoRGuideLine3D1.enabled = true;
                CopiesListX[4].SetActive(true);
            }
            else
            {
                RealtimeNoRGuideLine3D1.enabled = false;
                CopiesListX[4].SetActive(false);
            }
            if (cubeCollider[0].bounds.Contains(CopiesListX[10].transform.position))
            {
                CopiesListX[10].SetActive(true);
            }
            else
            {
                CopiesListX[10].SetActive(false);
            }
            if (cubeCollider[1].bounds.Contains(CopiesListZ[4].transform.position))
            {
                RealtimeNoRGuideLine3D2.enabled = true;
                CopiesListZ[4].SetActive(true);
            }
            else
            {
                RealtimeNoRGuideLine3D2.enabled = false;
                CopiesListZ[4].SetActive(false);
            }
            if (cubeCollider[1].bounds.Contains(CopiesListZ[10].transform.position))
            {
                CopiesListZ[10].SetActive(true);
            }
            else
            {
                CopiesListZ[10].SetActive(false);
            }

            GameObject PB11 = CopiesListX[6].transform.Find("PBall1").gameObject;
            GameObject RB11 = CopiesListX[7].transform.Find("RBall1").gameObject;
            GameObject PB12 = CopiesListX[6].transform.Find("PBall2").gameObject;
            GameObject RB12 = CopiesListX[7].transform.Find("RBall2").gameObject;
            GameObject PB21 = CopiesListZ[6].transform.Find("PBall1").gameObject;
            GameObject RB21 = CopiesListZ[7].transform.Find("RBall1").gameObject;
            GameObject PB22 = CopiesListZ[6].transform.Find("PBall2").gameObject;
            GameObject RB22 = CopiesListZ[7].transform.Find("RBall2").gameObject;
            GameObject PtransparentZ = CopiesListZ[6].transform.Find("Ptransparent").gameObject;
            GameObject PtransparentX = CopiesListX[6].transform.Find("Ptransparent").gameObject;

            V3DYFImageObject[0].transform.position = PB11.transform.position;
            V3DYFImageObject[0].transform.right = -CopiesListX[6].transform.up;
            Vector3 abDirectionAX = CopiesListX[4].transform.position - PB11.transform.position;
            Vector3 planeDriectionAX = PB11.transform.position - PB12.transform.position;
            Vector3 projectionDirectionAX = castdriection(abDirectionAX, planeDriectionAX);
            Quaternion V3DYFImageObject0rotationAX = Quaternion.FromToRotation(V3DYFImageObject[0].transform.up, projectionDirectionAX);
            V3DYFImageObject[0].transform.rotation = V3DYFImageObject0rotationAX * V3DYFImageObject[0].transform.rotation;

            V3DYFImageObject[1].transform.position = PB12.transform.position;
            V3DYFImageObject[1].transform.right = -CopiesListX[6].transform.up;
            Vector3 abDirectionBX = CopiesListX[5].transform.position - PB12.transform.position;
            Vector3 planeDriectionBX = PB12.transform.position - PB11.transform.position;
            Vector3 projectionDirectionBX = castdriection(abDirectionBX, planeDriectionBX);
            Quaternion V3DYFImageObject0rotationBX = Quaternion.FromToRotation(V3DYFImageObject[1].transform.up, projectionDirectionBX);
            V3DYFImageObject[1].transform.rotation = V3DYFImageObject0rotationBX * V3DYFImageObject[1].transform.rotation;

            V3DYFImageObject[2].transform.position = PB21.transform.position;
            V3DYFImageObject[2].transform.right = -CopiesListZ[6].transform.up;
            Vector3 abDirectionAZ = CopiesListZ[4].transform.position - PB21.transform.position;
            Vector3 planeDriectionAZ = PB21.transform.position - PB22.transform.position;
            Vector3 projectionDirectionAZ = castdriection(abDirectionAZ, planeDriectionAZ);
            Quaternion V3DYFImageObject0rotationAZ = Quaternion.FromToRotation(V3DYFImageObject[2].transform.up, projectionDirectionAZ);
            V3DYFImageObject[2].transform.rotation = V3DYFImageObject0rotationAZ * V3DYFImageObject[2].transform.rotation;

            V3DYFImageObject[3].transform.position = PB22.transform.position;
            V3DYFImageObject[3].transform.right = -CopiesListZ[6].transform.up;
            Vector3 abDirectionBZ = CopiesListZ[5].transform.position - PB22.transform.position;
            Vector3 planeDriectionBZ = PB22.transform.position - PB21.transform.position;
            Vector3 projectionDirectionBZ = castdriection(abDirectionBZ, planeDriectionBZ);
            Quaternion V3DYFImageObject0rotationBZ = Quaternion.FromToRotation(V3DYFImageObject[3].transform.up, projectionDirectionBZ);
            V3DYFImageObject[3].transform.rotation = V3DYFImageObject0rotationBZ * V3DYFImageObject[3].transform.rotation;

            if (A0isarrived && C0isarrived)
            {
                PresetLine3D.material = Linematerials[1];
                RealtimeGuideLine3D.material = Linematerials[1];
            }
            else if (A1isarrived || C1isarrived)
            {
                //预设线变白//实时线变黄
                PresetLine3D.material = Linematerials[0];
                RealtimeGuideLine3D.material = Linematerials[3];
            }
            else if (!A0isarrived && !C0isarrived)
            {
                PresetLine3D.material = Linematerials[0];
                RealtimeGuideLine3D.material = Linematerials[2];
            }

            if (!A1isarrived && !C1isarrived)
            {
                textMeshs[0].color = Color.white; textMeshs[1].color = Color.white; textMeshs[2].color = Color.white; textMeshs[3].color = Color.white;
                V3DMaterial.mainTexture = guidetexture[0];
                RealtimeIndicaterBallA.color = Color.magenta; RealtimeIndicaterBallB.color = Color.magenta;
                V3DYFImage[0].color = Color.magenta; V3DYFImage[1].color = Color.magenta; V3DYFImage[2].color = Color.magenta; V3DYFImage[3].color = Color.magenta;
            }
            if (A1isarrived && !C1isarrived)
            {
                if (A0isarrived)
                {
                    V3DMaterial.mainTexture = guidetexture[2]; textMeshs[0].color = Color.green; textMeshs[2].color = Color.green; RealtimeIndicaterBallA.color = Color.green; V3DYFImage[0].color = Color.green; V3DYFImage[2].color = Color.green;
                }
                else
                {
                    V3DMaterial.mainTexture = guidetexture[1]; textMeshs[0].color = Color.yellow; textMeshs[2].color = Color.yellow; RealtimeIndicaterBallA.color = Color.yellow; V3DYFImage[0].color = Color.yellow; V3DYFImage[2].color = Color.yellow;
                }
                textMeshs[1].color = Color.white; textMeshs[3].color = Color.white; RealtimeIndicaterBallB.color = Color.magenta; V3DYFImage[1].color = Color.magenta; V3DYFImage[3].color = Color.magenta;
            }
            if (!A1isarrived && C1isarrived)
            {
                if (C0isarrived)
                {
                    V3DMaterial.mainTexture = guidetexture[6]; textMeshs[1].color = Color.green; textMeshs[3].color = Color.green; RealtimeIndicaterBallB.color = Color.green; V3DYFImage[1].color = Color.green; V3DYFImage[3].color = Color.green;
                }
                else
                {
                    V3DMaterial.mainTexture = guidetexture[3]; textMeshs[1].color = Color.yellow; textMeshs[3].color = Color.yellow; RealtimeIndicaterBallB.color = Color.yellow; V3DYFImage[1].color = Color.yellow; V3DYFImage[3].color = Color.yellow;
                }
                textMeshs[0].color = Color.white; textMeshs[2].color = Color.white; RealtimeIndicaterBallA.color = Color.magenta; V3DYFImage[0].color = Color.magenta; V3DYFImage[2].color = Color.magenta;
            }
            if (A1isarrived && C1isarrived)
            {
                if (A0isarrived && C0isarrived)
                {
                    V3DMaterial.mainTexture = guidetexture[8]; textMeshs[0].color = Color.green; textMeshs[1].color = Color.green; textMeshs[2].color = Color.green; textMeshs[3].color = Color.green;
                    RealtimeIndicaterBallA.color = Color.green; RealtimeIndicaterBallB.color = Color.green;
                    V3DYFImage[0].color = Color.green; V3DYFImage[1].color = Color.green; V3DYFImage[2].color = Color.green; V3DYFImage[3].color = Color.green;
                }
                else
                {
                    if (A0isarrived && !C0isarrived)
                    {
                        V3DMaterial.mainTexture = guidetexture[5]; textMeshs[0].color = Color.green; textMeshs[1].color = Color.yellow; textMeshs[2].color = Color.green; textMeshs[3].color = Color.yellow;
                        RealtimeIndicaterBallA.color = Color.green; RealtimeIndicaterBallB.color = Color.yellow; 
                        V3DYFImage[0].color = Color.green; V3DYFImage[1].color = Color.yellow; V3DYFImage[2].color = Color.green; V3DYFImage[3].color = Color.yellow;
                    }
                    if (!A0isarrived && C0isarrived)
                    {
                        V3DMaterial.mainTexture = guidetexture[7]; textMeshs[0].color = Color.yellow; textMeshs[1].color = Color.green; textMeshs[2].color = Color.yellow; textMeshs[3].color = Color.green;
                        RealtimeIndicaterBallA.color = Color.yellow; RealtimeIndicaterBallB.color = Color.green; 
                        V3DYFImage[0].color = Color.yellow; V3DYFImage[1].color = Color.green; V3DYFImage[2].color = Color.yellow; V3DYFImage[3].color = Color.green;
                    }
                    if (!A0isarrived && !C0isarrived)
                    {
                        V3DMaterial.mainTexture = guidetexture[4]; textMeshs[0].color = Color.yellow; textMeshs[1].color = Color.yellow; textMeshs[2].color = Color.yellow; textMeshs[3].color = Color.yellow;
                        RealtimeIndicaterBallA.color = Color.yellow; RealtimeIndicaterBallB.color = Color.yellow; 
                        V3DYFImage[0].color = Color.yellow; V3DYFImage[1].color = Color.yellow; V3DYFImage[2].color = Color.yellow; V3DYFImage[3].color = Color.yellow;
                    }
                }
            }
            int decimalPlaces = 2; // 限制小数点后的位数
            float roundeEntryEviation = Mathf.Round(EntryEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
            float roundeExportEviation = Mathf.Round(ExportEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
            Vector3 RealtimeGuideDriection = RealtimeGuidePointB.transform.position - RealtimeGuidePointA.transform.position;
            PB11.SetActive(true); PB12.SetActive(true); PB21.SetActive(true); PB22.SetActive(true); PtransparentZ.SetActive(true); PtransparentX.SetActive(true);
            CopiesListX[6].SetActive(true); CopiesListZ[6].SetActive(true);
            V3DYFImageObject[0].SetActive(true); V3DYFImageObject[1].SetActive(true); V3DYFImageObject[2].SetActive(true); V3DYFImageObject[3].SetActive(true);
            if (ballisarrived)
            {
                SetObjectsState(textMeshs, false, 4, 5);
                SetObjectsState(textMeshs, true, 0, 1, 2, 3);
                V3DYFImageObject[0].SetActive(true); V3DYFImageObject[1].SetActive(true); V3DYFImageObject[2].SetActive(true); V3DYFImageObject[3].SetActive(true);
                //对齐条件——————到达B跟A后就执行/——开始植入
                if (A1isarrived && C1isarrived || InsertmaxValue > 0)
                {
                    if (A0isarrived && C0isarrived)
                    {     
                        //CopiesListX[6].SetActive(false); CopiesListZ[6].SetActive(false);
                        //PB11.SetActive(false); PB12.SetActive(false); PB21.SetActive(false); PB22.SetActive(false); 
                        //PtransparentZ.SetActive(false); PtransparentX.SetActive(false);
                        //V3DYFImageObject[0].SetActive(false); V3DYFImageObject[1].SetActive(false); V3DYFImageObject[2].SetActive(false); V3DYFImageObject[3].SetActive(false);
                        PresetLineNooriginal3D1.enabled = false; PresetLineNooriginal3D2.enabled = false;
                        RealtimeNoRGuideLine3D1.material = Linematerials[1]; RealtimeNoRGuideLine3D2.material = Linematerials[1];
                    }
                    else
                    {
                        //CopiesListX[6].SetActive(true); CopiesListZ[6].SetActive(true);
                        //PB11.SetActive(true); PB12.SetActive(true); PB21.SetActive(true); PB22.SetActive(true); 
                        PresetLineNooriginal3D1.enabled = true; PresetLineNooriginal3D2.enabled = true;
                        PresetLineNooriginal3D1.material = Linematerials[0]; PresetLineNooriginal3D2.material = Linematerials[0];
                        RealtimeNoRGuideLine3D1.material = Linematerials[2]; RealtimeNoRGuideLine3D2.material = Linematerials[2];
                    }
                    textMeshs[4].enabled = true; textMeshs[5].enabled = true;
                    for (int i = 0; i < 11; i++)
                    {
                        spawnedObjects1[i].gameObject.SetActive(true);
                        spawnedObjects2[i].gameObject.SetActive(true);
                    }

                    CopiesListX[8].SetActive(true); CopiesListZ[8].SetActive(true);
                    CopiesListX[8].transform.SetParent(CopiesListX[1].transform); CopiesListZ[8].transform.SetParent(CopiesListZ[1].transform);
                    CopiesListX[8].transform.up = CopiesListX[1].transform.up; CopiesListZ[8].transform.up = CopiesListZ[1].transform.up;
                    inderct1.transform.position = spawnedObjects1[10].position; inderct2.transform.position = spawnedObjects2[10].position;
                    inderct1.transform.SetParent(spawnedObjects1[10]); inderct2.transform.SetParent(spawnedObjects2[10]);
                    inderct1.transform.rotation = spawnedObjects1[10].rotation; inderct2.transform.rotation = spawnedObjects2[10].rotation;

                    currentSpacing = Vector3.Distance(PresetCastPoint.transform.position, presetnail.transform.position) / 11;
                    Vector3 Position1 = CopiesListX[1].transform.position + CopiesListX[8].transform.right * 0.02f;
                    for (int i = 0; i < 11; i++)
                    {
                        spawnedObjects1[i].gameObject.SetActive(true);
                        Vector3 newPosition = Position1 + CopiesListX[1].transform.up * i * currentSpacing;
                        spawnedObjects1[i].position = newPosition;
                        spawnedObjects1[i].up = CopiesListX[1].transform.up;
                    }
                    Vector3 Position2 = CopiesListZ[1].transform.position + CopiesListZ[8].transform.right * 0.02f;
                    for (int i = 0; i < 11; i++)
                    {
                        spawnedObjects2[i].gameObject.SetActive(true);
                        Vector3 newPosition = Position2 + CopiesListZ[1].transform.up * i * currentSpacing;
                        spawnedObjects2[i].position = newPosition;
                        spawnedObjects2[i].up = CopiesListZ[1].transform.up;
                    }
                    //跟随相机旋转
                    Plane cameraplaneA = new Plane(CopiesListX[1].transform.up, CopiesListX[1].transform.position);
                    Ray camerarayA = new Ray(mainCameraTransform.position, -CopiesListX[1].transform.up);
                    float rayDistanceofplaneA = 0f;
                    if (cameraplaneA.Raycast(camerarayA, out rayDistanceofplaneA))
                    {
                        Vector3 PointofA = camerarayA.GetPoint(rayDistanceofplaneA);
                        Vector3 DriectionA = PointofA - CopiesListX[1].transform.position;
                        Quaternion rotationA = Quaternion.FromToRotation(CopiesListX[8].transform.forward, DriectionA);
                        CopiesListX[8].transform.position = CopiesListX[1].transform.position;
                        CopiesListX[8].transform.rotation = rotationA * CopiesListX[8].transform.rotation;
                    }
                    //2
                    Plane cameraplaneB = new Plane(CopiesListZ[1].transform.up, CopiesListZ[1].transform.position);
                    Ray camerarayB = new Ray(mainCameraTransform.position, -CopiesListZ[1].transform.up);
                    float rayDistanceofplaneB = 0f;
                    if (cameraplaneB.Raycast(camerarayB, out rayDistanceofplaneB))
                    {
                        Vector3 PointofB = camerarayB.GetPoint(rayDistanceofplaneB);
                        Vector3 DriectionB = PointofB - CopiesListZ[1].transform.position;
                        Quaternion rotationB = Quaternion.FromToRotation(CopiesListZ[8].transform.forward, DriectionB);
                        CopiesListZ[8].transform.position = CopiesListZ[1].transform.position;
                        CopiesListZ[8].transform.rotation = rotationB * CopiesListZ[8].transform.rotation;
                    }
                    //显示数字,并跟随，限制文字位置
                    int decimalPlace0 = 0; // 限制小数点后的位数
                    float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                    float convernumber = currentSpacing * 10f / 0.1061943F;//初始长度除以现在长度换算后的scale总长
                    if (InsertmaxValue >= 0 && InsertmaxValue <= 100)
                    {
                        inderct1.SetActive(true);
                        inderct1.transform.localScale = new Vector3(1f, InsertmaxValue * convernumber * 0.01f, 1f);
                        Renderer cruberenderer1 = inderct1.GetComponent<Renderer>();
                        cruberenderer1.material = V3DYFmaterials[0];

                        inderct2.SetActive(true);
                        inderct2.transform.localScale = new Vector3(1f, InsertmaxValue * convernumber * 0.01f, 1f);
                        Renderer cruberenderer2 = inderct2.GetComponent<Renderer>();
                        cruberenderer2.material = V3DYFmaterials[0];

                        float distance = currentSpacing * 10;

                        textMeshs[4].transform.SetParent(CopiesListX[8].transform);
                        Vector3 initialPosition1 = spawnedObjects1[10].position + spawnedObjects1[10].right * 0.03f;
                        textMeshs[4].transform.position = initialPosition1 - CopiesListX[1].transform.up * distance * InsertmaxValue * 0.01f;

                        textMeshs[5].transform.SetParent(CopiesListZ[8].transform);
                        Vector3 initialPosition2 = spawnedObjects2[10].position + spawnedObjects2[10].right * 0.03f;
                        textMeshs[5].transform.position = initialPosition2 - CopiesListZ[1].transform.up * distance * InsertmaxValue * 0.01f;
                    }
                    else if (InsertmaxValue > 100)
                    {
                        inderct1.transform.localScale = new Vector3(1f, convernumber, 1f);
                        Renderer cruberenderer1 = inderct1.GetComponent<Renderer>();
                        cruberenderer1.material = V3DYFmaterials[1];
                        textMeshs[4].color = Color.red;

                        inderct2.transform.localScale = new Vector3(1f, convernumber, 1f);
                        Renderer cruberenderer2 = inderct2.GetComponent<Renderer>();
                        cruberenderer2.material = V3DYFmaterials[1];
                        textMeshs[5].color = Color.red;
                    }
                    textMeshs[4].transform.LookAt(textMeshs[1].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[4].text = DepthInsertmaxValue.ToString() + "%";
                    textMeshs[5].transform.LookAt(textMeshs[2].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[5].text = DepthInsertmaxValue.ToString() + "%";
                }
                else 
                {
                    for (int i = 0; i < 11; i++)
                    {
                        spawnedObjects1[i].gameObject.SetActive(false);
                        spawnedObjects2[i].gameObject.SetActive(false);
                    }
                    CopiesListX[8].SetActive(false); CopiesListZ[8].SetActive(false);
                }
                if (InsertmaxValue < 0.1)
                {
                    textMeshs[0].transform.position = PB11.transform.position + DisplayPlane[0].transform.right * 0.04f;
                    textMeshs[1].transform.position = PB12.transform.position + DisplayPlane[0].transform.right * 0.04f;
                    textMeshs[0].transform.LookAt(textMeshs[0].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[0].text = roundeEntryEviation.ToString() + "mm";
                    textMeshs[1].transform.LookAt(textMeshs[1].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[1].text = roundeExportEviation.ToString() + "mm";
                    textMeshs[2].transform.position = PB21.transform.position + DisplayPlane[0].transform.right * 0.04f;
                    textMeshs[3].transform.position = PB22.transform.position + DisplayPlane[0].transform.right * 0.04f;
                    textMeshs[2].transform.LookAt(textMeshs[2].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[2].text = roundeEntryEviation.ToString() + "mm";
                    textMeshs[3].transform.LookAt(textMeshs[3].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                    textMeshs[3].text = roundeExportEviation.ToString() + "mm";
                    textMeshs[0].fontSize = 0.1f; textMeshs[1].fontSize = 0.1f; textMeshs[2].fontSize = 0.1f; textMeshs[3].fontSize = 0.1f;

                    textMeshs[0].transform.SetParent(PB11.transform);
                    textMeshs[1].transform.SetParent(PB12.transform);
                    textMeshs[2].transform.SetParent(PB21.transform);
                    textMeshs[3].transform.SetParent(PB22.transform);
                }
            }
            else
            {
                V3DYFImageObject[0].SetActive(false); V3DYFImageObject[1].SetActive(false); V3DYFImageObject[2].SetActive(false); V3DYFImageObject[3].SetActive(false);
                textMeshs[0].enabled = false; textMeshs[1].enabled = false; textMeshs[2].enabled = false; textMeshs[3].enabled = false; textMeshs[4].enabled = true; textMeshs[5].enabled = true;
                textMeshs[4].transform.LookAt(textMeshs[0].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                textMeshs[5].transform.LookAt(textMeshs[1].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                textMeshs[4].transform.position = PB12.transform.position + CopiesListX[1].transform.up * 0.05f;
                textMeshs[4].transform.SetParent(PB12.transform);
                textMeshs[4].transform.localScale = new Vector3(1f, 1f, 1f);
                textMeshs[4].text = "Move here"; textMeshs[0].fontSize = 0.1f;
                textMeshs[5].transform.position = PB22.transform.position + CopiesListZ[1].transform.up * 0.05f;
                textMeshs[5].transform.SetParent(PB22.transform);
                textMeshs[5].transform.localScale = new Vector3(1f, 1f, 1f);
                textMeshs[5].text = "Move here"; textMeshs[1].fontSize = 0.1f;
            }
        }
    }
    public void StVision3DF()
    {
        Display3DX.SetActive(true); //3DX复制物体 显示
        Display3DZ.SetActive(true); //3DZ复制物体 显示
        //3DY
        for (int i = 0; i < 11; i++)
        {
            Vector3 spawnPosition = PresetCastPoint.transform.position + PresetCastPoint.transform.up * i * currentSpacing;
            spawnedObjects1[i] = Instantiate(V3DYcopyobject[0], spawnPosition, Quaternion.identity).transform;
            spawnedObjects1[i].SetParent(DisplayPlane[0].transform);
            spawnedObjects2[i] = Instantiate(V3DYcopyobject[0], spawnPosition, Quaternion.identity).transform;
            spawnedObjects2[i].SetParent(DisplayPlane[0].transform);
        }
        //copy物体显示
        for (int i = 0; i < V3DYIndicater.Length; i++)
        {
            V3DYIndicater[i].SetActive(true);
        }
        for (int i = 0; i < 11; i++)
        {
            spawnedObjects1[i].SetParent(null);
            spawnedObjects2[i].SetParent(null);
        }
        selectedmesh = SpineModels[xindexofdisplayplane];
        presetnail.SetActive(true); PresetPointB.SetActive(true); PresetPointA.SetActive(true);
        RealtimeGuidePointA.SetActive(true); RealtimeGuidePointB.SetActive(true); PresetCastPoint.SetActive(true);
        V3DYcopyobject[1].SetActive(true); V3DYcopyobject[0].SetActive(false); V3DYcopyobject[2].SetActive(false);
        V3DYFImageObject[0].SetActive(false); V3DYFImageObject[1].SetActive(false); V3DYFImageObject[2].SetActive(false); V3DYFImageObject[3].SetActive(false);
        //
        V3DYcopyobject[1].transform.parent = V3DYIndicater[6].transform.parent;
        cubeCollider[0].gameObject.SetActive(true);
        cubeCollider[1].gameObject.SetActive(true);
        Renderer renderer1 = cubeCollider[0].gameObject.GetComponent<Renderer>();
        renderer1.enabled = false;
        Renderer renderer2 = cubeCollider[1].gameObject.GetComponent<Renderer>();
        renderer2.enabled = false;
        V3DYIndicater[7].transform.SetParent(whole.transform);
        cubeCollider[0].gameObject.transform.position = Display3DX.transform.position;
        cubeCollider[0].gameObject.transform.rotation = Display3DX.transform.rotation;
        cubeCollider[1].gameObject.transform.position = Display3DZ.transform.position;
        cubeCollider[1].gameObject.transform.rotation = Display3DZ.transform.rotation;
        CopyList = new GameObject[] { selectedmesh, presetnail, PresetPointB, PresetPointA, RealtimeGuidePointA, RealtimeGuidePointB, V3DYIndicater[6], V3DYIndicater[7], V3DYcopyobject[1], PresetCastPoint, Drillmodel };
        RealtimeIndicaterBallA.color = Color.magenta; RealtimeIndicaterBallB.color = Color.magenta;
        presetnail.SetActive(false);
        presetnail.SetActive(false);
        V3DYcopyobject[1].SetActive(false); V3DYcopyobject[0].SetActive(false); V3DYcopyobject[2].SetActive(false);
        V3DYcopyobject[1].transform.parent = V3DYIndicater[6].transform;
        LineObjectList[3].SetActive(true);      //显示  复制  预设线1
        LineObjectList[4].SetActive(true);      //显示  复制  预设线2
        LineObjectList[8].SetActive(true);      //显示  复制  实时线1
        LineObjectList[9].SetActive(true);      //显示  复制  实时线2
        RealtimeIndicaterBallA.color = Color.magenta;
        RealtimeIndicaterBallB.color = Color.magenta;
        PresetLineNooriginal3D1.enabled = true; PresetLineNooriginal3D2.enabled = true;
        V3DFisactive = true;
    }

    public void UpVisionShadow()
    {

        CastPointUI.transform.position = PresetCastPoint.transform.position;
        CastPointUI.transform.forward = PresetCastPoint.transform.up;
        if (skinSurface.Raycast(RealtimeRay, out skinRealtimeRayHit, float.MaxValue))
        {
            Vector3 intersectionPoint = skinRealtimeRayHit.point; // 获取交点位置
            Vector3 normal = skinRealtimeRayHit.normal; // 获取法线向量
            RealtimeCastPoint.SetActive(true);
            RealtimeCastPoint.transform.position = intersectionPoint;
            RealtimeCastPoint.transform.up = normal;
            RealtimeGuideLine3D.SetPosition(0, RealtimeGuidePointB.transform.position);
            RealtimeGuideLine3D.SetPosition(1, RealtimeCastPoint.transform.position);
        }
        else
        {
            RealtimeCastPoint.SetActive(false);
            Vector3 deriection = RealtimeGuidePointB.transform.position - RealtimeGuidePointA.transform.position;
            RealtimeGuideLine3D.SetPosition(0, RealtimeGuidePointB.transform.position);
            RealtimeGuideLine3D.SetPosition(1, RealtimeGuidePointA.transform.position - deriection * 3f);
        }
        if (ballisarrived || InsertmaxValue > 0)
        {
            Vector3 shadowPoint = CastPointUI.transform.position + RealtimeGuidePointA.transform.right * 0.05f; Vector3 planenormal = CastPointUI.transform.forward;
            Vector3 endshadowpoint = castpoint(shadowPoint, CastPointUI.transform.position, planenormal);
            Vector3 driection = Vector3.Normalize(endshadowpoint - CastPointUI.transform.position);
            textMeshs[1].enabled = true;
            textMeshs[1].transform.forward = -CastPointUI.transform.forward;
            textMeshs[1].transform.position = CastPointUI.transform.position + driection * 0.0225f;
            rotationto(textMeshs[1].transform, textMeshs[1].transform.up, textMeshs[1].transform.position - CastPointUI.transform.position);
            if (A1isarrived|| InsertmaxValue > 0)
            {
                RealtimeGuideLine3D.enabled = false; PresetLine3D.enabled = true; PresetLine2DX.enabled = true;
                //
                if (A0isarrived)
                {
                    CastPointUI.color = Color.green;
                    textMeshs[1].color = Color.green;
                }
                else
                {
                    CastPointUI.color = Color.yellow;
                    textMeshs[1].color = Color.yellow;
                }
                //
                VShadowImage[0].enabled = true;
                VShadowImage[1].enabled = true;
                VShadowRPPoints[0].SetActive(true);
                VShadowRPPoints[1].SetActive(true);
                float distanceA = Vector3.Distance(RealtimeGuidePointA.transform.position, CastPointUI.transform.position) * 1000f;
                int decimalPlaces = 2; // 限制小数点后的位数
                float A = Mathf.Round(distanceA * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                float roundeExportEviation = Mathf.Round(EntryEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                textMeshs[1].text = roundeExportEviation.ToString() + "mm";
                Ray yRay1 = new Ray(Vector3.zero, Vector3.forward);
                Ray yRay2 = new Ray(Vector3.zero, Vector3.forward);
                yRay2.origin = RealtimeGuidePointB.transform.position;
                yRay2.direction = -Vector3.up;
                planeofdisplay = new Plane(PresetCastPoint.transform.up, PresetCastPoint.transform.position);
                float DistanceRAwithRB = Vector3.Distance(RealtimeGuidePointB.transform.position, RealtimeGuidePointA.transform.position);
                float z = 0;
                float PresetPointA;//返回预设目标点
                Vector3 ReplaceLinePresetPointB = PresetCastPoint.transform.position - PresetRay.direction * DistanceRAwithRB;
                yRay1.origin = ReplaceLinePresetPointB;
                yRay1.direction = -Vector3.up;
                Vector3 DriectionAB = Vector3.zero;
                Vector3 DriectionAC = Vector3.zero;
                float s = 0.05f;//射线长度总长0.05
                float x = 1f;//
                if (planeofdisplay.Raycast(yRay1, out PresetPointA))
                {
                    Vector3 LinePresetCastPointB = yRay1.GetPoint(PresetPointA);
                    Vector3 LineP_Driection = PresetCastPoint.transform.position - LinePresetCastPointB;
                    Vector3 NormalP_LineDriection = LineP_Driection.normalized;
                    Vector3 LinePresetPointA = PresetCastPoint.transform.position;
                    VShadowRPPoints[0].transform.position = PresetCastPoint.transform.position - NormalP_LineDriection * s;//预设位置
                    VShadowRPPoints[0].transform.up = PresetCastPoint.transform.up;
                    z = Vector3.Distance(LinePresetPointA, LinePresetCastPointB);//预设AB间真实距离z
                    x = s / z;//用于转换过去
                    PresetLine3D.SetPosition(0, LinePresetPointA);
                    PresetLine3D.SetPosition(1, VShadowRPPoints[0].transform.position);
                    DriectionAB = LinePresetPointA - VShadowRPPoints[0].transform.position;
                }
                float RealtimePointB;//返回实时碰撞点
                Renderer shadowpoint = VShadowRPPoints[1].GetComponent<Renderer>(); shadowpoint.material.color = Color.green;
                if (planeofdisplay.Raycast(yRay2, out RealtimePointB))
                {
                    Vector3 LineRealtimeCastPointB = yRay2.GetPoint(RealtimePointB);//B点投影点
                    Vector3 LineR_Driection = PresetCastPoint.transform.position - LineRealtimeCastPointB;//Pcast点与B点投影点连线方向
                    Vector3 NormalR_LineDriection = LineR_Driection.normalized;//单位化
                    Vector3 point2 = PresetCastPoint.transform.position - LineR_Driection * x;
                    Vector3 point1 = PresetCastPoint.transform.position;
                    float distance = Vector3.Distance(point1, point2);
                    if (distance < 0.022f)
                    {
                        VShadowRPPoints[1].transform.position = PresetCastPoint.transform.position - NormalR_LineDriection * 0.022f;//实时点的位置
                        shadowpoint.material.color = Color.red;
                        PresetLine2DX.material = Linematerials[5];
                        textMeshs[0].color = Color.red;
                    }
                    else if (distance > 0.08f)
                    {
                        VShadowRPPoints[1].transform.position = PresetCastPoint.transform.position - NormalR_LineDriection * 0.08f;//实时点的位置
                        shadowpoint.material.color = Color.red;
                        PresetLine2DX.material = Linematerials[5];
                        textMeshs[0].color = Color.red;
                    }
                    else
                    {
                        VShadowRPPoints[1].transform.position = PresetCastPoint.transform.position - LineR_Driection * x;//实时点的位置
                        if (C1isarrived)
                        {
     
                            if (C0isarrived)
                            {
                                shadowpoint.material.color = Color.green;
                                PresetLine2DX.material = Linematerials[4];
                                textMeshs[0].color = Color.green;
                            }
                            else if (!C0isarrived)
                            {

                                shadowpoint.material.color = Color.yellow;
                                PresetLine2DX.material = Linematerials[3];
                                textMeshs[0].color = Color.yellow;
                            }
                        }
                        else
                        {
                            shadowpoint.material.color = Color.magenta;
                            PresetLine2DX.material = Linematerials[2];
                            textMeshs[0].color = Color.magenta;
                            if (InsertmaxValue == 0) 
                            {
                                textMeshs[2].enabled = false;
                                VShadowImage[2].enabled = false;
                            }
                        }
                    }

                    VShadowRPPoints[1].transform.up = PresetCastPoint.transform.up;
                    Vector3 LineRealtimePointA = PresetCastPoint.transform.position;
                    Vector3 LineRealtimePointB = PresetCastPoint.transform.position - NormalR_LineDriection * s;
                    PresetLine2DX.SetPosition(0, PresetCastPoint.transform.position);
                    PresetLine2DX.SetPosition(1, VShadowRPPoints[1].transform.position);
                    DriectionAC = LineRealtimePointA - LineRealtimePointB;
                }
                Quaternion V2DFCastPoint0rotation = Quaternion.FromToRotation(VShadowRPPoints[0].transform.forward, DriectionAB);
                VShadowRPPoints[0].transform.rotation = V2DFCastPoint0rotation * VShadowRPPoints[0].transform.rotation;     // 应用旋转

                Quaternion V2DFCastPoint1rotation = Quaternion.FromToRotation(VShadowRPPoints[1].transform.forward, DriectionAC);
                VShadowRPPoints[1].transform.rotation = V2DFCastPoint1rotation * VShadowRPPoints[1].transform.rotation;     // 应用旋转

                //Debug.Log("z=" + z + "x=" + x);
                Vector3 direction1 = -DriectionAB;// 第一条线段的方向向量
                Vector3 direction2 = -DriectionAC;// 第二条线段的方向向量
                Vector3 centerDirection = (direction1 + direction2) / 2f;// 第二条线段的方向向量
                Vector3 halfRay = centerDirection.normalized * z * 3f; // 计算射线的一半长度向量
                float angle = Vector3.SignedAngle(direction1, direction2, -Vector3.up); // 计算夹角
                float roundedangle = Mathf.Round(angle * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                textMeshs[0].enabled = true;
                textMeshs[0].text = roundedangle.ToString() + "°";
                //Debug.Log("angle" + angle);
                Transform textTransform = textMeshs[0].transform;          // 获取TextMeshPro组件的Transform 
                textTransform.forward = -PresetCastPoint.transform.up;
                Vector3 textUPAxis = textTransform.up;                // 获取打印数字的up轴
                VShadowImage[1].transform.position = PresetCastPoint.transform.position;
                VShadowImage[1].transform.forward = PresetCastPoint.transform.up;
                Vector3 rightAxis = VShadowImage[1].transform.right;                // 获取填充图像的右轴
                Quaternion rotation = Quaternion.FromToRotation(rightAxis, direction1);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
                VShadowImage[1].transform.rotation = rotation * VShadowImage[1].transform.rotation;      // 应用旋转
                float Precent = 0f;
                VShadowImage[1].fillOrigin = 1;
                Vector3 midpointRay = PresetCastPoint.transform.position + halfRay*0.5f; // 以原点为起点，加上射线向量得到射线的终点
                LineObjectList[5].SetActive(false);

                if (angle >= 0f)
                {
                    VShadowImage[1].fillClockwise = true;
                    Precent = roundedangle / 360f;
                }
                else
                {
                    VShadowImage[1].fillClockwise = false;
                    Precent = -roundedangle / 360f;
                }

                VShadowImage[1].fillAmount = Precent;
                Quaternion Textrotation = Quaternion.FromToRotation(textUPAxis, halfRay);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
                textTransform.rotation = Textrotation * textTransform.rotation;     // 应用旋转
                textTransform.position = midpointRay;
                if (A1isarrived && C1isarrived || InsertmaxValue > 0)
                {
                    textMeshs[0].enabled = true;
                    textMeshs[2].enabled = true;
                    VShadowImage[2].enabled = true;
                    int decimalPlace0 = 0; // 限制小数点后的位数
                    float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                    textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
                    textMeshs[2].transform.position = VShadowImage[3].transform.position - DisplayPlane[0].transform.right * 0.08f;
                    textMeshs[2].transform.forward = -DisplayPlane[0].transform.up;
                    Quaternion Text2rotation = Quaternion.FromToRotation(textMeshs[2].transform.up, -DisplayPlane[0].transform.forward);        // 计算旋转的目标方向与物体 A 右轴的旋转角度
                    textMeshs[2].transform.rotation = Text2rotation * textMeshs[2].transform.rotation;     // 应用旋转
                    Renderer text2 = textMeshs[2].GetComponent<Renderer>();
                    PresetLine2DX.material = Linematerials[1];
                    if (InsertmaxValue >= 0 && InsertmaxValue <= 100)
                    {
                        VShadowImage[3].enabled = true;
                        for (int i = 2; i < VShadowImage.Length; i++)
                        {
                            VShadowImage[i].transform.position = PresetCastPoint.transform.position;
                            VShadowImage[i].transform.forward = -PresetCastPoint.transform.up;
                        }
                        VShadowImage[4].enabled = false;
                        VShadowImage[2].fillAmount = 1;
                        VShadowImage[3].fillAmount = InsertmaxValue / 100f;
                        text2.material.color = Color.white;
                    }
                    else if (InsertmaxValue > 100)
                    {
                        VShadowImage[3].enabled = false;
                        VShadowImage[4].enabled = true;
                        text2.material.color = Color.red;
                    }
                }
            }
            else
            {
                textMeshs[1].color = Color.white;
                textMeshs[1].enabled = false; textMeshs[2].enabled = false; textMeshs[0].enabled = false;
                VShadowImage[0].enabled = false;
                VShadowImage[1].enabled = false;
                VShadowImage[2].enabled = false;
                LineObjectList[5].SetActive(true);
                PresetLine3D.enabled = false;
                PresetLine2DX.enabled = false;
                RealtimeGuideLine3D.enabled = true;
                VShadowRPPoints[0].SetActive(false);
                VShadowRPPoints[1].SetActive(false);
                CastPointUI.color = Color.white;
                RealtimeGuideLine3D.material = Linematerials[2];
                textMeshs[1].enabled = false;
            }

        }
        else 
        {
            VShadowImage[2].enabled = false;
            textMeshs[1].enabled = false; textMeshs[2].enabled = false; textMeshs[0].enabled = false;
            VShadowImage[0].enabled = false;
            VShadowImage[1].enabled = false;
        }

    }
    public void StVisionShadow()
    {
        //Line
        RealtimeGuideList[0].SetActive(false);
        RealtimeGuideList[3].SetActive(false);
        RealtimeGuideLine3D.enabled = false; PresetLine3D.enabled = false; PresetLine2DX.enabled = false;
        LineObjectList[0].SetActive(true); LineObjectList[5].SetActive(true); LineObjectList[1].SetActive(true);
        PresetLine3D.material = Linematerials[1]; PresetLine2DX.material = Linematerials[3]; RealtimeGuideLine3D.material = Linematerials[2];
        //
        presetnail.SetActive(false);
        RealtimeCastPoint.SetActive(true);
        textMeshs[0].enabled = true;
        textMeshs[1].enabled = false;
        CastPointUI.enabled = true;
        UIenbledControl.SetActive(true);
        CastPointUI.transform.position = PresetCastPoint.transform.position;
        for (int i = 0; i < VShadowImage.Length; i++)
        {
            VShadowImage[i].enabled = false;
        }
        VShadowImage[3].fillAmount = 0;        //深度的环初始填充为0
    }

    public void UpVision2DF()
    {
        //z轴相机位置调整
        CameraOfz.transform.position = selectedPlaneofzTransform.position;
        //x轴相机位置调整
        CameraOfx.transform.position = selectedPlaneofxTransform.position;
        textMeshs[0].enabled = true; textMeshs[1].enabled = true;
        textMeshs[0].transform.position = DisplayPlane[2].transform.position + DisplayPlane[2].transform.forward * 0.08f + DisplayPlane[2].transform.up * 0.001f;
        textMeshs[0].transform.forward = -DisplayPlane[2].transform.up;
        Quaternion text0roation = Quaternion.FromToRotation(textMeshs[0].transform.up, -DisplayPlane[2].transform.forward);
        textMeshs[0].transform.rotation = text0roation * textMeshs[0].transform.rotation;     // 应用旋转
        textMeshs[1].transform.position = DisplayPlane[3].transform.position + DisplayPlane[3].transform.forward * 0.08f + DisplayPlane[3].transform.up * 0.001f;
        textMeshs[1].transform.forward = -DisplayPlane[3].transform.up;
        Quaternion text1roation = Quaternion.FromToRotation(textMeshs[1].transform.up, -DisplayPlane[3].transform.forward);
        textMeshs[1].transform.rotation = text1roation * textMeshs[1].transform.rotation;     // 应用旋转
        //获取x平面的信息
        GameObject selectedPlaneofx = selectedPlaneofxTransform.gameObject;
        //获取z平面的信息
        GameObject selectedPlaneofz = selectedPlaneofzTransform.gameObject;

        //生成3D实时向量
        Vector3 presetrayVector = RealtimeGuidePointA.transform.position - RealtimeGuidePointB.transform.position;

        //x平面上的2D实时向量
        Vector3 selectedplaneofxNormal = selectedPlaneofx.transform.up;
        Vector3 projectedVectorofx = Vector3.ProjectOnPlane(presetrayVector, selectedplaneofxNormal);
        //z平面上的2D实时向量
        Vector3 selectedplaneofzNormal = selectedPlaneofz.transform.up;
        Vector3 projectedVectorofz = Vector3.ProjectOnPlane(presetrayVector, selectedplaneofzNormal);
        RealtimecopynailX.transform.right = -skinSurface.transform.forward;//钉right轴垂直投影面
        RealtimecopynailZ.transform.forward = skinSurface.transform.right;//钉right轴垂直投影面

        // 计算投影点____要投影的点，投影平面上的一个点，投影平面法线
        Vector3 DrillRealPB = DrillPointA.transform.position + DrillPointA.transform.up;
        //X平面——线
        Vector3 R2DXPointAPosition1 = ProjectPointToPlane(RealtimeGuidePointA.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 R2DXPointBPosition1 = ProjectPointToPlane(RealtimeGuidePointB.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 R2DXDriection = R2DXPointAPosition1 - R2DXPointBPosition1;
        Vector3 R2DXPointAPosition2 = R2DXPointAPosition1 + R2DXDriection * 4;
        Vector3 R2DXPointBPosition2 = R2DXPointBPosition1 - R2DXDriection * 1;
        //X平面——钉
        Vector3 RP2DXDrillB1 = ProjectPointToPlane(DrillRealPB, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 RP2DXDrillA1 = ProjectPointToPlane(DrillPointA.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 RD2DXDrill = RP2DXDrillB1 - RP2DXDrillA1;//投影到x后钉方向
        RealtimecopynailX.transform.position = RP2DXDrillA1;
        Quaternion nailCopyXrotation = Quaternion.FromToRotation(RealtimecopynailX.transform.up, RD2DXDrill);//计算旋转多少
        RealtimecopynailX.transform.rotation = nailCopyXrotation * RealtimecopynailX.transform.rotation;     // 应用旋转
        //X平面——点
        PresetCastX.transform.position = ProjectPointToPlane(PresetCastPoint.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        //Z平面——线
        Vector3 R2DZPointAPosition1 = ProjectPointToPlane(RealtimeGuidePointA.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 R2DZPointBPosition1 = ProjectPointToPlane(RealtimeGuidePointB.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 R2DZDriection = R2DZPointAPosition1 - R2DZPointBPosition1;
        Vector3 R2DZPointAPosition2 = R2DZPointAPosition1 + R2DZDriection * 4;
        Vector3 R2DZPointBPosition2 = R2DZPointBPosition1 - R2DZDriection * 1;
        //Z平面——钉
        Vector3 RP2DZDrillB1 = ProjectPointToPlane(DrillRealPB, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 RP2DZDrillA1 = ProjectPointToPlane(DrillPointA.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 RD2DZDrill = RP2DZDrillB1 - RP2DZDrillA1;//投影到x后钉方向
        RealtimecopynailZ.transform.position = RP2DZDrillA1;
        Quaternion nailCopyZrotation = Quaternion.FromToRotation(RealtimecopynailZ.transform.up, RD2DZDrill);//计算旋转多少
        RealtimecopynailZ.transform.rotation = nailCopyZrotation * RealtimecopynailZ.transform.rotation;     // 应用旋转
        //Z平面——点
        PresetCastZ.transform.position = ProjectPointToPlane(PresetCastPoint.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;


        Vector3 PresretdirX = (PresetPointA2DX.transform.position - PresetPointB2DX.transform.position).normalized;
        Vector3 RealtimedirX = (R2DXPointAPosition2 - R2DXPointBPosition2).normalized;
        Vector3 PresretdirZ = (PresetPointA2DZ.transform.position - PresetPointB2DZ.transform.position).normalized;
        Vector3 RealtimedirZ = (R2DZPointAPosition2 - R2DZPointBPosition2).normalized;
        float angleX = Mathf.Acos(Vector3.Dot(PresretdirX, RealtimedirX)) * Mathf.Rad2Deg;
        float angleZ = Mathf.Acos(Vector3.Dot(PresretdirZ, RealtimedirZ)) * Mathf.Rad2Deg;
        if (angleX > 180)
        {
            angleX = 360 - angleX;
        }
        if (angleZ > 180)
        {
            angleZ = 360 - angleZ;
        }
        int decimalPlaces2 = 2; // 限制小数点后的位数
        float roundeangleX = Mathf.Round(angleX * Mathf.Pow(10, decimalPlaces2)) / Mathf.Pow(10, decimalPlaces2);
        float roundeangleZ = Mathf.Round(angleZ * Mathf.Pow(10, decimalPlaces2)) / Mathf.Pow(10, decimalPlaces2);
        AngleEviation1 = roundeangleX;
        AngleEviation2 = roundeangleZ;
        textMeshs[0].text = "deflected:" + AngleEviation1.ToString() + "°";
        textMeshs[1].text = "deflected:" + AngleEviation2.ToString() + "°";
        if (AngleEviation1 < 2.5)
        {
            if (AngleEviation1 < 1)
            {
                textMeshs[0].color = Color.green;
            }
            else
            { textMeshs[0].color = Color.yellow; }
        }
        else 
        {
            textMeshs[0].color = Color.white;
        }
        if (AngleEviation2 < 2.5)
        {
            if (AngleEviation2 < 1)
            {
                textMeshs[1].color = Color.green;
            }
            else
            { textMeshs[1].color = Color.yellow; }
        }
        else
        {
            textMeshs[1].color = Color.white;
        }


        //刷新预设线两点位置
        PresetLine2DX.SetPosition(0, PresetPointA2DX.transform.position);
        PresetLine2DX.SetPosition(1, PresetPointB2DX.transform.position);
        //刷新预设线两点位置
        PresetLine2DZ.SetPosition(0, PresetPointA2DZ.transform.position);
        PresetLine2DZ.SetPosition(1, PresetPointB2DZ.transform.position);
        //实时绘制两点位置——x平面
        RealtimeGuideLine2DX.SetPosition(0, R2DXPointAPosition2);

        RealtimeGuideLine2DX.SetPosition(1, R2DXPointBPosition2);
        //实时绘制两点位置——z平面
        RealtimeGuideLine2DZ.SetPosition(0, R2DZPointAPosition2);
        RealtimeGuideLine2DZ.SetPosition(1, R2DZPointBPosition2);

        if ((A1isarrived && C1isarrived) || InsertmaxValue > 0)
        {
            if (InsertmaxValue >= 0 && InsertmaxValue <= 100)
            {
                for (int i = 2; i < 4; i++)
                {
                    textMeshs[i].enabled = true;
                }
                if (A0isarrived && C0isarrived)
                {
                    RealtimeGuideLine2DX.material = Linematerials[4];
                    RealtimeGuideLine2DZ.material = Linematerials[4];
                    PresetLine2DX.enabled = false;
                    PresetLine2DZ.enabled = false;
                }
                else
                {
                    PresetLine2DX.enabled = true;
                    PresetLine2DZ.enabled = true;
                    RealtimeGuideLine2DX.material = Linematerials[3];
                    RealtimeGuideLine2DZ.material = Linematerials[3];
                }
                V2DFImage[0].SetActive(true); V2DFImage[1].SetActive(true);
                V2DFImage[2].SetActive(true); V2DFImage[3].SetActive(true);
                V2DFImage[0].transform.position = PresetCastX.transform.position;
                V2DFImage[0].transform.forward = skinSurface.transform.forward;
                Vector3 DriectionX = PresetPointB2DX.transform.position - PresetPointA2DX.transform.position;
                Quaternion V2DYCastImageX0rotation = Quaternion.FromToRotation(V2DFImage[0].transform.up, DriectionX);
                V2DFImage[0].transform.rotation = V2DYCastImageX0rotation * V2DFImage[0].transform.rotation;     // 应用旋转
                V2DFImage[0].transform.position = V2DFImage[0].transform.position - V2DFImage[0].transform.right * 0.02f;

                V2DFImage[2].transform.position = PresetCastZ.transform.position;
                V2DFImage[2].transform.forward = -skinSurface.transform.right;
                Vector3 DriectionZ = PresetPointB2DZ.transform.position - PresetPointA2DZ.transform.position;
                Quaternion V2DYCastImageZ0rotation = Quaternion.FromToRotation(V2DFImage[2].transform.up, DriectionZ);
                V2DFImage[2].transform.rotation = V2DYCastImageZ0rotation * V2DFImage[2].transform.rotation;     // 应用旋转
                V2DFImage[2].transform.position = V2DFImage[2].transform.position - V2DFImage[2].transform.right * 0.02f;

                if (InsertionPercentage >= InsertmaxValue)
                {
                    V2DFImage[1].transform.position = RealtimecopynailX.transform.position;
                    V2DFImage[1].transform.forward = skinSurface.transform.forward;
                    V2DFImage[1].transform.rotation = V2DYCastImageX0rotation * V2DFImage[1].transform.rotation;     // 应用旋转
                    textMeshs[2].transform.position = V2DFImage[1].transform.position + V2DFImage[1].transform.right * 0.02f;
                    textMeshs[2].transform.rotation = V2DFImage[1].transform.rotation;
                    V2DFImage[3].transform.position = RealtimecopynailZ.transform.position;
                    V2DFImage[3].transform.forward = -skinSurface.transform.right;
                    V2DFImage[3].transform.rotation = V2DYCastImageZ0rotation * V2DFImage[3].transform.rotation;     // 应用旋转
                    textMeshs[3].transform.position = V2DFImage[3].transform.position + V2DFImage[3].transform.right * 0.02f;
                    textMeshs[3].transform.rotation = V2DFImage[3].transform.rotation;
                }
                int decimalPlace0 = 0; // 限制小数点后的位数
                float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
                textMeshs[3].text = DepthInsertmaxValue.ToString() + "%";
                textMeshs[2].color = Color.white; textMeshs[3].color = Color.white;
            }
            else if (InsertmaxValue > 100)
            {
                int decimalPlace0 = 0; // 限制小数点后的位数
                float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
                textMeshs[3].text = DepthInsertmaxValue.ToString() + "%";
                textMeshs[2].color = Color.red; textMeshs[3].color = Color.red;

            }
        }
        else
        {
            LineObjectList[1].SetActive(true);  //显示  预设2DX线
            LineObjectList[2].SetActive(true);  //显示  预设2DZ线
            RealtimeGuideLine2DX.material = RealtimelineMaterial;
            RealtimeGuideLine2DZ.material = RealtimelineMaterial;
            for (int i = 2; i < textMeshs.Length; i++)
            {
                textMeshs[i].enabled = false;
            }
            for (int i = 0; i < V2DFImage.Length; i++)
            {
                V2DFImage[i].SetActive(false);
            }
        }
    }
    public void StVision2DF()
    {
        PresetLine3D.enabled = false;
        RealtimeGuideLine3D.enabled = false;
        //设置选中脊骨的材质
        PresetcopynailX = Instantiate(presetnail);
        PresetcopynailZ = Instantiate(presetnail);
        RealtimecopynailX = Instantiate(realtimenail);
        RealtimecopynailZ = Instantiate(realtimenail);
        PresetcopynailX.layer = 3;
        PresetcopynailZ.layer = 6;
        RealtimecopynailX.layer = 3;
        RealtimecopynailZ.layer = 6;
        DisplayPlane[1].SetActive(true);//2DX显示
        DisplayPlane[1].transform.localEulerAngles = new Vector3(60f, 180f, 0f);
        DisplayPlane[1].transform.localPosition = new Vector3(0.03f, 0.065f, 0.2f);
        DisplayPlane[2].SetActive(true);//2DX显示
        DisplayPlane[3].SetActive(true);//2DX显示
        LineObjectList[0].SetActive(true); LineObjectList[1].SetActive(true); LineObjectList[2].SetActive(true); LineObjectList[6].SetActive(true); LineObjectList[7].SetActive(true);  //显示  实时2DZ线
        PresetLine2DX.enabled = true; PresetLine2DZ.enabled = true; PresetLine2DX.material = Linematerials[1]; PresetLine2DZ.material = Linematerials[1];
        PresetcopynailX.SetActive(true);
        PresetcopynailZ.SetActive(true);
        PresetcopynailX.transform.SetParent(positions[currentIndex]);
        PresetcopynailZ.transform.SetParent(positions[currentIndex]);
        Renderer text0render = textMeshs[0].GetComponent<MeshRenderer>(); text0render.enabled = true;
        Renderer text1render = textMeshs[1].GetComponent<MeshRenderer>(); text1render.enabled = true;

        for (int i = 0; i < V2DFImage.Length; i++)
        {
            V2DFImage[i].SetActive(false);
        }
        // 遍历所有x平面并设置哪个平面可见
        for (int i = 0; i < planesofx.Length; i++)
        {
            if (i == xindexofdisplayplane)
            {
                planesofx[i].enabled = true; // 显示选中的平面
                selectedPlaneofxTransform = planesofx[i].transform; // 获取选中x平面的 Transform 组件
            }
            else
            {
                planesofx[i].enabled = false; // 隐藏未选中的平面
            }
        }
        // 遍历所有z平面并设置哪个平面可见
        if (zindexofdisplayplane == 0)
        {
            planesofz[0].enabled = true; // 显示选中的平面
            planesofz[1].enabled = false; // 隐藏未选中的平面
            selectedPlaneofzTransform = planesofz[0].transform; // 获取选中z平面的 Transform 组件
        }
        else
        {
            planesofz[1].enabled = true; // 显示选中的平面
            planesofz[0].enabled = false; // 隐藏未选中的平面
            selectedPlaneofzTransform = planesofz[1].transform; // 获取选中z平面的 Transform 组件
        }

        //获取x平面的信息
        GameObject selectedPlaneofx = selectedPlaneofxTransform.gameObject;
        //获取z平面的信息
        GameObject selectedPlaneofz = selectedPlaneofzTransform.gameObject;

        Vector3 selectedplaneofxNormal = selectedPlaneofx.transform.up;
        Vector3 projectedVectorofx = Vector3.ProjectOnPlane(PresetLineDirection, selectedplaneofxNormal);
        //得到要投影的预设轨迹线与投影平面的法线----得到投影平面Z的方向向量
        Vector3 selectedplaneofzNormal = selectedPlaneofz.transform.up;
        Vector3 projectedVectorofz = Vector3.ProjectOnPlane(PresetLineDirection, selectedplaneofzNormal);
        //预设轨迹——计算x平面投影后的起点PA2D和终点PB2D----------------预设轨迹 of x 
        Vector3 PP2DXA = ProjectPointToPlane(PresetPointA.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 PP2DXB = ProjectPointToPlane(PresetPointB.transform.position, selectedPlaneofxTransform.position, -skinSurface.transform.forward) - skinSurface.transform.forward * 0.001f;
        Vector3 PD2DX = PP2DXA - PP2DXB;
        PresetPointA2DX.transform.position = PP2DXA + PD2DX * 2;
        PresetPointB2DX.transform.position = PP2DXB - PD2DX * 2;
        PresetPointA2DX.transform.SetParent(positions[currentIndex]);
        PresetPointB2DX.transform.SetParent(positions[currentIndex]);
        //预设轨迹——计算z平面投影后的起点PC2D和终点PD2D----------------预设轨迹 of x
        Vector3 PP2DZA = ProjectPointToPlane(PresetPointA.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 PP2DZB = ProjectPointToPlane(PresetPointB.transform.position, selectedPlaneofzTransform.position, skinSurface.transform.right) + skinSurface.transform.right * 0.001f;
        Vector3 PD2DZ = PP2DZA - PP2DZB;
        PresetPointA2DZ.transform.position = PP2DZA + PD2DZ * 2;
        PresetPointB2DZ.transform.position = PP2DZB - PD2DZ * 2;
        PresetPointA2DZ.transform.SetParent(positions[currentIndex]);
        PresetPointB2DZ.transform.SetParent(positions[currentIndex]);
        PresetcopynailX.transform.position = PP2DXA;
        PresetcopynailX.transform.up = PresetPointB2DX.transform.position - PresetPointA2DX.transform.position;
        PresetcopynailX.transform.localScale = PresetPointA2DX.transform.localScale;
        PresetcopynailZ.transform.position = PP2DZA;
        PresetcopynailZ.transform.up = PresetPointB2DZ.transform.position - PresetPointA2DZ.transform.position;
        PresetcopynailZ.transform.localScale = PresetPointA2DZ.transform.localScale;

        //设置层可见---------------------------------------- x
        LineObjectList[1].layer = 3;//预设
        LineObjectList[6].layer = 3;//实时
        //设置层可见---------------------------------------- z
        LineObjectList[2].layer = 6;//预设
        LineObjectList[7].layer = 6;//实时
        textMeshs[2].gameObject.layer = 3;
        textMeshs[3].gameObject.layer = 6;
    }

    public void UpVision2DY()
    {
        bool V2DYiscast;
        V2DYImage[0].transform.position = PresetCastPoint.transform.position;
        V2DYImage[0].transform.forward = PresetCastPoint.transform.up;
        if (skinSurface.Raycast(RealtimeRay, out skinRealtimeRayHit, float.MaxValue))
        {
            Vector3 intersectionPoint = skinRealtimeRayHit.point; // 获取交点位置
            Vector3 normal = skinRealtimeRayHit.normal; // 获取法线向量
            RealtimeCastPoint.SetActive(true);
            RealtimeCastPoint.transform.position = intersectionPoint;
            RealtimeCastPoint.transform.up = normal;
            V2DYiscast = true;
            RealtimeGuideLine3D.SetPosition(0, RealtimeCastPoint.transform.position);
            RealtimeGuideLine3D.SetPosition(1, RealtimeGuidePointB.transform.position);
        }
        else
        { 
            RealtimeCastPoint.SetActive(false);
            V2DYiscast = false;
            Vector3 driection = Vector3.Normalize(RealtimeGuidePointA.transform.position - RealtimeGuidePointB.transform.position) ;
            Vector3 pointC = RealtimeGuidePointA.transform.position + driection * 0.5f;
            RealtimeGuideLine3D.SetPosition(0, pointC);
            RealtimeGuideLine3D.SetPosition(1, RealtimeGuidePointB.transform.position);
        }

        if (ballisarrived)
        {
            textMeshs[1].enabled = true;
            textMeshs[1].transform.forward = -CastPointUI.transform.forward;
            textMeshs[1].transform.position = CastPointUI.transform.position + CastPointUI.transform.up;
            float distanceA = Vector3.Distance(RealtimeGuidePointA.transform.position, V2DYImage[0].transform.position) * 1000f;
            int decimalPlaces = 2; // 限制小数点后的位数
            float roundeExportEviation = Mathf.Round(EntryEviation * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
            float A = Mathf.Round(distanceA * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
            textMeshs[1].text = roundeExportEviation.ToString() + "mm";
            textMeshs[1].color = Color.white;
            V2DYImage[0].color = Color.white;
            if (A1isarrived)
            {
                //text0_指示尾部，text1_指示针头
                PresetLine3D.enabled = true; RealtimeGuideLine3D.enabled = true; RealtimeNoRGuideLine3D1.enabled = true;
                RealtimeGuideLine3D.enabled = false; PresetLine3D.enabled = true; PresetLine2DX.enabled = true;
                textMeshs[0].enabled = true;
                if (A0isarrived)
                {
                    V2DYImage[0].color = Color.green;
                    textMeshs[1].color = Color.green;
                }
                else
                {
                    V2DYImage[0].color = Color.yellow;
                    textMeshs[1].color = Color.yellow;
                }
                if (C1isarrived)
                {
                    if (C0isarrived)
                    {
                        RealtimeGuideLine3D.material = Linematerials[4]; RealtimeNoRGuideLine3D1.material = Linematerials[4]; textMeshs[0].color = Color.green;
                        for (int i = 0; i < V2DYcopyImage.Length; i++)
                        {
                            V2DYcopyImage[i].color = Color.green;
                        }
                    }
                    else
                    {
                        RealtimeGuideLine3D.material = Linematerials[3]; RealtimeNoRGuideLine3D1.material = Linematerials[3]; textMeshs[0].color = Color.yellow;
                        for (int i = 0; i < V2DYcopyImage.Length; i++)
                        {
                            V2DYcopyImage[i].color = Color.yellow;
                        }
                    }
                }
                else
                {
                    RealtimeGuideLine3D.material = Linematerials[2]; RealtimeNoRGuideLine3D1.material = Linematerials[2]; textMeshs[0].color = Color.magenta;
                    for (int i = 0; i < V2DYcopyImage.Length; i++)
                    {
                        V2DYcopyImage[i].color = Color.magenta;
                    }
                }
                LineObjectList[8].SetActive(true);
                PresetLine3D.material = Linematerials[1];
                V2DYcopyImageParent.SetActive(true);
                float legthofAB = Vector3.Distance(RealtimeGuidePointA.transform.position, RealtimeGuidePointB.transform.position);
                Vector3 PresetPointB = PresetCastPoint.transform.position - PresetRay.direction * legthofAB;

                float DistanceofBB = Vector3.Distance(PresetPointB, RealtimeGuidePointB.transform.position);
                Vector3 DriectionBB = RealtimeGuidePointB.transform.position - PresetPointB;
                //渲染线条
                PresetLine3D.SetPosition(0, PresetCastPoint.transform.position);
                PresetLine3D.SetPosition(1, PresetPointB);
                RealtimeNoRGuideLine3D1.SetPosition(0, RealtimeGuidePointB.transform.position);
                RealtimeNoRGuideLine3D1.SetPosition(1, PresetPointB);
                PresetLine2DX.material = Linematerials[1]; PresetLine2DZ.material = Linematerials[1];
                //设置两端线条
                V2DYcopyImageParent.transform.position = PresetPointB;
                V2DYcopyImageParent.transform.up = -DriectionBB;
                float number = DistanceofBB / 0.008f;
                int decimalPlace0 = 0; // 限制小数点后的位数
                float numberactive = Mathf.Round(number * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                int intnumber = (int)numberactive;
                for (int i = 1; i < intnumber; i++)
                {
                    V2DYcopyImage[i].enabled = true;
                }
                for (int i = intnumber; i < 20; i++)
                {
                    V2DYcopyImage[i].enabled = false;
                }
                //跟随相机旋转
                Plane cameraplaneA = new Plane(-DriectionBB, PresetPointB);
                Ray camerarayA = new Ray(mainCameraTransform.position, DriectionBB);
                float rayDistanceofplaneA = 0f;
                if (cameraplaneA.Raycast(camerarayA, out rayDistanceofplaneA))
                {
                    Vector3 PointofA = camerarayA.GetPoint(rayDistanceofplaneA);
                    Vector3 DriectionA = PresetPointB - PointofA;
                    Quaternion rotationA = Quaternion.FromToRotation(V2DYcopyImageParent.transform.forward, DriectionA);
                    V2DYcopyImageParent.transform.rotation = rotationA * V2DYcopyImageParent.transform.rotation;
                }
                Vector3 Presretdir = (PresetCastPoint.transform.position - PresetPointB).normalized;
                Vector3 Realtimedir = (PresetCastPoint.transform.position - RealtimeGuidePointB.transform.position).normalized;
                Vector3 Midedir = (RealtimeGuidePointB.transform.position - PresetPointB).normalized;
                float angle = Mathf.Acos(Vector3.Dot(Presretdir, Realtimedir)) * Mathf.Rad2Deg;
                if (angle > 180)
                {
                    angle = 360 - angle;
                }
                float roundeangle = Mathf.Round(angle * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                textMeshs[0].transform.up = -RealtimeGuidePointB.transform.right;
                Quaternion Text0 = Quaternion.FromToRotation(textMeshs[0].transform.forward, Realtimedir);
                textMeshs[0].transform.rotation = Text0 * textMeshs[0].transform.rotation;
                textMeshs[1].transform.rotation = textMeshs[0].transform.rotation;
                if (isHande)
                {
                    textMeshs[0].transform.position = RealtimeGuidePointB.transform.position - RealtimeGuidePointB.transform.right * 0.03f;
                    textMeshs[1].transform.position = RealtimeGuidePointA.transform.position - RealtimeGuidePointA.transform.right * 0.03f;
                }
                else
                {
                    textMeshs[0].transform.position = RealtimeGuidePointB.transform.position + RealtimeGuidePointB.transform.right * 0.03f;
                    textMeshs[1].transform.position = RealtimeGuidePointA.transform.position + RealtimeGuidePointA.transform.right * 0.03f;
                }
                textMeshs[0].text = roundeangle.ToString() + "°";
            }
            else
            {
                V2DYImage[0].color = Color.white;
                textMeshs[1].color = Color.magenta;
                textMeshs[0].enabled = false; textMeshs[1].color = Color.magenta;
                LineObjectList[8].SetActive(false);
                RealtimeGuideLine3D.material = Linematerials[2];
                PresetLine3D.enabled = false; RealtimeGuideLine3D.enabled = true; RealtimeNoRGuideLine3D1.enabled = false;
                if (V2DYiscast)
                {
                    RealtimeGuideLine3D.SetPosition(0, RealtimeGuidePointB.transform.position);
                    RealtimeGuideLine3D.SetPosition(1, RealtimeCastPoint.transform.position);
                }
                else
                {
                    Vector3 deriection = RealtimeGuidePointB.transform.position - RealtimeGuidePointA.transform.position;
                    RealtimeGuideLine3D.SetPosition(0, RealtimeGuidePointB.transform.position);
                    RealtimeGuideLine3D.SetPosition(1, RealtimeGuidePointA.transform.position - deriection * 3f);
                }
                V2DYImage[0].color = Color.white;
                V2DYcopyImageParent.SetActive(false);
            }
            if (A1isarrived && C1isarrived || InsertmaxValue > 0)
            {
                V2DYcopyImage[0].enabled = false;
                textMeshs[2].enabled = true;
                V2DYImage[1].gameObject.SetActive(true);
                V2DYImage[1].transform.position = planesofy[xindexofdisplayplane].transform.position + DisplayPlane[0].transform.right * 0.1f;
                V2DYImage[1].transform.forward = -DisplayPlane[0].transform.up;
                Vector3 DisplayPlaneaxisforward = DisplayPlane[0].transform.forward;
                Quaternion V3DFParent3DXrotation = Quaternion.FromToRotation(V2DYImage[1].transform.up, DisplayPlaneaxisforward);
                V2DYImage[1].transform.rotation = V3DFParent3DXrotation * V2DYImage[1].transform.rotation;     // 应用旋转
                if (InsertmaxValue >= 0 && InsertmaxValue <= 100)
                {
                    for (int i = 0; i < 20; i++)
                    {
                        V2DYcopyImage[i].enabled = false;
                    }
                    V2DYImage[2].color = Color.green;
                    V2DYImage[3].color = Color.white;
                    V2DYImage[2].fillAmount = InsertmaxValue / 100f;
                    float beginposition = 0.05205f;
                    float step = beginposition * 2f * InsertmaxValue / 100f - beginposition;
                    V2DYImage[3].transform.position = new Vector3(V2DYImage[1].transform.position.x + step, V2DYImage[1].transform.position.y, V2DYImage[1].transform.position.z);
                    int decimalPlace0 = 0; // 限制小数点后的位数
                    float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);

                    textMeshs[2].transform.rotation = V2DYImage[3].transform.rotation;
                    textMeshs[2].transform.position = new Vector3(V2DYImage[3].transform.position.x + 0.01f, V2DYImage[3].transform.position.y, V2DYImage[3].transform.position.z);
                    textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
                    textMeshs[2].color = Color.white;
                }
                else if (InsertmaxValue > 100)
                {
                    int decimalPlace0 = 0; // 限制小数点后的位数
                    float DepthInsertmaxValue = Mathf.Round(InsertmaxValue * Mathf.Pow(10, decimalPlace0)) / Mathf.Pow(10, decimalPlace0);
                    textMeshs[2].text = DepthInsertmaxValue.ToString() + "%";
                    textMeshs[2].color = Color.red;
                    V2DYImage[2].color = Color.red;
                    V2DYImage[3].color = Color.red;
                    textMeshs[0].color = Color.red;
                    textMeshs[1].color = Color.red;
                }
            }
            else
            {
                V2DYImage[1].gameObject.SetActive(false);
                textMeshs[2].enabled = false;
            }
            if (InsertmaxValue > 0) { V2DYcopyImage[0].enabled = false; } else { V2DYcopyImage[0].enabled = true; }

        }
        else { 
            textMeshs[1].enabled = false;
            V2DYImage[0].color = Color.white;
        }

    }
    public void StVision2DY()
    {
        //创建引导
        RealtimeGuideList[0].SetActive(false);
        RealtimeGuideList[3].SetActive(false);
        RealtimeGuideLine3D.material = Linematerials[2];
        for (int i = 0; i < V2DYImage.Length; i++)
        {
            V2DYImage[i].enabled = true;
        }
        V2DYcopyImageParent.transform.SetParent(V2DYImage[4].transform.parent);
        float CopySpaceing = 0.008f;
        for (int i = 0; i < 20; i++)
        {
            V2DYcopyImage[i] = Instantiate(V2DYImage[4]);
            V2DYcopyImage[i].transform.SetParent(V2DYcopyImageParent.transform);
            V2DYcopyImage[i].transform.localScale = new Vector3(1f, 1f, 1f);
            V2DYcopyImage[i].transform.position = V2DYcopyImageParent.transform.position - V2DYcopyImageParent.transform.up * CopySpaceing * i;
            V2DYcopyImage[i].transform.up = V2DYcopyImageParent.transform.up;
            Quaternion rotationA = Quaternion.FromToRotation(V2DYcopyImage[i].transform.forward, V2DYcopyImageParent.transform.forward);
            V2DYcopyImage[i].transform.rotation = rotationA * V2DYcopyImage[i].transform.rotation;
        }
        V2DYcopyImageParent.SetActive(false);
        LineObjectList[0].SetActive(true); LineObjectList[8].SetActive(false); LineObjectList[5].SetActive(true);
        PresetLine3D.enabled = false; RealtimeNoRGuideLine3D1.enabled = false; RealtimeGuideLine3D.enabled = false;
        V2DYImage[2].fillAmount = 0;
        V2DYImage[2].transform.SetParent(V2DYImage[1].transform);
        // 遍历所有y平面并设置哪个平面可见
        for (int i = 0; i < planesofy.Length; i++)
        {
            if (i == xindexofdisplayplane)
            {
                planesofy[i].enabled = true; // 显示选中的平面                
            }
            else
            {
                planesofy[i].enabled = false; // 隐藏未选中的平面
            }
        }
        V2DYImage[0].enabled = true;
        V2DYImage[1].gameObject.SetActive(false);
        V2DYImage[0].transform.position = PresetCastPoint.transform.position;
        V2DYImage[4].enabled = false;


    }
    //投影点计算

    public bool guideindex()
    {
        return isHande;//手持或机器
    }

    private void OnApplicationQuit()
    {
        Debug.Log("应用程序即将关闭");
    }
    public GameObject[] Copyobjs()
    {
        return CopyList;//骨钻
    }
    public bool tocopy3DF()
    {
        return V3DFisactive;//是否已标定好
    }

    public void submit() { 
        submitnumber++;
        Debug.Log("count1number" + count1number+ "submitnumber"+ submitnumber);
        if (submitnumber == count1number)
        {
            isChangeTrack = true;
        }
        else
        {
            isChangeTrack = false;
        }
        if (submitnumber == IndexesCount)
        {
            DisplayPlane[4].SetActive(false);
            StartCoroutine(DisappearAfterDelay(5f, Endtips));
        }
    }
    public void SendTaskBegin()
    {
        byte[] byte0 = new byte[4];      byte[] byte1 = new byte[4];        byte[] byte2 = new byte[4];        byte[] byte3 = new byte[4];        byte[] byte4 = new byte[4];        byte[] byte5 = new byte[4];
        byte[] byte6 = new byte[4];        byte[] byte7 = new byte[4];        byte[] byte8 = new byte[4];        byte[] byte9 = new byte[4];
        byte0 = Communication.ToByte(2);
        byte1 = Communication.ToByte(Number );//传输_执行次数
        byte2 = Communication.ToByte(indexNumberMap[currentIndex]);//传输_方案序号
        byte3 = Communication.ToByte(originalIndex);//传输_植入位置     
        byte4 = Communication.ToByte(PresetCastBone.transform.position.x);//传输_预设_入口点X
        byte5 = Communication.ToByte(PresetCastBone.transform.position.y);//传输_预设_入口点Y
        byte6 = Communication.ToByte(PresetCastBone.transform.position.z);//传输_预设_入口点Z
        byte7 = Communication.ToByte(presetnail.transform.position.x);//传输_预设_出口点X
        byte8 = Communication.ToByte(presetnail.transform.position.y);//传输_预设_出口点Y
        byte9 = Communication.ToByte(presetnail.transform.position.z);//传输_预设_出口点Z
        Communication script_Communication = FindObjectOfType<Communication>();
        script_Communication.Exdate(byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9);
        Debug.Log("执行次数:"+ Number + "方案序号:" + indexNumberMap[currentIndex] + "植入位置:" + originalIndex + "入口点X:" + PresetCastBone.transform.position.x + "入口点Y:" + PresetCastBone.transform.position.y + "入口点Z:" + PresetCastBone.transform.position.z + "出口点X:" + byte7 + "出口点Y:" + byte8 + "出口点Z:" + byte9 );
    }
    public void SendTaskEnd()
    {
        byte[] byte0 = new byte[4]; byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4]; byte[] byte5 = new byte[4];
        byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4]; byte[] byte8 = new byte[4]; byte[] byte9 = new byte[4];
        byte0 = Communication.ToByte(3);
        byte1 = Communication.ToByte(RealtimeCastBone.transform.position.x);//传输_入口点_实际_X
        byte2 = Communication.ToByte(RealtimeCastBone.transform.position.y);//传输_入口点_实际_Y
        byte3 = Communication.ToByte(RealtimeCastBone.transform.position.z);//传输_入口点_实际_Z
        byte4 = Communication.ToByte(Realtimeendnail.x);//传输_出口点_实际_X
        byte5 = Communication.ToByte(Realtimeendnail.y);//传输_出口点_实际_Y
        byte6 = Communication.ToByte(Realtimeendnail.z);//传输_出口点_实际_Z
        byte7 = Communication.ToByte(EnterOffset);//传输_入口偏移
        byte8 = Communication.ToByte(ExportOffset);//传输_出口偏移
        byte9 = Communication.ToByte(angleDegrees);//传输_偏转角度
        Communication script_Communication = FindObjectOfType<Communication>();
        script_Communication.Exdate(byte0,byte1, byte2, byte3, byte4, byte5, byte6, byte7, byte8, byte9);
        Debug.Log("入实际_X:" + byte1 + "入实际_Y:" + byte2 + "入实际_Z:" + byte3 + "出实际_X:" + byte4 + "出实际_Y:" + byte5 + "出实际_Z:" + byte6 + "入口偏移:" + EnterOffset + "出口偏移:" + ExportOffset + "偏转角度:" + angleDegrees);
        EnterOffset = 0; ExportOffset = 0; angleDegrees = 0; Realtimeendnail = new Vector3(0, 0, 0); RealtimeCastBone.transform.position = new Vector3(0, 0, 0);
    }
    public void SendQuseitionNaire()
    {
        byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4]; byte[] byte5 = new byte[4]; byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4];
        float timedate = EndTimeStamp- StartTimeStamp;
        byte1 = Communication.ToByte(MedianOffset);//冠状面偏移
        byte2 = Communication.ToByte(CoronalOffset);//矢状面偏移
        byte3 = Communication.ToByte(TransverseOffset);//水平面偏移
        byte4 = Communication.ToByte(timedate);//传输_任务时长
        byte5 = Communication.ToByte(ArrivedCount[0]*100f+ ArrivedCount[1]*1f);//入口点——A1到达次数——A0到达次数
        byte6 = Communication.ToByte(ArrivedCount[2] * 100f + ArrivedCount[3] * 1f);//出口点——C1到达次数——C0到达次数
        byte7 = Communication.ToByte(ArrivedCount[4] * 100f + ArrivedCount[5] * 1f);//整体——1都到达次数——0都到达次数
        Communication script_Communication = FindObjectOfType<Communication>();
        script_Communication.Sendnaire( byte1, byte2, byte3, byte4, byte5, byte6, byte7);
        Debug.Log("冠状面偏移:" + MedianOffset + "矢状面偏移:" + CoronalOffset + "水平面偏移:" + TransverseOffset + "任务时长:" + timedate + "A1到达次数——A0到达次数:" + ArrivedCount[0] * 100f + ArrivedCount[1] * 1f + "C1到达次数——C0到达次数:" + ArrivedCount[2] * 100f + ArrivedCount[3] * 1f + "1都到达次数——0都到达次数:" + ArrivedCount[4] * 100f + ArrivedCount[5] * 1f);
        MedianOffset = 0; CoronalOffset = 0; TransverseOffset = 0;
        for (int i = 0; i < ArrivedCount.Length; i++)
        {
            ArrivedCount[i] = 0;
        }
    }
    public bool headisrecord()
    {
        return isrecord;//是否已标定好
    }    

    void rotationto(Transform roationtrans, Vector3 fromroation, Vector3 toroation)
    {
        Quaternion rotation = Quaternion.FromToRotation(fromroation, toroation);
        roationtrans.rotation = rotation * roationtrans.rotation;     // 应用旋转
    }

    void setmaterial(GameObject obj, Material mat)
    {
        Renderer render = obj.GetComponent<Renderer>();
        render.material = mat; 
    }
    //投影一个点
    Vector3 castpoint(Vector3 point, Vector3 planepoint, Vector3 normal)//1要投影的点//2要投影的平面上的一点//.要投影平面的法线
    {
        var dir = point - planepoint;
        var pj = Vector3.ProjectOnPlane(dir, normal);
        Vector3 castpoint = planepoint + pj;
        return castpoint;
    }
    //投影一个向量
    Vector3 castdriection(Vector3 castvector, Vector3 planeDriection)//1要投影的向量//2要投影的平面的法线
    {
        Vector3 planeNormal = planeDriection.normalized;
        Vector3 projectionA = Vector3.ProjectOnPlane(castvector, planeNormal);        // 计算线段AB在平面上的投影向量
        Vector3 driection = projectionA.normalized;
        return driection;
    }
    //执行一次

    // 泛型方法用于启用或禁用指定索引的数组元素
    public void SetObjectsState<T>(T[] objs, bool isActive, params int[] indices) where T : Behaviour
    {
        foreach (int idx in indices)
        {
            if (idx >= 0 && idx < objs.Length)
            {
                objs[idx].enabled = isActive;
            }
            else
            {
                Debug.LogWarning("Index out of range: " + idx);
            }
        }
    }
    Vector3 ProjectPointToPlane(Vector3 point, Vector3 planePoint, Vector3 planeNormal)
    {
        // 计算点到平面上的向量
        Vector3 pointToPlane = point - planePoint;

        // 计算投影点
        Vector3 projection = point - Vector3.Dot(pointToPlane, planeNormal) * planeNormal;

        return projection;
    }
    private IEnumerator DisappearAfterDelay(float time, GameObject hideobject)
    {
        hideobject.SetActive(true);
        yield return new WaitForSeconds(time);
        hideobject.SetActive(false);
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;
#else
            Application.Quit();
#endif
        Debug.Log("执行了延迟");
    }
}