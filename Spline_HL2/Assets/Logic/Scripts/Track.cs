using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UIElements;
using System.Threading;
using Microsoft.MixedReality.Toolkit.Input;

public class Track : MonoBehaviour
{
    // Start is called before the first frame update
    public GameObject communication;
    public GameObject RandomPosition;
    public GameObject na_Bone;
    //public GameObject na_Cube760;
    public GameObject na_RobotGuide339;
    public GameObject na_HandeGuide449;
    public GameObject na_RealtimeNailA;
    public GameObject na_Drill864760;
    public GameObject na_Cube072;
    public GameObject na_Probe;
    public Transform BuildingPoint;
    public Transform BuildingDriectionA;
    public Transform BuildingDriectionB;
    public GameObject RobotGuide;
    public GameObject HandeGuide;
    private float[] nav_Bone = new float[7];
    private float[] nav_CubeProbe = new float[7];
    private float[] nav_Guide = new float[7];
    private float[] nav_Drill864760 = new float[7];
    private float[] nav_Cail072 = new float[7];
    public bool flag = true;
    public bool ToolTrick = true;
    public bool tempactive = true;
    private bool isBuilding ;
    private bool isCail;
    private bool isRegistration;
    private bool DrillisBuildingPoint ; 
    private bool DrillisBuildingDriection ;
    private bool DrillDriection = false;
    private bool HandeisBuilding;
    private bool HandeisBuildingDriection;
    private bool RobotisBuilding;
    private bool RobotisBuildingDriection;
    private bool isHande;
    public Material[] DrillMaterial;//ʵʱ�߲���
    public GameObject Drilla;
    //public TextMeshPro text;
    public GameObject rootGameObject;
    //public GameObject jiazi760;
    private Transform mainCameraTransform;   //�����
    public Transform Display;   //
    private float[] colorValues;
    private Vector3 initialLocalPosition;
    private Vector3 initiallocalEulerAngles;
    private GameObject boneparent;
    public GameObject Buttonenbled;
    private GameObject Lineobject_1;
    private GameObject Lineobject_2;

    public GameObject Point_1;
    public GameObject Point_2;
    public GameObject Point_3;
    public TextMeshPro[] textMeshs;
    public Material[] Linematerials;//ʵʱ�߲���

    void Start()
    {
        isBuilding = false;
        DrillisBuildingPoint = false;
        DrillisBuildingDriection = false;
        HandeisBuilding = false;
        HandeisBuildingDriection = false;
        RobotisBuilding = false;
        RobotisBuildingDriection = false;
        isCail = false;
        isHande = false;
        na_Probe.SetActive(false);
        Renderer Drill = Drilla.GetComponent<Renderer>();
        Drill.material = DrillMaterial[0];
        mainCameraTransform = Camera.main.transform;
        initialLocalPosition = Display.transform.localPosition;
        initiallocalEulerAngles = Display.transform.localEulerAngles;
        boneparent = new GameObject("boneparent");
        boneparent.transform.parent = na_Bone.transform.parent;
        na_RobotGuide339.SetActive(false);
        for (int i = 0; i< 7;i++)
        {
            nav_Bone[i] = 0;
            nav_CubeProbe[i] = 0; 
            nav_Guide[i] = 0;
            nav_Drill864760[i] = 0;
            nav_Cail072[i] = 0;
        }
        na_Probe.SetActive(false); na_Bone.SetActive(false); na_HandeGuide449.SetActive(false); na_RobotGuide339.SetActive(false); na_Drill864760.SetActive(false);

        Lineobject_1 = new GameObject("RanderLine_1"); //newһ����������
        Lineobject_1.AddComponent<LineRenderer>();//������������LineRenderer���
        Lineobject_1.GetComponent<LineRenderer>().widthMultiplier = 0.005f;
        Lineobject_1.GetComponent<LineRenderer>().positionCount = 2;

        Lineobject_2= new GameObject("RanderLine_2"); //newһ����������
        Lineobject_2.AddComponent<LineRenderer>();//������������LineRenderer���
        Lineobject_2.GetComponent<LineRenderer>().widthMultiplier = 0.005f;
        Lineobject_2.GetComponent<LineRenderer>().positionCount = 2;
        Lineobject_1.GetComponent<LineRenderer>().enabled = false; Lineobject_2.GetComponent<LineRenderer>().enabled = false;
    }

