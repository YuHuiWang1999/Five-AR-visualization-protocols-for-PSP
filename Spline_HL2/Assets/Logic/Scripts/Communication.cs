using UnityEngine;
using UnityEngine.UI;
using System;
using System.Net;
using System.Net.Sockets;//引入socket命名空间
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
    private bool isRunning = true; // 添加一个标志来表示是否继续运行接收循环
    public GameObject RandomPosition;
    public GameObject Buttonenbled;
    public GameObject cailposition;
    public GameObject buttonselect;
    public Image[] resigermages;             //采取了几个配准点
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
    private bool iscail = false; // 添加一个标志来表示是否继续运行接收循环
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
    private byte[] btn1;//控制连接后数据传输
    private byte[] flag2;//控制C++中程序不要一直刷新标定的位置
    private byte[] btn3;//向C++发送标定指令
    private byte[] btn4;//向C++发送采点指令
    private byte[] btn5;//向C++发送配准指令


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


    //新加
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
        return ColorChange;//颜色改变
    }
    public float[] getNavigationPos()
    {
        return NavigationPosOfBone;//脊骨
    }
    public float[] getNavigationProbe()
    {
        return NavigationPosOfProbe;//盒子
    }
    public float[] getNavigationRealtimeHand()
    {
        return NavigationPosOfRealtimeHand8700339;//引导器
    }
    public float[] getNavigationRealtimeDrill()
    {
        return NavigationPosOfDrill8700449;//骨钻
    }
    public float[] getCailPosition()
    {
        return NavigationPositionofCube072;//标定
    }
    public bool getisRegistration()
    {
        return isRegistration;//是否已标定好
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
        //Lineobj.AddComponent<LineRenderer>();//给这个物体添加LineRenderer组件
        //Lineobj.GetComponent<LineRenderer>().widthMultiplier = 0.0005f;
        //Lineobj.GetComponent<LineRenderer>().positionCount = 2;

        Submittips.SetActive(false);
        numbers = new int[5];
        //新加
        ManSpline.SetActive(false); ShowPlane0.SetActive(false);
        //相当于一直监听
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
        // 累计时间间隔
        timeSinceLastPrint += Time.deltaTime;
        //新加 //显示一下开始连接界面
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
            Debug.Log("开始上传");
            startrotateimage();
        }
        if (isfineresig)
        {
            Debug.Log("开始配准");
            startrotateimage();
        }

        if (startRecord) 
        {

            GameObject newBall2 = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);
            newBall2.transform.parent = ballparent.transform;
        }


        // 计算每秒接收数据的次数
        if (timeSinceLastPrint >= 1.0f)
        {
            float receiveRate = receivedDataCount / timeSinceLastPrint;
            //Debug.Log("接收频率（每秒接收次数）：" + receiveRate);

            // 重置计数器和时间间隔
            receivedDataCount = 0;
            timeSinceLastPrint = 0f;
        }

        if (ColorChange[0] == 8 && nettest)
        {
            endTimeStamp = Time.time;
            float elapsedTime = (endTimeStamp - startTimeStamp )* 1000f;
            nettestnumber++;
            Debug.Log("程序开始时间戳: " + startTimeStamp+"程序结束时间戳: " + endTimeStamp+"第" + nettestnumber + "次点击的"+"时间差为: " + elapsedTime + " ms");
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
                Debug.Log("连接成功");
                // 创建线程，执行读取服务器消息
                // 发送数据
                byte1 = ToByte(1);
                send();
                stateimage.color = Color.green;
                isReceived = true;
                Debug.Log("isConnected2222222222" + isConnected);
            }
            catch (SocketException ex)
            {
                Debug.Log("Socket异常: " + ex.Message);
                stateimage.color = Color.red;
            }
            catch (FormatException ex)
            {
                Debug.Log("IP地址或端口号格式错误: " + ex.Message);
                stateimage.color = Color.red;
            }
            catch (System.Exception)
            {
                stateimage.color = Color.red;
                Debug.Log("IP端口号错误或者服务器未开启");
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
        //    Debug.Log("关闭客户端连接");
        //}
        //catch (System.Exception)
        //{
        //    Debug.Log("未连接");
        //}
        for (int i = 0; i < 5; i++)
        {
            ColorChange[i] = 0;
        }
        stateimage.color = Color.red;
        isReceived = false;
        udpClient.Close();
    }


    // 在 c_thread 中设置定时器并执行读取服务器消息的方法
    private void SetupTimerAndReceived()
    {
        // 创建一个 Timer 对象，第一个参数是回调方法，第二个参数是状态对象（可以为 null），第三个参数是延迟启动的时间，第四个参数是间隔时间
        //timer = new System.Timers.Timer(5); // 1ms
        //timer.Elapsed += TimerElapsed;
        //timer.AutoReset = true; // 设置为 true 以启用定时器重复触发
        //timer.Start();

        // 读取服务器消息的逻辑
        Received();
    }

    private void ReceiveCallback(IAsyncResult ar)
    {
        if (isReceived)
        {
            try
            {
                byte[] allbuffer = new byte[1024];//收来的所有数
                byte[] buffer = new byte[40 * 4];//收来的所有数
                                                 // 从UDP客户端接收数据
                buffer = udpClient.EndReceive(ar, ref endPoint);

                 // 处理接收到的数据，这里可以根据实际需求进行处理
                //string receivedData = System.Text.Encoding.UTF8.GetString(buffer);
                //Debug.Log("Received UDP data: " + receivedData);

                string timestamp0 = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
                //Debug.Log("开始时间为" + timestamp0);
                Debug.Log("接收数据正常");



                //int len = socket_client.Receive(allbuffer);
                //int counts = (len / 160);
                //if (len >= 160)
                //{
                //    Array.Copy(allbuffer, len - 160, buffer, 0, 160);
                //}
                //Debug.Log("len长度为"+len+ "counts为" + counts);

                byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4];
                byte[] byte5 = new byte[4]; byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4];
                //收到计算后的Cube架子760位置
                byte[] byte8 = new byte[4]; byte[] byte9 = new byte[4]; byte[] byte10 = new byte[4]; byte[] byte11 = new byte[4];
                byte[] byte12 = new byte[4]; byte[] byte13 = new byte[4]; byte[] byte14 = new byte[4];
                //收到计算后的Guide架子339
                byte[] byte15 = new byte[4]; byte[] byte16 = new byte[4]; byte[] byte17 = new byte[4]; byte[] byte18 = new byte[4];
                byte[] byte19 = new byte[4]; byte[] byte20 = new byte[4]; byte[] byte21 = new byte[4];
                //收到计算后的Drill架子449
                byte[] byte22 = new byte[4]; byte[] byte23 = new byte[4]; byte[] byte24 = new byte[4]; byte[] byte25 = new byte[4];
                byte[] byte26 = new byte[4]; byte[] byte27 = new byte[4]; byte[] byte28 = new byte[4];
                //收到标定数据
                byte[] byte29 = new byte[4]; byte[] byte30 = new byte[4]; byte[] byte31 = new byte[4]; byte[] byte32 = new byte[4];
                byte[] byte33 = new byte[4]; byte[] byte34 = new byte[4]; byte[] byte35 = new byte[4];
                //颜色变化
                byte[] byte36 = new byte[4]; byte[] byte37 = new byte[4]; byte[] byte38 = new byte[4]; byte[] byte39 = new byte[4]; byte[] byte40 = new byte[4];

                //脊柱位置采点/盒子位置/取决于是否配准成功
                Array.Copy(buffer, 0, byte1, 0, 4);
                Array.Copy(buffer, 4, byte2, 0, 4);
                Array.Copy(buffer, 8, byte3, 0, 4);
                Array.Copy(buffer, 12, byte4, 0, 4);
                Array.Copy(buffer, 16, byte5, 0, 4);
                Array.Copy(buffer, 20, byte6, 0, 4);
                Array.Copy(buffer, 24, byte7, 0, 4);
                //Probe架子-Probe
                Array.Copy(buffer, 28, byte8, 0, 4);
                Array.Copy(buffer, 32, byte9, 0, 4);
                Array.Copy(buffer, 36, byte10, 0, 4);
                Array.Copy(buffer, 40, byte11, 0, 4);
                Array.Copy(buffer, 44, byte12, 0, 4);
                Array.Copy(buffer, 48, byte13, 0, 4);
                Array.Copy(buffer, 52, byte14, 0, 4);
                //Guide架子-339
                Array.Copy(buffer, 56, byte15, 0, 4);
                Array.Copy(buffer, 60, byte16, 0, 4);
                Array.Copy(buffer, 64, byte17, 0, 4);
                Array.Copy(buffer, 68, byte18, 0, 4);
                Array.Copy(buffer, 72, byte19, 0, 4);
                Array.Copy(buffer, 76, byte20, 0, 4);
                Array.Copy(buffer, 80, byte21, 0, 4);
                //CDrill架子-449
                Array.Copy(buffer, 84, byte22, 0, 4);
                Array.Copy(buffer, 88, byte23, 0, 4);
                Array.Copy(buffer, 92, byte24, 0, 4);
                Array.Copy(buffer, 96, byte25, 0, 4);
                Array.Copy(buffer, 100, byte26, 0, 4);
                Array.Copy(buffer, 104, byte27, 0, 4);
                Array.Copy(buffer, 108, byte28, 0, 4);
                //标定小球
                Array.Copy(buffer, 112, byte29, 0, 4);
                Array.Copy(buffer, 116, byte30, 0, 4);
                Array.Copy(buffer, 120, byte31, 0, 4);
                Array.Copy(buffer, 124, byte32, 0, 4);
                Array.Copy(buffer, 128, byte33, 0, 4);
                Array.Copy(buffer, 132, byte34, 0, 4);
                Array.Copy(buffer, 136, byte35, 0, 4);
                //颜色
                Array.Copy(buffer, 140, byte36, 0, 4);
                Array.Copy(buffer, 144, byte37, 0, 4);
                Array.Copy(buffer, 148, byte38, 0, 4);
                Array.Copy(buffer, 152, byte39, 0, 4);
                Array.Copy(buffer, 156, byte40, 0, 4);

                //探针-probe---0-7(0)
                NavigationPosOfProbe[0] = ToFloat(byte1);
                NavigationPosOfProbe[1] = ToFloat(byte2);
                NavigationPosOfProbe[2] = ToFloat(byte3);
                NavigationPosOfProbe[3] = ToFloat(byte4);
                NavigationPosOfProbe[4] = ToFloat(byte5);
                NavigationPosOfProbe[5] = ToFloat(byte6);
                NavigationPosOfProbe[6] = ToFloat(byte7);

                //标定块8-14(1)
                NavigationPositionofCube072[0] = ToFloat(byte8);
                NavigationPositionofCube072[1] = ToFloat(byte9);
                NavigationPositionofCube072[2] = ToFloat(byte10);
                NavigationPositionofCube072[3] = ToFloat(byte11);
                NavigationPositionofCube072[4] = ToFloat(byte12);
                NavigationPositionofCube072[5] = ToFloat(byte13);
                NavigationPositionofCube072[6] = ToFloat(byte14);
                //脊骨架子-760(2)
                NavigationPosOfBone[0] = ToFloat(byte15);
                NavigationPosOfBone[1] = ToFloat(byte16);
                NavigationPosOfBone[2] = ToFloat(byte17);
                NavigationPosOfBone[3] = ToFloat(byte18);
                NavigationPosOfBone[4] = ToFloat(byte19);
                NavigationPosOfBone[5] = ToFloat(byte20);
                NavigationPosOfBone[6] = ToFloat(byte21);
                //Guide架子-339(3)
                NavigationPosOfRealtimeHand8700339[0] = ToFloat(byte22);
                NavigationPosOfRealtimeHand8700339[1] = ToFloat(byte23);
                NavigationPosOfRealtimeHand8700339[2] = ToFloat(byte24);
                NavigationPosOfRealtimeHand8700339[3] = ToFloat(byte25);
                NavigationPosOfRealtimeHand8700339[4] = ToFloat(byte26);
                NavigationPosOfRealtimeHand8700339[5] = ToFloat(byte27);
                NavigationPosOfRealtimeHand8700339[6] = ToFloat(byte28);
                //Drill架子 - 760(4)
                NavigationPosOfDrill8700449[0] = ToFloat(byte29);
                NavigationPosOfDrill8700449[1] = ToFloat(byte30);
                NavigationPosOfDrill8700449[2] = ToFloat(byte31);
                NavigationPosOfDrill8700449[3] = ToFloat(byte32);
                NavigationPosOfDrill8700449[4] = ToFloat(byte33);
                NavigationPosOfDrill8700449[5] = ToFloat(byte34);
                NavigationPosOfDrill8700449[6] = ToFloat(byte35);
                //色彩指示
                ColorChange[0] = ToFloat(byte36);
                ColorChange[1] = ToFloat(byte37);
                ColorChange[2] = ToFloat(byte38);
                ColorChange[3] = ToFloat(byte39);
                Debug.Log("颜色为" + NavigationPositionofCube072[0] + "颜色为" + NavigationPositionofCube072[1] + "颜色为" + NavigationPositionofCube072[2] + "颜色为" + NavigationPositionofCube072[3]);
                ColorChange[4] = ToFloat(byte40);
                // 记录接收数据的次数
                receivedDataCount++;
                receivednumber++;
                string timestamp = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
                //Debug.Log("第 " + receivednumber + "次发送时间为" + timestamp /*+ "bufferlen" + len*/);
                if (ColorChange[0]==10) {  issplineshow = true; }

                // 继续异步接收下一条数据
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
        //        //Debug.Log("开始时间为" + timestamp0);
        //        //Debug.Log("接收数据正常");

        //        byte[] allbuffer = new byte[1024];//收来的所有数
        //        byte[] buffer = new byte[40 * 4];//收来的所有数
                   

        //        int len = socket_client.Receive(allbuffer);
        //        int counts = (len / 160);
        //        if (len >= 160)
        //        {
        //            Array.Copy(allbuffer, len - 160, buffer, 0, 160);
        //        }
        //        //Debug.Log("len长度为"+len+ "counts为" + counts);

        //        byte[] byte1 = new byte[4]; byte[] byte2 = new byte[4]; byte[] byte3 = new byte[4]; byte[] byte4 = new byte[4];
        //        byte[] byte5 = new byte[4]; byte[] byte6 = new byte[4]; byte[] byte7 = new byte[4];
        //        //收到计算后的Cube架子760位置
        //        byte[] byte8 = new byte[4]; byte[] byte9 = new byte[4]; byte[] byte10 = new byte[4]; byte[] byte11 = new byte[4];
        //        byte[] byte12 = new byte[4]; byte[] byte13 = new byte[4]; byte[] byte14 = new byte[4];
        //        //收到计算后的Guide架子339
        //        byte[] byte15 = new byte[4]; byte[] byte16 = new byte[4]; byte[] byte17 = new byte[4]; byte[] byte18 = new byte[4];
        //        byte[] byte19 = new byte[4]; byte[] byte20 = new byte[4]; byte[] byte21 = new byte[4];
        //        //收到计算后的Drill架子449
        //        byte[] byte22 = new byte[4]; byte[] byte23 = new byte[4]; byte[] byte24 = new byte[4]; byte[] byte25 = new byte[4];
        //        byte[] byte26 = new byte[4]; byte[] byte27 = new byte[4]; byte[] byte28 = new byte[4];
        //        //收到标定数据
        //        byte[] byte29 = new byte[4]; byte[] byte30 = new byte[4]; byte[] byte31 = new byte[4]; byte[] byte32 = new byte[4];
        //        byte[] byte33 = new byte[4]; byte[] byte34 = new byte[4]; byte[] byte35 = new byte[4];
        //        //颜色变化
        //        byte[] byte36 = new byte[4]; byte[] byte37 = new byte[4]; byte[] byte38 = new byte[4]; byte[] byte39 = new byte[4]; byte[] byte40 = new byte[4];


        //        //脊柱位置采点/盒子位置/取决于是否配准成功
        //        Array.Copy(buffer, 0, byte1, 0, 4);
        //        Array.Copy(buffer, 4, byte2, 0, 4);
        //        Array.Copy(buffer, 8, byte3, 0, 4);
        //        Array.Copy(buffer, 12, byte4, 0, 4);
        //        Array.Copy(buffer, 16, byte5, 0, 4);
        //        Array.Copy(buffer, 20, byte6, 0, 4);
        //        Array.Copy(buffer, 24, byte7, 0, 4);
        //        //Probe架子-Probe
        //        Array.Copy(buffer, 28, byte8, 0, 4);
        //        Array.Copy(buffer, 32, byte9, 0, 4);
        //        Array.Copy(buffer, 36, byte10, 0, 4);
        //        Array.Copy(buffer, 40, byte11, 0, 4);
        //        Array.Copy(buffer, 44, byte12, 0, 4);
        //        Array.Copy(buffer, 48, byte13, 0, 4);
        //        Array.Copy(buffer, 52, byte14, 0, 4);
        //        //Guide架子-339
        //        Array.Copy(buffer, 56, byte15, 0, 4);
        //        Array.Copy(buffer, 60, byte16, 0, 4);
        //        Array.Copy(buffer, 64, byte17, 0, 4);
        //        Array.Copy(buffer, 68, byte18, 0, 4);
        //        Array.Copy(buffer, 72, byte19, 0, 4);
        //        Array.Copy(buffer, 76, byte20, 0, 4);
        //        Array.Copy(buffer, 80, byte21, 0, 4);
        //        //CDrill架子-449
        //        Array.Copy(buffer, 84, byte22, 0, 4);
        //        Array.Copy(buffer, 88, byte23, 0, 4);
        //        Array.Copy(buffer, 92, byte24, 0, 4);
        //        Array.Copy(buffer, 96, byte25, 0, 4);
        //        Array.Copy(buffer, 100, byte26, 0, 4);
        //        Array.Copy(buffer, 104, byte27, 0, 4);
        //        Array.Copy(buffer, 108, byte28, 0, 4);
        //        //标定小球
        //        Array.Copy(buffer, 112, byte29, 0, 4);
        //        Array.Copy(buffer, 116, byte30, 0, 4);
        //        Array.Copy(buffer, 120, byte31, 0, 4);
        //        Array.Copy(buffer, 124, byte32, 0, 4);
        //        Array.Copy(buffer, 128, byte33, 0, 4);
        //        Array.Copy(buffer, 132, byte34, 0, 4);
        //        Array.Copy(buffer, 136, byte35, 0, 4);
        //        //颜色
        //        Array.Copy(buffer, 140, byte36, 0, 4);
        //        Array.Copy(buffer, 144, byte37, 0, 4);
        //        Array.Copy(buffer, 148, byte38, 0, 4);
        //        Array.Copy(buffer, 152, byte39, 0, 4);
        //        Array.Copy(buffer, 156, byte40, 0, 4);

        //        //脊骨
        //        NavigationPosOfBone[0] = ToFloat(byte1);
        //        NavigationPosOfBone[1] = ToFloat(byte2);
        //        NavigationPosOfBone[2] = ToFloat(byte3);
        //        NavigationPosOfBone[3] = ToFloat(byte4);
        //        NavigationPosOfBone[4] = ToFloat(byte5);
        //        NavigationPosOfBone[5] = ToFloat(byte6);
        //        NavigationPosOfBone[6] = ToFloat(byte7);
    
        //        //Cybe架子-760
        //        NavigationPosOfProbe[0] = ToFloat(byte8);
        //        NavigationPosOfProbe[1] = ToFloat(byte9);
        //        NavigationPosOfProbe[2] = ToFloat(byte10);
        //        NavigationPosOfProbe[3] = ToFloat(byte11);
        //        NavigationPosOfProbe[4] = ToFloat(byte12);
        //        NavigationPosOfProbe[5] = ToFloat(byte13);
        //        NavigationPosOfProbe[6] = ToFloat(byte14);
        //        //Guide架子-339
        //        NavigationPosOfRealtimeHand8700339[0] = ToFloat(byte15);
        //        NavigationPosOfRealtimeHand8700339[1] = ToFloat(byte16);
        //        NavigationPosOfRealtimeHand8700339[2] = ToFloat(byte17);
        //        NavigationPosOfRealtimeHand8700339[3] = ToFloat(byte18);
        //        NavigationPosOfRealtimeHand8700339[4] = ToFloat(byte19);
        //        NavigationPosOfRealtimeHand8700339[5] = ToFloat(byte20);
        //        NavigationPosOfRealtimeHand8700339[6] = ToFloat(byte21);
        //        //Drill架子 - 449
        //        NavigationPosOfDrill8700449[0] = ToFloat(byte22);
        //        NavigationPosOfDrill8700449[1] = ToFloat(byte23);
        //        NavigationPosOfDrill8700449[2] = ToFloat(byte24);
        //        NavigationPosOfDrill8700449[3] = ToFloat(byte25);
        //        NavigationPosOfDrill8700449[4] = ToFloat(byte26);
        //        NavigationPosOfDrill8700449[5] = ToFloat(byte27);
        //        NavigationPosOfDrill8700449[6] = ToFloat(byte28);
        //        //标定块
        //        NavigationPositionofCube072[0] = ToFloat(byte29);
        //        NavigationPositionofCube072[1] = ToFloat(byte30);
        //        NavigationPositionofCube072[2] = ToFloat(byte31);
        //        NavigationPositionofCube072[3] = ToFloat(byte32);
        //        NavigationPositionofCube072[4] = ToFloat(byte33);
        //        NavigationPositionofCube072[5] = ToFloat(byte34);
        //        NavigationPositionofCube072[6] = ToFloat(byte35);
        //        //色彩指示
        //        ColorChange[0] = ToFloat(byte36);
        //        ColorChange[1] = ToFloat(byte37);
        //        ColorChange[2] = ToFloat(byte38);
        //        ColorChange[3] = ToFloat(byte39);
        //        //Debug.Log("颜色为"+ ColorChange[3]);
        //        ColorChange[4] = ToFloat(byte40);
        //        // 记录接收数据的次数
        //        receivedDataCount++;
        //        receivednumber++;
        //        string timestamp = System.DateTime.Now.ToString("yyyy.MM.dd.HH.mm.ss.fff");
        //        //Debug.Log("第 " + receivednumber + "次发送时间为"+ timestamp+ "bufferlen" + len);

        //    }
        //    catch (System.Exception)
        //    {
        //        Debug.Log("接收异常");
        //    }
        //}
    }
    public void recordposition()
    {
        byte0 = ToByte(0);//阶段0
        byte2 = ToByte(1);//记录位置-1平常-0cailposition
        //记录位姿
        byte1 = ToByte(cailposition.transform.position.x);
        byte4 = ToByte(cailposition.transform.position.y);//点1位置信息x
        byte5 = ToByte(cailposition.transform.position.z);//点1位置信息y
        byte6 = ToByte(cailposition.transform.rotation.w);//点1位置信息z
        byte7 = ToByte(cailposition.transform.rotation.x);//点2位置信息x
        byte8 = ToByte(cailposition.transform.rotation.y);//点2位置信息y
        byte9 = ToByte(cailposition.transform.rotation.z);//点2位置信息z

        //记录点
        //byte4 = ToByte(pointOne.transform.position.x);//点1位置信息x
        //byte5 = ToByte(pointOne.transform.position.y);//点1位置信息y
        //byte6 = ToByte(pointOne.transform.position.z);//点1位置信息z
        //byte7 = ToByte(pointTwo.transform.position.x);//点2位置信息x
        //byte8 = ToByte(pointTwo.transform.position.y);//点2位置信息y
        //byte9 = ToByte(pointTwo.transform.position.z);//点2位置信息z
        //Debug.Log("one点位置" + pointOne.transform.position + "two点位置" + pointTwo.transform.position);
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

        //flag2 = ToByte(1); //C++中刷新一下位置
        send();

        byte2 = ToByte(0);//记录位置-1平常-0
        byte4 = ToByte(0);//点1位置信息x平常-0
        byte5 = ToByte(0);//点1位置信息y平常-0
        byte6 = ToByte(0);//点1位置信息z平常-0
        byte7 = ToByte(0);//点2位置信息x平常-0
        byte8 = ToByte(0);//点2位置信息y平常-0
        byte9 = ToByte(0);//点2位置信息z平常-0
        //flag2 = ToByte(0);//刷新完就关掉不持续刷新
    }

    //标定按钮——阶段0
    public void calibration()
    {
        int CalCount = Buttonenbled.GetComponent<Buttonenbled>().getCalCount();
        if (CalCount < 1)//判断是否有0//有0的话
        {
            StartCoroutine(DisappearAfterDelay(1f, Cailtips));
        }
        else 
        {
            btn3 = ToByte(1);
            byte0 = ToByte(0);//阶段0
            byte3 = ToByte(1);//标定-1重标-2平常-0
            //xyz.SetActive(true);
            //xyz.transform.position = cliaobj.transform.position;
            //xyz.transform.rotation = cliaobj.transform.rotation;
            iscail = true;
            //
            send();

            byte3 = ToByte(0);//标定-1重标-2平常-0

            btn3 = ToByte(0);
        }

    }
    //清除标定点——阶段0
    public void caliClear() 
    {
        byte0 = ToByte(0);//阶段0
        byte3 = ToByte(2);//标定-1重标-2平常-0
        send();
        byte3 = ToByte(0);//标定-1重标-2平常-0

    }
    //采取配准点——阶段1
    public void Acquiring()
    {
        //byte0 = ToByte(1);//阶段1
        //byte1 = ToByte(1);//采集-1平常-0
        //send();
        //byte1 = ToByte(0);//采集-1平常-0
        GameObject newBall = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);

    }
    //脊骨配准——阶段1
    public void startRegistration()
    {
        startRecord = true;
        ballparent.SetActive(true);
        //Regtips.SetActive(false);
        //int AcqCount = Buttonenbled.GetComponent<Buttonenbled>().getAcqCount();
        //if (AcqCount<8)//判断是否有0//有0的话
        //{
        //    StartCoroutine(DisappearAfterDelay(1f, Regtips));
        //}
        //else 
        //{
        //    isRegistration = true;
        //    byte0 = ToByte(1);//阶段1
        //    byte2 = ToByte(1);//配准-1重配-2平常-0
        //    send();
        //    byte2 = ToByte(0);//配准-1重配-2平常-0
        //}

    }
    public void StopRegistration()
    {
        startRecord = false;
    }
    //清除配准点——阶段1
    public void AcqClear()
    {
        //byte0 = ToByte(1);//阶段1
        //byte2 = ToByte(2);//配准-1重配-2平常-0
        //send();
        //byte2 = ToByte(0);//配准-1重配-2平常-0
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
        // 当经过时间小于指定时间转
        if (Time.time - startTime < 10f)
        {
            // 每帧旋转物体
            Debug.Log("开始旋转了");
            roteimage.transform.localEulerAngles = new Vector3(0, 0, 800f * (Time.time - startTime));
        }
        else
        {
            Debug.Log("结束旋转了");
            roteimage.transform.localEulerAngles = new Vector3(0, 0, 0);
            // 旋转结束后，你可以在这里添加停止旋转后的操作
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
        Debug.Log("end输出");
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
    //按下按钮后发送当前标定块位置
    public void Guideindexset()
    {
        isHande = RandomPosition.GetComponent<RandomPosition>().guideindex();
        if (isHande)
        {
            Guideindex = ToByte(0);
            Debug.Log("Guideindex是手持" + ToFloat(Guideindex));
        }
        else
        {
            Guideindex = ToByte(1);
            Debug.Log("Guideindex是机器" + ToFloat(Guideindex));
        }

    }

    public void recordguideposition()
    {
        GameObject newBall1 = Instantiate(testBallPrefab,guide.transform.position, guide.transform.rotation);
        GameObject newBall2 = Instantiate(testBallPrefab, probe.transform.position, probe.transform.rotation);
        newBall2.transform.parent = newBall1.transform;
        newBall1.SetActive(true); newBall2.SetActive(true);
        //// 添加小球坐标数据到列表中
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
        if (ContainsZero(numbers))//判断是否有0//有0的话
        {
            StartCoroutine(DisappearAfterDelay(1f, Submittips));
        }
        else //没有0就发送数据
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
            Debug.Log("combinedNumber为"+combinedNumber);

            send();
            for (int i = 0; i < numbers.Length; i++) //发送后隐藏且都归零
            {
                numbers[i] = 0;
            }
            buttonselect Buttonselectscript = FindObjectOfType<buttonselect>();
            // 调用ScriptA中的方法
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
        Debug.Log("执行了延迟");
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
                Debug.Log("发送异常");
            }        
    }


    //在应用程序即将关闭时触发
    private void OnApplicationQuit()
    {
        Debug.Log("应用程序即将关闭");
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

    //判断是否有0的数
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