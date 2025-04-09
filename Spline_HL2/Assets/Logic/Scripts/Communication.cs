using UnityEngine;
using UnityEngine.UI;
using System;
using System.Net;
using System.Net.Sockets;//����socket�����ռ�
using System.Threading;
using System.Text;
using UnityEngine.SceneManagement;
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.Generic;
using static Unity.IO.LowLevel.Unsafe.AsyncReadManagerMetrics;
using Unity.VisualScripting;
using Microsoft.MixedReality.Toolkit.UI;
using System.Timers;
using TMPro;
using System.Threading.Tasks;
using System.Reflection;

public class Communication : MonoBehaviour
{
    private bool isRunning = true; // ���һ����־����ʾ�Ƿ�������н���ѭ��
    public GameObject RandomPosition;
    public GameObject Buttonenbled;
    public GameObject cailposition;
    public GameObject buttonselect;
    public Image[] resigermages;             //��ȡ�˼�����׼��
    private float[] NavigationPosOfBone = { 0.5f, 0.5f, -0.5f, -0.5f, 0.15f, 0f, 0.3f };
    private float[] NavigationPosOfProbe = { 0.5f, 0.5f, -0.5f, -0.5f, 0.15f, 0f, 0.3f };
    private float[] NavigationPosOfRealtimeHand8700339 = { 0.5f, 0.5f, -0.5f, -0.5f, 0.15f, 0f, 0.3f };
    private float[] NavigationPosOfDrill8700449 = { 0.5f, 0.5f, -0.5f, -0.5f, 0.15f, 0f, 0.3f };
    private float[] NavigationPositionofCube072 = { 0.5f, 0.5f, -0.5f, -0.5f, 0.15f, 0f, 0.3f };
    private float[] PositionIn8700449 = { 0, 0, 0, 0 };
    private float[] DriectionIn8700449 = { 0.5f, 0.5f, -0.5f, -0.5f };
    private float[] ColorChange = { 0, 0, 0,0,0 };
    public GameObject Positiondata_1 ;
    public GameObject Positiondata_2;



    public Material BallMaterial;
    public Image stateimage;
    //xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    //private float[] CaliPos = { 1f, 2f, 3.4f, 0.24f, 5.64f, -14.15f, 2.5f };
    public GameObject pointOne;
    public GameObject pointTwo;

    //
    public GameObject cliaobj;
    //public GameObject xyz;
    private bool iscail = false; // ���һ����־����ʾ�Ƿ�������н���ѭ��
    //private GameObject Lineobj;
    //public TextMeshPro text00;

    //
    //private List<byte[]> byteList;
    private byte[] sendbuffer;
    private byte[] byte0;
    private byte[] byte1;
    private byte[] byte2;
    private byte[] byte3;
    private byte[] byte4;
    private byte[] byte5;
    private byte[] byte6;
    //
    private byte[] byte7;
    private byte[] byte8;
    private byte[] byte9;

    //
    private byte[] Guideindex;
    private byte[] btn1;//�������Ӻ����ݴ���
    private byte[] flag2;//����C++�г���Ҫһֱˢ�±궨��λ��
    private byte[] btn3;//��C++���ͱ궨ָ��
    private byte[] btn4;//��C++���Ͳɵ�ָ��
    private byte[] btn5;//��C++������׼ָ��


    private int[] numbers;
    public GameObject SubmitButton;
    public GameObject Submittips;
    public GameObject Regtips;
    public GameObject Cailtips;
    public GameObject QusitionNaire;
    public GameObject Plane3;


    private float timeSinceLastPrint;
    private int receivedDataCount;
    private int receivednumber = 0;

    private bool isHande;
    private bool isRegistration;

    private bool isConnected = false;
    private bool isReceived = false;

    private float startTimeStamp;
    private float endTimeStamp;

    private bool nettest;

    private int nettestnumber;

    private Socket socket_client;
    private System.Timers.Timer timer;

    private UdpClient udpClient;
    private IPEndPoint endPoint;


    public GameObject guide;
    public GameObject probe;
    public GameObject testBallPrefab;

    private List<Transform> ballPositions_1 = new List<Transform>();
    private List<Transform> ballPositions_2 = new List<Transform>();


    //�¼�
    public Transform ShowPosition;
    public GameObject ManSpline;
    public GameObject ShowPlane0;
    private bool startRecord = false;
    private GameObject ballparent;
    public GameObject Bone0;
    public GameObject roteimage;
    public GameObject roteimage_background;
    public GameObject icon;
    public Sprite[] iconbackgrounds;
    private bool issplineshow = false;
    private bool isfineresig = false;
    private float startTime = 0f;
    //public GameObject Bone1;

    public float[] getClolrChange()
    {
        return ColorChange;//��ɫ�ı�
    }
    public float[] getNavigationPos()
    {
        return NavigationPosOfBone;//����
    }
    public float[] getNavigationProbe()
    {
        return NavigationPosOfProbe;//����
    }
    public float[] getNavigationRealtimeHand()
    {
        return NavigationPosOfRealtimeHand8700339;//������
    }
    public float[] getNavigationRealtimeDrill()
    {
        return NavigationPosOfDrill8700449;//����
    }
    public float[] getCailPosition()
    {
        return NavigationPositionofCube072;//�궨
    }
    public bool getisRegistration()
    {
        return isRegistration;//�Ƿ��ѱ궨��
    }
    void Start()
    {
        Debug.Log("start");
        sendbuffer = new byte[48];
        btn1 = ToByte(1);
        flag2 = ToByte(0);
        btn3 = ToByte(0);
        btn4 = ToByte(0);
        btn5 = ToByte(0);
        byte1 = new byte[4]; byte2 = new byte[4]; byte3 = new byte[4];byte4 = new byte[4];
        byte5 = new byte[4]; byte6 = new byte[4]; /*Guideindex = new byte[4]; */byte7 = new byte[4];
        isRegistration = false;
        nettest = false;
        nettestnumber = 0;
        //
        //xyz.SetActive(false);
        //Lineobj = new GameObject("lineobj");
        //Lineobj.AddComponent<LineRenderer>();//������������LineRenderer���
        //Lineobj.GetComponent<LineRenderer>().widthMultiplier = 0.0005f;
        //Lineobj.GetComponent<LineRenderer>().positionCount = 2;

        Submittips.SetActive(false);
        numbers = new int[5];
        //�¼�
        ManSpline.SetActive(false); ShowPlane0.SetActive(false);
        //�൱��һֱ����
        udpClient = new UdpClient(6666);
        endPoint = new IPEndPoint(IPAddress.Any, 0);
        isReceived = true;
        try
        {
            udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
        }
        catch (Exception ex)
        {
            Debug.LogError("Error starting UDP receive: " + ex.Message);
        }
        ballparent = new GameObject("BallParent");
        icon.SetActive(false);
        //
    }