        // Update is called once per frame
        void Update()
    {  
        //���ٱ궨��
        if (isCail)
        {
            colorValues = communication.GetComponent<Communication>().getClolrChange();//communication�д����Ƿ�ɼ����ź�
            isRegistration = communication.GetComponent<Communication>().getisRegistration();
            //������̽�뵼��
            nav_Bone = communication.GetComponent<Communication>().getNavigationPos();//communication�д��������������λ�ø���Ҫ����������

            //�궨�׶�//�궨��
            if (colorValues[1] == 2)
            {
                na_Cube072.SetActive(true);
                nav_Cail072 = communication.GetComponent<Communication>().getCailPosition();
                na_Cube072.transform.position = new Vector3(nav_Cail072[4], nav_Cail072[5], nav_Cail072[6]);
                na_Cube072.transform.rotation = new Quaternion(nav_Cail072[0], nav_Cail072[1], nav_Cail072[2], nav_Cail072[3]);
                Display.transform.LookAt(Display.transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);                //�������     
            }
            else if (colorValues[1] == 0)
            {
                na_Cube072.SetActive(false);
            }
            //NEW
            else if (colorValues[1] == 3)
            {
                na_Cube072.SetActive(false);
                Point_3.transform.position = new Vector3(nav_Cail072[1] , nav_Cail072[2],nav_Cail072[3] );
                //��ʾ��
                textMeshs[0].enabled = true; textMeshs[0].transform.LookAt(textMeshs[0].transform.position + mainCameraTransform.rotation * Vector3.forward, mainCameraTransform.rotation * Vector3.up);
                int decimalPlaces = 2; // ����С������λ��
                float data = Mathf.Round(nav_Cail072[0] * Mathf.Pow(10, decimalPlaces)) / Mathf.Pow(10, decimalPlaces);
                textMeshs[0].text = data.ToString() + "% To Medial";
                //��Ⱦ��
                Lineobject_1.GetComponent<LineRenderer>().enabled = true;
                Lineobject_1.GetComponent<LineRenderer>().material = Linematerials[0];
                Lineobject_1.GetComponent<LineRenderer>().SetPosition(0, Point_3.transform.position);
                Lineobject_1.GetComponent<LineRenderer>().SetPosition(1, Point_2.transform.position);
                Lineobject_2.GetComponent<LineRenderer>().enabled = true;
                Lineobject_2.GetComponent<LineRenderer>().material = Linematerials[1];
                Lineobject_2.GetComponent<LineRenderer>().SetPosition(0, Point_3.transform.position);
                Lineobject_2.GetComponent<LineRenderer>().SetPosition(1, Point_1.transform.position);
            }
            else { textMeshs[0].enabled = false; Lineobject_1.GetComponent<LineRenderer>().enabled = false; Lineobject_2.GetComponent<LineRenderer>().enabled = false; na_Cube072.SetActive(false); }
            //NEW

            //̽��
            if (colorValues[0] == 1)
            {
                //Debug.Log("��ʼ���ٿ�ʼ����");
                nav_CubeProbe = communication.GetComponent<Communication>().getNavigationProbe();
                na_Probe.SetActive(true);
                na_Probe.transform.position = new Vector3(nav_CubeProbe[4], nav_CubeProbe[5], nav_CubeProbe[6]);
                na_Probe.transform.rotation = new Quaternion(nav_CubeProbe[0], nav_CubeProbe[1], nav_CubeProbe[2], nav_CubeProbe[3]);             //�������
            }
            else
            {
                na_Probe.SetActive(false);
            }

            //��׼�׶�
            //if (isRegistration)
            //{
                //jiazi760.SetActive(false);
                //���Ǹ���
                if (colorValues[2] == 3)
                {
                    //��ͷ����
                    na_Bone.SetActive(true);
                    //na_Bone.transform.parent = boneparent.transform.parent;
                    na_Bone.transform.position = new Vector3(nav_Bone[4], nav_Bone[5], nav_Bone[6]);
                    na_Bone.transform.rotation = new Quaternion(nav_Bone[0], nav_Bone[1], nav_Bone[2], nav_Bone[3]);
                    //���ӵ���
                }
                else
                {
                na_Bone.SetActive(false);
            }
            //}
            ////û��׼��
            //else 
            //{
            //    na_Bone.SetActive(false);
            //}

            nav_Guide = communication.GetComponent<Communication>().getNavigationRealtimeHand();
            //Debug.Log("�Ƿ�ɼ�"+colorValues[3]);
            if (colorValues[3] == 5)
            {
                na_HandeGuide449.SetActive(true);
                na_RobotGuide339.SetActive(false);
                //Debug.Log("�ֳ�449");
                //������Guide����449
                na_HandeGuide449.transform.position = new Vector3(nav_Guide[4], nav_Guide[5], nav_Guide[6]);
                na_HandeGuide449.transform.rotation = new Quaternion(nav_Guide[0], nav_Guide[1], nav_Guide[2], nav_Guide[3]);
            }
            if (colorValues[3] == 4)
            {
                na_RobotGuide339.SetActive(true);
                na_HandeGuide449.SetActive(false);
                //Debug.Log("����339");
                //������Guide����339
                na_RobotGuide339.transform.position = new Vector3(nav_Guide[4], nav_Guide[5], nav_Guide[6]);
                na_RobotGuide339.transform.rotation = new Quaternion(nav_Guide[0], nav_Guide[1], nav_Guide[2], nav_Guide[3]);
            }
            if (colorValues[3] == 0)
            {
                na_HandeGuide449.SetActive(false);
                na_RobotGuide339.SetActive(false);
            }

            if (colorValues[4] == 6)
            {
                //����Drill����449
                na_Drill864760.SetActive(true);
                nav_Drill864760 = communication.GetComponent<Communication>().getNavigationRealtimeDrill();
                na_Drill864760.transform.position = new Vector3(nav_Drill864760[4], nav_Drill864760[5], nav_Drill864760[6]);
                na_Drill864760.transform.rotation = new Quaternion(nav_Drill864760[0], nav_Drill864760[1], nav_Drill864760[2], nav_Drill864760[3]);
            }
            else
            {
                na_Drill864760.SetActive(false);
            }
            if (DrillisBuildingPoint)
            {
                Drilla.transform.SetParent(na_RealtimeNailA.transform);
                na_RealtimeNailA.transform.position = BuildingPoint.position;
                Drilla.transform.SetParent(na_Drill864760.transform);
                DrillisBuildingPoint = false;
            }
            if (DrillisBuildingDriection)
            {
                Drilla.transform.SetParent(na_RealtimeNailA.transform);
                Vector3 Driection = BuildingDriectionA.transform.position - BuildingDriectionB.transform.position;

                Vector3 nailUPAxis = na_RealtimeNailA.transform.up;                // ��ȡ��ӡ���ֵ�up��
                Quaternion nailroation = Quaternion.FromToRotation(nailUPAxis, Driection);        // ������ת��Ŀ�귽�������� A �������ת�Ƕ�
                na_RealtimeNailA.transform.rotation = nailroation * na_RealtimeNailA.transform.rotation;     // Ӧ����ת

                Drilla.transform.SetParent(na_Drill864760.transform);
                DrillisBuildingDriection = false;
            }
            if (DrillDriection) 
            {
                //GameObject newBall = Instantiate(testball, new Vector3(x, y, z), Quaternion.identity);

            }
            //�ֳ��������궨
            if (HandeisBuilding)
            {
                Debug.Log("�Ѿ��궨�ֳ�������");
                HandeGuide.transform.position = na_RealtimeNailA.transform.position;
                Vector3 nailUPAxis = na_RealtimeNailA.transform.up;                // ��ȡ��ӡ���ֵ�up��
                Quaternion nailroation = Quaternion.FromToRotation(HandeGuide.transform.up,- nailUPAxis);        // ������ת��Ŀ�귽�������� A �������ת�Ƕ�
                HandeGuide.transform.rotation = nailroation * HandeGuide.transform.rotation;     // Ӧ����ת
                HandeisBuilding = false;
            }
            //�����������궨
            if (RobotisBuilding)
            {
                Debug.Log("�Ѿ��궨����������");
                RobotGuide.transform.position = na_RealtimeNailA.transform.position;
                Vector3 nailUPAxis = na_RealtimeNailA.transform.up;                // ��ȡ��ӡ���ֵ�up��
                Quaternion nailroation = Quaternion.FromToRotation(RobotGuide.transform.up, nailUPAxis);        // ������ת��Ŀ�귽�������� A �������ת�Ƕ�
                RobotGuide.transform.rotation = nailroation * RobotGuide.transform.rotation;     // Ӧ����ת
                RobotisBuilding = false;
            }
        }
        else 
        {
            //na_Drill864760.SetActive(false);
            //na_Bone.SetActive(false);
        }

        //Vector3 newPosition = na_Bone.transform.position; // ��ȡλ�ò���
        ////text.text = "Position:" + na_RealtimeNailA.transform.localRotation + "Driection:" + na_RealtimeNailA.transform.localEulerAngles;
        //if (float.IsInfinity(newPosition.x) || float.IsInfinity(newPosition.y) || float.IsInfinity(newPosition.z))
        //{
        //    newPosition = Vector3.zero; // ��λ�ò�������Ϊ (0, 0, 0)
        //}
        //na_Bone.transform.position = newPosition;
    }
    public void isActive()
    {
        if(!flag)
        {
            flag = true;
            Debug.Log("false �� true");
        }else
        {
            flag = false;
            Debug.Log("true �� false");
        }
    }
    public void isCailtion()
    {
        int CalCount = Buttonenbled.GetComponent<Buttonenbled>().getCalCount();
        if (CalCount < 1) { Debug.Log("������ʼ����"); }
        else
        {
            if (!isCail)
            {
                isCail = true;
                EnableAllMeshRenderersInHierarchy();
                Debug.Log("false �� true");
            }
            else
            {
                isCail = false;
                na_Cube072.transform.localPosition = new Vector3(0.03817f, -0.005f, 0.025f);
                na_Cube072.transform.localEulerAngles = new Vector3(180f, -90, 90);
                Debug.Log("true �� false");
                Display.transform.localPosition = initialLocalPosition;
                Display.transform.localEulerAngles = initiallocalEulerAngles;
            }
        }

    }
    public void BuildingComplete()
    {
            isBuilding = true;
            Debug.Log("false �� true");
    }
    public void BuildingPointActive()
    {
        DrillisBuildingPoint = true;
        Debug.Log("isBuildingPoint �� true");
        Renderer Drill = Drilla.GetComponent<Renderer>();
        Drill.material =  DrillMaterial[1];
    }
    public void BuildingDriectionActive()
    {
        DrillisBuildingDriection = true;
        Debug.Log("isBuildingDriection �� true");
        Renderer Drill = Drilla.GetComponent<Renderer>();
        Drill.material = DrillMaterial[2];
    }
    public void HandeGuideActive()
    {
        HandeisBuilding = true;
        Debug.Log("HandeisBuilding �� true");
    }
    public void RobotGuideActive()
    {
        RobotisBuilding = true;
        Debug.Log("RobotisBuilding �� true");
    }
    public void EnableAllMeshRenderersInHierarchy()
    {
        if (rootGameObject != null)
        {
            EnableMeshRenderersRecursively(rootGameObject.transform);
        }
    }

    // �ݹ�ر��������㼶��������MeshRenderer��enabled����Ϊtrue
    private void EnableMeshRenderersRecursively(Transform parentTransform)
    {
        // ��ȡ�������µ�����������
        foreach (Transform child in parentTransform)
        {
            // ��ȡ�������ϵ�MeshRenderer���
            MeshRenderer meshRenderer = child.GetComponent<MeshRenderer>();

            // �����MeshRenderer���������������enabledΪtrue
            if (meshRenderer != null)
            {
                meshRenderer.enabled = true;
            }

            // �ݹ�ص���������������������������������
            EnableMeshRenderersRecursively(child);
        }
    }
}