    // Update is called once per frame
    void Update()
    {
        if (ColorChange[1] == 2)
        {
            BallMaterial.color = Color.green;
        }
        else if (ColorChange[1] == 0)
        {
            BallMaterial.color = Color.red;
        }
        // �ۼ�ʱ����
        timeSinceLastPrint += Time.deltaTime;
        //�¼� //��ʾһ�¿�ʼ���ӽ���
        if (ColorChange[0] == 9)
        {
            ShowPlane0.SetActive(true);
            Debug.Log("wwwwwwwwwwwwwwwwwwwwwwwwwwwww");
            ShowPlane0.transform.position = ShowPosition.position; ShowPlane0.transform.rotation = ShowPosition.rotation;
            //new
            Positiondata_1.transform.localPosition = new Vector3(NavigationPosOfProbe[0]/1000, NavigationPosOfProbe[1] / 1000,- NavigationPosOfProbe[2] / 1000);
            Positiondata_2.transform.localPosition = new Vector3(NavigationPosOfProbe[3] / 1000, NavigationPosOfProbe[4] / 1000,- NavigationPosOfProbe[5] / 1000);
            //new
            ColorChange[0] = 0;
        }
        if (ColorChange[0] == 10)
        {
            icon.SetActive(true);
            icon.transform.position = ShowPosition.position; icon.transform.rotation = ShowPosition.transform.rotation;
            Image iconbackground = roteimage_background.GetComponent<Image>();
            iconbackground.sprite = iconbackgrounds[0];
            startTime = Time.time;
            issplineshow = true;
            ColorChange[0] = 0;
        }
        if (issplineshow)
        {
            Debug.Log("��ʼ�ϴ�");
            startrotateimage();
        }
        if (isfineresig)
        {
            Debug.Log("��ʼ��׼");
            startrotateimage();
        }

        if (startRecord) 
        {

            GameObject newBall2 = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);
            newBall2.transform.parent = ballparent.transform;
        }


        // ����ÿ��������ݵĴ���
        if (timeSinceLastPrint >= 1.0f)
        {
            float receiveRate = receivedDataCount / timeSinceLastPrint;
            //Debug.Log("����Ƶ�ʣ�ÿ����մ�������" + receiveRate);

            // ���ü�������ʱ����
            receivedDataCount = 0;
            timeSinceLastPrint = 0f;
        }

        if (ColorChange[0] == 8 && nettest)
        {
            endTimeStamp = Time.time;
            float elapsedTime = (endTimeStamp - startTimeStamp )* 1000f;
            nettestnumber++;
            Debug.Log("����ʼʱ���: " + startTimeStamp+"�������ʱ���: " + endTimeStamp+"��" + nettestnumber + "�ε����"+"ʱ���Ϊ: " + elapsedTime + " ms");
            nettest = false;
        }
        if (iscail) 
        {
            //Lineobj.SetActive(true);
            //text00.gameObject.SetActive(true);
            //LineRenderer lineobjrender = Lineobj.GetComponent<LineRenderer>();
            //lineobjrender.SetPosition(0,xyz.transform.position);
            //lineobjrender.SetPosition(1, cliaobj.transform.position);
            //float distance = Vector3.Distance(xyz.transform.position, cliaobj.transform.position)*1000;
            //text00.text = "distance:" + distance.ToString() + "mm";
        }
        else {/* Lineobj.SetActive(false); text00.gameObject.SetActive(false); */}
    }

    public void ConnectServer()
    {
        Debug.Log("isConnected" + isConnected);
        if (!isConnected)
        {
            try
            {
                isConnected = true;
                IPAddress pAddress = endPoint.Address;
                IPEndPoint pEndPoint = new IPEndPoint(pAddress, 8888);
                socket_client = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket_client.Connect(pEndPoint);
                Debug.Log("���ӳɹ�");
                // �����̣߳�ִ�ж�ȡ��������Ϣ
                // ��������
                byte1 = ToByte(1);
                send();
                stateimage.color = Color.green;
                isReceived = true;
                Debug.Log("isConnected2222222222" + isConnected);
            }
            catch (SocketException ex)
            {
                Debug.Log("Socket�쳣: " + ex.Message);
                stateimage.color = Color.red;
            }
            catch (FormatException ex)
            {
                Debug.Log("IP��ַ��˿ںŸ�ʽ����: " + ex.Message);
                stateimage.color = Color.red;
            }
            catch (System.Exception)
            {
                stateimage.color = Color.red;
                Debug.Log("IP�˿ںŴ�����߷�����δ����");
            }
        }
        else 
        {
            byte1 = ToByte(1);
            send();
            //stateimage.color = Color.green;
            isReceived = true;
            //udpClient = new UdpClient(6666);
            //udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
        }
    }


    public void Disconnect()
    {
        byte1 = ToByte(2);
        send();
        //try
        //{
        //    socket_client.Close();
        //    Debug.Log("�رտͻ�������");
        //}
        //catch (System.Exception)
        //{
        //    Debug.Log("δ����");
        //}
        for (int i = 0; i < 5; i++)
        {
            ColorChange[i] = 0;
        }
        stateimage.color = Color.red;
        isReceived = false;
        udpClient.Close();
    }


    // �� c_thread �����ö�ʱ����ִ�ж�ȡ��������Ϣ�ķ���
    private void SetupTimerAndReceived()
    {
        // ����һ�� Timer ���󣬵�һ�������ǻص��������ڶ���������״̬���󣨿���Ϊ null�����������������ӳ�������ʱ�䣬���ĸ������Ǽ��ʱ��
        //timer = new System.Timers.Timer(5); // 1ms
        //timer.Elapsed += TimerElapsed;
        //timer.AutoReset = true; // ����Ϊ true �����ö�ʱ���ظ�����
        //timer.Start();

        // ��ȡ��������Ϣ���߼�
        Received();
    }

    private void ReceiveCallback(IAsyncResult ar)
    {
        if (isReceived)
        {
            try
            {
                byte[] allbuffer = new byte[1024];//������������
                byte[] buffer = new byte[40 * 4];//������������
                                                 // ��UDP�ͻ��˽�������
                buffer = udpClient.EndReceive(ar, ref endPoint);

                 // ������յ������ݣ�������Ը���ʵ��������д���
                //string receivedData = System.Text.Encoding.UTF8.GetString(buffer);
                //Debug.Log("Received UDP data: " + receivedData);

                string timestamp0 = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
                //Debug.Log("��ʼʱ��Ϊ" + timestamp0);
                Debug.Log("������������");



                //int len = socket_client.Receive(allbuffer);
                //int counts = (len / 160);
                //if (len >= 160)
                //{
                //    Array.Copy(allbuffer, len - 160, buffer, 0, 160);
                //}
                //Debug.Log("len����Ϊ"+len+ "countsΪ" + counts);

                byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4];
                byte[] byte5 = new byte[4]; byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4];
                //�յ�������Cube����760λ��
                byte[] byte8 = new byte[4]; byte[] byte9 = new byte[4]; byte[] byte10 = new byte[4]; byte[] byte11 = new byte[4];
                byte[] byte12 = new byte[4]; byte[] byte13 = new byte[4]; byte[] byte14 = new byte[4];
                //�յ�������Guide����339
                byte[] byte15 = new byte[4]; byte[] byte16 = new byte[4]; byte[] byte17 = new byte[4]; byte[] byte18 = new byte[4];
                byte[] byte19 = new byte[4]; byte[] byte20 = new byte[4]; byte[] byte21 = new byte[4];
                //�յ�������Drill����449
                byte[] byte22 = new byte[4]; byte[] byte23 = new byte[4]; byte[] byte24 = new byte[4]; byte[] byte25 = new byte[4];
                byte[] byte26 = new byte[4]; byte[] byte27 = new byte[4]; byte[] byte28 = new byte[4];
                //�յ��궨����
                byte[] byte29 = new byte[4]; byte[] byte30 = new byte[4]; byte[] byte31 = new byte[4]; byte[] byte32 = new byte[4];
                byte[] byte33 = new byte[4]; byte[] byte34 = new byte[4]; byte[] byte35 = new byte[4];
                //��ɫ�仯
                byte[] byte36 = new byte[4]; byte[] byte37 = new byte[4]; byte[] byte38 = new byte[4]; byte[] byte39 = new byte[4]; byte[] byte40 = new byte[4];

                //����λ�òɵ�/����λ��/ȡ�����Ƿ���׼�ɹ�
                Array.Copy(buffer, 0, byte1, 0, 4);
                Array.Copy(buffer, 4, byte2, 0, 4);
                Array.Copy(buffer, 8, byte3, 0, 4);
                Array.Copy(buffer, 12, byte4, 0, 4);
                Array.Copy(buffer, 16, byte5, 0, 4);
                Array.Copy(buffer, 20, byte6, 0, 4);
                Array.Copy(buffer, 24, byte7, 0, 4);
                //Probe����-Probe
                Array.Copy(buffer, 28, byte8, 0, 4);
                Array.Copy(buffer, 32, byte9, 0, 4);
                Array.Copy(buffer, 36, byte10, 0, 4);
                Array.Copy(buffer, 40, byte11, 0, 4);
                Array.Copy(buffer, 44, byte12, 0, 4);
                Array.Copy(buffer, 48, byte13, 0, 4);
                Array.Copy(buffer, 52, byte14, 0, 4);
                //Guide����-339
                Array.Copy(buffer, 56, byte15, 0, 4);
                Array.Copy(buffer, 60, byte16, 0, 4);
                Array.Copy(buffer, 64, byte17, 0, 4);
                Array.Copy(buffer, 68, byte18, 0, 4);
                Array.Copy(buffer, 72, byte19, 0, 4);
                Array.Copy(buffer, 76, byte20, 0, 4);
                Array.Copy(buffer, 80, byte21, 0, 4);
                //CDrill����-449
                Array.Copy(buffer, 84, byte22, 0, 4);
                Array.Copy(buffer, 88, byte23, 0, 4);
                Array.Copy(buffer, 92, byte24, 0, 4);
                Array.Copy(buffer, 96, byte25, 0, 4);
                Array.Copy(buffer, 100, byte26, 0, 4);
                Array.Copy(buffer, 104, byte27, 0, 4);
                Array.Copy(buffer, 108, byte28, 0, 4);
                //�궨С��
                Array.Copy(buffer, 112, byte29, 0, 4);
                Array.Copy(buffer, 116, byte30, 0, 4);
                Array.Copy(buffer, 120, byte31, 0, 4);
                Array.Copy(buffer, 124, byte32, 0, 4);
                Array.Copy(buffer, 128, byte33, 0, 4);
                Array.Copy(buffer, 132, byte34, 0, 4);
                Array.Copy(buffer, 136, byte35, 0, 4);
                //��ɫ
                Array.Copy(buffer, 140, byte36, 0, 4);
                Array.Copy(buffer, 144, byte37, 0, 4);
                Array.Copy(buffer, 148, byte38, 0, 4);
                Array.Copy(buffer, 152, byte39, 0, 4);
                Array.Copy(buffer, 156, byte40, 0, 4);

                //̽��-probe---0-7(0)
                NavigationPosOfProbe[0] = ToFloat(byte1);
                NavigationPosOfProbe[1] = ToFloat(byte2);
                NavigationPosOfProbe[2] = ToFloat(byte3);
                NavigationPosOfProbe[3] = ToFloat(byte4);
                NavigationPosOfProbe[4] = ToFloat(byte5);
                NavigationPosOfProbe[5] = ToFloat(byte6);
                NavigationPosOfProbe[6] = ToFloat(byte7);

                //�궨��8-14(1)
                NavigationPositionofCube072[0] = ToFloat(byte8);
                NavigationPositionofCube072[1] = ToFloat(byte9);
                NavigationPositionofCube072[2] = ToFloat(byte10);
                NavigationPositionofCube072[3] = ToFloat(byte11);
                NavigationPositionofCube072[4] = ToFloat(byte12);
                NavigationPositionofCube072[5] = ToFloat(byte13);
                NavigationPositionofCube072[6] = ToFloat(byte14);
                //���Ǽ���-760(2)
                NavigationPosOfBone[0] = ToFloat(byte15);
                NavigationPosOfBone[1] = ToFloat(byte16);
                NavigationPosOfBone[2] = ToFloat(byte17);
                NavigationPosOfBone[3] = ToFloat(byte18);
                NavigationPosOfBone[4] = ToFloat(byte19);
                NavigationPosOfBone[5] = ToFloat(byte20);
                NavigationPosOfBone[6] = ToFloat(byte21);
                //Guide����-339(3)
                NavigationPosOfRealtimeHand8700339[0] = ToFloat(byte22);
                NavigationPosOfRealtimeHand8700339[1] = ToFloat(byte23);
                NavigationPosOfRealtimeHand8700339[2] = ToFloat(byte24);
                NavigationPosOfRealtimeHand8700339[3] = ToFloat(byte25);
                NavigationPosOfRealtimeHand8700339[4] = ToFloat(byte26);
                NavigationPosOfRealtimeHand8700339[5] = ToFloat(byte27);
                NavigationPosOfRealtimeHand8700339[6] = ToFloat(byte28);
                //Drill���� - 760(4)
                NavigationPosOfDrill8700449[0] = ToFloat(byte29);
                NavigationPosOfDrill8700449[1] = ToFloat(byte30);
                NavigationPosOfDrill8700449[2] = ToFloat(byte31);
                NavigationPosOfDrill8700449[3] = ToFloat(byte32);
                NavigationPosOfDrill8700449[4] = ToFloat(byte33);
                NavigationPosOfDrill8700449[5] = ToFloat(byte34);
                NavigationPosOfDrill8700449[6] = ToFloat(byte35);
                //ɫ��ָʾ
                ColorChange[0] = ToFloat(byte36);
                ColorChange[1] = ToFloat(byte37);
                ColorChange[2] = ToFloat(byte38);
                ColorChange[3] = ToFloat(byte39);
                Debug.Log("��ɫΪ" + NavigationPositionofCube072[0] + "��ɫΪ" + NavigationPositionofCube072[1] + "��ɫΪ" + NavigationPositionofCube072[2] + "��ɫΪ" + NavigationPositionofCube072[3]);
                ColorChange[4] = ToFloat(byte40);
                // ��¼�������ݵĴ���
                receivedDataCount++;
                receivednumber++;
                string timestamp = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
                //Debug.Log("�� " + receivednumber + "�η���ʱ��Ϊ" + timestamp /*+ "bufferlen" + len*/);
                if (ColorChange[0]==10) {  issplineshow = true; }

                // �����첽������һ������
                udpClient.BeginReceive(new AsyncCallback(ReceiveCallback), null);
            }
            catch (Exception ex)
            {
                Debug.Log("Error receiving UDP data: " + ex.Message);
            }
        }
    }

    public void Received()
    {

        //while (true)
        //{
        //    try
        //    {
        //        string timestamp0 = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
        //        //Debug.Log("��ʼʱ��Ϊ" + timestamp0);
        //        //Debug.Log("������������");

        //        byte[] allbuffer = new byte[1024];//������������
        //        byte[] buffer = new byte[40 * 4];//������������
                   

        //        int len = socket_client.Receive(allbuffer);
        //        int counts = (len / 160);
        //        if (len >= 160)
        //        {
        //            Array.Copy(allbuffer, len - 160, buffer, 0, 160);
        //        }
        //        //Debug.Log("len����Ϊ"+len+ "countsΪ" + counts);

        //        byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4];
        //        byte[] byte5 = new byte[4]; byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4];
        //        //�յ�������Cube����760λ��
        //        byte[] byte8 = new byte[4]; byte[] byte9 = new byte[4]; byte[] byte10 = new byte[4]; byte[] byte11 = new byte[4];
        //        byte[] byte12 = new byte[4]; byte[] byte13 = new byte[4]; byte[] byte14 = new byte[4];
        //        //�յ�������Guide����339
        //        byte[] byte15 = new byte[4]; byte[] byte16 = new byte[4]; byte[] byte17 = new byte[4]; byte[] byte18 = new byte[4];
        //        byte[] byte19 = new byte[4]; byte[] byte20 = new byte[4]; byte[] byte21 = new byte[4];
        //        //�յ�������Drill����449
        //        byte[] byte22 = new byte[4]; byte[] byte23 = new byte[4]; byte[] byte24 = new byte[4]; byte[] byte25 = new byte[4];
        //        byte[] byte26 = new byte[4]; byte[] byte27 = new byte[4]; byte[] byte28 = new byte[4];
        //        //�յ��궨����
        //        byte[] byte29 = new byte[4]; byte[] byte30 = new byte[4]; byte[] byte31 = new byte[4]; byte[] byte32 = new byte[4];
        //        byte[] byte33 = new byte[4]; byte[] byte34 = new byte[4]; byte[] byte35 = new byte[4];
        //        //��ɫ�仯
        //        byte[] byte36 = new byte[4]; byte[] byte37 = new byte[4]; byte[] byte38 = new byte[4]; byte[] byte39 = new byte[4]; byte[] byte40 = new byte[4];


        //        //����λ�òɵ�/����λ��/ȡ�����Ƿ���׼�ɹ�
        //        Array.Copy(buffer, 0, byte1, 0, 4);
        //        Array.Copy(buffer, 4, byte2, 0, 4);
        //        Array.Copy(buffer, 8, byte3, 0, 4);
        //        Array.Copy(buffer, 12, byte4, 0, 4);
        //        Array.Copy(buffer, 16, byte5, 0, 4);
        //        Array.Copy(buffer, 20, byte6, 0, 4);
        //        Array.Copy(buffer, 24, byte7, 0, 4);
        //        //Probe����-Probe
        //        Array.Copy(buffer, 28, byte8, 0, 4);
        //        Array.Copy(buffer, 32, byte9, 0, 4);
        //        Array.Copy(buffer, 36, byte10, 0, 4);
        //        Array.Copy(buffer, 40, byte11, 0, 4);
        //        Array.Copy(buffer, 44, byte12, 0, 4);
        //        Array.Copy(buffer, 48, byte13, 0, 4);
        //        Array.Copy(buffer, 52, byte14, 0, 4);
        //        //Guide����-339
        //        Array.Copy(buffer, 56, byte15, 0, 4);
        //        Array.Copy(buffer, 60, byte16, 0, 4);
        //        Array.Copy(buffer, 64, byte17, 0, 4);
        //        Array.Copy(buffer, 68, byte18, 0, 4);
        //        Array.Copy(buffer, 72, byte19, 0, 4);
        //        Array.Copy(buffer, 76, byte20, 0, 4);
        //        Array.Copy(buffer, 80, byte21, 0, 4);
        //        //CDrill����-449
        //        Array.Copy(buffer, 84, byte22, 0, 4);
        //        Array.Copy(buffer, 88, byte23, 0, 4);
        //        Array.Copy(buffer, 92, byte24, 0, 4);
        //        Array.Copy(buffer, 96, byte25, 0, 4);
        //        Array.Copy(buffer, 100, byte26, 0, 4);
        //        Array.Copy(buffer, 104, byte27, 0, 4);
        //        Array.Copy(buffer, 108, byte28, 0, 4);
        //        //�궨С��
        //        Array.Copy(buffer, 112, byte29, 0, 4);
        //        Array.Copy(buffer, 116, byte30, 0, 4);
        //        Array.Copy(buffer, 120, byte31, 0, 4);
        //        Array.Copy(buffer, 124, byte32, 0, 4);
        //        Array.Copy(buffer, 128, byte33, 0, 4);
        //        Array.Copy(buffer, 132, byte34, 0, 4);
        //        Array.Copy(buffer, 136, byte35, 0, 4);
        //        //��ɫ
        //        Array.Copy(buffer, 140, byte36, 0, 4);
        //        Array.Copy(buffer, 144, byte37, 0, 4);
        //        Array.Copy(buffer, 148, byte38, 0, 4);
        //        Array.Copy(buffer, 152, byte39, 0, 4);
        //        Array.Copy(buffer, 156, byte40, 0, 4);

        //        //����
        //        NavigationPosOfBone[0] = ToFloat(byte1);
        //        NavigationPosOfBone[1] = ToFloat(byte2);
        //        NavigationPosOfBone[2] = ToFloat(byte3);
        //        NavigationPosOfBone[3] = ToFloat(byte4);
        //        NavigationPosOfBone[4] = ToFloat(byte5);
        //        NavigationPosOfBone[5] = ToFloat(byte6);
        //        NavigationPosOfBone[6] = ToFloat(byte7);
    
        //        //Cybe����-760
        //        NavigationPosOfProbe[0] = ToFloat(byte8);
        //        NavigationPosOfProbe[1] = ToFloat(byte9);
        //        NavigationPosOfProbe[2] = ToFloat(byte10);
        //        NavigationPosOfProbe[3] = ToFloat(byte11);
        //        NavigationPosOfProbe[4] = ToFloat(byte12);
        //        NavigationPosOfProbe[5] = ToFloat(byte13);
        //        NavigationPosOfProbe[6] = ToFloat(byte14);
        //        //Guide����-339
        //        NavigationPosOfRealtimeHand8700339[0] = ToFloat(byte15);
        //        NavigationPosOfRealtimeHand8700339[1] = ToFloat(byte16);
        //        NavigationPosOfRealtimeHand8700339[2] = ToFloat(byte17);
        //        NavigationPosOfRealtimeHand8700339[3] = ToFloat(byte18);
        //        NavigationPosOfRealtimeHand8700339[4] = ToFloat(byte19);
        //        NavigationPosOfRealtimeHand8700339[5] = ToFloat(byte20);
        //        NavigationPosOfRealtimeHand8700339[6] = ToFloat(byte21);
        //        //Drill���� - 449
        //        NavigationPosOfDrill8700449[0] = ToFloat(byte22);
        //        NavigationPosOfDrill8700449[1] = ToFloat(byte23);
        //        NavigationPosOfDrill8700449[2] = ToFloat(byte24);
        //        NavigationPosOfDrill8700449[3] = ToFloat(byte25);
        //        NavigationPosOfDrill8700449[4] = ToFloat(byte26);
        //        NavigationPosOfDrill8700449[5] = ToFloat(byte27);
        //        NavigationPosOfDrill8700449[6] = ToFloat(byte28);
        //        //�궨��
        //        NavigationPositionofCube072[0] = ToFloat(byte29);
        //        NavigationPositionofCube072[1] = ToFloat(byte30);
        //        NavigationPositionofCube072[2] = ToFloat(byte31);
        //        NavigationPositionofCube072[3] = ToFloat(byte32);
        //        NavigationPositionofCube072[4] = ToFloat(byte33);
        //        NavigationPositionofCube072[5] = ToFloat(byte34);
        //        NavigationPositionofCube072[6] = ToFloat(byte35);
        //        //ɫ��ָʾ
        //        ColorChange[0] = ToFloat(byte36);
        //        ColorChange[1] = ToFloat(byte37);
        //        ColorChange[2] = ToFloat(byte38);
        //        ColorChange[3] = ToFloat(byte39);
        //        //Debug.Log("��ɫΪ"+ ColorChange[3]);
        //        ColorChange[4] = ToFloat(byte40);
        //        // ��¼�������ݵĴ���
        //        receivedDataCount++;
        //        receivednumber++;
        //        string timestamp = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
        //        //Debug.Log("�� " + receivednumber + "�η���ʱ��Ϊ"+ timestamp+ "bufferlen" + len);

        //    }
        //    catch (System.Exception)
        //    {
        //        Debug.Log("�����쳣");
        //    }
        //}
    }
    public void recordposition()
    {
        byte0 = ToByte(0);//�׶�0
        byte2 = ToByte(1);//��¼λ��-1ƽ��-0cailposition
        //��¼λ��
        byte1 = ToByte(cailposition.transform.position.x);
        byte4 = ToByte(cailposition.transform.position.y);//��1λ����Ϣx
        byte5 = ToByte(cailposition.transform.position.z);//��1λ����Ϣy
        byte6 = ToByte(cailposition.transform.rotation.w);//��1λ����Ϣz
        byte7 = ToByte(cailposition.transform.rotation.x);//��2λ����Ϣx
        byte8 = ToByte(cailposition.transform.rotation.y);//��2λ����Ϣy
        byte9 = ToByte(cailposition.transform.rotation.z);//��2λ����Ϣz

        //��¼��
        //byte4 = ToByte(pointOne.transform.position.x);//��1λ����Ϣx
        //byte5 = ToByte(pointOne.transform.position.y);//��1λ����Ϣy
        //byte6 = ToByte(pointOne.transform.position.z);//��1λ����Ϣz
        //byte7 = ToByte(pointTwo.transform.position.x);//��2λ����Ϣx
        //byte8 = ToByte(pointTwo.transform.position.y);//��2λ����Ϣy
        //byte9 = ToByte(pointTwo.transform.position.z);//��2λ����Ϣz
        //Debug.Log("one��λ��" + pointOne.transform.position + "two��λ��" + pointTwo.transform.position);
        //

        //byte1 = ToByte(cliaobj.transform.position.x);
        //byte2 = ToByte(cliaobj.transform.position.y);
        //byte3 = ToByte(cliaobj.transform.position.z);
        //byte4 = ToByte(cliaobj.transform.rotation.w);
        //byte5 = ToByte(cliaobj.transform.rotation.x);
        //byte6 = ToByte(cliaobj.transform.rotation.y);
        //byte7 = ToByte(cliaobj.transform.rotation.z);
        iscail = false;
        //

        //flag2 = ToByte(1); //C++��ˢ��һ��λ��
        send();

        byte2 = ToByte(0);//��¼λ��-1ƽ��-0
        byte4 = ToByte(0);//��1λ����Ϣxƽ��-0
        byte5 = ToByte(0);//��1λ����Ϣyƽ��-0
        byte6 = ToByte(0);//��1λ����Ϣzƽ��-0
        byte7 = ToByte(0);//��2λ����Ϣxƽ��-0
        byte8 = ToByte(0);//��2λ����Ϣyƽ��-0
        byte9 = ToByte(0);//��2λ����Ϣzƽ��-0
        //flag2 = ToByte(0);//ˢ����͹ص�������ˢ��
    }

    //�궨��ť�����׶�0
    public void calibration()
    {
        int CalCount = Buttonenbled.GetComponent<Buttonenbled>().getCalCount();
        if (CalCount < 1)//�ж��Ƿ���0//��0�Ļ�
        {
            StartCoroutine(DisappearAfterDelay(1f, Cailtips));
        }
        else 
        {
            btn3 = ToByte(1);
            byte0 = ToByte(0);//�׶�0
            byte3 = ToByte(1);//�궨-1�ر�-2ƽ��-0
            //xyz.SetActive(true);
            //xyz.transform.position = cliaobj.transform.position;
            //xyz.transform.rotation = cliaobj.transform.rotation;
            iscail = true;
            //
            send();

            byte3 = ToByte(0);//�궨-1�ر�-2ƽ��-0

            btn3 = ToByte(0);
        }

    }
    //����궨�㡪���׶�0
    public void caliClear() 
    {
        byte0 = ToByte(0);//�׶�0
        byte3 = ToByte(2);//�궨-1�ر�-2ƽ��-0
        send();
        byte3 = ToByte(0);//�궨-1�ر�-2ƽ��-0

    }
    //��ȡ��׼�㡪���׶�1
    public void Acquiring()
    {
        //byte0 = ToByte(1);//�׶�1
        //byte1 = ToByte(1);//�ɼ�-1ƽ��-0
        //send();
        //byte1 = ToByte(0);//�ɼ�-1ƽ��-0
        GameObject newBall = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);

    }
    //������׼�����׶�1
    public void startRegistration()
    {
        startRecord = true;
        ballparent.SetActive(true);
        //Regtips.SetActive(false);
        //int AcqCount = Buttonenbled.GetComponent<Buttonenbled>().getAcqCount();
        //if (AcqCount<8)//�ж��Ƿ���0//��0�Ļ�
        //{
        //    StartCoroutine(DisappearAfterDelay(1f, Regtips));
        //}
        //else 
        //{
        //    isRegistration = true;
        //    byte0 = ToByte(1);//�׶�1
        //    byte2 = ToByte(1);//��׼-1����-2ƽ��-0
        //    send();
        //    byte2 = ToByte(0);//��׼-1����-2ƽ��-0
        //}

    }
    public void StopRegistration()
    {
        startRecord = false;
    }
    //�����׼�㡪���׶�1
    public void AcqClear()
    {
        //byte0 = ToByte(1);//�׶�1
        //byte2 = ToByte(2);//��׼-1����-2ƽ��-0
        //send();
        //byte2 = ToByte(0);//��׼-1����-2ƽ��-0
        ballparent.SetActive(false);
    }
    public void Coarseregistration()
    {
        ballparent.SetActive(true);
        Bone0.SetActive(true);
    }
    public void Fineregistration()
    {
        icon.SetActive(true);
        icon.transform.position = ShowPosition.position; icon.transform.rotation = ShowPosition.transform.rotation;
        Image iconbackground = roteimage_background.GetComponent<Image>();
        iconbackground.sprite = iconbackgrounds[1];
        startTime = Time.time;
        isfineresig = true; 
    }

    public void startrotateimage()
    {
        // ������ʱ��С��ָ��ʱ��ת
        if (Time.time - startTime < 10f)
        {
            // ÿ֡��ת����
            Debug.Log("��ʼ��ת��");
            roteimage.transform.localEulerAngles = new Vector3(0, 0, 800f * (Time.time - startTime));
        }
        else
        {
            Debug.Log("������ת��");
            roteimage.transform.localEulerAngles = new Vector3(0, 0, 0);
            // ��ת��������������������ֹͣ��ת��Ĳ���
            if (issplineshow)
            {
                ManSpline.SetActive(true);
                ManSpline.transform.position = ShowPosition.position; ManSpline.transform.rotation = ShowPosition.rotation;
                issplineshow = false;
                icon.SetActive(false);
            }
            if (isfineresig)
            {
                Bone0.transform.localPosition = new Vector3(0, 0, 0);
                Bone0.transform.localEulerAngles = new Vector3(0, 0, 0);
                isfineresig = false;
                icon.SetActive(false);
            }
        }

    }


    public void Exdate(byte[] byte_0, byte[] byte_1, byte[] byte_2, byte[] byte_3, byte[] byte_4, byte[] byte_5, byte[] byte_6, byte[] byte_7, byte[] byte_8, byte[] byte_9)
    {
        byte0 = byte_0;
        byte1 = byte_1;
        byte2 = byte_2;
        byte3 = byte_3;
        byte4 = byte_4;
        byte5 = byte_5;
        byte6 = byte_6;
        byte7 = byte_7;
        byte8 = byte_8;
        byte9 = byte_9;
        send();
        Debug.Log("end���");
        byte1 = ToByte(0);
        byte2 = ToByte(0);
        byte3 = ToByte(0);
        byte4 = ToByte(0);
        byte5 = ToByte(0);
        byte6 = ToByte(0);
        byte7 = ToByte(0);
        byte8 = ToByte(0);
        byte9 = ToByte(0);
    }

    public void NteTest()
    {
        btn5 = ToByte(8);
        startTimeStamp = Time.time;
        send();
        nettest = true;
        btn5 = ToByte(0);
    }
    //���°�ť���͵�ǰ�궨��λ��
    public void Guideindexset()
    {
        isHande = RandomPosition.GetComponent<RandomPosition>().guideindex();
        if (isHande)
        {
            Guideindex = ToByte(0);
            Debug.Log("Guideindex���ֳ�" + ToFloat(Guideindex));
        }
        else
        {
            Guideindex = ToByte(1);
            Debug.Log("Guideindex�ǻ���" + ToFloat(Guideindex));
        }

    }

    public void recordguideposition()
    {
        GameObject newBall1 = Instantiate(testBallPrefab,guide.transform.position, guide.transform.rotation);
        GameObject newBall2 = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);
        newBall2.transform.parent = newBall1.transform;
        newBall1.SetActive(true); newBall2.SetActive(true);
        //// ���С���������ݵ��б���
        ballPositions_1.Add(newBall1.transform);
        ballPositions_2.Add(newBall2.transform);
    }
    public void matposition()
    {
        for (int i = 1; i < ballPositions_1.Count; i++)
        {
            ballPositions_1[i].parent = ballPositions_1[0];
            ballPositions_1[i].localPosition = new Vector3(0, 0, 0);
            ballPositions_1[i].localEulerAngles = new Vector3(0, 0, 0);
            ballPositions_1[i].parent = ballPositions_1[0].parent;
        }
        float averageposition_1_x = ballPositions_2[0].position.x + ballPositions_2[1].position.x + ballPositions_2[2].position.x + ballPositions_2[3].position.x;
        float averageposition_1_y = ballPositions_2[0].position.y + ballPositions_2[1].position.y + ballPositions_2[2].position.y + ballPositions_2[3].position.y;
        float averageposition_1_z = ballPositions_2[0].position.z + ballPositions_2[1].position.z + ballPositions_2[2].position.z + ballPositions_2[3].position.z;
        GameObject Ball1 = Instantiate(testBallPrefab, new Vector3(averageposition_1_x / 4, averageposition_1_y / 4, averageposition_1_z / 4), Quaternion.identity);
        float averageposition_2_x = ballPositions_2[4].position.x + ballPositions_2[5].position.x + ballPositions_2[6].position.x + ballPositions_2[7].position.x;
        float averageposition_2_y = ballPositions_2[4].position.y + ballPositions_2[5].position.y + ballPositions_2[6].position.y + ballPositions_2[7].position.y;
        float averageposition_2_z = ballPositions_2[4].position.z + ballPositions_2[5].position.z + ballPositions_2[6].position.z + ballPositions_2[7].position.z;
        GameObject Ball2 = Instantiate(testBallPrefab, new Vector3(averageposition_2_x / 4, averageposition_2_y / 4, averageposition_2_z / 4), Quaternion.identity);
        Ball1.SetActive(true); Ball2.SetActive(true);
        Vector3 driection = Ball2.transform.position - Ball1.transform.position;
        Ball1.transform.up = driection; Ball2.transform.up = driection;
    }


    public void Sendnaire(byte[] byte_1, byte[] byte_2, byte[] byte_3, byte[] byte_4, byte[] byte_5, byte[] byte_6, byte[] byte_7)
    {
        numbers = buttonselect.GetComponent<buttonselect>().getnumbers();
        //numbers[0] = 1; numbers[1] = 1; numbers[2] = 1; numbers[3] = 1; numbers[4] = 1;
        if (ContainsZero(numbers))//�ж��Ƿ���0//��0�Ļ�
        {
            StartCoroutine(DisappearAfterDelay(1f, Submittips));
        }
        else //û��0�ͷ�������
        {
            SubmitButton.SetActive(false);
            byte0 = ToByte(4);
            byte1 = byte_1;
            byte2 = byte_2;
            byte3 = byte_3;
            byte4 = byte_4;
            float q1 = numbers[0]; float q2 = numbers[1]; float q3 = numbers[2]; float q4 = numbers[3]; float q5 = numbers[4];
            float combinedNumber = q1 * 10000f + q2 * 1000f + q3 * 100f + q4 * 10f + q5 * 1f;
            byte5 = byte_5;
            byte6 = byte_6;
            byte7 = byte_7;
            byte8 = ToByte(0);
            byte9 = ToByte(combinedNumber);
            Debug.Log("combinedNumberΪ"+combinedNumber);

            send();
            for (int i = 0; i < numbers.Length; i++) //���ͺ������Ҷ�����
            {
                numbers[i] = 0;
            }
            buttonselect Buttonselectscript = FindObjectOfType<buttonselect>();
            // ����ScriptA�еķ���
            Buttonselectscript.checkboxreset0();
            QusitionNaire.SetActive(false);
            Plane3.SetActive(true);
            RandomPosition randomscript = FindObjectOfType<RandomPosition>();
            randomscript.submit();
            byte1 = ToByte(0);
            byte2 = ToByte(0);
            byte3 = ToByte(0);
            byte4 = ToByte(0);
            byte5 = ToByte(0);
            byte6 = ToByte(0);
            byte7 = ToByte(0);
            byte8 = ToByte(0);
            byte9 = ToByte(0);
        }
    }
    private IEnumerator DisappearAfterDelay(float time,GameObject hideobject)
    {
        hideobject.SetActive(true);
        yield return new WaitForSeconds(time);
        hideobject.SetActive(false);
        Debug.Log("ִ�����ӳ�");
    }
    public void send()
    {                
            try
            {
            Array.Copy(byte0, 0, sendbuffer, 0, 4);
            Array.Copy(byte1, 0, sendbuffer, 4, 4);
            Array.Copy(byte2, 0, sendbuffer, 8, 4);
            Array.Copy(byte3, 0, sendbuffer, 12, 4);
            Array.Copy(byte4, 0, sendbuffer, 16, 4);
            Array.Copy(byte5, 0, sendbuffer, 20, 4);
            Array.Copy(byte6, 0, sendbuffer, 24, 4);   
            Array.Copy(byte7, 0, sendbuffer, 28, 4);
            Array.Copy(byte8, 0, sendbuffer, 32, 4);
            Array.Copy(byte9, 0, sendbuffer, 36, 4);

            socket_client.Send(sendbuffer);
        }
            catch (System.Exception)
            {
                Debug.Log("�����쳣");
            }        
    }


    //��Ӧ�ó��򼴽��ر�ʱ����
    private void OnApplicationQuit()
    {
        Debug.Log("Ӧ�ó��򼴽��ر�");
        byte0 = ToByte(5);
        send();
        if (udpClient != null)
        {
            udpClient.Close();
        }
    }
    public static float ToFloat(byte[] data)
    {
        float a = 0;
        byte i;
        byte[] x = data;
        unsafe
        {
            void* pf;
            fixed (byte* px = x)
            {
                pf = &a;
                for (i = 0; i < data.Length; i++)
                {
                    *((byte*)pf + i) = *(px + i);
                }
            }
        }
        return a;
    }

    public static byte[] ToByte(float data)
    {
        unsafe
        {
            byte* pdata = (byte*)&data;
            byte[] byteArray = new byte[sizeof(float)];
            for (int i = 0; i < sizeof(float); ++i)
                byteArray[i] = *pdata++;
            return byteArray;
        }
    }

    //�ж��Ƿ���0����
    static bool ContainsZero(int[] array)
    {
        foreach (int number in array)
        {
            if (number == 0)
            {
                return true;
            }
        }
        return false;
    }

}