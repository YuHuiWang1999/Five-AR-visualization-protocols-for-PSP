#include "AR.h"

//#include <time.h>

using namespace std;
AR::AR(QWidget *parent)
	: QMainWindow(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
	ui.setupUi(this);
	ui.close_listen->setDisabled(true);
	calibration = new Calibration();
	connect(ui.btnClose, &QPushButton::clicked, this, &AR::onButtonCloseClicked);
	connect(ui.btn_next, &QPushButton::clicked, this, &AR::nextpage);
	connect(ui.toolbtn00, &QPushButton::clicked, this, &AR::tool00);
	connect(ui.toolbtn01, &QPushButton::clicked, this, &AR::tool01);
	connect(ui.toolbtn02, &QPushButton::clicked, this, &AR::tool02);
	connect(ui.toolbtn03, &QPushButton::clicked, this, &AR::tool03);
	connect(ui.toolbtn04, &QPushButton::clicked, this, &AR::tool04);
	connect(ui.sex00, &QPushButton::clicked, this, &AR::Sexselect_man);
	connect(ui.sex01, &QPushButton::clicked, this, &AR::Sexselect_women);
	connect(ui.spline_X, &QPushButton::clicked, this, &AR::Splineselect_X);
	connect(ui.spline_L, &QPushButton::clicked, this, &AR::Splineselect_L);
	connect(ui.btn_next_up, &QPushButton::clicked, this, &AR::uploademode);

	connect(ui.send_data, &QPushButton::clicked, this, &AR::send_data_slot);
	connect(ui.close_listen, &QPushButton::clicked, this, &AR::closeServer);
	connect(ui.connect_Ndi, &QPushButton::clicked, calibration, &Calibration::NDI_Connect);
	connect(ui.connect_Ndi, &QPushButton::clicked, this, &AR::Send_startTimer);
	connect(calibration, &Calibration::setlabel, this, &AR::setlabel);
	connect(ui.pbt_record, &QPushButton::clicked, calibration, &Calibration::record);
	connect(this, &AR::cali, calibration, &Calibration::cali);
	connect(ui.transmat, &QPushButton::clicked, calibration, &Calibration::transmat);

	connect(ui.MarkPoint_record, &QPushButton::clicked, this, &AR::coarse_GetPoint);
	connect(ui.Coarse_Register, &QPushButton::clicked, this, &AR::coarse_Resiger);
	connect(ui.PointClouds_Start, &QPushButton::clicked, this, &AR::ICPCouldRecord);
	connect(ui.PointClouds_Stop, &QPushButton::clicked, this, &AR::inputPointClouds);
	connect(ui.ICP_Register, &QPushButton::clicked, this, &AR::PointCloudRegister);

	connect(ui.cali_clear, &QPushButton::clicked, this, &AR::cali_clear);
	connect(ui.cali_clear, &QPushButton::clicked, calibration, &Calibration::caliClear);
	connect(ui.registion, &QPushButton::clicked, calibration, &Calibration::registion);

	connect(ui.test_date_1, &QPushButton::clicked, this, &AR::testdate1);
	connect(ui.test_date_2, &QPushButton::clicked, this, &AR::testdate2);

	connect(ui.patient_load_model, &QPushButton::clicked, this, &AR::loadPatientModel);//�����ֹ�ģ��
	connect(ui.patientWindow, &MyRenderWindow::rightSignal, this, &AR::ShowWinMenu);
	connect(holoTimer, &QTimer::timeout, this, &AR::SendPosition);
	connect(holoTimer, &QTimer::timeout, this, &AR::RenderTool);
	connect(ui.file_recordP, &QPushButton::clicked, this, &AR::testdate1);
	//connect(ui.record_data, &QPushButton::clicked, this, &AR::Open_Write_txt);
	connect(ui.display_data, &QPushButton::clicked, this, &AR::Open_Read_txt);
	
	connect(ui.cail_US, &QPushButton::clicked, this, &AR::cailpage);
	connect(ui.record_position_1, &QPushButton::clicked, this, &AR::record_position_1);
	connect(ui.cali_phantom_1, &QPushButton::clicked, this, &AR::cali_phantom_1);
	connect(ui.record_position_2, &QPushButton::clicked, this, &AR::record_position_2);
	connect(ui.calio_USprobe_2, &QPushButton::clicked, this, &AR::calio_USprobe_2);
	

	//11-12�¼�US
	initpy();
	initpath();
	initcamera();
	connect(ui.patientWindow, &MyRenderWindow::KeyPressSignal, this, &AR::keyPressEvent);
	connect(ui.deviceComboBox, (void (QComboBox::*)(int)) & QComboBox::currentIndexChanged, this, &AR::changeVideoSource);
	connect(ui.seg_Image, &QPushButton::clicked, this, &AR::segmentimage);
	connect(ui.rig_Image, &QPushButton::clicked, this, &AR::inputPointClouds);
	connect(ui.US_Select_Delete, &QPushButton::clicked, this, &AR::ImagDelete);
	connect(ui.record_data_2, &QPushButton::clicked, this, &AR::PickUsData);
	

	//
	connect(ui.patientWindow, &MyRenderWindow::TranslatePressSignal, this, &AR::translatMode);//E��ƽ��
	connect(ui.patientWindow, &MyRenderWindow::RoatePressSignal, this, &AR::roateMode);//W����ת
	connect(ui.patientWindow, &MyRenderWindow::mouseMoveSignal, this, &AR::targetMouseMove);
	connect(ui.patientWindow, &MyRenderWindow::leftMouseDownSignal, this, &AR::PickThisModle);
	connect(AllTimer, &QTimer::timeout, this, &AR::ChangeCamera_3D);
	connect(ui.btnLoadCT_2, &QPushButton::clicked, this, &AR::intPickdata);
	
	connect(L1SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	connect(L2SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	connect(L3SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	connect(L4SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	connect(L5SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	connect(L6SelectAction, &QAction::triggered, this, &AR::SelectSplineL1);
	//



	////����̽ͷ�궨����

	//double USCaliM[16] = { 0.866057,0.496132,-0.061626,101.579536,
	//						-0.070892,-0.000149,-0.997484,3.886276,
	//						-0.494893,0.868247,0.035043,-116.641130,
	//						0,	0,	0,	1 }; /////////////////////////////////ˮ����ʱ궨���
	double USCaliM[16] = { 0.866057,0.496132,-0.061626,101.579536,
						-0.070892,-0.000149,-0.997484,3.886276,
						-0.494893,0.868247,0.035043,-116.641130,
						0,	0,	0,	1 }; /////////////////////////////////ˮ����ʱ궨���

	//double USroate[16] = {  1,0,0,0,
	//						0,-1,0,0,
	//						0,0,-1,0,
	//						0,0,0,1 }; /////////////////////////////////ˮ����ʱ궨���
	//Matrix_rotate->DeepCopy(USroate);
	Matrix_USToTemp->DeepCopy(USCaliM);
	//vtkMatrix4x4::Multiply4x4(Matrix_USToTemp, Matrix_rotate, Matrix_USToTemp);

	double actortoimage[16] = { 0,-1,0,458 * SIZE_PIXEL_x ,
		1,0,0,0,
		0,0,1,0,
		0,0,0,1 };

	Matrix_actorToimage->DeepCopy(actortoimage);

	//double intnumber[16] = 
	//{ 0.627559,0.775678,-0.0670338,185.789,
	//  -0.7776,0.620158,-0.103642,90.1975,
	//  -0.0388215, 0.117167, 0.992353, 149.245,
	//  0,0,0,1 }; 

	//double intnumber[16] =
	//{ 1.75120035e-02,-9.57106120e-03,9.99800842e-01 ,-1.25428100e+02,
	//  -1.66624894e-02,-9.99818111e-01 ,-9.27937481e-03, 2.06318298e+02,
	//  9.99707803e-01, -1.64966705e-02, -1.76682960e-02, 8.10462397e+01,
	//  0,0,0,1 };

	double intnumber[16] =
	{ 0.0125832, -0.0121259, 0.999847  ,-129.857,
	  -0.0148091,-0.999819 , -0.0119392, 203.403,
	 0.999811, -0.0146566  , -0.0127605, 81.3098,
	  0,0,0,1 };

	Matrix_coarse->DeepCopy(intnumber);
	Matrix_show->DeepCopy(intnumber);
	Matrix_boneToFarDRO->DeepCopy(intnumber);
	connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &AR::onImageCaptured);

	//11-12�¼�US

	ui.frame_UsCali->setVisible(false);
	ui.farme_UsSeg->setVisible(false);
	ui.frame_222->setVisible(false);
	ui.frame_333->setVisible(false);
	ui.frame_444->setVisible(false);
	ui.state02->setVisible(false);
	ui.label_00->setVisible(false);
	ui.label_01->setVisible(false);
	ui.label_02->setVisible(false);
	ui.label_03->setVisible(false);
	ui.label_04->setVisible(false);
	ui.btn_next_up->setVisible(false);	


	initPatientPage();
	//startsend = true;//��ʼ����λ��
}

AR::~AR()
{}
void AR::onButtonCloseClicked()
{
	this->close();
}

//�������������м���
void AR::on_set_listen_clicked()
{
	mServer = new QTcpServer();
	connect(mServer, &QTcpServer::newConnection, this, &AR::seclet);
	//unsigned short port = ui.port->text().toUShort();
	mServer->listen(QHostAddress::Any, 8888);
	qDebug() << "serverAddress:" << mServer->serverAddress().toString();

	mUdpSocket = new QUdpSocket(this);
	mUdpSocket->bind(QHostAddress::Any, 6666);
	qDebug() << "�η���ʱ��Ϊ" << timestamp;
	startsend = true; flagDatas[0] = 9;
	SendPosition();
	startsend = false; flagDatas[0] = 0;
}
void AR::uploademode()
{
	startsend = true; flagDatas[0] = 10;
	SendPosition(); startsend = true;
	flagDatas[0] = 0;
}
void AR::seclet()
{
	Socket1 = mServer->nextPendingConnection();
	connect(Socket1, &QTcpSocket::readyRead, this, &AR::recieve1);
	Send_startTimer();//����
	qDebug() << Socket1->peerAddress().toString() << "duankou" << QString::number(Socket1->peerPort());
	startsend = true;//��ʼ����λ��
}
void AR::SendPosition()
{
	if (startsend)
	{
		QUdpSocket udpSocket;
		QHostAddress targetAddress("192.168.137.80"); // �۾�

		//QHostAddress targetAddress("192.168.137.50"); // ����
		quint16 targetPort = 6666; // 
		udpSocket.connectToHost(targetAddress, targetPort);
		QByteArray send_data;
		send_data.resize(160);
		//QString time1 = QDateTime::currentDateTime().toString("HH:mm:ss:zzz");
		unsigned char data[160];
		for (int i = 0; i < 35; i++)
		{
			memcpy(&(data[i * 4]), &(ProTransf[i]), sizeof(ProTransf[i]));
		}

		memcpy(&(data[35 * 4]), &flagDatas, sizeof(flagDatas));
	/*	qDebug() << "jjjjjjjjj" << flagDatas[0] << "jjjjjjjjj" << flagDatas[1] << "jjjjjjjjj" << flagDatas[2] << "jjjjjjjjj" << flagDatas[3] << "jjjjjjjjj" << flagDatas[4];*/
		for (int i = 0; i < 160; i++)
		{
			send_data[i] = data[i];
		}
		if (udpSocket.state() == QAbstractSocket::BoundState || udpSocket.state() == QAbstractSocket::ConnectedState) {
			udpSocket.writeDatagram(send_data, targetAddress, targetPort);
			//qDebug() << "isright";
		}
		else {
			qDebug() << "Socket is not in a valid state for writing datagrams.";
		}
	}
}
void AR::recieve1()
{
	float byte0 = 0;	float byte1 = 0;	float byte2 = 0;	float byte3 = 0;	float byte4 = 0;
	float byte5 = 0;	float byte6 = 0;	float byte7 = 0;	float byte8 = 0;	float byte9 = 0;

	static qint64 length = 0;
	QByteArray rece_data;
	unsigned char B[40] = { 0 };
	rece_data = Socket1->readAll();
	length = rece_data.size();
	qDebug() << rece_data.size();
	for (int i = 0; i < 40; i++)
	{
		B[i] = rece_data[i];
	}
	memcpy(&byte0, &(B[0]), sizeof(byte0));
	memcpy(&byte1, &(B[4]), sizeof(byte1));
	memcpy(&byte2, &(B[8]), sizeof(byte2));
	memcpy(&byte3, &(B[12]), sizeof(byte3));
	memcpy(&byte4, &(B[16]), sizeof(byte4));
	memcpy(&byte5, &(B[20]), sizeof(byte5));
	memcpy(&byte6, &(B[24]), sizeof(byte6));
	memcpy(&byte7, &(B[28]), sizeof(byte7));
	memcpy(&byte8, &(B[32]), sizeof(byte8));
	memcpy(&byte9, &(B[36]), sizeof(byte9));
	qDebug() << "byte0-" << byte0 << "byte1-" << byte1 << "byte2-" << byte2 << "byte3-" << byte3 << "byte4-" << byte4 << "byte5- " << byte5;
	if (byte0 == 0)//����0
	{
		if (byte1 == 1)
		{
			startsend = true;//��ʼ����λ��
		}
		if (byte1 == 2)
		{
			startsend = false;//�Ͽ�
		}
		if (byte2 == 1)
		{
			////����׼
			//mulUnityTrans.push_back(Vector3d(byte4 * 1000, byte5 * 1000, -byte6 * 1000));
			//mulUnityTrans.push_back(Vector3d(byte7 * 1000, byte8 * 1000, -byte9 * 1000));

			//λ����׼
			unityTrans[0] = byte1 * 1000;
			unityTrans[1] = byte4 * 1000;
			unityTrans[2] = byte5 * 1000;
			unityTrans[3] = byte6;
			unityTrans[4] = byte7;
			unityTrans[5] = byte8;
			unityTrans[6] = byte9;


			emit cali();
		}
		if (byte3 == 1)
		{
			calibration->transmat();//�궨			
			iscail = true;
		}
		if (byte3 == 2)
		{
			calibration->caliClear();//����궨��
			iscail = false;
		}

	}//����0

	if (byte0 == 1)//����1-������׼
	{
		if (byte1 == 1)
		{
			coarse_GetPoint();//����׼_�ɵ�
		}
		if (byte1 == 2)
		{
			coarse_Resiger(); //����׼_�ɵ�
		}

		if (byte2 == 1)
		{
			isICPcollection = true;		//��ʼ�ɼ�����	
		}
		if (byte2 == 2)
		{
			isICPcollection = false;	//�����ɼ�����	
		}
		if (byte2 == 3)
		{
			//PointCloudRegister();//������׼
		}
		if (byte2 == 4)
		{
			//PointCloudRegister();//�����׼���
		}
	}//����1

	if (byte0 == 2)//����2
	{
		//// ��¼��ǰʱ���
		//auto now = std::chrono::system_clock::now();
		//std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
		//// ʹ��ostringstream��ʱ���ת��Ϊ�ַ���
		//std::ostringstream oss;
		//oss << std::put_time(std::localtime(&timestamp), "%Y.%m.%d.%H.%M.%S.")
		//	<< std::setw(3) << std::setfill('0') << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
		////��ostringstream�����תΪstd::string
		OutFiledate.open("recorddate.csv", ios::out | ios::app);
		OutFiledate << '\n' << "��������:" << "," << "���ӷ���" << "," << "ֲ��λ��" << "," << "���ƫ��" << "," << "����ƫ��" << "," << "ƫת�Ƕ�" << "," << "��״ƫ��" << "," << "ʸ״ƫת" << "," << "ˮƽƫת" << "," << "����ʱ��" << "," << "�ʾ�1" << "," << "�ʾ�2" << "," << "�ʾ�3" << "," << "�ʾ�4" << "," << "�ʾ�5" << "," << "A1����" << "," << "A0����" << "," << "C1����" << "," << "C0����" << "," << "1���д���" << "," << "0���д���" << "," << "ʱ���" << '\n';
		OutFiledate << byte1 << "," << byte2 << "," << byte3 << ",";
		OutPointdate.open("recordpoint.csv", ios::out | ios::app);
		OutPointdate << "��������" << "," << "Ԥ�����X" << "," << "Ԥ�����Y" << "," << "Ԥ�����Z" << "," << "Ԥ�����X" << "," << "Ԥ�����Y" << "," << "Ԥ�����Z" << "," << "ʵʱ���X" << "," << "ʵʱ���Y" << "," << "ʵʱ���Z" << "," << "ʵʱ����X" << "," << "ʵʱ����Y" << "," << "ʵʱ����Z" << '\n';
		OutPointdate << byte1 << "," << byte4 << "," << byte5 << "," << byte6 << "," << byte7 << "," << byte8 << "," << byte9;
		OutPointdate.close();
		OutFiledate.close();
		int number = byte2;
		if (number % 2 == 1) {
			GuideIndex = 1;
		}
		else {
			GuideIndex = 0;
		}
	}//����2
	if (byte0 == 3)//����3
	{
		OutFiledate.open("recorddate.csv", ios::out | ios::app);
		OutFiledate << byte7 << "," << byte8 << "," << byte9 << ",";


		OutPointdate.open("recordpoint.csv", ios::out | ios::app);
		OutPointdate << "," << byte1 << "," << byte2 << "," << byte3 << "," << byte4 << "," << byte5 << "," << byte6 << '\n';
		OutPointdate.close();
		OutFiledate.close();

	}//����4
	if (byte0 == 4)//����4
	{
		auto now = std::chrono::system_clock::now();
		timestamp = std::chrono::system_clock::to_time_t(now);
		std::ostringstream oss;
		oss << std::put_time(std::localtime(&timestamp), "%Y.%m.%d.%H.%M.%S.")
			<< std::setw(3) << std::setfill('0') << std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
		std::string timestampStr = oss.str();
		OutFiledate.open("recorddate.csv", ios::out | ios::app);
		int Intnaire = static_cast<int>(byte9);
		int IntA = static_cast<int>(byte5);
		int IntC = static_cast<int>(byte6);
		int IntAll = static_cast<int>(byte7);
		// ��ȡ��λ����
		int q1 = (Intnaire / 10000) % 10;
		int q2 = (Intnaire / 1000) % 10;
		int q3 = (Intnaire / 100) % 10;
		int q4 = (Intnaire / 10) % 10;
		int q5 = Intnaire % 10;
		int A1 = (IntA / 100) % 10;		int A0 = IntA % 10;
		int C1 = (IntC / 100) % 10;		int C0 = IntC % 10;
		int All1 = (IntAll / 100) % 10;		int All0 = IntAll % 10;
		OutFiledate << byte1 << "," << byte2 << "," << byte3 << "," << byte4 << "," << q1 << "," << q2 << "," << q3 << "," << q4 << "," << q5 << "," << A1 << "," << A0 << "," << C1 << "," << C0 << "," << All1 << "," << All0 << "," << timestampStr << '\n';
		OutFiledate.close();

	}//����4
}
void AR::send_data_slot()
{
	QByteArray rece_data;
	rece_data.resize(2);
	rece_data[0] = 1;
	rece_data[1] = 2;
	Socket1->write(rece_data);
}
void  AR::Test()
{
}
void AR::disconnectClient()
{
	//if (Socket1) {
	//	// �Ͽ�����
	//	Socket1->disconnectFromHost();
	//	if (Socket1->state() == QAbstractSocket::UnconnectedState || Socket1->waitForDisconnected(1000)) {
	//		qDebug() << QStringLiteral("�豸�Ͽ����ӣ�");
	//		// �ڴ˿���ִ��һЩ�����ĶϿ����Ӻ�Ĳ���
	//	}
	//	stopTimer();
	//	ui.label_15->setStyleSheet("background-color: rgb(182, 0, 0)");//����״̬
	//	qDebug() << QStringLiteral("ֹͣ���ݴ��䣡");
	//}
}
void AR::closeServer()
{
	// ֹͣ�����µ���������
	mServer->close();

	// �Ͽ����пͻ�������
	QList<QTcpSocket*> clientList = mServer->findChildren<QTcpSocket*>();
	for (QTcpSocket* client : clientList) {
		client->disconnectFromHost();
	}
	qDebug() << QStringLiteral("�ѹرշ�������");
	ui.set_listen->setDisabled(false);
	ui.close_listen->setDisabled(true);
}
//��ʱ��
void  AR::Send_startTimer()
{
	holoTimer->setTimerType(Qt::PreciseTimer);
	holoTimer->start(50);	/*holoTimer->start(5);*/
	patientRender->RemoveActor(patient_Actor);
	patientRender->AddActor(NDI_Actor);
}
//����connectTimer
void  AR::Send_stopTimer()
{
	holoTimer->stop();
}

//ui------------------------------------------------------------------------------------------
void AR::nextpage()
{
	isUS_Cail = false;
	qDebug() << "statenumber is" << statenumber << "    " << endl;
	if (statenumber == 0)
	{
		ui.frame_UsCali->setVisible(false);
		ui.frame_111->setVisible(false);
		ui.frame_222->setVisible(true);
		ui.frame_333->setVisible(false);
		ui.frame_444->setVisible(false);
		ui.frame_555->setVisible(false);
		ui.frame_666->setVisible(false);
		ui.state01->setStyleSheet(
			"image: url(:/new/prefix1/state03.png);}");
		patientRender->RemoveActor(Spline_Show_Actor);
		patientRender->RemoveActor(Man_Actor);
		patientRender->RemoveActor(Man_Body_Actor);
		patientRender->RemoveActor(patient_Actor);
		patientRender->RemoveActor(Spline_L_Actor);
		patientRender->RemoveActor(Spline_X_Actor);
		patientRender->AddActor(NDI_Actor);
		patientRender->GetRenderWindow()->Render();
		//����NDI
	}
	//�����ɼ�
	if (statenumber == 1)
	{
		ui.snapImageList->clear();
		ui.frame_UsCali->setVisible(false);
		ui.farme_UsSeg->setVisible(true);
		ui.frame_111->setVisible(false);
		ui.frame_222->setVisible(false);
		ui.frame_333->setVisible(true);
		ui.frame_444->setVisible(false);
		ui.frame_555->setVisible(false);
		ui.frame_666->setVisible(false);
		patient_Actor->SetVisibility(true);
		//patientRender->AddActor(patient_Actor);
		patientRender->GetRenderWindow()->Render();
	}
	//��׼
	if (statenumber == 2) {
		ui.frame_UsCali->setVisible(false);
		ui.frame_111->setVisible(false);
		ui.frame_222->setVisible(false);
		ui.frame_333->setVisible(false);
		ui.frame_444->setVisible(true);
		ui.frame_555->setVisible(false);
		ui.frame_666->setVisible(false);
		ui.state01->setVisible(true);
		ui.state02->setVisible(false);
		//ui.btn_next->setVisible(false);
		//ui.btn_next_up->setVisible(true);
		ui.state01->setStyleSheet(
			"image: url(:/new/prefix1/state02.png);}");
		//patientRender->RemoveActor(NDI_Actor);
		//patientRender->RemoveActor(patient_Actor);USActor1
		//patientRender->AddActor(Line_Actor);
		//patientRender->AddActor(P_Actor);
		//patientRender->AddActor(Spline_Show_Actor);
		//Spline_Show_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
		//Line_Actor->GetProperty()->SetColor(84 / 255, 255 / 255, 159 / 255);
		
		camera_3D->SetClippingRange(0.1, 20000.0); // ����Near Clipping PlaneΪ0.1��Far Clipping PlaneΪ10.0
		camera_3D->SetPosition(800, 0, 0); camera_3D->SetViewUp(0, 0, 0);//���ӽ�λ��
		patientRender->SetActiveCamera(camera_3D);

		patientRender->RemoveActor(USActor1);
		patientRender->RemoveActor(USsegment_actor);
		patientRender->RemoveActor(NDI_Actor);
		patientRender->RemoveActor(Tool_actors[2]);
		patientRender->RemoveActor(Drill_Actor);

		patientRender->AddActor(patient_Actor);
		Send_stopTimer();
		patientRender->GetRenderWindow()->Render();
		intPickdata();
	}
	if (statenumber == 3) {
		ui.frame_UsCali->setVisible(false);
		ui.frame_111->setVisible(false);
		ui.frame_222->setVisible(false);
		ui.frame_333->setVisible(false);
		ui.frame_444->setVisible(false);
		ui.frame_555->setVisible(true);
		ui.frame_666->setVisible(false);
		ui.state01->setVisible(true);
		ui.state02->setVisible(false);
		//ui.btn_next->setVisible(false);
		//ui.btn_next_up->setVisible(true);
		ui.state01->setStyleSheet(
			"image: url(:/new/prefix1/state03.png);}");

		vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
		camera->SetClippingRange(0.1, 20000.0); // ����Near Clipping PlaneΪ0.1��Far Clipping PlaneΪ10.0
		camera->SetPosition(500, 0, 0); camera->SetViewUp(0, 0, 0);//���ӽ�λ��
		patientRender->SetActiveCamera(camera_3D);
		for (int i = 0; i < 6;i++) 
		{
			patientRender->RemoveActor(us_data_group->getChild(i));
		}
		patientRender->RemoveActor(USdata_Actor);
		//patientRender->RemoveActor(Tool_actors[2]);
		patientRender->AddActor(USActor1);
		patientRender->AddActor(patient_Actor);
		patientRender->AddActor(NDI_Actor);
		patientRender->GetRenderWindow()->Render();
		Send_startTimer();
		//����&�ϴ�
	}
	statenumber++;
}
void AR::cailpage()
{
	isUS_Cail = true;
	ui.frame_UsCali->setVisible(true);
	ui.farme_UsSeg->setVisible(false);
	ui.snapImageList->clear();
	ui.frame_111->setVisible(false);
	ui.frame_222->setVisible(false);
	ui.frame_333->setVisible(true);
	ui.frame_444->setVisible(false);
	ui.frame_666->setVisible(false);
}
void AR::tool00()
{
	tool000 = !tool000;
	settool();
}
void AR::tool01()
{
	tool001 = !tool001;

	settool();
}
void AR::tool02()
{
	tool002 = !tool002;

	settool();
}
void AR::tool03()
{
	tool003 = !tool003;
	settool();
}
void AR::tool04()
{
	tool004 = !tool004;
}
void AR::settool()
{
	ui.state01->setVisible(false);
	ui.state02->setVisible(true);
	//̽��Probe
	if (tool000)
	{
		ui.label_00->setVisible(true);
		ui.toolbtn00->setStyleSheet(
			"QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_clicked0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_clicked1.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn00->setFont(QFont("����", 22));
	}
	else
	{
		ui.label_00->setVisible(false);
		ui.toolbtn00->setStyleSheet("QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_defult0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_hover0.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn00->setFont(QFont("����", 22));

	}
	//AR�궨��
	if (tool001)
	{
		ui.label_01->setVisible(true);
		ui.toolbtn01->setStyleSheet(
			"QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_clicked0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_clicked1.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn01->setFont(QFont("����", 22));
	}
	else
	{
		ui.label_01->setVisible(false);
		ui.toolbtn01->setStyleSheet("QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_defult0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_hover0.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn01->setFont(QFont("����", 22));
	}
	//���߼���
	if (tool002)
	{
		ui.label_02->setVisible(true);
		ui.toolbtn02->setStyleSheet(
			"QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_clicked0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_clicked1.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn02->setFont(QFont("����", 22));
	}
	else
	{
		ui.label_02->setVisible(false);
		ui.toolbtn02->setStyleSheet("QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_defult0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_hover0.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn02->setFont(QFont("����", 22));
	}
	//����̽ͷ
	if (tool003)
	{
		ui.label_03->setVisible(true);
		ui.toolbtn03->setStyleSheet(
			"QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_clicked0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_clicked1.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn03->setFont(QFont("����", 22));
	}
	else
	{
		ui.label_03->setVisible(false);
		ui.toolbtn03->setStyleSheet("QPushButton:!hover{color: rgb(175, 175, 175);image: url(:/new/prefix1/tool_defult0.png);border:none;}"
			"QPushButton:hover{color: rgb(255, 255, 255);image: url(:/new/prefix1/tool_hover0.png);border:none;}"
			"QPushButton:{image: url(:/new/prefix1/tool_hover0.png);border:none;border:0px solid;border-radius:5px;}");
		ui.toolbtn03->setFont(QFont("����", 22));
	}
}
void AR::Sexselect_man()
{
	ui.sex00->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(70,66,73);image: url(:/new/prefix1/man02.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(97,85,106);image: url(:/new/prefix1/man02.png);border-radius:10px;}");
	ui.sex01->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(46,46,46);image: url(:/new/prefix1/women00.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(57,57,57);image: url(:/new/prefix1/women01.png);border-radius:10px;}");

	camera_3D->SetClippingRange(0.1, 20000.0); // ����Near Clipping PlaneΪ0.1��Far Clipping PlaneΪ10.0
	camera_3D->SetPosition(0, 0, -4000); camera_3D->SetViewUp(0, 0, 0);//���ӽ�λ��
	patientRender->SetActiveCamera(camera_3D);
	//patientRender->AddActor(Man_Actor);
	//patientRender->AddActor(Spline_X_Actor);
	patientRender->AddActor(Spline_L_Actor);
	//patientRender->AddActor(Man_Body_Actor);
	Man_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Spline_X_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Spline_L_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Man_Body_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Man_Body_Actor->GetProperty()->SetOpacity(0.5);	Spline_X_Actor->GetProperty()->SetOpacity(0.7);	Spline_L_Actor->GetProperty()->SetOpacity(0.7);
	patientRender->GetRenderWindow()->Render();
}
void AR::Sexselect_women()
{
	ui.sex00->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(46,46,46);image: url(:/new/prefix1/man00.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(57,57,57);image: url(:/new/prefix1/man01.png);border-radius:10px;}");
	ui.sex01->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(70,66,73);image: url(:/new/prefix1/women02.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(97,85,106);image: url(:/new/prefix1/women02.png);border-radius:10px;}");
}
void AR::Splineselect_L()
{
	Spline_X_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Spline_L_Actor->GetProperty()->SetColor(84 / 255, 255 / 255, 159 / 255);
	Spline_X_Actor->GetProperty()->SetOpacity(0.7);	Spline_L_Actor->GetProperty()->SetOpacity(1);
	patientRender->GetRenderWindow()->Render();
	ui.spline_L->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(70,66,73);image: url(:/new/prefix1/spline_L2.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(97,85,106);image: url(:/new/prefix1/spline_L2.png);border-radius:10px;}");
	ui.spline_X->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(46,46,46);image: url(:/new/prefix1/spline_X0.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(57,57,57);image: url(:/new/prefix1/spline_X1.png);border-radius:10px;}");
}
void AR::Splineselect_X()
{
	Spline_L_Actor->GetProperty()->SetColor(205.0 / 255, 205.0 / 255, 180.0 / 255);
	Spline_X_Actor->GetProperty()->SetColor(84 / 255, 255 / 255, 159 / 255);
	Spline_X_Actor->GetProperty()->SetOpacity(1);	Spline_L_Actor->GetProperty()->SetOpacity(0.7);
	patientRender->GetRenderWindow()->Render();
	ui.spline_X->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(70,66,73);image: url(:/new/prefix1/spline_X2.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(97,85,106);image: url(:/new/prefix1/spline_X2.png);border-radius:10px;}");
	ui.spline_L->setStyleSheet(
		"QPushButton:!hover{background-color: rgb(46,46,46);image: url(:/new/prefix1/spline_L0.png);border-radius:10px;}"
		"QPushButton:hover{background-color: rgb(57,57,57);image: url(:/new/prefix1/spline_L1.png);border-radius:10px;}");
}
//us�궨���ui
void AR::record_position_1()
{	
	P_Record_USPhantomCali = true;
}//�궨���-�ɼ�probe��UsCailPlantom����
void AR::cali_phantom_1() 
{ 
	//ofstream outFile;
	////outFile.open("./data_cal/us_cali_mat1.txt", ios::out | ios::trunc);
	//outFile.open("./data_cal/data/points_in_phantom.txt", ios::out | ios::trunc);
	//for (int i = 0; i < UsCailProbeToWorld.size(); i++) 
	//{
	//	vtkSmartPointer<vtkMatrix4x4> Matrix_UsCailProbeToworld = vtkSmartPointer<vtkMatrix4x4>::New();//probe��temp��world��
	//	vtkSmartPointer<vtkMatrix4x4> Matrix_UsCaliPhantomToworld = vtkSmartPointer<vtkMatrix4x4>::New();//us��temp��world��
	//	NDIDataToMat(UsCailProbeToWorld[i], Matrix_UsCailProbeToworld);
	//	NDIDataToMat(UsCaliPhantomToWorld[i], Matrix_UsCaliPhantomToworld);
	//	outFile << i << ". [[";
	//	for (int j = 0; j < 4; ++j) {
	//		for (int k = 0; k < 4; ++k) {
	//			if (k == 3 && j == 3)
	//			{
	//				outFile << Matrix_UsCailProbeToworld->GetElement(j, k) ;	
	//			}
	//			else {
	//				outFile << Matrix_UsCailProbeToworld->GetElement(j, k) << ",";
	//			}
	//		}
	//	}
	//	outFile << "], [";
	//	for (int j = 0; j < 4; ++j) {
	//		for (int k = 0; k < 4; ++k) {
	//			if (k == 3 && j == 3)
	//			{	
	//				outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k);
	//			}
	//			else {
	//				outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k) << ",";
	//			}
	//		}
	//	}
	//	outFile << "]]" << '\n';
	//}
	//outFile.close();
	cail_probe(2, 0); 
}//�궨���
void AR::record_position_2() { 
	isUS_collect = true;
	//cail_probe(3, 0); }//�궨���
}
void AR::calio_USprobe_2() 
{
	ofstream outFile;
	//outFile.open("./data_cal/us_cali_mat2.txt", ios::out | ios::trunc);
	outFile.open("./data_cal/data/backup/ndi_read0.txt", ios::out | ios::trunc);

	for (int i = 0; i < UsCailUSProbeToWorld.size(); i++)
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix_UsCailUSProbeToworld = vtkSmartPointer<vtkMatrix4x4>::New();//probe��temp��world��
		vtkSmartPointer<vtkMatrix4x4> Matrix_UsCaliPhantomToworld = vtkSmartPointer<vtkMatrix4x4>::New();//us��temp��world��
		NDIDataToMat(UsCailUSProbeToWorld[i], Matrix_UsCailUSProbeToworld);
		NDIDataToMat(UsCaliPhantomToWorld[i], Matrix_UsCaliPhantomToworld);
		outFile << i << ". [[";
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				if (k == 3&& j==3)
				{
					outFile << Matrix_UsCailUSProbeToworld->GetElement(j, k);
				}
				else {
					outFile << Matrix_UsCailUSProbeToworld->GetElement(j, k) << ",";
				}
			}
		}
		outFile << "], [";
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				if (k == 3 && j == 3)
				{
					outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k);
				}
				else {
					outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k) << ",";
				}
			}
		}
		outFile << "]]" << '\n';
	}
	outFile.close();
	outFile.open("./data_cal/data/ndi_read0.txt", ios::out | ios::trunc);
	for (int i = 0; i < UsCailUSProbeToWorld.size(); i++)
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix_UsCailUSProbeToworld = vtkSmartPointer<vtkMatrix4x4>::New();//probe��temp��world��
		vtkSmartPointer<vtkMatrix4x4> Matrix_UsCaliPhantomToworld = vtkSmartPointer<vtkMatrix4x4>::New();//us��temp��world��
		NDIDataToMat(UsCailUSProbeToWorld[i], Matrix_UsCailUSProbeToworld);
		NDIDataToMat(UsCaliPhantomToWorld[i], Matrix_UsCaliPhantomToworld);
		outFile << i << ". [[";
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				if (k == 3 && j == 3)
				{
					outFile << Matrix_UsCailUSProbeToworld->GetElement(j, k);
				}
				else {
					outFile << Matrix_UsCailUSProbeToworld->GetElement(j, k) << ",";
				}
			}
		}
		outFile << "], [";
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				if (k == 3 && j == 3)
				{
					outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k);
				}
				else {
					outFile << Matrix_UsCaliPhantomToworld->GetElement(j, k) << ",";
				}
			}
		}
		outFile << "]]" << '\n';
	}
	outFile.close();
	cail_probe(5, 0); 
}//�궨���
//ui------------------------------------------------------------------------------------------
void AR::setlabel(int L)
{
	switch (L) {
	case 1:
		ui.label_00->setStyleSheet("image: url(:/new/prefix1/probe0.png)");//̽��red

		break;
	case 2:
		ui.label_00->setStyleSheet("image: url(:/new/prefix1/probe1.png)");//̽��green

		break;
	case 3:
		n++;
		str0 = QString::number(n);
		ui.lineEdit->setText(str0);
		break;
	case 4:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("NDI���ӳɹ�"), QStringLiteral("ȷ��"));
		break;
	case 5:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("NDI����ʧ��"), QStringLiteral("ȷ��"));
		break;
	case 6:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("NDI���ӳɹ�"), QStringLiteral("ȷ��"));
		break;
	case 7:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("��ɼ�3-6���㣡"), QStringLiteral("ȷ��"));
		break;
	case 8:
		ui.label_01->setStyleSheet("image: url(:/new/prefix1/cube0.png);");//�궨��

		break;
	case 9:
		ui.label_01->setStyleSheet("image: url(:/new/prefix1/cube1.png);");//�궨��

		break;
	case 10:
		ui.label_02->setStyleSheet("image: url(:/new/prefix1/spline0.png);");//����
		//flagDatas[2] = 0;
		break;
	case 11:
		ui.label_02->setStyleSheet("image: url(:/new/prefix1/spline1.png);");//����
		//flagDatas[2] = 1;
		break;

		//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	case 13:
		n1++;
		str1 = QString::number(n1);
		//ui.lineEdit_2->setText(str1);
		break;
	case 14:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("��ʵ�ռ�궨��ɣ�"), QStringLiteral("ȷ��"));
		break;
	case 15:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("�궨�㼯�������ȡ��ȷ�㼯��"), QStringLiteral("ȷ��"));
		break;
	case 16:
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("��׼��ɣ�"), QStringLiteral("ȷ��"));
		break;
	case 17:
		ui.label_04->setStyleSheet("image: url(:/new/prefix1/drill0.png);");//Drill
		break;
	case 18:
		ui.label_04->setStyleSheet("image: url(:/new/prefix1/drill1.png);");//Drill

		break;
	case 19:
		//ui.label_11->setStyleSheet("background-color: rgb(182, 0, 0)");//449

		break;
	case 20:
		//ui.label_11->setStyleSheet("background-color: rgb(67, 198, 172)");//449

		break;
	case 21:
		ui.label_03->setStyleSheet("image: url(:/new/prefix1/US0.png);");//����

		break;
	case 22:
		ui.label_03->setStyleSheet("image: url(:/new/prefix1/US1.png);");//����

		break;
	case 23:
		//ui.label_18->setStyleSheet("background-color: rgb(182, 0, 0)");//tibia

		break;
	case 24:
		//ui.label_18->setStyleSheet("background-color: rgb(67, 198, 172)");//tibia

		break;
		//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	}

}
void AR::regis_clear()
{
	ui.lineEdit->setText("0");
	n = 0;
}
void AR::test_clear()
{
	n1 = 0;
	//ui.lineEdit_2->setText("0");
}
void AR::cali_clear()
{
	//ui.tableWidget_2->clear();
	//ui.lineEdit_3->setText("0");
	n2 = 0;
}
//����Ҽ���ʾ
void AR::ShowWinMenu(QPoint point, QPoint localPoint)
{
	if (Pick_Area->isSelected)
	{
		L1SelectAction->setEnabled(true);		L2SelectAction->setEnabled(true);		L3SelectAction->setEnabled(true);
		L4SelectAction->setEnabled(true);		L5SelectAction->setEnabled(true);		L6SelectAction->setEnabled(true);
		std::cout << "����ѡ������" << std::endl;
		RightMenu.setStyleSheet(
			"QMenu::item { padding:10px 10px 10px 10px; }"
			"QMenu{ background-color: #2e2e2e; }"
			"QMenu{ color: #F5F5F5; }"
			"QMenu::item:selected{color: rgba(255, 255, 255, 1);background: rgb(59, 59, 60);}"
		);
	}
	else
	{
		L1SelectAction->setEnabled(false);		L2SelectAction->setEnabled(false);		L3SelectAction->setEnabled(false);
		L4SelectAction->setEnabled(false);		L5SelectAction->setEnabled(false);		L6SelectAction->setEnabled(false);
		std::cout << "����ѡ������" << std::endl;
		RightMenu.setStyleSheet(
			"QMenu::item { padding:10px 10px 10px 10px; }"
			"QMenu{ background-color: #2e2e2e; }"
			"QMenu{ color: rgba(100, 100, 100, 1); }"
			"QMenu::item:selected{color: rgba(100, 100, 100, 1);background: #2e2e2e;}"
		);
	}
	QFont font;
	//������������
	font.setFamily("����");
	//�������ִ�СΪ50����
	font.setPixelSize(10);
	//��������Ϊ����
	font.setBold(true);
	//�������ִ�С
	font.setPointSize(10);
	rightClikPoint = localPoint;
	//RightMenu.setFixedWidth(130);
	//RightMenu.setFixedHeight(150);
	RightMenu.setFont(font);
	RightMenu.exec(point);
}
//��ʼ������
void AR::initPatientPage()////��ʼ������
{
	patientRender->SetBackground(0.2, 0.2, 0.2);
	patientRender->ResetCamera();
	patientRender->GetActiveCamera()->ParallelProjectionOn();

	camera_3D->SetClippingRange(0.1, 20000); // ����Near Clipping PlaneΪ0.1��Far Clipping PlaneΪ10.0
	camera_3D->SetPosition(0, 1, 0);

	ui.patientWindow->GetRenderWindow()->SetNumberOfLayers(2);
	patientRender->SetLayer(0);
	AxisRender->SetLayer(1);
	ui.patientWindow->GetRenderWindow()->AddRenderer(patientRender);//ģ�Ͳ�
	ui.patientWindow->GetRenderWindow()->AddRenderer(AxisRender);	//����ϵ��

	patientRender->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::RightButtonPressEvent);
	patientRender->GetRenderWindow()->Render();
	patientRender->SetActiveCamera(camera_3D);
	AxisRender->SetActiveCamera(camera_3D);
	targetHelper->addInRender(AxisRender);//targetHelper�а�group��child����ӽ����render��
	targetHelper->render = AxisRender;


	// ����һ�� vtkAxesActor ����ʾ����ϵ
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->AxisLabelsOff();
	axes->SetTotalLength(100, 100, 100);
	patientRender->AddActor(axes);

	//����Man
	vtkSmartPointer<vtkSTLReader> Man_reader = vtkSmartPointer<vtkSTLReader>::New();
	Man_reader->SetFileName("./Modle/Man.STL");
	Man_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Man_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Man_mapper->SetInputConnection(Man_reader->GetOutputPort());
	Man_Actor->SetMapper(Man_mapper);
	//����Man_Body
	vtkSmartPointer<vtkSTLReader> Man_Body_reader = vtkSmartPointer<vtkSTLReader>::New();
	Man_Body_reader->SetFileName("./Modle/Man_Body.STL");
	Man_Body_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Man_Body_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Man_Body_mapper->SetInputConnection(Man_Body_reader->GetOutputPort());
	Man_Body_Actor->SetMapper(Man_Body_mapper);
	//����Spline_X
	vtkSmartPointer<vtkSTLReader> Spline_X_reader = vtkSmartPointer<vtkSTLReader>::New();
	Spline_X_reader->SetFileName("./Modle/Spline_X.STL");
	Spline_X_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Spline_X_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Spline_X_mapper->SetInputConnection(Spline_X_reader->GetOutputPort());
	Spline_X_Actor->SetMapper(Spline_X_mapper);
	Spline_X_Actor->SetPickable(false);

	//����Spline_L
	vtkSmartPointer<vtkSTLReader> Spline_L_reader = vtkSmartPointer<vtkSTLReader>::New();
	Spline_L_reader->SetFileName("./Modle/Spline_L.STL");
	Spline_L_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Spline_L_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Spline_L_mapper->SetInputConnection(Spline_L_reader->GetOutputPort());
	Spline_L_Actor->SetMapper(Spline_L_mapper);
	Spline_L_Actor->SetPickable(false);

	//����Spline_Show
	vtkSmartPointer<vtkSTLReader> Spline_Show_reader = vtkSmartPointer<vtkSTLReader>::New();
	Spline_Show_reader->SetFileName("./Modle/Spline_Show.STL");	
	Spline_Show_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper>Spline_Show_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Spline_Show_mapper->SetInputConnection(Spline_Show_reader->GetOutputPort());
	Spline_Show_Actor->SetMapper(Spline_Show_mapper);
	Spline_Show_Actor->SetPickable(false);

	//����Line
	vtkSmartPointer<vtkSTLReader> Line_reader = vtkSmartPointer<vtkSTLReader>::New();
	Line_reader->SetFileName("./Modle/Line.STL");
	Line_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper>Line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Line_mapper->SetInputConnection(Line_reader->GetOutputPort());
	Line_Actor->SetMapper(Line_mapper);

	//����P
	vtkSmartPointer<vtkSTLReader> P_reader = vtkSmartPointer<vtkSTLReader>::New();
	P_reader->SetFileName("./Modle/P.STL");
	P_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper>P_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	P_mapper->SetInputConnection(P_reader->GetOutputPort());
	P_Actor->SetMapper(P_mapper);


	//����Ball
	vtkSmartPointer<vtkSTLReader> Ball_reader = vtkSmartPointer<vtkSTLReader>::New();
	Ball_reader->SetFileName("./Modle/Ball.STL");
	Ball_reader->Update();
	Ball_mapper->SetInputConnection(Ball_reader->GetOutputPort());

	//����NDI
	vtkSmartPointer<vtkSTLReader> NDI_reader = vtkSmartPointer<vtkSTLReader>::New();
	NDI_reader->SetFileName("./Modle/NDI.STL");
	NDI_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> NDI_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	NDI_mapper->SetInputConnection(NDI_reader->GetOutputPort());
	NDI_Actor->SetMapper(NDI_mapper);
	NDI_Actor->GetProperty()->SetOpacity(0.3);
	NDI_Actor->SetPickable(false);
	//patientRender->AddActor(NDI_Actor);

	//����Probe����0
	vtkSmartPointer<vtkSTLReader> stl_reader = vtkSmartPointer<vtkSTLReader>::New();
	stl_reader->SetFileName("./Modle/Probe.STL");
	stl_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> probe_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	probe_mapper->SetInputConnection(stl_reader->GetOutputPort());
	Probe_Actor->SetMapper(probe_mapper);
	Probe_Actor->SetPickable(false);

	//����Cube����1
	vtkSmartPointer<vtkSTLReader> cube_reader = vtkSmartPointer<vtkSTLReader>::New();
	cube_reader->SetFileName("./Modle/cube.STL");
	cube_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Cube_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Cube_mapper->SetInputConnection(cube_reader->GetOutputPort());
	Cube_Actor->SetMapper(Cube_mapper);


	//����Bone760����2
	vtkSmartPointer<vtkSTLReader> Bone760_reader = vtkSmartPointer<vtkSTLReader>::New();
	Bone760_reader->SetFileName("./Modle/SSS.STL");
	//Bone760_reader->SetFileName("./Modle/Bone760.STL");
	Bone760_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Bone_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Bone_mapper->SetInputConnection(Bone760_reader->GetOutputPort());
	Bone_Actor->SetMapper(Bone_mapper);
	Bone_Actor->SetPickable(false);

	//����Guide339����3
	vtkSmartPointer<vtkSTLReader> Guide339_reader = vtkSmartPointer<vtkSTLReader>::New();
	Guide339_reader->SetFileName("./Modle/Cal_US.STL");
	Guide339_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Guide339_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Guide339_mapper->SetInputConnection(Guide339_reader->GetOutputPort());
	Guide339_Actor->SetMapper(Guide339_mapper);
	Guide339_Actor->SetPickable(false);

	//����Guide449����4
	vtkSmartPointer<vtkSTLReader> Guide449_reader = vtkSmartPointer<vtkSTLReader>::New();
	Guide449_reader->SetFileName("./Modle/Guide449.STL");
	Guide449_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Guide449_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Guide449_mapper->SetInputConnection(Guide449_reader->GetOutputPort());
	Guide449_Actor->SetMapper(Guide449_mapper);
	Guide449_Actor->SetPickable(false);

	//����US_Probe����5
	vtkSmartPointer<vtkSTLReader> drill_reader = vtkSmartPointer<vtkSTLReader>::New();
	//drill_reader->SetFileName("./Modle/US_Probe_684760.STL");
	drill_reader->SetFileName("./Modle/US_Probe_8700339.STL");
	drill_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> Drill_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	Drill_mapper->SetInputConnection(drill_reader->GetOutputPort());
	Drill_Actor->SetMapper(Drill_mapper);
	Drill_Actor->SetPickable(false);
	
	// ����Tibia����6
	vtkSmartPointer<vtkSTLReader> tibia_reader = vtkSmartPointer<vtkSTLReader>::New();
	tibia_reader->SetFileName("./Modle/TibiaDRO.STL");
	tibia_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> tibia_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	tibia_mapper->SetInputConnection(tibia_reader->GetOutputPort());
	tibia_Actor->SetMapper(tibia_mapper);
	tibia_Actor->SetPickable(false);

	//ͼ��
	vtkSmartPointer<vtkPlaneSource> USplane = vtkSmartPointer<vtkPlaneSource>::New();
	USplane->SetOrigin(0, 0, 0);
	USplane->SetPoint1(458 * SIZE_PIXEL_x , 0, 0);
	USplane->SetPoint2(0, 431 * SIZE_PIXEL_y, 0);
	USplane->Update();
	vtkSmartPointer<vtkPolyDataMapper> USmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	USmapper->SetInputData(USplane->GetOutput());
	USActor1->SetMapper(USmapper);
	USActor1->GetProperty()->SetColor(255 / 255, 255 / 255, 255 / 255);
	USActor1->GetProperty()->SetOpacity(0.5);
	USActor1->SetPickable(false);

	Tool_actors.push_back(Probe_Actor);//0
	Tool_actors.push_back(Cube_Actor);//1
	Tool_actors.push_back(Bone_Actor);//2
	Tool_actors.push_back(Guide339_Actor);//3
	Tool_actors.push_back(Guide449_Actor);//4
	Tool_actors.push_back(Drill_Actor);//5
	Tool_actors.push_back(tibia_Actor);//6

	//̽��ɼ�ʹ��-��ͷ
	PresetPoints_mark_0[0] = -2.355;	PresetPoints_mark_0[1] = 104.873;	PresetPoints_mark_0[2] = 45.6;
	PresetPoints_mark_1[0] = -15.614;	PresetPoints_mark_1[1] = -107.309;	PresetPoints_mark_1[2] = 25.605;
	PresetPoints_mark_2[0] = 32.143;	PresetPoints_mark_2[1] = -104.998;	PresetPoints_mark_2[2] = 28.186;
	//US�ɼ�ʹ��
	//PresetPoints_mark_0[0] = -2.355;	PresetPoints_mark_0[1] = 70.1232;	PresetPoints_mark_0[2] = 48.0691;
	//PresetPoints_mark_1[0] = -2.355;	PresetPoints_mark_1[1] = 12.4969;	PresetPoints_mark_1[2] = 40.7681;
	//PresetPoints_mark_2[0] = -2.355;	PresetPoints_mark_2[1] = -52.4073;	PresetPoints_mark_2[2] = 43.3493;
	//��ģ
	//PresetPoints_mark_0[0] = 30;	PresetPoints_mark_0[1] = -44;	PresetPoints_mark_0[2] = 55;
	//PresetPoints_mark_1[0] = 130;	PresetPoints_mark_1[1] = -44;	PresetPoints_mark_1[2] = 40;
	//PresetPoints_mark_2[0] = 30;	PresetPoints_mark_2[1] = -44;	PresetPoints_mark_2[2] = 5;

	Matrix_BonetoDRO->SetElement(0, 0, 0.999488);
	Matrix_BonetoDRO->SetElement(0, 1, -0.00843928);
	Matrix_BonetoDRO->SetElement(0, 2, -0.0308718);
	Matrix_BonetoDRO->SetElement(0, 3, 49.8443);

	Matrix_BonetoDRO->SetElement(1, 0, 0.00815824);
	Matrix_BonetoDRO->SetElement(1, 1, 0.999924);
	Matrix_BonetoDRO->SetElement(1, 2, -0.00921812);
	Matrix_BonetoDRO->SetElement(1, 3, -200.434);

	Matrix_BonetoDRO->SetElement(2, 0, 0.0309472);
	Matrix_BonetoDRO->SetElement(2, 1, 0.00896154);
	Matrix_BonetoDRO->SetElement(2, 2, 0.999481);
	Matrix_BonetoDRO->SetElement(2, 3, -128.911);

	Matrix_BonetoDRO->SetElement(3, 0, 0.0);
	Matrix_BonetoDRO->SetElement(3, 1, 0.0);
	Matrix_BonetoDRO->SetElement(3, 2, 0.0);
	Matrix_BonetoDRO->SetElement(3, 3, 1.0);
	targetHelper->mode = "translate";
	//����Ҽ�
	RightMenu.addAction(L1SelectAction);
	RightMenu.addAction(L2SelectAction);
	RightMenu.addAction(L3SelectAction);
	RightMenu.addAction(L4SelectAction);
	RightMenu.addAction(L5SelectAction);
	RightMenu.addAction(L6SelectAction);
	RightMenu.setStyleSheet(
		"QMenu::item { padding:10px 10px 10px 10px; }"
		"QMenu{ background-color: #2e2e2e; }"
		"QMenu{ color: rgba(100, 100, 100, 1); }"
		"QMenu::item:selected{color: rgba(100, 100, 100, 1);background: #2e2e2e;}"
	);
	L1SelectAction->setCheckable(true);	L2SelectAction->setCheckable(true);	L3SelectAction->setCheckable(true);
	L4SelectAction->setCheckable(true);	L5SelectAction->setCheckable(true);	L6SelectAction->setCheckable(true);
	for (int i = 0; i < 6; ++i)
	{
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		us_data_group->addChild(actor);
		actor->SetVisibility(false);
		actor->GetProperty()->SetColor(0,1,0);
		actor->SetPickable(false);
		double intnumber[16] =
		{ 0,0,0,1,
		  0,0,0,1,
		  0,0,0,1,
		  0,0,0,1 };
		vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();
		mat->DeepCopy(intnumber);
		Matrix_splineToDRO.push_back(mat);
	}
	AllTimer->setTimerType(Qt::PreciseTimer);

	//std::list<int> mList = { 1, 2, 3, 4, 5 };
	//mList.pop_front();
	//mList.push_back(6);
	//// ��ӡ�б�
	//for (int value : mList) {
	//	std::cout << value << " ";
	//}
	//std::cout << std::endl;



}
///������Ϣ¼��������� 
void AR::loadPatientModel()
{
	//patientRender->RemoveActor(patientActor);
	QString path = QFileDialog::getOpenFileName(this, QStringLiteral("�����ֹ�ģ��"), "./", "*.STL;*.stl");
	if (path == "") return;
	patient_spline_reader->SetFileName(path.toUtf8().data());
	qDebug() << path.toUtf8().data();
	patient_spline_reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> patient_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	patient_mapper->SetInputConnection(patient_spline_reader->GetOutputPort());

	///���㵼��ģ�͵�OBB
	vtkSmartPointer<vtkOBBTree> obbTree = vtkSmartPointer<vtkOBBTree>::New();
	obbTree->SetDataSet(patient_spline_reader->GetOutput());
	obbTree->SetMaxLevel(10);
	obbTree->BuildLocator();
	double corner[3] = { 0.0, 0.0, 0.0 };
	double max[3] = { 0.0, 0.0, 0.0 };
	double mid[3] = { 0.0, 0.0, 0.0 };
	double min[3] = { 0.0, 0.0, 0.0 };
	double size[3] = { 0.0, 0.0, 0.0 };
	obbTree->ComputeOBB(patient_spline_reader->GetOutput(), corner, max, mid, min, size);
	Vec_Add(max, mid, Search_pos);
	Vec_Add(Search_pos, min, Search_pos);
	Search_pos[0] = Search_pos[0] / 2;
	Search_pos[1] = Search_pos[1] / 2;
	Search_pos[2] = Search_pos[2] / 2;
	Vec_Add(corner, Search_pos, Search_pos);
	Vec_Normalize(max, Search_vec);
	qDebug() << "Center:\t" << Search_pos[0] << ", " << Search_pos[1] << ", " << Search_pos[2];
	qDebug() << "Max:\t" << Search_vec[0] << ", " << Search_vec[1] << ", " << Search_vec[2];
	patient_Actor->SetMapper(patient_mapper);


	for (int i = 0; i < 6; ++i)
	{
		vtkSmartPointer<vtkSTLReader> bonestl_reader = vtkSmartPointer<vtkSTLReader>::New();
		std::string filename = "./Modle/m/" + std::to_string(i) + ".STL";
		bonestl_reader->SetFileName(filename.c_str());
		bonestl_reader->Update();
		vtkSmartPointer<vtkPolyDataMapper> bonestl_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		bonestl_mapper->SetInputConnection(bonestl_reader->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(bonestl_mapper);
		actor->SetVisibility(false);
		actor->GetProperty()->SetColor(0, 1, 0);
		actor->SetPickable(false);
		stl_data_group->addChild(actor);

		///���㵼��ģ�͵�OBB
		vtkSmartPointer<vtkOBBTree> obbTree = vtkSmartPointer<vtkOBBTree>::New();
		obbTree->SetDataSet(bonestl_reader->GetOutput());
		obbTree->SetMaxLevel(10);
		obbTree->BuildLocator();
		double corner[3] = { 0.0, 0.0, 0.0 };
		double max[3] = { 0.0, 0.0, 0.0 };
		double mid[3] = { 0.0, 0.0, 0.0 };
		double min[3] = { 0.0, 0.0, 0.0 };
		double size[3] = { 0.0, 0.0, 0.0 };
		obbTree->ComputeOBB(bonestl_reader->GetOutput(), corner, max, mid, min, size);
		double Search_pos[3];
		double Search_vec[3];
		Vec_Add(max, mid, Search_pos);
		Vec_Add(Search_pos, min, Search_pos);
		Search_pos[0] = Search_pos[0] / 2;
		Search_pos[1] = Search_pos[1] / 2;
		Search_pos[2] = Search_pos[2] / 2;
		Vec_Add(corner, Search_pos, Search_pos);
		Vec_Normalize(max, Search_vec);
		vector<double> sp(3);
		sp[0] = Search_pos[0];
		sp[1] = Search_pos[1];
		sp[2] = Search_pos[2];
		Search_pos_sigle.push_back(sp);
		vector<double> sv(3);
		sv[0] = Search_vec[0];
		sv[1] = Search_vec[1];
		sv[2] = Search_vec[2];
		Search_vec_sigle.push_back(sv);
	}

	//Tool_actors.push_back(patient_Actor);//5


	vtkSmartPointer<vtkActor> Ball_1_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Ball_2_Actor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> Ball_3_Actor = vtkSmartPointer<vtkActor>::New();
	Ball_1_Actor->SetMapper(Ball_mapper);
	Ball_2_Actor->SetMapper(Ball_mapper);
	Ball_3_Actor->SetMapper(Ball_mapper);
	patientRender->AddActor(Ball_1_Actor);
	patientRender->AddActor(Ball_2_Actor);
	patientRender->AddActor(Ball_3_Actor);
	Ball_1_Actor->SetPosition(PresetPoints_mark_0[0], PresetPoints_mark_0[1], PresetPoints_mark_0[2]);
	Ball_2_Actor->SetPosition(PresetPoints_mark_1[0], PresetPoints_mark_1[1], PresetPoints_mark_1[2]);
	Ball_3_Actor->SetPosition(PresetPoints_mark_2[0], PresetPoints_mark_2[1], PresetPoints_mark_2[2]);

	patientRender->RemoveActor(Spline_X_Actor);
	patientRender->RemoveActor(Spline_L_Actor);
	patientRender->AddActor(patient_Actor);
	//patientRender->AddActor(patient_Actor);
	Spline_Show_Actor->GetProperty()->SetColor(84 / 255, 255 / 255, 159 / 255);
	Ball_1_Actor->GetProperty()->SetColor(255.0 / 255, 0 / 255, 0 / 255);
	Ball_2_Actor->GetProperty()->SetColor(255.0 / 255, 0 / 255, 0 / 255);
	Ball_3_Actor->GetProperty()->SetColor(255.0 / 255, 0 / 255, 0 / 255);		Spline_Show_Actor->GetProperty()->SetOpacity(1);
	//patientRender->ResetCamera();

	//patientRender->GetActiveCamera()->ParallelProjectionOn();
	ui.patientWindow->GetRenderWindow()->Render();

	//// ��ȡ��ȡ�� PolyData ���ݣ�����patient_spline_reader��ͷ�ĵ������ݣ�
	//vtkSmartPointer<vtkPolyData> inputPolyData = patient_spline_reader->GetOutput();
	//// ���㷨����
	//vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	//normals->SetInputData(inputPolyData);
	//normals->SplittingOff(); // �رղ��
	//normals->ComputePointNormalsOn();
	//normals->ComputeCellNormalsOff();
	//normals->Update();
	//// �������� PolyData ���ݸ������ vtkPolyData ����
	//vtkSmartPointer<vtkPolyData> Bonedata = normals->GetOutput();
	//// ������ʹ�� processedPolyData ������к����Ĳ���
	//vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	//Loop->SetInputData(Bonedata);
	//Loop->SetNumberOfSubdivisions(1);
	//Loop->Update();
	//vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//points = Loop->GetOutput()->GetPoints();//points�д������е����ͷ�ĵ�������	
	//vector<double> p_x, p_y, p_z;
	//for (int i = 0; i < points->GetNumberOfPoints(); i++)
	//{
	//	double pi[3];
	//	points->GetPoint(i, pi);
	//	p_x.push_back(pi[0]);
	//	p_y.push_back(pi[1]);
	//	p_z.push_back(pi[2]);
	//}
	////����ȡ�ı�������ת��ΪvtkPolyData
	//vtkSmartPointer<vtkPoints> vtkpoints_Bone = vtkSmartPointer<vtkPoints>::New();
	//vtkSmartPointer<vtkCellArray> vtkcell_Bone = vtkSmartPointer<vtkCellArray>::New();
	//vector<vector<double>> Bone_points;
	//for (int i = 0; i < p_x.size(); i++)
	//{
	//	vector<double> sp(3);
	//	sp[0] = p_x[i];
	//	sp[1] = p_y[i];
	//	sp[2] = p_z[i];
	//	Bone_points.push_back(sp);
	//	vtkIdType pid1[1], pid2[1];
	//	pid1[0] = vtkpoints_Bone->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
	//	vtkcell_Bone->InsertNextCell(1, pid1);
	//}
	//OutFiledate.open("./predict/BoneCould_data.txt", ios::out | ios::trunc);
	//for (int i = 0; i < Bone_points.size(); i++)
	//{
	//	OutFiledate << Bone_points[i][0] << " " << Bone_points[i][1] << " " << Bone_points[i][2] << '\n';
	//}
	//OutFiledate.close();
}

//------------------------------���Ʋɼ�����׼�׶�

void AR::StartPointClouds(vtkSmartPointer<vtkActor> actor_target, vtkSmartPointer<vtkActor> actor_source, vector<double> Search_p, vector<double> Search_v, vtkSmartPointer<vtkMatrix4x4> Matrix_boneToDRO)
{
	vtkSmartPointer<vtkMatrix4x4> M1 = patient_Actor->GetMatrix();

	bool isreadActor = false;
	if (isreadActor)
	{
		// ��ȡ��ȡ�� PolyData ���ݣ�����patient_spline_reader��ͷ�ĵ������ݣ�
		vtkSmartPointer<vtkPolyData> inputPolyData = patient_spline_reader->GetOutput();
		// ���㷨����
		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInputData(inputPolyData);
		normals->SplittingOff(); // �رղ��
		normals->ComputePointNormalsOn();
		normals->ComputeCellNormalsOff();
		normals->Update();
		// �������� PolyData ���ݸ������ vtkPolyData ����
		vtkSmartPointer<vtkPolyData> Bonedata = normals->GetOutput();
		// ������ʹ�� processedPolyData ������к����Ĳ���
		vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
		Loop->SetInputData(Bonedata);
		Loop->SetNumberOfSubdivisions(1);
		Loop->Update();
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points = Loop->GetOutput()->GetPoints();//points�д������е����ͷ�ĵ�������
		//points = Bonedata->GetPoints();//points�д������е����ͷ�ĵ�������
		//��ȡ�ɼ��������㸽���Ĺ�ͷ�ĵ�������������׼Ч��
		vector<double> p_x, p_y, p_z;
		for (int i = 0; i < points->GetNumberOfPoints(); i++)
		{
			double pi[3];
			points->GetPoint(i, pi);
			double norm = sqrt((pi[0] - PresetPoints_mark_0[0]) * (pi[0] - PresetPoints_mark_0[0]) + (pi[1] - PresetPoints_mark_0[1]) * (pi[1] - PresetPoints_mark_0[1]) + (pi[2] - PresetPoints_mark_0[2]) * (pi[2] - PresetPoints_mark_0[2]));
			double norm1 = sqrt((pi[0] - PresetPoints_mark_1[0]) * (pi[0] - PresetPoints_mark_1[0]) + (pi[1] - PresetPoints_mark_1[1]) * (pi[1] - PresetPoints_mark_1[1]) + (pi[2] - PresetPoints_mark_1[2]) * (pi[2] - PresetPoints_mark_1[2]));
			double norm2 = sqrt((pi[0] - PresetPoints_mark_2[0]) * (pi[0] - PresetPoints_mark_2[0]) + (pi[1] - PresetPoints_mark_2[1]) * (pi[1] - PresetPoints_mark_2[1]) + (pi[2] - PresetPoints_mark_2[2]) * (pi[2] - PresetPoints_mark_2[2]));
			if (norm < 60 || norm1 < 50 || norm2 < 50)
			{
				p_x.push_back(pi[0]);
				p_y.push_back(pi[1]);
				p_z.push_back(pi[2]);
			}
		}
		//����ȡ�ı�������ת��ΪvtkPolyData
		vtkSmartPointer<vtkPoints> vtkpoints_Bone = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> vtkcell_Bone = vtkSmartPointer<vtkCellArray>::New();
		vector<vector<double>> Bone_points;

		for (int i = 0; i < p_x.size(); i++)
		{
			vector<double> sp(3);
			sp[0] = p_x[i];
			sp[1] = p_y[i];
			sp[2] = p_z[i];
			Bone_points.push_back(sp);
			vtkIdType pid1[1], pid2[1];
			pid1[0] = vtkpoints_Bone->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
			vtkcell_Bone->InsertNextCell(1, pid1);
		}
	}

	bool isreadfromActor = true;
	if (isreadfromActor)
	{
		patient_Actor->SetUserMatrix(M1);
		patient_Actor->SetVisibility(true);
		actor_target->SetUserMatrix(M1);
		actor_target->SetVisibility(true);
		patientRender->AddActor(actor_target);
		patientRender->AddActor(patient_Actor);
		patientRender->GetRenderWindow()->Render();
		//stl_data����----------------------------------------------------------------------------------------------------------------------
		vtkMapper *mapper = actor_target->GetMapper();
		vtkPolyData *inputPolyData = vtkPolyData::SafeDownCast(mapper->GetInput());
		// ���㷨����
		vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
		normals->SetInputData(inputPolyData);
		normals->SplittingOff(); // �رղ��
		normals->ComputePointNormalsOn();
		normals->ComputeCellNormalsOff();
		normals->Update();
		// �������� PolyData ���ݸ������ vtkPolyData ����
		vtkSmartPointer<vtkPolyData> Bonedata = normals->GetOutput();
		// ������ʹ�� processedPolyData ������к����Ĳ���
		vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
		Loop->SetInputData(Bonedata);
		Loop->SetNumberOfSubdivisions(1);
		Loop->Update();
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		points = Loop->GetOutput()->GetPoints();//points�д������е����ͷ�ĵ�������
		cout << "points������" << points->GetNumberOfPoints()<<"\n";
		//��ȡ�ɼ��������㸽���Ĺ�ͷ�ĵ�������������׼Ч��
		vector<double> p_x, p_y, p_z;
		for (int i = 0; i < points->GetNumberOfPoints(); i++)
		{
			double pi[3];
			points->GetPoint(i, pi);		
			p_x.push_back(pi[0]);
			p_y.push_back(pi[1]);
			p_z.push_back(pi[2]);
		}
		////����ȡ�ı�������ת��ΪvtkPolyData
		vtkSmartPointer<vtkPoints> points1 = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> cell1 = vtkSmartPointer<vtkCellArray>::New();
		for (int i = 0; i < p_x.size(); i++)
		{
			vtkIdType pid1[1], pid2[1];
			pid1[0] = points1->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
			cell1->InsertNextCell(1, pid1);
		}
		vtkSmartPointer<vtkPolyData> vtkpolydata_Bone = vtkSmartPointer<vtkPolyData>::New();
		vtkpolydata_Bone->SetPoints(points1);
		vtkpolydata_Bone->SetVerts(cell1);

		//��vtkPolyData�������ݾ�������׼�������
		vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
		Transform->SetMatrix(M1);
		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInputData(vtkpolydata_Bone);
		transformFilter->SetTransform(Transform);
		transformFilter->Update();

		//us_data����----------------------------------------------------------------------------------------------------------------------
		vtkMapper *mapper_target = actor_source->GetMapper();
		// �� mapper ��ȡ�������ݣ������������������ vtkPolyData ����
		vtkPolyData *polyData_target = vtkPolyData::SafeDownCast(mapper_target->GetInput());
		vector<vector<double>> USseg_points;
		if (polyData_target)
		{
			vtkPoints *points = polyData_target->GetPoints();
			// ����Ƿ��е�
			if (points)
			{
				// ���� vtkPoints �е�ÿ����
				for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
				{
					// ��ȡ�������
					double p[3];
					points->GetPoint(i, p);
					// �����������ӵ� US_points ��
					USseg_points.push_back(std::vector<double>{p[0], p[1], p[2]});
				}
			}
		}
		int Num_DROpoints = USseg_points.size();
		if (Num_DROpoints < 5000)
		{
			return;
		}
		//cout << "����ĳ�ʼ������" << Num_DROpoints << "\n";
		vector<vector<double>> Reg_points;
		//���Ȼ�ȡ5000���ɼ��ĵ�
		vector<int> insertnum;
		NumSelect(Num_DROpoints, 5000, insertnum);
		for (int i = 0; i < insertnum.size(); i++)
		{
			int index = insertnum[i];
			vector<double> sp(3);
			sp[0] = USseg_points[index][0];
			sp[1] = USseg_points[index][1];
			sp[2] = USseg_points[index][2];
			Reg_points.push_back(sp);
		}
		cout << "�����ɸѡ���������" << Reg_points.size() << "\n";
		cout << "�����Ŀ��������" << transformFilter->GetOutput()->GetNumberOfPoints() << "\n";
		cout << "---------ICP Start---------------\n";
		UQICPRegister MyICP;
		MyICP.InputSourceData(Reg_points);
		MyICP.InputTargetData(transformFilter->GetOutput());
		vtkSmartPointer<vtkMatrix4x4> Matrix_MyICP = vtkSmartPointer<vtkMatrix4x4>::New();//
		MyICP.GetICPResult(Matrix_MyICP);
		Matrix_MyICP->Invert();////////////��¼��ʼICP��׼���(��->С)
		double ICPDistance = MyICP.GetICPDistance();
		cout << "---------ICP END---------------\n";
		int Search_Num = 0;
		bool is_Search = true;
		Matrix<double, 3, 3> M_vec_dis;/////////////��¼ÿ���ڵ�������ۺ���
		vector<vtkSmartPointer<vtkMatrix4x4> > M_Matrixs;/////////////��¼ÿ���ڵ������Ŷ���̬���λ��
		vector<vtkSmartPointer<vtkMatrix4x4> > M_AstarMatrixs;/////////////��¼ÿ�������Ŷ���̬���ICP��׼���(��->С)
		vtkSmartPointer<vtkMatrix4x4> M_Register = vtkSmartPointer<vtkMatrix4x4>::New();///��ʼ����׼���� ����׼+��ʼICP
		vtkMatrix4x4::Multiply4x4(Matrix_MyICP, M1, M_Register);


		//double M_vec1[3], M_pos1[3];
		//double Search_ve[3], Search_po[3];
		//for (int i =0; i<3; i++)
		//{
		//	Search_ve[i] = Search_v[i];
		//	Search_po[i] = Search_p[i];
		//}
		//VecSwith(M_Register, Search_ve, M_vec1);///��ʼ���Ŷ���
		//PointSwith(M_Register, Search_po, M_pos1);
		//double  old_dis = ICPDistance;
		//while (is_Search && Search_Num <= 10)
		//{
		//	M_Matrixs.clear();
		//	M_AstarMatrixs.clear();
		//	for (int i = -1; i <= 1; i++)
		//	{
		//		for (int j = -1; j <= 1; j++)
		//		{
		//			cout << "---------ICP ����---------------\n";
		//			double dis, angle;
		//			double ind = 0.5;
		//			dis = i * ind;
		//			angle = j * ind * 2;
		//			vtkSmartPointer<vtkTransform> Per_Transform = vtkSmartPointer<vtkTransform>::New();
		//			Per_Transform->Translate(M_pos1);
		//			Per_Transform->RotateWXYZ(angle, M_vec1);
		//			Per_Transform->Translate(-M_pos1[0], -M_pos1[1], -M_pos1[2]);
		//			Per_Transform->Translate(dis * M_vec1[0], dis * M_vec1[1], dis  * M_vec1[2]);
		//			vtkSmartPointer<vtkMatrix4x4> M_Perturb = vtkSmartPointer<vtkMatrix4x4>::New();/////////��¼�Ŷ��任
		//			M_Perturb = Per_Transform->GetMatrix();
		//			vtkSmartPointer<vtkMatrix4x4> M_new = vtkSmartPointer<vtkMatrix4x4>::New();/////////////��¼ģ�͵�ǰ�Ŷ���λ��
		//			vtkMatrix4x4::Multiply4x4(M_Perturb, M_Register, M_new);
		//			///ִ��ICP
		//			M_Matrixs.push_back(M_new);
		//			//�����ģ����λ������
		//			vtkSmartPointer<vtkTransform> Cur_Transform = vtkSmartPointer<vtkTransform>::New();
		//			Cur_Transform->SetMatrix(M_new);
		//			vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		//			transformFilter->SetInputData(vtkpolydata_Bone);
		//			transformFilter->SetTransform(Cur_Transform);
		//			transformFilter->Update();
		//			UQICPRegister CurICP;
		//			CurICP.InputSourceData(Reg_points);
		//			CurICP.InputTargetData(transformFilter->GetOutput());
		//			vtkSmartPointer<vtkMatrix4x4> Matrix_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
		//			CurICP.GetICPResult(Matrix_CurICP);
		//			double CurICPDistance = CurICP.GetICPDistance();
		//			M_vec_dis(i + 1, j + 1) = CurICPDistance;
		//			Matrix_CurICP->Invert();
		//			M_AstarMatrixs.push_back(Matrix_CurICP);/////////////��¼��ǰ�Ŷ�λ���µ�ICP��׼���
		//		}
		//	}
		//	MatrixXd::Index minRow, minCol;
		//	double min = M_vec_dis.minCoeff(&minRow, &minCol);////////////��ȡ��С���ۺ���������
		//	if (min < old_dis && Search_Num < 10)
		//	{
		//		old_dis = min;
		//		vector<int> index(2);
		//		index[0] = minRow - 1;
		//		index[1] = minCol - 1;
		//		vtkSmartPointer<vtkMatrix4x4> M_CurMaxis = vtkSmartPointer<vtkMatrix4x4>::New();
		//		vtkSmartPointer<vtkMatrix4x4> M_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
		//		M_CurMaxis->DeepCopy(M_Matrixs[3 * minRow + minCol]);
		//		M_CurICP->DeepCopy(M_AstarMatrixs[3 * minRow + minCol]);
		//		vtkMatrix4x4::Multiply4x4(M_CurICP, M_CurMaxis, M_Register);////////////��ȡ��׼���󣬼�������
		//		VecSwith(M_Register, Search_ve, M_vec1);///�����Ŷ���
		//		PointSwith(M_Register, Search_po, M_pos1);
		//	}
		//	else
		//	{
		//		qDebug() << "Final Meandistance: " << old_dis << endl;
		//		is_Search = false;/////////����������ֹͣ����
		//	}
		//	Search_Num++;
		//}
		
		Matrix_boneToDRO->DeepCopy(M_Register);
		patient_Actor->SetUserMatrix(M1);
		actor_target->SetUserMatrix(M_Register);
		patientRender->AddActor(actor_target);
		patientRender->AddActor(patient_Actor);
		patientRender->GetRenderWindow()->Render();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				qDebug() << "M_Register" << M_Register->GetElement(i, j) << "    " << endl;
			}
		}
		qDebug() << QStringLiteral("�����ICP��׼��");
	}

	bool ifreadfromtxt=false;
	if (ifreadfromtxt) {
		//����ɼ�������

	//����ȡ�ı�������ת��ΪvtkPolyData
		vector<double> p_x, p_y, p_z;
		vtkSmartPointer<vtkPoints> vtkpoints_Bone = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> vtkcell_Bone = vtkSmartPointer<vtkCellArray>::New();

		QString file = "./predict/BoneCould_data3.txt";
		ifstream fin(file.toStdString(), ios::in);
		vector<double> collect_sp(3);
		double pos_pixel[3];
		while (!fin.eof())
		{
			fin >> pos_pixel[0] >> pos_pixel[1] >> pos_pixel[2];
			p_x.push_back(pos_pixel[0]);
			p_y.push_back(pos_pixel[1]);
			p_z.push_back(pos_pixel[2]);
		}
		fin.close();
		for (int i = 0; i < p_x.size(); i++)
		{
			vtkIdType pid1[1], pid2[1];
			pid1[0] = vtkpoints_Bone->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
			vtkcell_Bone->InsertNextCell(1, pid1);
		}

		vtkSmartPointer<vtkPolyData> vtkpolydata_Bone = vtkSmartPointer<vtkPolyData>::New();
		vtkpolydata_Bone->SetPoints(vtkpoints_Bone);
		vtkpolydata_Bone->SetVerts(vtkcell_Bone);
		//���ӻ�_Bone��

		vtkSmartPointer<vtkPolyDataMapper> vtkmapper_Bone = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkmapper_Bone->SetInputData(vtkpolydata_Bone);
		vtkSmartPointer<vtkActor> vtkactor_Bone = vtkSmartPointer<vtkActor>::New();
		vtkactor_Bone->SetMapper(vtkmapper_Bone);
		vtkactor_Bone->GetProperty()->SetColor(0 / 255, 0 / 255, 255 / 255);
		patientRender->AddActor(vtkactor_Bone);
		vtkactor_Bone->SetUserMatrix(M1);
		Matrix_coarse->Invert();
		//vtkactor_Bone->SetUserMatrix(Matrix_coarse);

		//��vtkPolyData�������ݾ�������׼�������
		vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
		Transform->SetMatrix(M1);
		vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		transformFilter->SetInputData(vtkpolydata_Bone);
		transformFilter->SetTransform(Transform);
		transformFilter->Update();

		//����ɼ���US����-----------
		int Num_DROpoints = US_points.size();
		if (Num_DROpoints < 10000)
		{
			return;
		}
		vector<vector<double>> Reg_points;
		//////���Ȼ�ȡ10000���ɼ��ĵ�
		vector<int> insertnum;
		NumSelect(Num_DROpoints, 10000, insertnum);
		for (int i = 0; i < insertnum.size(); i++)
		{
			int index = insertnum[i];
			vector<double> sp(3);
			sp[0] = US_points[index][0];
			sp[1] = US_points[index][1];
			sp[2] = US_points[index][2];
			Reg_points.push_back(sp);
		}
	}


}
void AR::inputPointClouds()
{
	if (L1SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(0), us_data_group->getChild(0), Search_pos_sigle[0], Search_vec_sigle[0], Matrix_splineToDRO[0]); }
	if (L2SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(1), us_data_group->getChild(1), Search_pos_sigle[1], Search_vec_sigle[1], Matrix_splineToDRO[1]); }
	if (L3SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(2), us_data_group->getChild(2), Search_pos_sigle[2], Search_vec_sigle[2], Matrix_splineToDRO[2]); }
	if (L4SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(3), us_data_group->getChild(3), Search_pos_sigle[3], Search_vec_sigle[3], Matrix_splineToDRO[3]); }
	if (L5SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(4), us_data_group->getChild(4), Search_pos_sigle[4], Search_vec_sigle[4], Matrix_splineToDRO[4]); }
	if (L6SelectAction->isChecked()) { StartPointClouds(stl_data_group->getChild(5), us_data_group->getChild(5), Search_pos_sigle[5], Search_vec_sigle[5], Matrix_splineToDRO[5]); }
		
}
//------------------------------------̽��ɵ���׼----------------------------------------
void AR::coarse_GetPoint()
{
	if (Mark_Number < 3)
	{
		if (temp0Flag && temp2Flag)
		{
			//����ܵ�ǰλ��
			vtkSmartPointer<vtkMatrix4x4> Matrix_DROToWorld = vtkSmartPointer<vtkMatrix4x4>::New();
			NDIDataToMat(temp2, Matrix_DROToWorld);
			Matrix_DROToWorld->Invert();
			double Position_Probe[3] = { temp0[4] ,temp0[5] ,temp0[6] };
			//���ӻ��ɼ���
			vtkSmartPointer<vtkMatrix4x4> Matrix_Point = vtkSmartPointer<vtkMatrix4x4>::New();//����
			vtkSmartPointer<vtkActor> Ball_Actor = vtkSmartPointer<vtkActor>::New();
			NDIDataToMat(temp0, Matrix_Point);
			Ball_Actor->SetMapper(Ball_mapper);
			Ball_Actor->SetUserMatrix(Matrix_Point);
			Ball_Actor->GetProperty()->SetColor(0 / 255, 255 / 255, 0 / 255);
			ball_actors.push_back(Ball_Actor);
			patientRender->AddActor(Ball_Actor);
			///ע��ֲ������λ��
			double out[3];
			PointSwith(Matrix_DROToWorld, Position_Probe, out);
			vector<double> out_date(out, out + 3);
			GatherPoints_mark.push_back(out_date);//���ڴ���׼��������-�����˵һһ��Ӧ
			Mark_Number++;
			qDebug() << QStringLiteral("�Ѳɼ��ɹ���");
		}
		else
		{
			qDebug() << QStringLiteral("̽���������ܲ��ɼ���");
		}
	}
	else {
		Mark_Number = 0;
		GatherPoints_mark.clear();
		for (int i = 0; i < 3; i++)
		{
			patientRender->RemoveActor(ball_actors[i]);
		}
		ball_actors.clear();
		qDebug() << QStringLiteral("�����¿�ʼ�ɼ���");
	}

}
void AR::coarse_Resiger()
{
	if (Mark_Number == 3)
	{
		double GatherPoints_mark_0[3];	double GatherPoints_mark_1[3];	double GatherPoints_mark_2[3];
		for (int i = 0; i < 3; ++i) {
			GatherPoints_mark_0[i] = GatherPoints_mark[0][i];
			GatherPoints_mark_1[i] = GatherPoints_mark[1][i];
			GatherPoints_mark_2[i] = GatherPoints_mark[2][i];
		}
		Threepointregister(PresetPoints_mark_0, PresetPoints_mark_1, PresetPoints_mark_2, GatherPoints_mark_0, GatherPoints_mark_1, GatherPoints_mark_2, Matrix_coarse);

		//
		ofstream outFile;
		outFile.open("./data_cal/data/cra.txt", ios::out | ios::trunc);
		
			outFile <<  "[";
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					if (k == 3 && j == 3)
					{
						outFile << Matrix_coarse->GetElement(j, k);
					}
					else {
						outFile << Matrix_coarse->GetElement(j, k) << ",";
					}
				}
			}
			outFile << "]";
		outFile.close();
		//


		vtkSmartPointer<vtkPolyDataMapper> CoarseBone_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		CoarseBone_mapper->SetInputConnection(patient_spline_reader->GetOutputPort());
		CoarseBone_Actor->SetMapper(CoarseBone_mapper);
		qDebug() << QStringLiteral("�ѽ��д���׼��");
		CoarseBone_Actor->GetProperty()->SetOpacity(0.9);
		isCoarseReg = true;
	}
	else { qDebug() << QStringLiteral("���ٲ�ȡ�����㣡"); }

}
void AR::PointCloudRegister()
{
	int Num_DROpoints = DRO_points.size();
	if (Num_DROpoints < 500)
	{
		//box.setMessage(QStringLiteral("ʰȡ�����Ӧ����500!!"), false);
		return;
	}
	//�Ƴ���ʾ�ɼ��ĵ�
	patientRender->RemoveActor(icp_point_actor);
	vector<vector<double>> Reg_points;
	//////���Ȼ�ȡ500���ɼ��ĵ�
	vector<int> insertnum;
	NumSelect(Num_DROpoints, 500, insertnum);
	for (int i = 0; i < insertnum.size(); i++)
	{
		int index = insertnum[i];
		vector<double> sp(3);
		sp[0] = DRO_points[index][0];
		sp[1] = DRO_points[index][1];
		sp[2] = DRO_points[index][2];
		Reg_points.push_back(sp);//ѡȡ��500����װ��Reg_points��
	}
	//double GatherPoints_mark_0[3];	double GatherPoints_mark_1[3];	double GatherPoints_mark_2[3];
	//for (int i = 0; i < 3; ++i) {
	//	GatherPoints_mark_0[i] = GatherPoints_mark[0][i];
	//	GatherPoints_mark_1[i] = GatherPoints_mark[1][i];
	//	GatherPoints_mark_2[i] = GatherPoints_mark[2][i];
	//}
	//int wight = 5;////�ظ�ϵ��-����һЩ����׼���ɼ��ĵ㵽�ܵĵ��ƿ�Reg_points��
	//for (int i = 0; i < wight; i++)
	//{
	//	vector<double> sp(3);
	//	sp[0] = GatherPoints_mark_0[0];
	//	sp[1] = GatherPoints_mark_0[1];
	//	sp[2] = GatherPoints_mark_0[2];
	//	Reg_points.push_back(sp);
	//	sp[0] = GatherPoints_mark_1[0];
	//	sp[1] = GatherPoints_mark_1[1];
	//	sp[2] = GatherPoints_mark_1[2];
	//	Reg_points.push_back(sp);
	//	sp[0] = GatherPoints_mark_2[0];
	//	sp[1] = GatherPoints_mark_2[1];
	//	sp[2] = GatherPoints_mark_2[2];
	//	Reg_points.push_back(sp);
	//}
	//����ɸѡ����ʰȡ����ӻ�
	{
		vtkSmartPointer<vtkPoints> sourcepoints = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkPolyData> sourcepoly = vtkSmartPointer<vtkPolyData>::New();
		for (int i = 0; i < Reg_points.size(); i++)
		{
			vtkIdType pid[1];
			pid[0] = sourcepoints->InsertNextPoint(Reg_points[i][0], Reg_points[i][1], Reg_points[i][2]);
			cell->InsertNextCell(1, pid);
		}
		sourcepoly->SetPoints(sourcepoints);
		sourcepoly->SetVerts(cell);
		vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper1->SetInputData(sourcepoly);
		pointsActor->SetMapper(mapper1);
		pointsActor->GetProperty()->SetColor(0, 1, 0);
		pointsActor->GetProperty()->SetPointSize(5.0);
	}
	//���ڽ��ʱ�־�Ĵ���׼

	// ��ȡ��ȡ�� PolyData ���ݣ�����patient_spline_reader��ͷ�ĵ������ݣ�
	vtkSmartPointer<vtkPolyData> inputPolyData = patient_spline_reader->GetOutput();

	// ���㷨����
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData(inputPolyData);
	normals->SplittingOff(); // �رղ��
	normals->ComputePointNormalsOn();
	normals->ComputeCellNormalsOff();
	normals->Update();
	// �������� PolyData ���ݸ������ vtkPolyData ����
	vtkSmartPointer<vtkPolyData> Bonedata = normals->GetOutput();
	// ������ʹ�� processedPolyData ������к����Ĳ���
	vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	Loop->SetInputData(Bonedata);
	Loop->SetNumberOfSubdivisions(1);
	Loop->Update();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points = Loop->GetOutput()->GetPoints();//points�д������е����ͷ�ĵ�������
	//��ȡ�ɼ��������㸽���Ĺ�ͷ�ĵ�������������׼Ч��
	vector<double> p_x, p_y, p_z;
	qDebug() << QStringLiteral("���������") << points->GetNumberOfPoints();
	for (int i = 0; i < points->GetNumberOfPoints(); i++)
	{
		double pi[3];
		points->GetPoint(i, pi);
		double norm = sqrt((pi[0] - PresetPoints_mark_0[0]) * (pi[0] - PresetPoints_mark_0[0]) + (pi[1] - PresetPoints_mark_0[1]) * (pi[1] - PresetPoints_mark_0[1]) + (pi[2] - PresetPoints_mark_0[2]) * (pi[2] - PresetPoints_mark_0[2]));
		double norm1 = sqrt((pi[0] - PresetPoints_mark_1[0]) * (pi[0] - PresetPoints_mark_1[0]) + (pi[1] - PresetPoints_mark_1[1]) * (pi[1] - PresetPoints_mark_1[1]) + (pi[2] - PresetPoints_mark_1[2]) * (pi[2] - PresetPoints_mark_1[2]));
		double norm2 = sqrt((pi[0] - PresetPoints_mark_2[0]) * (pi[0] - PresetPoints_mark_2[0]) + (pi[1] - PresetPoints_mark_2[1]) * (pi[1] - PresetPoints_mark_2[1]) + (pi[2] - PresetPoints_mark_2[2]) * (pi[2] - PresetPoints_mark_2[2]));
		if (norm < 70 || norm1 < 40 || norm2 < 40)
		{
			p_x.push_back(pi[0]);
			p_y.push_back(pi[1]);
			p_z.push_back(pi[2]);
		}
	}//��points��ɸѡ��������־�㸽�������ڵĵ�װ��points1
	////����ȡ�ı�������ת��ΪvtkPolyData
	vtkSmartPointer<vtkPoints> points1 = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cell1 = vtkSmartPointer<vtkCellArray>::New();
	for (int i = 0; i < p_x.size(); i++)
	{
		vtkIdType pid1[1], pid2[1];
		pid1[0] = points1->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
		cell1->InsertNextCell(1, pid1);
	}
	vtkSmartPointer<vtkPolyData> polydata1 = vtkSmartPointer<vtkPolyData>::New();
	polydata1->SetPoints(points1);
	polydata1->SetVerts(cell1);
	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(polydata1);
	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);
	//actor->GetProperty()->SetColor(0 / 255, 255.0 / 255, 0 / 255);
	//patientRender->AddActor(actor);

	//vtkSmartPointer<vtkPolyData> Bonedata = vtkSmartPointer<vtkPolyData>::New();
	//Bonedata = (patient_spline_reader->GetOutput());
	//vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	//Loop->SetInputData(Bonedata);
	//Loop->SetNumberOfSubdivisions(1);
	//Loop->Update();
	//vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	//points = Loop->GetOutput()->GetPoints();
	////////��ȡ�ɹǽ��˼�Զ�˹Ǳ�����������Ч��
	//vector<double> p_x, p_y, p_z;
	//for (int i = 0; i < points->GetNumberOfPoints(); i++)
	//{
	//	double pi[3];
	//	points->GetPoint(i, pi);
	//	double norm = sqrt((pi[0] - PresetPoints_mark_0[0]) * (pi[0] - PresetPoints_mark_0[0]) + (pi[1] - PresetPoints_mark_0[1]) * (pi[1] - PresetPoints_mark_0[1]) + (pi[2] - PresetPoints_mark_0[2]) * (pi[2] - PresetPoints_mark_0[2]));
	//	double norm1 = sqrt((pi[0] - PresetPoints_mark_1[0]) * (pi[0] - PresetPoints_mark_1[0]) + (pi[1] - PresetPoints_mark_1[1]) * (pi[1] - PresetPoints_mark_1[1]) + (pi[2] - PresetPoints_mark_1[2]) * (pi[2] - PresetPoints_mark_1[2]));
	//	double norm2 = sqrt((pi[0] - PresetPoints_mark_2[0]) * (pi[0] - PresetPoints_mark_2[0]) + (pi[1] - PresetPoints_mark_2[1]) * (pi[1] - PresetPoints_mark_2[1]) + (pi[2] - PresetPoints_mark_2[2]) * (pi[2] - PresetPoints_mark_2[2]));
	//	if (norm < 150 || norm1 < 50 || norm2 < 50)
	//	{
	//		p_x.push_back(pi[0]);
	//		p_y.push_back(pi[1]);
	//		p_z.push_back(pi[2]);

	//	}
	//}
	////////����ȡ�ı�������ת��ΪvtkPolyData
	//vtkSmartPointer<vtkPoints> points1 = vtkSmartPointer<vtkPoints>::New();
	//vtkSmartPointer<vtkCellArray> cell1 = vtkSmartPointer<vtkCellArray>::New();
	//for (int i = 0; i < p_x.size(); i++)
	//{
	//	vtkIdType pid1[1], pid2[1];
	//	pid1[0] = points1->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
	//	cell1->InsertNextCell(1, pid1);
	//}

	//vtkSmartPointer<vtkPolyData> polydata1 = vtkSmartPointer<vtkPolyData>::New();
	//polydata1->SetPoints(points1);
	//polydata1->SetVerts(cell1);
	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(polydata1);
	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//////��vtkPolyData�������ݾ�������׼�������
	vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
	Transform->SetMatrix(Matrix_coarse);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(polydata1);
	transformFilter->SetTransform(Transform);
	transformFilter->Update();
	//progressDlg->setValue(10);
	qDebug() << "---------ICP Start---------------" << endl;
	UQICPRegister MyICP;
	MyICP.InputSourceData(Reg_points);
	MyICP.InputTargetData(transformFilter->GetOutput());
	vtkSmartPointer<vtkMatrix4x4> Matrix_MyICP = vtkSmartPointer<vtkMatrix4x4>::New();//
	MyICP.GetICPResult(Matrix_MyICP);
	Matrix_MyICP->Invert();////////////��¼��ʼICP��׼���(��->С)
	double ICPDistance = MyICP.GetICPDistance();
	qDebug() << "---------ICP End---------------" << endl;
	//progressDlg->setValue(20);
	///////////////////////////////////////����Ѱ�ŷ������
	int Search_Num = 0;
	bool is_Search = true;
	Matrix<double, 3, 3> M_vec_dis;/////////////��¼ÿ���ڵ�������ۺ���
	vector<vtkSmartPointer<vtkMatrix4x4> > M_Matrixs;/////////////��¼ÿ���ڵ������Ŷ���̬���λ��
	vector<vtkSmartPointer<vtkMatrix4x4> > M_AstarMatrixs;/////////////��¼ÿ�������Ŷ���̬���ICP��׼���(��->С)
	vtkSmartPointer<vtkMatrix4x4> M_Register = vtkSmartPointer<vtkMatrix4x4>::New();///��ʼ����׼���� ����׼+��ʼICP
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "111maxt_is" << Matrix_MyICP->GetElement(i, j) << "    " << endl;
		}
	}
	vtkMatrix4x4::Multiply4x4(Matrix_MyICP, Matrix_coarse, M_Register);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "222maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	std::cout << "Extracted "<< " cells." << "isSelected��" << std::endl;
	double M_vec1[3], M_pos1[3];

	VecSwith(M_Register, Search_vec, M_vec1);///��ʼ���Ŷ���
	PointSwith(M_Register, Search_pos, M_pos1);

	double  old_dis = ICPDistance;
	while (is_Search && Search_Num <= 10)
	{
		M_Matrixs.clear();
		M_AstarMatrixs.clear();
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				double dis, angle;
				double ind = 0.5;
				dis = i * ind;
				angle = j * ind * 2;
				vtkSmartPointer<vtkTransform> Per_Transform = vtkSmartPointer<vtkTransform>::New();

				Per_Transform->Translate(M_pos1);
				Per_Transform->RotateWXYZ(angle, M_vec1);
				Per_Transform->Translate(-M_pos1[0], -M_pos1[1], -M_pos1[2]);
				Per_Transform->Translate(dis * M_vec1[0], dis * M_vec1[1], dis  * M_vec1[2]);
				vtkSmartPointer<vtkMatrix4x4> M_Perturb = vtkSmartPointer<vtkMatrix4x4>::New();/////////��¼�Ŷ��任
				M_Perturb = Per_Transform->GetMatrix();
				vtkSmartPointer<vtkMatrix4x4> M_new = vtkSmartPointer<vtkMatrix4x4>::New();/////////////��¼ģ�͵�ǰ�Ŷ���λ��
				vtkMatrix4x4::Multiply4x4(M_Perturb, M_Register, M_new);
				///ִ��ICP
				M_Matrixs.push_back(M_new);
				//�����ģ����λ������
				vtkSmartPointer<vtkTransform> Cur_Transform = vtkSmartPointer<vtkTransform>::New();
				Cur_Transform->SetMatrix(M_new);
				vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
				transformFilter->SetInputData(polydata1);
				transformFilter->SetTransform(Cur_Transform);
				transformFilter->Update();
				UQICPRegister CurICP;
				CurICP.InputSourceData(Reg_points);
				CurICP.InputTargetData(transformFilter->GetOutput());
				vtkSmartPointer<vtkMatrix4x4> Matrix_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
				CurICP.GetICPResult(Matrix_CurICP);
				double CurICPDistance = CurICP.GetICPDistance();
				M_vec_dis(i + 1, j + 1) = CurICPDistance;
				Matrix_CurICP->Invert();
				M_AstarMatrixs.push_back(Matrix_CurICP);/////////////��¼��ǰ�Ŷ�λ���µ�ICP��׼���
			}
		}
		MatrixXd::Index minRow, minCol;
		double min = M_vec_dis.minCoeff(&minRow, &minCol);////////////��ȡ��С���ۺ���������
		if (min < old_dis && Search_Num < 10)
		{
			old_dis = min;
			vector<int> index(2);
			index[0] = minRow - 1;
			index[1] = minCol - 1;
			vtkSmartPointer<vtkMatrix4x4> M_CurMaxis = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> M_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
			M_CurMaxis->DeepCopy(M_Matrixs[3 * minRow + minCol]);
			M_CurICP->DeepCopy(M_AstarMatrixs[3 * minRow + minCol]);
			vtkMatrix4x4::Multiply4x4(M_CurICP, M_CurMaxis, M_Register);////////////��ȡ��׼���󣬼�������

			VecSwith(M_Register, Search_vec, M_vec1);///�����Ŷ���
			PointSwith(M_Register, Search_pos, M_pos1);
		}
		else
		{
			qDebug() << "Final Meandistance: " << old_dis << endl;
			is_Search = false;/////////����������ֹͣ����

		}
		Search_Num++;
	}
	//���ģ������Ⱦ
	//progressDlg->setValue(100);
	//progressDlg->close();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "333maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	Matrix_boneToFarDRO->DeepCopy(M_Register);
	patient_Actor->SetUserMatrix(Matrix_boneToFarDRO);
	patient_Actor->GetProperty()->SetColor(1.0, 1.0, 0);
	patient_Actor->GetProperty()->SetOpacity(1.0);
	patientRender->AddActor(patient_Actor);
	patientRender->RemoveActor(CoarseBone_Actor);
	//patientRender->AddActor(pointsActor);
	patientRender->ResetCamera();
	patientRender->GetRenderWindow()->Render();
	QString text = "Final iteration distance: ";
	QString str = QString::number(old_dis, 'f', 3);
	QString text2 = "mm";
	text = text + str;
	text = text + text2;
	//box.setMessage(text, true);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "M_Register_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	isICPReg = true;
	qDebug() << QStringLiteral("�����ICP��׼��");
}
void AR::ICPCouldRecord()
{
	if (isICPcollection)
	{
		isICPcollection = false;
	}
	else if (!isICPcollection)
	{
		isICPcollection = true;
	}
}
void AR::ICP_GetPoint()
{
	if (temp0Flag&&temp2Flag)
	{
		process_points1.push_back(temp0);
		int num = process_points1.size();
		int recordnum = icp_points.size();

		if (recordnum > 2)
		{
			double dis = sqrt((process_points1[num - 1][tx] - icp_points[recordnum - 1][0])*(process_points1[num - 1][tx] - icp_points[recordnum - 1][0]) +
				(process_points1[num - 1][ty] - icp_points[recordnum - 1][1])*(process_points1[num - 1][ty] - icp_points[recordnum - 1][1]) +
				(process_points1[num - 1][tz] - icp_points[recordnum - 1][2])*(process_points1[num - 1][tz] - icp_points[recordnum - 1][2]));
			if (dis < 0.5)
			{
				return;
			}
			qDebug() << QStringLiteral("��������") << dis;
		}
		qDebug() << QStringLiteral("���������") << recordnum;
		vector<double> data;
		data.push_back(temp0[4]);
		data.push_back(temp0[5]);
		data.push_back(temp0[6]);
		icp_points.push_back(data);
		//����ܵ�ǰλ��
		vtkSmartPointer<vtkMatrix4x4> Matrix_DROToWorld = vtkSmartPointer<vtkMatrix4x4>::New();
		NDIDataToMat(temp2, Matrix_DROToWorld);
		Matrix_DROToWorld->Invert();
		double P_probe[3] = { temp0[4] ,temp0[5] ,temp0[6] };
		///ע��ֲ������λ��
		double sp[3];
		PointSwith(Matrix_DROToWorld, P_probe, sp);
		vector<double> data1;
		data1.push_back(sp[0]);
		data1.push_back(sp[1]);
		data1.push_back(sp[2]);
		DRO_points.push_back(data1);

		//���������ת������-�������ֱ��ɼ��Ĳ�λ

		////���������ת������-�������ֱ��ɼ��Ĳ�λ
		//double  vec_x[3], vec_y[3], vec_z[3];
		//double cam_focal[3], cam_pos1[3], cam_pos2[3];
		//double in[4], out[4];
		//in[0] = t_x[0];
		//in[1] = t_x[1];
		//in[2] = t_x[2];
		//in[3] = 0;
		//Matrix_current->MultiplyPoint(in, out);
		//vec_x[0] = out[0];
		//vec_x[1] = out[1];
		//vec_x[2] = out[2];
		//in[0] = t_y[0];
		//in[1] = t_y[1];
		//in[2] = t_y[2];
		//in[3] = 0;
		//Matrix_current->MultiplyPoint(in, out);
		//vec_y[0] = out[0];
		//vec_y[1] = out[1];
		//vec_y[2] = out[2];
		//in[0] = t_z[0];
		//in[1] = t_z[1];
		//in[2] = t_z[2];
		//in[3] = 0;
		//Matrix_current->MultiplyPoint(in, out);
		//vec_z[0] = out[0];
		//vec_z[1] = out[1];
		//vec_z[2] = out[2];
		//in[0] = Mtibia_knee_center_point[0];
		//in[1] = Mtibia_knee_center_point[1];
		//in[2] = Mtibia_knee_center_point[2];
		//in[3] = 1;
		//Matrix_current->MultiplyPoint(in, out);
		//cam_focal[0] = out[0] - 60 * vec_x[0];
		//cam_focal[1] = out[1] - 60 * vec_x[1];
		//cam_focal[2] = out[2] - 60 * vec_x[2];
		//cam_pos1[0] = cam_focal[0] + 500 * vec_z[0];
		//cam_pos1[1] = cam_focal[1] + 500 * vec_z[1];
		//cam_pos1[2] = cam_focal[2] + 500 * vec_z[2];
		//SetCamera(regRender, cam_pos1, cam_focal, vec_x);

		//���������ת������-�������ֱ��ɼ��Ĳ�λ
		int length = icp_points.size();
		//QString text = "Number of points: " + QString::number(length);
		qDebug() << QStringLiteral("���������") << length;
		//ui.openGLWidget_11->SetNumText(text.toStdString().c_str(), 0.1, 0.8, 0.3, 0.1, regRender);
		vtkSmartPointer<vtkPoints> pts = vtkSmartPointer<vtkPoints>::New();
		for (int i = 0; i < length; ++i)
		{
			pts->InsertNextPoint(icp_points[i][0], icp_points[i][1], icp_points[i][2]);
		}
		vtkSmartPointer<vtkPolyData> polygonPolyData = vtkSmartPointer<vtkPolyData>::New();
		polygonPolyData->SetPoints(pts);
		//����ͼԪ����
		vtkSmartPointer<vtkVertexGlyphFilter>  sourceGlyph = vtkSmartPointer<vtkVertexGlyphFilter>::New();
		sourceGlyph->SetInputData(polygonPolyData);
		sourceGlyph->Update();
		/*vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();*/

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(sourceGlyph->GetOutput());
		icp_point_actor->SetMapper(mapper);
		icp_point_actor->GetProperty()->SetColor(0, 255, 0);
		icp_point_actor->GetProperty()->SetPointSize(5.0);
		patientRender->AddActor(icp_point_actor);
		patientRender->GetRenderWindow()->Render();
	}
}
//----------------------------��Ⱦ����----------------------------
void AR::RenderTool()
{
	if (isConnect)
	{
		//Probe��0����0-6��
		TrackTool(0, temp0Flag, temp0);
		////Cailcube��1����7-13��
		TrackTool(1, temp1Flag, temp1);
		////Bone760��2����14-20��
		TrackTool(2, temp2Flag, temp2);
		////Guide��3/4����21-27��
		//if (GuideIndex == 0)
		//{
		//	TrackTool(4, temp4Flag, temp4);
		//}
		//else if (GuideIndex == 1)
		//{
			TrackTool(3, temp3Flag, temp3);
		//}
		////Drill��5����28-34��
		TrackTool(5, temp5Flag, temp5);


		if (isCoarseReg && !isICPReg)
		{
			if (temp2Flag)
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Tool = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkSmartPointer<vtkMatrix4x4> Matrix_CoarseBone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				NDIDataToMat(temp2, Matrix_Tool);
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_coarse, Matrix_CoarseBone);
				CoarseBone_Actor->SetUserMatrix(Matrix_CoarseBone);
				patientRender->AddActor(CoarseBone_Actor);
			}
			else { patientRender->RemoveActor(CoarseBone_Actor); }
		}
		if (isICPReg)
		{
			patientRender->RemoveActor(CoarseBone_Actor);
			if (temp2Flag)
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Tool = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkSmartPointer<vtkMatrix4x4> Matrix_ICPBone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkSmartPointer<vtkMatrix4x4> Matrix_ICPPoint = vtkSmartPointer<vtkMatrix4x4>::New();//����
				NDIDataToMat(temp2, Matrix_Tool);
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_boneToFarDRO, Matrix_ICPBone);
				patient_Actor->SetUserMatrix(Matrix_ICPBone);
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_boneToFarDRO, Matrix_ICPPoint);
				pointsActor->SetUserMatrix(Matrix_ICPPoint);
				patientRender->AddActor(patient_Actor);
				patientRender->AddActor(pointsActor);
			}
			else {
				patientRender->RemoveActor(patient_Actor); patientRender->RemoveActor(pointsActor);
			}
		}
		if (isICPcollection)
		{
			ICP_GetPoint();
		}
		if (temp5Flag)
		{
			vtkSmartPointer<vtkMatrix4x4> Matrix_USTempToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_USToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			NDIDataToMat(temp5, Matrix_USTempToworld);
			vtkMatrix4x4::Multiply4x4(Matrix_USTempToworld, Matrix_USToTemp, Matrix_USToworld);
			USActor1->SetUserMatrix(Matrix_USToworld);
			patientRender->AddActor(USActor1);
		}
		else {
			patientRender->RemoveActor(USActor1);
		}

		//US
		if (ui.frame_333->isVisible())
		{			
			if (temp2Flag && temp5Flag)
			{
				if (isUS_collect)
				{
					if (ui.farme_UsSeg->isVisible())
					{
						tibiaToWorld.push_back(temp2);
						USToWorld.push_back(temp5);
					}
					imageCapture->capture();
				}
			}
			if (temp3Flag && temp5Flag)
			{
				if (isUS_collect)
				{
					if (ui.frame_UsCali->isVisible())
					{
						UsCaliPhantomToWorld.push_back(temp3);
						UsCailUSProbeToWorld.push_back(temp5);
						isUS_collect = false;
					}
					imageCapture->capture();
				}
			}
			//us_probe_Phantom�궨
			if (temp0Flag && temp3Flag) {
				if (P_Record_USPhantomCali)
				{
					UsCailProbeToWorld.push_back(temp0);
					UsCaliPhantomToWorld.push_back(temp3);

					vtkSmartPointer<vtkMatrix4x4> Matrix_Point = vtkSmartPointer<vtkMatrix4x4>::New();//����
					vtkSmartPointer<vtkActor> Ball_Actor = vtkSmartPointer<vtkActor>::New();
					NDIDataToMat(temp0, Matrix_Point);
					Ball_Actor->SetMapper(Ball_mapper);
					Ball_Actor->SetUserMatrix(Matrix_Point);
					Ball_Actor->GetProperty()->SetColor(0 / 255, 255 / 255, 0 / 255);
					ball_actors.push_back(Ball_Actor);
					patientRender->AddActor(Ball_Actor);
				}
				P_Record_USPhantomCali = false;
			}
		}
		//US
		patientRender->GetRenderWindow()->Render();
	}
}
void AR::TrackTool(int Toolindex, bool tempFlag, vector<double> temp)
{
	vtkSmartPointer<vtkMatrix4x4> Matrix_Tool = vtkSmartPointer<vtkMatrix4x4>::New();//����
	if (tempFlag&&temp.size() >= 7)
	{
		NDIDataToMat(temp, Matrix_Tool);
		if (Toolindex == 2)
		{
			vtkSmartPointer<vtkMatrix4x4> Matrix_Tool = vtkSmartPointer<vtkMatrix4x4>::New();//����

			NDIDataToMat(temp2, Matrix_Tool);
			if (L1SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[0], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(0)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(0));
			}
			if (L2SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[1], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(1)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(1));
			}
			if (L3SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[2], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(2)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(2));
			}
			if (L4SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[3], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(3)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(3));
			}
			if (L5SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[4], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(4)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(4));
			}
			if (L6SelectAction->isChecked())
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_Bone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_splineToDRO[5], Matrix_Bone);//�궨�ú��
				stl_data_group->getChild(5)->SetUserMatrix(Matrix_Bone);
				patientRender->AddActor(stl_data_group->getChild(5));
			}
			//vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_BonetoDRO, Matrix_ICPBone);//�궨�ú��
			if (statenumber == 4)
			{
				vtkSmartPointer<vtkMatrix4x4> Matrix_ICPBone = vtkSmartPointer<vtkMatrix4x4>::New();//����
				vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_show, Matrix_ICPBone);
				patient_Actor->SetUserMatrix(Matrix_ICPBone);
				patientRender->AddActor(patient_Actor);
			}
			vtkSmartPointer<vtkMatrix4x4> Matrix_ICPBone = vtkSmartPointer<vtkMatrix4x4>::New();//����
			vtkMatrix4x4::Multiply4x4(Matrix_Tool, Matrix_show, Matrix_ICPBone);
			patient_Actor->SetUserMatrix(Matrix_ICPBone);
			patientRender->AddActor(patient_Actor);
			Tool_actors[Toolindex]->SetUserMatrix(Matrix_Tool);
			patientRender->AddActor(Tool_actors[Toolindex]);
		}
		else if(Toolindex == 3)
		{
			vtkSmartPointer<vtkMatrix4x4> Matrix_cal_us = vtkSmartPointer<vtkMatrix4x4>::New();//����
			double USCaliM[16] = { 0.0140348, 0.999881, 0.00641353, 133.875,
						0.00628979, 0.00632575, -0.99996, 32.7893,
						-0.999882, 0.0140746, -0.00620026 ,-34.7835,
						0,	0,	0,	1 };

			//double USCaliM[16] = {1, 0, 0, 0,
			//						0, 1, 0, 0,
			//						0, 0, 1 ,0,
			//						0, 0, 0, 1 };
			Matrix_cal_us->DeepCopy(USCaliM);
			vtkSmartPointer<vtkMatrix4x4> Matrix_Tool_3 = vtkSmartPointer<vtkMatrix4x4>::New();//����
			NDIDataToMat(temp3, Matrix_Tool_3);
			vtkSmartPointer<vtkMatrix4x4> Matrix_ICPBone3 = vtkSmartPointer<vtkMatrix4x4>::New();//����
			vtkMatrix4x4::Multiply4x4(Matrix_Tool_3, Matrix_cal_us, Matrix_ICPBone3);
			Tool_actors[Toolindex]->SetUserMatrix(Matrix_ICPBone3);
			patientRender->AddActor(Tool_actors[Toolindex]);

			//vtkSmartPointer<vtkMatrix4x4> Matrix_Point = vtkSmartPointer<vtkMatrix4x4>::New();//����
//vtkSmartPointer<vtkActor> Ball_Actor = vtkSmartPointer<vtkActor>::New();
//double USCaliB[16] = { 1, 0, 0, 120 ,
//			0, 1, 0, -44,
//			0, 0, 1 ,55,
//			0, 0, 0, 1 };
//vtkSmartPointer<vtkMatrix4x4> Matrix_cal_BALL = vtkSmartPointer<vtkMatrix4x4>::New();//����
//Matrix_cal_BALL->DeepCopy(USCaliB);
//Ball_Actor->SetMapper(Ball_mapper);
//Ball_Actor->SetUserMatrix(Matrix_cal_BALL);
//Ball_Actor->GetProperty()->SetColor(0 / 255, 255 / 255, 0 / 255);
//patientRender->AddActor(Ball_Actor);


		}
		else
		{
			Tool_actors[Toolindex]->SetUserMatrix(Matrix_Tool);
			patientRender->AddActor(Tool_actors[Toolindex]);
			//patientRender->AddActor(patient_Actor);
		}
		Quaterniond q_odom_curr_tmp;//����һ��Eigen�����Ԫ��
		q_odom_curr_tmp.x() = temp[1];
		q_odom_curr_tmp.y() = temp[2];
		q_odom_curr_tmp.z() = temp[3];
		q_odom_curr_tmp.w() = temp[0];
		Matrix3d R_odom_curr_tmp;//����һ��Eigen���3*3����ת����
		//��Ԫ��תΪ��ת����--�ȹ�һ����תΪ��ת����
		R_odom_curr_tmp = q_odom_curr_tmp.normalized().toRotationMatrix();
		//xyz����������ת������
		Matrix4d mat_pre;
		mat_pre <<
			R_odom_curr_tmp(0, 0), R_odom_curr_tmp(0, 1), R_odom_curr_tmp(0, 2), temp[4],
			R_odom_curr_tmp(1, 0), R_odom_curr_tmp(1, 1), R_odom_curr_tmp(1, 2), temp[5],
			R_odom_curr_tmp(2, 0), R_odom_curr_tmp(2, 1), R_odom_curr_tmp(2, 2), temp[6],
			0, 0, 0, 1;
		//��ȡ��������λ��---�޸ģ�
		Matrix4d S_z;
		S_z <<
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, -1, 0,
			0, 0, 0, 1;
		Matrix4d mat;
		if (Toolindex == 2)
		{
			Eigen::Matrix4d additional_transform_matrix;
			additional_transform_matrix <<
				0.999788, -0.00661072, -0.019487, 49.2286,
				0.00683155, 0.999913, 0.0112872, -201.25,
				0.0194107, -0.0114179, 0.999746, -127.191,
				0, 0, 0, 1;
			////----------------------����׼
			//mat = mat_between_ULeft_NDIRight * mat_pre * additional_transform_matrix* S_z;//����˷�	
			//---------------------λ����׼
			mat = mat_between * mat_pre * additional_transform_matrix * S_z;//����˷�	
		}
		else {
			////----------------------����׼
			//mat = mat_between_ULeft_NDIRight * mat_pre * S_z;//����˷�
			//----------------------����׼
			mat = mat_between * mat_pre * S_z;//����˷�
		}
		Matrix3d R_odom;
		R_odom <<
			mat(0, 0), mat(0, 1), mat(0, 2),
			mat(1, 0), mat(1, 1), mat(1, 2),
			mat(2, 0), mat(2, 1), mat(2, 2);
		//��mat����ת����ת��Ϊ��Ԫ��
		Quaterniond q_odom = (Quaterniond(R_odom)).normalized();

		if (Toolindex == 3)
		{
			flagDatas[3] = Toolindex + 1;
			for (int i = 7 * 3; i < 7 * 3 + 7; i++)
			{
				ProTransf[i] = 0;
			}
			int i = 7 * 3;
			ProTransf[i] = q_odom.x();
			ProTransf[i + 1] = q_odom.y();
			ProTransf[i + 2] = q_odom.z();
			ProTransf[i + 3] = q_odom.w();
			ProTransf[i + 4] = mat(0, 3) / 1000;
			ProTransf[i + 5] = mat(1, 3) / 1000;
			ProTransf[i + 6] = mat(2, 3) / 1000;
		}
		else if (Toolindex == 4)
		{
			flagDatas[3] = Toolindex + 1;
			for (int i = 7 * 3; i < 7 * 3 + 7; i++)
			{
				ProTransf[i] = 0;
			}
			int i = 7 * 3;
			ProTransf[i] = q_odom.x();
			ProTransf[i + 1] = q_odom.y();
			ProTransf[i + 2] = q_odom.z();
			ProTransf[i + 3] = q_odom.w();
			ProTransf[i + 4] = mat(0, 3) / 1000;
			ProTransf[i + 5] = mat(1, 3) / 1000;
			ProTransf[i + 6] = mat(2, 3) / 1000;
		}
		else if (Toolindex == 5)
		{
			flagDatas[4] = Toolindex + 1;
			for (int i = 7 * 4; i < 7 * 4 + 7; i++)
			{
				ProTransf[i] = 0;
			}
			int i = 7 * 4;
			ProTransf[i] = q_odom.x();
			ProTransf[i + 1] = q_odom.y();
			ProTransf[i + 2] = q_odom.z();
			ProTransf[i + 3] = q_odom.w();
			ProTransf[i + 4] = mat(0, 3) / 1000;
			ProTransf[i + 5] = mat(1, 3) / 1000;
			ProTransf[i + 6] = mat(2, 3) / 1000;
		}
		else
		{
			flagDatas[Toolindex] = Toolindex + 1;
			for (int i = 7 * Toolindex; i < 7 * Toolindex + 7; i++)
			{
				ProTransf[i] = 0;
			}
			int i = 7 * Toolindex;
			ProTransf[i] = q_odom.x();
			ProTransf[i + 1] = q_odom.y();
			ProTransf[i + 2] = q_odom.z();
			ProTransf[i + 3] = q_odom.w();
			ProTransf[i + 4] = mat(0, 3) / 1000;
			ProTransf[i + 5] = mat(1, 3) / 1000;
			ProTransf[i + 6] = mat(2, 3) / 1000;
		}
	}
	else
	{
	if (Toolindex == 2) {
		patientRender->RemoveActor(stl_data_group->getChild(0));		patientRender->RemoveActor(stl_data_group->getChild(1));
		patientRender->RemoveActor(stl_data_group->getChild(2));		patientRender->RemoveActor(stl_data_group->getChild(3));
		patientRender->RemoveActor(stl_data_group->getChild(4));		patientRender->RemoveActor(stl_data_group->getChild(5));
	}
		patientRender->RemoveActor(Tool_actors[Toolindex]);
		if (Toolindex == 3) { flagDatas[3] = 0; }
		else if (Toolindex == 4) { flagDatas[3] = 0; }
		else if (Toolindex == 5) { flagDatas[4] = 0; }
		else { flagDatas[Toolindex] = 0; }

	}
}
void AR::SetCamera(vtkRenderer* render, double cameraposition[3], double focalposition[3], double viewup[3])
{

	render->GetActiveCamera()->SetPosition(cameraposition);    //���λ��
	render->GetActiveCamera()->SetFocalPoint(focalposition);    //����λ��
	render->GetActiveCamera()->SetViewUp(viewup);    //���Ϸ���
	render->GetActiveCamera()->SetClippingRange(0.1, 20000);
	render->GetActiveCamera()->SetParallelScale(10);
}
//�ļ��洢����
void AR::Open_Write_txt()
{	//�ɼ��ĵ�
	OutFiledate.open("./predict/UsCould_data_intemp.txt", ios::out | ios::trunc);

	//for (int i = 0; i < DRO_points.size(); i++)
	//{
	//	OutFiledate << DRO_points[i][0] << " " << DRO_points[i][1] << " " << DRO_points[i][2] << '\n';
	//}
	for (int i = 0; i < USsegment_points_inTemp.size(); i++)
	{
		OutFiledate << USsegment_points_inTemp[i][0] << " " << USsegment_points_inTemp[i][1] << " " << USsegment_points_inTemp[i][2] << '\n';
	}
	OutFiledate.close();
	//OutFiledate.open("./predict/UsCould_Maxdata.txt", ios::out | ios::trunc);
	//for (int i = 0; i < USsegment_image_Max_points.size(); i++)
	//{
	//	OutFiledate << USsegment_image_Max_points[i][0] << " " << USsegment_image_Max_points[i][1] << " " << USsegment_image_Max_points[i][2] << '\n';
	//}
	//OutFiledate.close();
}
void AR::Open_Read_txt()
{
	patient_Actor->SetVisibility(true);
	patientRender->AddActor(patient_Actor);	
	patientRender->RemoveActor(NDI_Actor);	
	vtkSmartPointer<vtkMatrix4x4> M1 = patient_Actor->GetMatrix();
	M1->DeepCopy(m);

	//����ɼ�������
	QString file = "./predict/UsCould_data_3.txt";
	ifstream fin(file.toStdString(), ios::in);
	vector<double> collect_sp(3);
	double pos_pixel[3];
	while (!fin.eof())
	{
		fin >> pos_pixel[0] >> pos_pixel[1] >> pos_pixel[2];
		collect_sp[0] = pos_pixel[0] ;
		collect_sp[1] = pos_pixel[1] ;
		collect_sp[2] = pos_pixel[2] ;
		US_points.push_back(collect_sp);
	}
	fin.close();
	vtkSmartPointer<vtkPoints> vtkpoints_US = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vtkcell_US = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> vtkpoly_US = vtkSmartPointer<vtkPolyData>::New();
	for (int i = 0; i < US_points.size(); i++)
	{
		vtkIdType pid[1];
		pid[0] = vtkpoints_US->InsertNextPoint(US_points[i][0], US_points[i][1], US_points[i][2]);
		vtkcell_US->InsertNextCell(1, pid);
	}
	vtkpoly_US->SetPoints(vtkpoints_US);
	vtkpoly_US->SetVerts(vtkcell_US);
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputData(vtkpoly_US);
	vtkSmartPointer<vtkActor> USsegment_actor = vtkSmartPointer<vtkActor>::New();
	USsegment_actor->SetMapper(mapper1);
	USsegment_actor->GetProperty()->SetColor(0, 255, 0);
	USsegment_actor->GetProperty()->SetPointSize(2.0);
	patientRender->AddActor(USsegment_actor);
	USsegment_actor->SetPickable(false);
	   	 

	//����ȡ�ı�������ת��ΪvtkPolyData
	vector<double> p_x, p_y, p_z;
	vtkSmartPointer<vtkPoints> vtkpoints_Bone = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vtkcell_Bone = vtkSmartPointer<vtkCellArray>::New();

	QString file2 = "./predict/BoneCould_data3.txt";
	ifstream fin2(file2.toStdString(), ios::in);
	vector<double> collect_sp2(3);
	double pos_pixel2[3];
	while (!fin2.eof())
	{
		fin2 >> pos_pixel2[0] >> pos_pixel2[1] >> pos_pixel2[2];
		p_x.push_back(pos_pixel2[0]);
		p_y.push_back(pos_pixel2[1]);
		p_z.push_back(pos_pixel2[2]);
	}
	fin2.close();
	for (int i = 0; i < p_x.size(); i++)
	{
		vtkIdType pid1[1], pid2[1];
		pid1[0] = vtkpoints_Bone->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
		vtkcell_Bone->InsertNextCell(1, pid1);
	}

	vtkSmartPointer<vtkPolyData> vtkpolydata_Bone = vtkSmartPointer<vtkPolyData>::New();
	vtkpolydata_Bone->SetPoints(vtkpoints_Bone);
	vtkpolydata_Bone->SetVerts(vtkcell_Bone);
	//���ӻ�_Bone��

	vtkSmartPointer<vtkPolyDataMapper> vtkmapper_Bone = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkmapper_Bone->SetInputData(vtkpolydata_Bone);
	vtkSmartPointer<vtkActor> vtkactor_Bone = vtkSmartPointer<vtkActor>::New();
	vtkactor_Bone->SetMapper(vtkmapper_Bone);
	vtkactor_Bone->GetProperty()->SetColor(0 / 255, 0 / 255, 255 / 255);
	vtkactor_Bone->SetVisibility(true);
	patientRender->AddActor(vtkactor_Bone);

	//patient_Actor->SetUserMatrix(M1);
	//vtkactor_Bone->SetUserMatrix(M1);

	patientRender->GetRenderWindow()->Render();

}

//----------------------------��ѧ����----------------------------
void AR::Vec_Add(double a[3], double b[3], double c[3])
{
	c[0] = a[0] + b[0];
	c[1] = a[1] + b[1];
	c[2] = a[2] + b[2];
}
void AR::Vec_Normalize(double Vec[3], double Normalize[3])
{
	double norm = sqrt(Vec[0] * Vec[0] + Vec[1] * Vec[1] + Vec[2] * Vec[2]);
	qDebug() << norm;
	Normalize[0] = Vec[0] / norm;
	Normalize[1] = Vec[1] / norm;
	Normalize[2] = Vec[2] / norm;
}
//�������任
void AR::Threepointregister(double sourcePoint1[3], double sourcePoint2[3], double sourcePoint3[3], double targetPoint1[3], double targetPoint2[3], double targetPoint3[3], vtkMatrix4x4* Matrix)
{
	vtkSmartPointer<vtkPoints> sourcePoints = vtkSmartPointer<vtkPoints>::New();
	sourcePoints->InsertNextPoint(sourcePoint1);
	sourcePoints->InsertNextPoint(sourcePoint2);
	sourcePoints->InsertNextPoint(sourcePoint3);
	vtkSmartPointer<vtkPoints> targetPoints = vtkSmartPointer<vtkPoints>::New();
	targetPoints->InsertNextPoint(targetPoint1);
	targetPoints->InsertNextPoint(targetPoint2);
	targetPoints->InsertNextPoint(targetPoint3);
	vtkSmartPointer<vtkLandmarkTransform> landmarkTransform = vtkSmartPointer<vtkLandmarkTransform>::New();
	landmarkTransform->SetSourceLandmarks(sourcePoints);
	landmarkTransform->SetTargetLandmarks(targetPoints);
	landmarkTransform->SetModeToRigidBody();
	landmarkTransform->Update();
	landmarkTransform->GetMatrix(Matrix);
}
void AR::VecSwith(vtkMatrix4x4* Matrix, double in[3], double out[3])
{
	double inchange[4], outchange[4];
	inchange[0] = in[0];
	inchange[1] = in[1];
	inchange[2] = in[2];
	inchange[3] = 0;
	Matrix->MultiplyPoint(inchange, outchange);
	out[0] = outchange[0];
	out[1] = outchange[1];
	out[2] = outchange[2];
}
//����*in����=out
void AR::PointSwith(vtkMatrix4x4* Matrix, double in[3], double out[3])
{
	double inchange[4], outchange[4];
	inchange[0] = in[0];
	inchange[1] = in[1];
	inchange[2] = in[2];
	inchange[3] = 1;
	Matrix->MultiplyPoint(inchange, outchange);
	out[0] = outchange[0];
	out[1] = outchange[1];
	out[2] = outchange[2];
}
void AR::NDIDataToMat(vector<double> temp, vtkMatrix4x4* Matrix)
{
	Matrix3d  r;
	UQtoMat(temp[0], temp[1], temp[2], temp[3], r);
	Matrix->SetElement(0, 0, r(0, 0));
	Matrix->SetElement(0, 1, r(0, 1));
	Matrix->SetElement(0, 2, r(0, 2));
	Matrix->SetElement(0, 3, temp[4]);
	Matrix->SetElement(1, 0, r(1, 0));
	Matrix->SetElement(1, 1, r(1, 1));
	Matrix->SetElement(1, 2, r(1, 2));
	Matrix->SetElement(1, 3, temp[5]);
	Matrix->SetElement(2, 0, r(2, 0));
	Matrix->SetElement(2, 1, r(2, 1));
	Matrix->SetElement(2, 2, r(2, 2));
	Matrix->SetElement(2, 3, temp[6]);
	Matrix->SetElement(3, 0, 0);
	Matrix->SetElement(3, 1, 0);
	Matrix->SetElement(3, 2, 0);
	Matrix->SetElement(3, 3, 1);
}
void AR::UQtoMat(double q0, double qx, double qy, double qz, Matrix3d &Matrix)
{
	Matrix(0, 0) = (q0 * q0) + (qx * qx) - (qy * qy) - (qz * qz);
	Matrix(0, 1) = 2 * ((qx * qy) - (q0 * qz));
	Matrix(0, 2) = 2 * ((qx * qz) + (q0 * qy));
	Matrix(1, 0) = 2 * ((qx * qy) + (q0 * qz));
	Matrix(1, 1) = (q0 * q0) - (qx * qx) + (qy * qy) - (qz * qz);
	Matrix(1, 2) = 2 * ((qy * qz) - (q0 * qx));
	Matrix(2, 0) = 2 * ((qx * qz) - (q0 * qy));
	Matrix(2, 1) = 2 * ((qy * qz) + (q0 * qx));
	Matrix(2, 2) = (q0 * q0) - (qx * qx) - (qy * qy) + (qz * qz);
}
void AR::NumSelect(int num_all, int num_part, vector<int> &result)
{
	//while (result.size() < num_part)
	//{
	//	result.push_back(rand() % num_all);
	//	sort(result.begin(), result.end());
	//	result.erase(unique(result.begin(), result.end()), result.end());
	//}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, num_all - 1);

	for (int i = 0; i < num_part; ++i) {
		int num = dis(gen);
		// ����Ƿ��Ѿ����ڣ���������������
		if (std::find(result.begin(), result.end(), num) == result.end()) {
			result.push_back(num);
		}
	}
}
//plc
//��������u������v֮��ļн�
void calRotation(Eigen::Vector3f u, Eigen::Vector3f v, double &angle, Eigen::Vector3f &vec)
{
	angle = acos(u.dot(v) / (u.norm()*v.norm()));
	if (angle > M_PI / 2)
	{
		u = -u;
		angle = M_PI - angle;
	}
	float i, j, k;
	i = u(1)*v(2) - u(2)*v(1), j = v(0)*u(2) - v(2)*u(0), k = u(0)*v(1) - u(1)*v(0);
	vec << i, j, k;
	double value = sqrt(i*i + j * j + k * k);
	vec(0) = vec(0) / value;
	vec(1) = vec(1) / value;
	vec(2) = vec(2) / value;
}
//��ȡ�޵����˹��ת����
Eigen::Matrix4f RodriguesMatrixTranslation(Eigen::Vector3f n, double angle)
{
	//�޵����˹��ʽ����ת����
	Eigen::Matrix4f x_transform(Eigen::Matrix4f::Identity());
	x_transform(0, 0) = cos(angle) + n(0)*n(0)*(1 - cos(angle));
	x_transform(1, 0) = n(2)*sin(angle) + n(0)*n(1)*(1 - cos(angle));
	x_transform(2, 0) = -n(1)*sin(angle) + n(0)*n(2)*(1 - cos(angle));
	x_transform(0, 1) = n(0)*n(1)*(1 - cos(angle)) - n(2)*sin(angle);
	x_transform(1, 1) = cos(angle) + n(1)*n(1)*(1 - cos(angle));
	x_transform(2, 1) = n(0)*sin(angle) + n(1)*n(2)*(1 - cos(angle));
	x_transform(0, 2) = n(1)*sin(angle) + n(0)*n(2)*(1 - cos(angle));
	x_transform(1, 2) = -n(0)*sin(angle) + n(1)*n(2)*(1 - cos(angle));
	x_transform(2, 2) = cos(angle) + n(2)*n(2)*(1 - cos(angle));

	return  x_transform;
}
//
void AR::testdate1()
{
	if (buildnumber < 4)
	{
		if (temp0Flag && temp4Flag)
		{
			//�궨339
			vtkSmartPointer<vtkMatrix4x4> Matrix_DROToWorld = vtkSmartPointer<vtkMatrix4x4>::New();
			NDIDataToMat(temp4, Matrix_DROToWorld);
			Matrix_DROToWorld->Invert();
			double Position_Probe[3] = { temp0[4] ,temp0[5] ,temp0[6] };
			///ע��ֲ������λ��
			double out[3];
			PointSwith(Matrix_DROToWorld, Position_Probe, out);
			vector<double> out_date(out, out + 3);
			build_point.push_back(out_date);
			//�ɼ��ĸ����������ĵ�
			qDebug() << "-----------------------------------------------------------\t";
			if (buildnumber == 3)
			{
				double averagePosition[3]; double x; double y; double z;
				x = build_point[0][0] + build_point[1][0] + build_point[2][0] + build_point[3][0];
				y = build_point[0][1] + build_point[1][1] + build_point[2][0] + build_point[3][1];
				z = build_point[0][2] + build_point[1][2] + build_point[2][2] + build_point[3][2];
				// ����ƽ��ֵ
				averagePosition[0] = x / 4;				averagePosition[1] = y / 4;				averagePosition[2] = z / 4;
				qDebug() << "averagePosition__X:\t" << averagePosition[0] << ",Y: " << averagePosition[1] << ",Z: " << averagePosition[2];
			}
			buildnumber++;
		}
		else {
			qDebug() << "����������������������������DROisunvisable\t";
		}
	}
	else
	{
		buildnumber = 0;
		build_point.clear();
		qDebug() << "nonono\t";
	}
}
//
void AR::testdate2() {}

//US-probe�궨���
void AR::cail_probe(int mode, int group)
{
	const char* dir = "./data_cal";
	//PyObject* pModule = NULL;
	//PyObject* pFunc = NULL;

	pModule_Cal = PyImport_ImportModule("cal.main");         //ע���ļ����ִ�Сд
	if (pModule_Cal == nullptr) {
		PyErr_Print();
		return;
	}
	pFunc_Cal = PyObject_GetAttrString(pModule_Cal, "main"); //��ȡ��������

	PyObject* pArg = PyTuple_New(3);
	PyTuple_SetItem(pArg, 0, Py_BuildValue("i", mode));
	PyTuple_SetItem(pArg, 1, Py_BuildValue("i", group));
	PyTuple_SetItem(pArg, 2, Py_BuildValue("s", dir));
	cout << "Hello args \n";

	PyObject* presult = PyObject_CallObject(pFunc_Cal, pArg);
	if (presult == NULL) {
		// ������������ӡ������Ϣ��������Դ
		cout << "Hello res " << 0 << "\n";
	}
	else {
		char * res;
		PyArg_Parse(presult, "s", &res);
		cout << "Hello res " << res << "\n";
	}
}

//US-�������ץȡ
void AR::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == Qt::Key_F2)
	{
		qDebug() << "US F2 is pressed:";
		if (ui.farme_UsSeg->isVisible())
		{
			if (isUS_collect)
			{
				isUS_collect = false;
			}
			else if (!isUS_collect) 
			{
				isUS_collect = true; 
			}
			return;
		}
	}
	//qDebug() << "2222222222222:";
	//�е����⣿������������������������
	QWidget::keyPressEvent(ev);
	//�е����⣿������������������������
}
void AR::initcamera()
{
	//�����ӵ��������ͷ�豸����ȡ��ʹ�õ�����ͷ��Ϣ
	CameraInfoList = QCameraInfo::availableCameras();
	{
		//������������ͷ��Ϣ�����ӵ������б���
		QList<QCameraInfo>::const_iterator it;
		for (it = CameraInfoList.begin(); it != CameraInfoList.end(); it++)
		{
			ui.deviceComboBox->addItem((*it).description());
		}
	}
	if (CameraInfoList.size() == 1)
	{
		// ����Ϊ����ģʽ���Ա��ܹ����������µ�����ͷ
		camera = new QCamera(CameraInfoList[CameraInfoList.size() - 1]);
		camera->setCaptureMode(QCamera::CaptureStillImage);
		// ���µ�QCameraViewfinder�ؼ�����Ϊ�µ�����ͷ
		viewfinder = new QCameraViewfinder(ui.CameraLabel_show);
		viewfinder->setGeometry(-245, -160, 935, 800);  // ������ͼ��ʾ������ (100, 50) λ�ã���СΪ 800x600
		// ��ʼ�µ�����ͷ
		QSize resolution(1024, 768);////////����DP-20
		QCameraViewfinderSettings set;
		set.setResolution(resolution);
		camera->setViewfinderSettings(set);
	}
	else
	{
		camera = new QCamera(this);
		viewfinder = new QCameraViewfinder(ui.CameraLabel_show);
		viewfinder->setGeometry(-245, -160, 935, 800);  // ������ͼ��ʾ������ (100, 50) λ�ã���СΪ 800x600
	}
	imageCapture = new QCameraImageCapture(camera);
	camera->setViewfinder(viewfinder);
	camera->start();

	//��ʾ֡��
	//showCameraRes_FPS();

	ui.snapImageList->setViewMode(QListWidget::IconMode);//��ʾģʽ
	ui.snapImageList->setIconSize(QSize(105, 105));//����ͼƬ��С
	ui.snapImageList->setSpacing(10);//���
	ui.snapImageList->setResizeMode(QListView::Adjust);//��Ӧ���ֵ���
	ui.snapImageList->setMovement(QListView::Static);//�����ƶ�
	ui.snapImageList->setSelectionMode(QAbstractItemView::MultiSelection);          //���ÿ��Զ�ѡ
}
void AR::changeVideoSource(int index)
{
	camera->stop();
	// ����Ϊ����ģʽ���Ա��ܹ����������µ�����ͷ
	camera->setCaptureMode(QCamera::CaptureStillImage);
	camera = new QCamera(CameraInfoList[index]);
	imageCapture = new QCameraImageCapture(camera);
	camera->setCaptureMode(QCamera::CaptureStillImage);
	// ���µ�QCameraViewfinder�ؼ�����Ϊ�µ�����ͷ
	camera->setViewfinder(viewfinder);
	// ��ʼ�µ�����ͷ
	QSize resolution(1024, 768);
	QCameraViewfinderSettings set;
	set.setResolution(resolution);

	camera->setViewfinderSettings(set);
	camera->start();

}
void AR::onImageCaptured(int id, const QImage& preview)
{
		int count = ui.snapImageList->count();
		QString filename;
		// ͼ��·��/��
		if (ui.farme_UsSeg->isVisible())
		{
			filename = "./predict/USimage/" + QString::number(count) + ".png";
		}
		if(ui.frame_UsCali->isVisible())
		{
			filename = "./data_cal/imgs/raw0/imgs/" + QString::number(count) + ".png";
		}
		// �ü�ͼ��
		QImage croppedImage = preview.copy(272, 121, 431, 458);
		//����ͼ��
		croppedImage.save(filename);
		//ͼ����ӵ��б�
		QListWidgetItem* item = new QListWidgetItem(QIcon(QPixmap::fromImage(croppedImage)), QString::number(count), ui.snapImageList);
		ui.snapImageList->addItem(item);
		item->setTextColor("white");
		
		if (ui.frame_UsCali->isVisible())
		{
			//��ʾ����Ⱦ����
			vtkSmartPointer<vtkPNGReader> PNGReader1 = vtkSmartPointer<vtkPNGReader>::New();
			vtkSmartPointer<vtkImageActor> Image1 = vtkSmartPointer<vtkImageActor>::New();
			PNGReader1->SetFileName(filename.toStdString().c_str());
			double space[3] = { SIZE_PIXEL_x, SIZE_PIXEL_y, 0 };
			PNGReader1->SetDataSpacing(space);
			PNGReader1->Update();
			Image1->SetInputData(PNGReader1->GetOutput());
			vtkSmartPointer<vtkMatrix4x4> Matrix_tibiaToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_USTotibia = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_trackToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_imageToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_actorToBoneTemp = vtkSmartPointer<vtkMatrix4x4>::New();//��Թ�ͷ����ܵ�λ��
			vtkSmartPointer<vtkMatrix4x4> Matrix_actorToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			NDIDataToMat(UsCailUSProbeToWorld[count], Matrix_trackToworld);//US_Temp_World
			NDIDataToMat(UsCaliPhantomToWorld[count], Matrix_tibiaToworld); //Bone_Temp_World
			Matrix_tibiaToworld->Invert();
			vtkMatrix4x4::Multiply4x4(Matrix_trackToworld, Matrix_USToTemp, Matrix_imageToworld);
			vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_imageToworld, Matrix_USTotibia);
			vtkMatrix4x4::Multiply4x4(Matrix_USTotibia, Matrix_actorToimage, Matrix_actorToBoneTemp);
			Matrix_tibiaToworld->Invert();
			vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_actorToBoneTemp, Matrix_actorToworld);
			Image1->SetUserMatrix(Matrix_actorToworld);
			Image1->SetOpacity(0.8);
			patientRender->AddActor(Image1);
			patientRender->GetRenderWindow()->Render();
		}

		if (ui.farme_UsSeg->isVisible())
		{
			//��ʾ����Ⱦ����
			vtkSmartPointer<vtkPNGReader> PNGReader1 = vtkSmartPointer<vtkPNGReader>::New();
			vtkSmartPointer<vtkImageActor> Image1 = vtkSmartPointer<vtkImageActor>::New();
			PNGReader1->SetFileName(filename.toStdString().c_str());
			double space[3] = { SIZE_PIXEL_x, SIZE_PIXEL_y, 0 };
			PNGReader1->SetDataSpacing(space);
			PNGReader1->Update();
			Image1->SetInputData(PNGReader1->GetOutput());
			vtkSmartPointer<vtkMatrix4x4> Matrix_tibiaToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_USTotibia = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_trackToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_imageToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> Matrix_actorToBoneTemp = vtkSmartPointer<vtkMatrix4x4>::New();//��Թ�ͷ����ܵ�λ��
			vtkSmartPointer<vtkMatrix4x4> Matrix_actorToworld = vtkSmartPointer<vtkMatrix4x4>::New();
			NDIDataToMat(USToWorld[count], Matrix_trackToworld);//US_Temp_World
			NDIDataToMat(tibiaToWorld[count], Matrix_tibiaToworld); //Bone_Temp_World
			Matrix_tibiaToworld->Invert();
			vtkMatrix4x4::Multiply4x4(Matrix_trackToworld, Matrix_USToTemp, Matrix_imageToworld);
			vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_imageToworld, Matrix_USTotibia);
			vtkMatrix4x4::Multiply4x4(Matrix_USTotibia, Matrix_actorToimage, Matrix_actorToBoneTemp);
			Matrix_tibiaToworld->Invert();
			vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_actorToBoneTemp, Matrix_actorToworld);
			Image1->SetUserMatrix(Matrix_actorToworld);
			Image1->SetOpacity(0.8);
			patientRender->AddActor(Image1);
			patientRender->GetRenderWindow()->Render();
		}
}
void AR::ImagDelete()
{
	QListWidgetItem* currentItem = ui.snapImageList->currentItem();
	if (currentItem) {
		// ����ǰѡ�е���Ŀ
	}
	QList<QListWidgetItem*> selectedItems = ui.snapImageList->selectedItems();
	foreach(QListWidgetItem* item, selectedItems) {
		ui.snapImageList->takeItem(ui.snapImageList->row(item));
	}
}
//US-PY���
void AR::segmentimage()
{
	///*WinExec("./main/main.exe", SW_SHOWNORMAL);*/
	const char* img = "./predict/USimage";
	//�ļ�Ϊ1������Ϊ�ļ���
	Do_Segment(img, 2);
	for (int i = Imag_Index; i < USToWorld.size(); i++)
	{
		vtkSmartPointer<vtkMatrix4x4> Matrix_trackToworld = vtkSmartPointer<vtkMatrix4x4>::New();//us��temp��world��
		vtkSmartPointer<vtkMatrix4x4> Matrix_tibiaToworld = vtkSmartPointer<vtkMatrix4x4>::New();//bone��temp��world��
		vtkSmartPointer<vtkMatrix4x4> Matrix_trackTotibia = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkSmartPointer<vtkMatrix4x4> Matrix_imageTotibia = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkSmartPointer<vtkMatrix4x4> Matrix_imageToworld = vtkSmartPointer<vtkMatrix4x4>::New();

		NDIDataToMat(USToWorld[i], Matrix_trackToworld);//us��temp
		NDIDataToMat(tibiaToWorld[i], Matrix_tibiaToworld);//��ͷ��temp
		Matrix_tibiaToworld->Invert();
		vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_trackToworld, Matrix_trackTotibia);
		vtkMatrix4x4::Multiply4x4(Matrix_trackTotibia, Matrix_USToTemp, Matrix_imageTotibia);
		Matrix_tibiaToworld->Invert();
		vtkMatrix4x4::Multiply4x4(Matrix_tibiaToworld, Matrix_imageTotibia, Matrix_imageToworld);
		QString file = "./predict/label_txt/" + QString::number(i) + ".txt";
		ifstream fin(file.toStdString(), ios::in);
		int pos_pixel[2];
		//new
		vector<double> pointsp(2);
		vector<vector<double>> currentFilePoints;
		bool isFirstLine = true;
		//new
		while (!fin.eof())
		{
			fin >> pos_pixel[0] >> pos_pixel[1];
			double point[3], position_uspoint_inWorld[3], position_uspoint_inTemp[3];
			point[0] = SIZE_PIXEL_x * pos_pixel[0] ;
			point[1] = SIZE_PIXEL_y * pos_pixel[1];
			point[2] = 0;
			PointSwith(Matrix_imageToworld, point, position_uspoint_inWorld);
			vector<double> sp(3);
			sp[0] = position_uspoint_inWorld[0];
			sp[1] = position_uspoint_inWorld[1];
			sp[2] = position_uspoint_inWorld[2];
			USsegment_points_inWorld.push_back(sp);//��λ�ô洢-�ָ������������ϵ�µ�λ��-��ʾ
			PointSwith(Matrix_imageTotibia, point, position_uspoint_inTemp);
			sp[0] = position_uspoint_inTemp[0];
			sp[1] = position_uspoint_inTemp[1];
			sp[2] = position_uspoint_inTemp[2];
			USsegment_points_inTemp.push_back(sp);//��λ�ô洢-�ָ����Temp����ϵ�µ�λ��-����
			//new
			pointsp[0] = pos_pixel[0];
			pointsp[1] = pos_pixel[1];
			currentFilePoints.push_back(pointsp);
			if (isFirstLine) {
				std::cout << "�Ѷ�ȡ��һ��: ";
				isFirstLine = false; // ��ȡ��һ�к󣬽�isFirstLine����Ϊfalse
				USsegment_image_Max_points.push_back(sp);
			}
			//new
		}
		//new
		USsegment_image_points.push_back(currentFilePoints);
		//new
		fin.close();
	}

	vtkSmartPointer<vtkPoints> sourcepoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> sourcepoly = vtkSmartPointer<vtkPolyData>::New();
	for (int i = 0; i < USsegment_points_inWorld.size(); i++)
	{
		vtkIdType pid[1];
		pid[0] = sourcepoints->InsertNextPoint(USsegment_points_inWorld[i][0], USsegment_points_inWorld[i][1], USsegment_points_inWorld[i][2]);
		cell->InsertNextCell(1, pid);
	}
	sourcepoly->SetPoints(sourcepoints);
	sourcepoly->SetVerts(cell);
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputData(sourcepoly);

	USsegment_actor->SetMapper(mapper1);
	USsegment_actor->GetProperty()->SetColor(0, 255, 0);
	USsegment_actor->GetProperty()->SetPointSize(0.1);
	patientRender->AddActor(USsegment_actor);
	patientRender->GetRenderWindow()->Render();
	//
	const char* outputFolder = "./predict/USimage_Segment";
	// ȷ������ļ��д���
	QDir dir(outputFolder);
	if (!dir.exists()) {
		dir.mkpath(".");  // �����ļ���
	}
	int c = USsegment_image_points.size();
	drawPointsOnImage(img, outputFolder, USsegment_image_points);
	Open_Write_txt();
}
void AR::initpy()
{
	Py_SetPythonHome(L"C:\\Users\\Administrator\\miniconda3\\envs\\gnn");
	Py_Initialize();
	const char* dir1 = "best_model.pth";
	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;
	pModule = PyImport_ImportModule("predict");         //ע���ļ����ִ�Сд
	pFunc = PyObject_GetAttrString(pModule, "instantiate"); //��ȡ��������
	PyObject* pArg = PyTuple_New(1);
	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", dir1));
	PyEval_CallObject(pFunc, pArg);

	//PyObject* pModule_Cal = NULL;
	//PyObject* pFunc_cal = NULL;
	pModule_Cal = PyImport_ImportModule("cal.main");
	pFunc_Cal = PyObject_GetAttrString(pModule_Cal, "main"); //��ȡ��������
}
void AR::Do_Segment(const char * img, int target)
{
	const char* dir2 = img;
	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;
	pModule = PyImport_ImportModule("predict");         //ע���ļ����ִ�Сд
	if (target == 1)
	{
		pFunc = PyObject_GetAttrString(pModule, "seg_single"); //��ȡ��������
	}
	else
	{
		pFunc = PyObject_GetAttrString(pModule, "seg_group"); //��ȡ��������
	}

	PyObject* pArg = PyTuple_New(1);
	PyTuple_SetItem(pArg, 0, Py_BuildValue("s", dir2));

	PyEval_CallObject(pFunc, pArg);
}
void AR::initpath()
{
	QString path = "USimage";
	if (path.isEmpty())
	{
		return;
	}
	QDir dir(QDir::currentPath());
	if (!dir.exists(path))
	{
		dir.mkdir(path);
		return;
	}
	else
	{
		QDir dir("./predict/USimage");
		dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //���ù���
		QFileInfoList fileList = dir.entryInfoList(); // ��ȡ���е��ļ���Ϣ
		foreach(QFileInfo file, fileList)
		{
			//�����ļ���Ϣ
			file.dir().remove(file.fileName());
			qDebug() << file.fileName();
		}
	}
}
void AR::drawPointsOnImage(const char* imgPath, const QString& outputFolder, vector<vector<vector<double>>>  allUSsegmentPoints)
{
	// ʹ�� QDir ��ȡ�ļ����е�����ͼ���ļ�
	QDir dir(imgPath);
	// �����ļ�����������ȡ���г�����ͼ���ʽ�����Ը�����Ҫ���ӻ��޸ģ�
	dir.setNameFilters({ "*.jpg", "*.png", "*.bmp", "*.jpeg" });
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	// ��ȡ�ļ��������з����������ļ�
	QFileInfoList files = dir.entryInfoList();
	// ����ÿ���ļ�
	for (int i = Imag_Index; i < files.size(); i++)
	{
		const QFileInfo& fileInfo = files[i];
		QString imgPath = fileInfo.absoluteFilePath();
		// ���ļ�������ȡ��������
		QString fileName = fileInfo.fileName();
		int imageIndex;
		// ʹ��������ʽ��ȡ�ļ����е�����
		QRegExp rx("(\\d+)\\.png");  // ֻƥ������ "0.jpg", "1.jpg" �������ļ���
		if (rx.exactMatch(fileName)) {
			imageIndex = rx.cap(1).toInt();  // ��ȡ���ֲ�����Ϊ����
		}
		else 
		{
			continue;  // ������ƥ����ļ�
		}
		// ����ͼ��
		Mat img = imread(imgPath.toStdString());
		// ���õ����ɫ�ʹ�С
		Scalar pointColor(0, 255, 0);  // ��ɫ
		int pointSize = 1;
		// ��ȡ��Ӧ�ĵ㼯������ÿ��ͼ���в�ͬ�ĵ㼯��
		vector<vector<double>> currentImagePoints = allUSsegmentPoints[imageIndex];  // ÿ��ͼ���Ӧ�ĵ㼯
		// ������ͼ��ĵ㼯������ͼ���ϻ��Ƶ�
		for (int j = Imag_Index; j < currentImagePoints.size(); j++)
		{
			int x = static_cast<int>(currentImagePoints[j][1]); // x ����
			int y = static_cast<int>(currentImagePoints[j][0]); // y ����
			// ȷ��������ͼ��Χ��
			if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
			{
				circle(img, Point(x, y), pointSize, pointColor, -1);  // ��ͼ���ϻ���һ��Բ��---  -1 ��ʾ���Բ��
			}
		}
		// ��������ļ�·����ʹ��ԭ�ļ���
		QString outputFilePath = outputFolder + "/" + fileInfo.fileName();
		// �����޸ĺ��ͼ��
		imwrite(outputFilePath.toStdString(), img);
		// �滻��ʾ��ͼƬ
		// ��ȡҪ�滻���б���
		QListWidgetItem* item = ui.snapImageList->item(i);
		// ��� item ��Ϊ�գ����滻ͼ��
		if (item) {
			// �����µ��ļ�·��������ͼƬ�ļ���Ϊ����ֵ + ".png"��
			QString filename = "./predict/USimage_Segment/" + QString::number(i) + ".png";
			// ����ͼƬ
			QImage newImage(filename);
			// ȷ��ͼƬ���سɹ�
			if (!newImage.isNull()) {
				// �滻�б����ͼ��
				item->setIcon(QIcon(QPixmap::fromImage(newImage)));
				// �����б�����ı�Ϊ����ֵ
				item->setText(QString::number(i));
			}
			else {
				qDebug() << "ͼƬ����ʧ�ܣ�" << filename;
			}
			// ��Ⱦ���֣����ز���ʾ�±����ͼ��
			vtkSmartPointer<vtkPNGReader> PNGReader1 = vtkSmartPointer<vtkPNGReader>::New();
			vtkSmartPointer<vtkImageActor> Image1 = vtkSmartPointer<vtkImageActor>::New();
			PNGReader1->SetFileName(filename.toStdString().c_str());
			double space[3] = { SIZE_PIXEL_x, SIZE_PIXEL_y, 0 };
			PNGReader1->SetDataSpacing(space);
			PNGReader1->Update();
			Image1->SetInputData(PNGReader1->GetOutput());
		}
	}
	Imag_Index = files.size();
}
//US-��׼���
void AR::USPointCloudRegister()
{
	//int Num_DROpoints = USsegment_points_inTemp.size();
	//qDebug() << "Final size: " << Num_DROpoints << endl;
	//if (Num_DROpoints < 500)
	//{
	//	//box.setMessage(QStringLiteral("ʰȡ�����Ӧ����500!!"), false);
	//	return;
	//}
	////�Ƴ���ʾ�ɼ��ĵ�
	////patientRender->RemoveActor(icp_point_actor);
	//vector<vector<double>> Reg_points;
	////////���Ȼ�ȡ500���ɼ��ĵ�
	//vector<int> insertnum;
	//NumSelect(Num_DROpoints, 500, insertnum);
	//for (int i = 0; i < insertnum.size(); i++)
	//{
	//	int index = insertnum[i];
	//	vector<double> sp(3);
	//	sp[0] = USsegment_points_inTemp[index][0];
	//	sp[1] = USsegment_points_inTemp[index][1];
	//	sp[2] = USsegment_points_inTemp[index][2];
	//	Reg_points.push_back(sp);//ѡȡ��500����װ��Reg_points��
	//}	
	//�ɼ�����ӻ�
	vtkSmartPointer<vtkPoints> sourcepoints = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cell = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> sourcepoly = vtkSmartPointer<vtkPolyData>::New();
	for (int i = 0; i < USsegment_points_inTemp.size(); i++)
	{
		vtkIdType pid[1];
		pid[0] = sourcepoints->InsertNextPoint(USsegment_points_inTemp[i][0], USsegment_points_inTemp[i][1], USsegment_points_inTemp[i][2]);
		cell->InsertNextCell(1, pid);
	}
	sourcepoly->SetPoints(sourcepoints);
	sourcepoly->SetVerts(cell);
	vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper1->SetInputData(sourcepoly);
	pointsActor->SetMapper(mapper1);
	pointsActor->GetProperty()->SetColor(0, 0, 1);
	pointsActor->GetProperty()->SetPointSize(5.0);
	
	// ��ȡ��ȡ�� PolyData ���ݣ�����patient_spline_reader��ͷ�ĵ������ݣ�
	vtkSmartPointer<vtkPolyData> inputPolyData = patient_spline_reader->GetOutput();

	// ���㷨����
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	normals->SetInputData(inputPolyData);
	normals->SplittingOff(); // �رղ��
	normals->ComputePointNormalsOn();
	normals->ComputeCellNormalsOff();
	normals->Update();
	// �������� PolyData ���ݸ������ vtkPolyData ����
	vtkSmartPointer<vtkPolyData> Bonedata = normals->GetOutput();
	// ������ʹ�� processedPolyData ������к����Ĳ���
	vtkSmartPointer<vtkLoopSubdivisionFilter> Loop = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
	Loop->SetInputData(Bonedata);
	Loop->SetNumberOfSubdivisions(1);
	Loop->Update();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points = Loop->GetOutput()->GetPoints();//points�д������е����ͷ�ĵ�������
	//��ȡ�ɼ��������㸽���Ĺ�ͷ�ĵ�������������׼Ч��
	vector<double> p_x, p_y, p_z;
	qDebug() << QStringLiteral("���������") << points->GetNumberOfPoints();
	for (int i = 0; i < points->GetNumberOfPoints(); i++)
	{
		double pi[3];
		points->GetPoint(i, pi);
		double norm = sqrt((pi[0] - PresetPoints_mark_0[0]) * (pi[0] - PresetPoints_mark_0[0]) + (pi[1] - PresetPoints_mark_0[1]) * (pi[1] - PresetPoints_mark_0[1]) + (pi[2] - PresetPoints_mark_0[2]) * (pi[2] - PresetPoints_mark_0[2]));
		double norm1 = sqrt((pi[0] - PresetPoints_mark_1[0]) * (pi[0] - PresetPoints_mark_1[0]) + (pi[1] - PresetPoints_mark_1[1]) * (pi[1] - PresetPoints_mark_1[1]) + (pi[2] - PresetPoints_mark_1[2]) * (pi[2] - PresetPoints_mark_1[2]));
		double norm2 = sqrt((pi[0] - PresetPoints_mark_2[0]) * (pi[0] - PresetPoints_mark_2[0]) + (pi[1] - PresetPoints_mark_2[1]) * (pi[1] - PresetPoints_mark_2[1]) + (pi[2] - PresetPoints_mark_2[2]) * (pi[2] - PresetPoints_mark_2[2]));
		if (norm < 60 || norm1 < 50 || norm2 < 50)
		{
			p_x.push_back(pi[0]);
			p_y.push_back(pi[1]);
			p_z.push_back(pi[2]);
		}
	}		
	////����ȡ�ı�������ת��ΪvtkPolyData
	vtkSmartPointer<vtkPoints> points1 = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> cell1 = vtkSmartPointer<vtkCellArray>::New();
	vector<vector<double>> Bone_points;
	for (int i = 0; i < p_x.size(); i++)
	{
		vector<double> sp(3);
		sp[0] = p_x[i];
		sp[1] = p_y[i];
		sp[2] = p_z[i];
		Bone_points.push_back(sp);
		vtkIdType pid1[1], pid2[1];
		pid1[0] = points1->InsertNextPoint(p_x[i], p_y[i], p_z[i]);
		cell1->InsertNextCell(1, pid1);
	}

	vtkSmartPointer<vtkPolyData> polydata1 = vtkSmartPointer<vtkPolyData>::New();
	polydata1->SetPoints(points1);
	polydata1->SetVerts(cell1);


	//��vtkPolyData�������ݾ�������׼�������
	vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
	Transform->SetMatrix(Matrix_coarse);
	//�������
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "Matrix_coarse_is" << Matrix_coarse->GetElement(i, j) << "    " << endl;
		}
	}

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetInputData(polydata1);
	transformFilter->SetTransform(Transform);
	transformFilter->Update();
	//ɸѡ��Ŀ��ӻ�
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polydata1);
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(0 / 255, 255.0 / 255, 0 / 255);
	patientRender->AddActor(actor);
	patientRender->GetRenderWindow()->Render();
	//

	//progressDlg->setValue(10);
	qDebug() << "---------ICP Start---------------" << endl;
	UQICPRegister MyICP;
	MyICP.InputSourceData(USsegment_points_inTemp);
	MyICP.InputTargetData(transformFilter->GetOutput());
	vtkSmartPointer<vtkMatrix4x4> Matrix_MyICP = vtkSmartPointer<vtkMatrix4x4>::New();//
	MyICP.GetICPResult(Matrix_MyICP);
	Matrix_MyICP->Invert();////////////��¼��ʼICP��׼���(��->С)
	double ICPDistance = MyICP.GetICPDistance();
	qDebug() << "---------ICP End---------------" << endl;
	//progressDlg->setValue(20);
	///////////////////////////////////////����Ѱ�ŷ������

	int Search_Num = 0;
	bool is_Search = true;
	Matrix<double, 3, 3> M_vec_dis;/////////////��¼ÿ���ڵ�������ۺ���
	vector<vtkSmartPointer<vtkMatrix4x4> > M_Matrixs;/////////////��¼ÿ���ڵ������Ŷ���̬���λ��
	vector<vtkSmartPointer<vtkMatrix4x4> > M_AstarMatrixs;/////////////��¼ÿ�������Ŷ���̬���ICP��׼���(��->С)
	vtkSmartPointer<vtkMatrix4x4> M_Register = vtkSmartPointer<vtkMatrix4x4>::New();///��ʼ����׼���� ����׼+��ʼICP
	vtkMatrix4x4::Multiply4x4(Matrix_MyICP, Matrix_coarse, M_Register);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			//qDebug() << "222maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
			std::cout << "M_Register����Ԫ���ǣ� "<< M_Register->GetElement(i, j) << "    " << std::endl;
		}
	}


	double M_vec1[3], M_pos1[3];

	VecSwith(M_Register, Search_vec, M_vec1);///��ʼ���Ŷ���
	PointSwith(M_Register, Search_pos, M_pos1);

	double  old_dis = ICPDistance;
	while (is_Search && Search_Num <= 10)
	{
		M_Matrixs.clear();
		M_AstarMatrixs.clear();
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				double dis, angle;
				double ind = 0.5;
				dis = i * ind;
				angle = j * ind * 2;
				vtkSmartPointer<vtkTransform> Per_Transform = vtkSmartPointer<vtkTransform>::New();

				Per_Transform->Translate(M_pos1);
				Per_Transform->RotateWXYZ(angle, M_vec1);
				Per_Transform->Translate(-M_pos1[0], -M_pos1[1], -M_pos1[2]);
				Per_Transform->Translate(dis * M_vec1[0], dis * M_vec1[1], dis  * M_vec1[2]);
				vtkSmartPointer<vtkMatrix4x4> M_Perturb = vtkSmartPointer<vtkMatrix4x4>::New();/////////��¼�Ŷ��任
				M_Perturb = Per_Transform->GetMatrix();
				vtkSmartPointer<vtkMatrix4x4> M_new = vtkSmartPointer<vtkMatrix4x4>::New();/////////////��¼ģ�͵�ǰ�Ŷ���λ��
				vtkMatrix4x4::Multiply4x4(M_Perturb, M_Register, M_new);
				///ִ��ICP
				M_Matrixs.push_back(M_new);
				//�����ģ����λ������
				vtkSmartPointer<vtkTransform> Cur_Transform = vtkSmartPointer<vtkTransform>::New();
				Cur_Transform->SetMatrix(M_new);
				vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
				transformFilter->SetInputData(polydata1);
				transformFilter->SetTransform(Cur_Transform);
				transformFilter->Update();
				UQICPRegister CurICP;
				CurICP.InputSourceData(USsegment_points_inTemp);
				CurICP.InputTargetData(transformFilter->GetOutput());
				vtkSmartPointer<vtkMatrix4x4> Matrix_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
				CurICP.GetICPResult(Matrix_CurICP);
				double CurICPDistance = CurICP.GetICPDistance();
				M_vec_dis(i + 1, j + 1) = CurICPDistance;
				Matrix_CurICP->Invert();
				M_AstarMatrixs.push_back(Matrix_CurICP);/////////////��¼��ǰ�Ŷ�λ���µ�ICP��׼���
			}
		}
		MatrixXd::Index minRow, minCol;
		double min = M_vec_dis.minCoeff(&minRow, &minCol);////////////��ȡ��С���ۺ���������
		if (min < old_dis && Search_Num < 10)
		{
			old_dis = min;
			vector<int> index(2);
			index[0] = minRow - 1;
			index[1] = minCol - 1;
			vtkSmartPointer<vtkMatrix4x4> M_CurMaxis = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> M_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
			M_CurMaxis->DeepCopy(M_Matrixs[3 * minRow + minCol]);
			M_CurICP->DeepCopy(M_AstarMatrixs[3 * minRow + minCol]);
			vtkMatrix4x4::Multiply4x4(M_CurICP, M_CurMaxis, M_Register);////////////��ȡ��׼���󣬼�������

			VecSwith(M_Register, Search_vec, M_vec1);///�����Ŷ���
			PointSwith(M_Register, Search_pos, M_pos1);
		}
		else
		{
			qDebug() << "Final Meandistance: " << old_dis << endl;
			is_Search = false;/////////����������ֹͣ����

		}
		Search_Num++;
	}
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "333maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	Matrix_boneToFarDRO->DeepCopy(M_Register);
	patient_Actor->SetUserMatrix(Matrix_boneToFarDRO);
	patient_Actor->GetProperty()->SetColor(1.0, 1.0, 0);
	patient_Actor->GetProperty()->SetOpacity(1.0);
	patientRender->RemoveActor(CoarseBone_Actor);
	patientRender->AddActor(pointsActor);
	patientRender->ResetCamera();
	patientRender->GetRenderWindow()->Render();
	//QString text = "Final iteration distance: ";
	//QString str = QString::number(old_dis, 'f', 3);
	//QString text2 = "mm";
	//text = text + str;
	//text = text + text2;
	//box.setMessage(text, true);
	//for (int i = 0; i < 4; ++i) {
	//	for (int j = 0; j < 4; ++j) {
	//		qDebug() << "maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
	//	}
	//}
	qDebug() << QStringLiteral("�����ICP��׼��");
}

//
//����E����
void AR::translatMode()
{
	if (!isPick) 
	{
		targetHelper->mode = "translate";
		targetHelper->helperChildVisibility(isselectmodle, true, scale);
		AxisRender->GetRenderWindow()->Render();
	}
}
//����W����
void AR::roateMode()
{
	if (!isPick)
	{
		targetHelper->mode = "rotate";
		targetHelper->helperChildVisibility(isselectmodle, false, scale);
		AxisRender->GetRenderWindow()->Render();
	}
}
void AR::targetMouseMove(int x, int y, bool isMousePressed)
{

	QSize size = ui.patientWindow->size();
	targetHelper->mouseMove(x, y, isMousePressed, AxisRender, size);
	AxisRender->GetRenderWindow()->Render();
	return;
}
void AR::ChangeCamera_3D()
{
	if (ui.patientWindow->fullScreenFlag)
	{
		camera_3D->SetClippingRange(0.1, 20000);
		AxisRender->GetRenderWindow()->Render();
	}
	else { return; }

}
void AR::PickThisModle(int x, int y)
{
	QSize size = ui.patientWindow->size();
	vtkSmartPointer<vtkPropPicker> cellPicker = vtkSmartPointer<vtkPropPicker>::New();
	vtkSmartPointer<vtkPropPicker> AxicellPicker = vtkSmartPointer<vtkPropPicker>::New();
	cellPicker->Pick(x, size.height() - y - 1, 0, patientRender);
	AxicellPicker->Pick(x, size.height() - y - 1, 0, AxisRender);
	//���ѡ����ĳ������
	if (cellPicker->GetActor() != nullptr)
	{
		isselectmodle = true;
		AllTimer->start(10);
		LastPickedActor = cellPicker->GetActor();
		LastPickedActor->GetProperty()->SetColor(191.0 / 255.0, 52.0 / 255.0, 61.0 / 255.0);
		targetHelper->setObject(LastPickedActor);

		double bounds[6];
		LastPickedActor->GetBounds(bounds);
		double size = sqrt((bounds[1] - bounds[0]) * (bounds[1] - bounds[0]) +
			(bounds[3] - bounds[2]) * (bounds[3] - bounds[2]) +
			(bounds[5] - bounds[4]) * (bounds[5] - bounds[4]));
		//scale = size / 1000; //

		if (targetHelper->mode == "rotate")
		{
			targetHelper->helperChildVisibility(isselectmodle, false, scale);
		}
		else if (targetHelper->mode == "translate")
		{ 
			targetHelper->helperChildVisibility(isselectmodle, true, scale);

		}

	}
	else
	{
		if (LastPickedActor != nullptr && isselectmodle)
		{
			if (AxicellPicker->GetActor() != nullptr)
			{

			}
			else
			{
				AllTimer->stop();
				isselectmodle = false;
				LastPickedActor->GetProperty()->SetColor(255 / 255.0, 255 / 255.0, 255 / 255.0);
				targetHelper->helperChildVisibility(isselectmodle, false, scale);
			}
		}
	}
}
//��ʼ����Ҫѡ��Ķ���
void AR::intPickdata()
{
	//����ɼ�������
	QString file = "./predict/UsCould_data_intemp.txt";
	ifstream fin(file.toStdString(), ios::in);
	vector<double> collect_sp(3);
	double pos_pixel[3];
	while (!fin.eof())
	{
		fin >> pos_pixel[0] >> pos_pixel[1] >> pos_pixel[2];
		collect_sp[0] = pos_pixel[0];
		collect_sp[1] = pos_pixel[1];
		collect_sp[2] = pos_pixel[2];
		US_points.push_back(collect_sp);
	}
	fin.close();
	vtkSmartPointer<vtkPoints> vtkpoints_US = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vtkcell_US = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkPolyData> vtkpoly_US = vtkSmartPointer<vtkPolyData>::New();
	for (int i = 0; i < US_points.size(); i++)
	{
		vtkIdType pid[1];
		pid[0] = vtkpoints_US->InsertNextPoint(US_points[i][0], US_points[i][1], US_points[i][2]);
		vtkcell_US->InsertNextCell(1, pid);
	}
	vtkpoly_US->SetPoints(vtkpoints_US);
	vtkpoly_US->SetVerts(vtkcell_US);

	targetHelper->mode = " "; 
	vtkNew<vtkNamedColors> colors;
	vtkNew<vtkIdFilter> idFilter;
	idFilter->SetInputData(vtkpoly_US);
	idFilter->SetIdsArrayName("OriginalIds");
	idFilter->Update();

	// This is needed to convert the ouput of vtkIdFilter (vtkDataSet) back to vtkPolyData
	vtkNew<vtkDataSetSurfaceFilter> surfaceFilter;
	surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
	surfaceFilter->Update();
	polydata_Us_Input = surfaceFilter->GetOutput();

	// Create a mapper and actor
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputData(vtkpoly_US);
	mapper->ScalarVisibilityOff();
	
	USdata_Actor->SetMapper(mapper);
	USdata_Actor->GetProperty()->SetPointSize(1);
	USdata_Actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Peacock").GetData());
	USdata_Actor->SetPickable(false);
	// Visualize
	patientRender->UseHiddenLineRemovalOn();
	patientRender->AddActor(USdata_Actor);
	patientRender->GetRenderWindow()->Render();
	Pick_Area->SetPolyData(polydata_Us_Input);
	Pick_Area->SetCurrentRenderer(patientRender);
	Pick_Area->GetRender(patientRender,Pick_Area);
}
void AR::PickUsData()
{
	if (isPick)
	{
		patient_Actor->SetPickable(true);
		vtkSmartPointer<vtkInteractorStyleTrackballCamera> normalstyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		patientRender->GetRenderWindow()->GetInteractor()->SetInteractorStyle(normalstyle);
		isPick = false;
		std::cout << "isPick---false----�������� " << std::endl;
	}
	else if (!isPick)
	{
		patient_Actor->SetPickable(false);
		vtkNew<vtkAreaPicker> areaPicker;
		patientRender->GetRenderWindow()->GetInteractor()->SetInteractorStyle(Pick_Area);
		patientRender->GetRenderWindow()->GetInteractor()->SetPicker(areaPicker);
		isPick = true;
		std::cout << "isPick---true----��ѡ���� " << std::endl;
	}
}
void AR::SelectSplineL1()
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (action)
	{
		QString actionName = action->text();
		// ���� actionName ִ�в�ͬ�Ĳ���
		if (actionName == "Spline_L1")
		{
			double color = 0.1;
			AreaPickBoolCut(0, L1SelectAction->isChecked(), color);
		}
		else if (actionName == "Spline_L2") {
			double color = 0.3;
			AreaPickBoolCut(1, L2SelectAction->isChecked(), color);
		}
		else if (actionName == "Spline_L3") {
			double color = 0.5;
			AreaPickBoolCut(2, L3SelectAction->isChecked(), color);
		}
		else if (actionName == "Spline_L4") {
			double color = 0.7;
			AreaPickBoolCut(3, L4SelectAction->isChecked(),color);
		}
		else if (actionName == "Spline_L5") {
			double color = 0.9;
			AreaPickBoolCut(4, L5SelectAction->isChecked(), color);
		}
		else if (actionName == "Spline_L6") {
			double color = 1;
			AreaPickBoolCut(5, L5SelectAction->isChecked(), color);
		}
	}
}
void AR::AreaPickBoolCut(int index,bool isclicked, double color)
{
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	if (isclicked) 
	{
		Pick_Area->SelectedActor->SetVisibility(false);
		//����ѡ�е�Acotr
		vtkSmartPointer<vtkPolyDataMapper> mapperx = vtkSmartPointer<vtkPolyDataMapper>::New();
		vtkPolyData* copiedPolyData = vtkPolyData::New();
		copiedPolyData->DeepCopy(Pick_Area->SelectedMapper->GetInput());
		mapperx->SetInputData(copiedPolyData);
		mapperx->ScalarVisibilityOff();//�رձ�����ɫ
		mapperx->Update();
		us_data_group->getChild(index)->SetMapper(mapperx);
		us_data_group->getChild(index)->SetVisibility(true);
		us_data_group->getChild(index)->GetProperty()->SetColor(0, color,0);
		us_data_group->getChild(index)->GetProperty()->SetPointSize(3);
		patientRender->AddActor(us_data_group->getChild(index));
		patientRender->GetRenderWindow()->Render();
		std::cout << "ִ���ˣ�����" << std::endl;
	}
	else 
	{	
		us_data_group->getChild(index)->SetVisibility(false);
		patientRender->RemoveActor(us_data_group->getChild(index));
		patientRender->GetRenderWindow()->Render();
	}
}
void AR::ResigterUS(vtkSmartPointer<vtkPolyData> polydata_bone, vector<vector<double>> points)
{
	//��vtkPolyData�������ݾ�������׼�������
	vtkSmartPointer<vtkTransform> Transform = vtkSmartPointer<vtkTransform>::New();
	Transform->SetMatrix(Matrix_coarse);
	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetTransform(Transform);
	transformFilter->SetInputData(polydata_bone);
	transformFilter->Update();

	int Num_DROpoints = points.size();
	if (Num_DROpoints < 20000)
	{
		//box.setMessage(QStringLiteral("ʰȡ�����Ӧ����500!!"), false);
		return;
	}
	//�Ƴ���ʾ�ɼ��ĵ�
	//patientRender->RemoveActor(icp_point_actor);
	vector<vector<double>> Reg_points;
	//////���Ȼ�ȡ500���ɼ��ĵ�
	vector<int> insertnum;
	NumSelect(Num_DROpoints, 20000, insertnum);
	for (int i = 0; i < insertnum.size(); i++)
	{
		int index = insertnum[i];
		vector<double> sp(3);
		sp[0] = points[index][0];
		sp[1] = points[index][1];
		sp[2] = points[index][2];
		std::cout << "index�� " << index << std::endl;
		Reg_points.push_back(sp);//ѡȡ��20000����װ��Reg_points��
	}
	qDebug() << "Final size: " << Reg_points.size() << endl;
	//progressDlg->setValue(10);
	cout << "---------ICP Start---------------\n";
	//qDebug() << "---------ICP Start---------------" << endl;
	UQICPRegister MyICP;
	MyICP.InputSourceData(Reg_points);
	MyICP.InputTargetData(transformFilter->GetOutput());
	vtkSmartPointer<vtkMatrix4x4> Matrix_MyICP = vtkSmartPointer<vtkMatrix4x4>::New();//
	MyICP.GetICPResult(Matrix_MyICP);
	Matrix_MyICP->Invert();////////////��¼��ʼICP��׼���(��->С)
	double ICPDistance = MyICP.GetICPDistance();
	cout << "---------ICP END---------------\n";
	//qDebug() << "---------ICP End---------------" << endl;
	//progressDlg->setValue(20);
	///////////////////////////////////////����Ѱ�ŷ������

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "maxt_is" << Matrix_MyICP->GetElement(i, j) << "    " << endl;
		}
	}
	int Search_Num = 0;
	bool is_Search = true;
	Matrix<double, 3, 3> M_vec_dis;/////////////��¼ÿ���ڵ�������ۺ���
	vector<vtkSmartPointer<vtkMatrix4x4> > M_Matrixs;/////////////��¼ÿ���ڵ������Ŷ���̬���λ��
	vector<vtkSmartPointer<vtkMatrix4x4> > M_AstarMatrixs;/////////////��¼ÿ�������Ŷ���̬���ICP��׼���(��->С)
	vtkSmartPointer<vtkMatrix4x4> M_Register = vtkSmartPointer<vtkMatrix4x4>::New();///��ʼ����׼���� ����׼+��ʼICP
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "111maxt_is" << Matrix_MyICP->GetElement(i, j) << "    " << endl;
		}
	}
	vtkMatrix4x4::Multiply4x4(Matrix_MyICP, Matrix_coarse, M_Register);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "222maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	qDebug() << "---------ICP 2222222222---------------" << endl;
	double M_vec1[3], M_pos1[3];

	VecSwith(M_Register, Search_vec, M_vec1);///��ʼ���Ŷ���
	PointSwith(M_Register, Search_pos, M_pos1);

	double  old_dis = ICPDistance;
	while (is_Search && Search_Num <= 10)
	{
		M_Matrixs.clear();
		M_AstarMatrixs.clear();
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				double dis, angle;
				double ind = 0.5;
				dis = i * ind;
				angle = j * ind * 2;
				vtkSmartPointer<vtkTransform> Per_Transform = vtkSmartPointer<vtkTransform>::New();

				Per_Transform->Translate(M_pos1);
				Per_Transform->RotateWXYZ(angle, M_vec1);
				Per_Transform->Translate(-M_pos1[0], -M_pos1[1], -M_pos1[2]);
				Per_Transform->Translate(dis * M_vec1[0], dis * M_vec1[1], dis  * M_vec1[2]);
				vtkSmartPointer<vtkMatrix4x4> M_Perturb = vtkSmartPointer<vtkMatrix4x4>::New();/////////��¼�Ŷ��任
				M_Perturb = Per_Transform->GetMatrix();
				vtkSmartPointer<vtkMatrix4x4> M_new = vtkSmartPointer<vtkMatrix4x4>::New();/////////////��¼ģ�͵�ǰ�Ŷ���λ��
				vtkMatrix4x4::Multiply4x4(M_Perturb, M_Register, M_new);
				///ִ��ICP
				M_Matrixs.push_back(M_new);
				//�����ģ����λ������
				vtkSmartPointer<vtkTransform> Cur_Transform = vtkSmartPointer<vtkTransform>::New();
				Cur_Transform->SetMatrix(M_new);
				vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
				transformFilter->SetInputData(polydata_bone);
				transformFilter->SetTransform(Cur_Transform);
				transformFilter->Update();
				UQICPRegister CurICP;
				CurICP.InputSourceData(Reg_points);
				CurICP.InputTargetData(transformFilter->GetOutput());
				vtkSmartPointer<vtkMatrix4x4> Matrix_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
				CurICP.GetICPResult(Matrix_CurICP);
				double CurICPDistance = CurICP.GetICPDistance();
				M_vec_dis(i + 1, j + 1) = CurICPDistance;
				Matrix_CurICP->Invert();
				M_AstarMatrixs.push_back(Matrix_CurICP);/////////////��¼��ǰ�Ŷ�λ���µ�ICP��׼���
			}
		}
		MatrixXd::Index minRow, minCol;
		double min = M_vec_dis.minCoeff(&minRow, &minCol);////////////��ȡ��С���ۺ���������
		if (min < old_dis && Search_Num < 10)
		{
			old_dis = min;
			vector<int> index(2);
			index[0] = minRow - 1;
			index[1] = minCol - 1;
			vtkSmartPointer<vtkMatrix4x4> M_CurMaxis = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkSmartPointer<vtkMatrix4x4> M_CurICP = vtkSmartPointer<vtkMatrix4x4>::New();
			M_CurMaxis->DeepCopy(M_Matrixs[3 * minRow + minCol]);
			M_CurICP->DeepCopy(M_AstarMatrixs[3 * minRow + minCol]);
			vtkMatrix4x4::Multiply4x4(M_CurICP, M_CurMaxis, M_Register);////////////��ȡ��׼���󣬼�������

			VecSwith(M_Register, Search_vec, M_vec1);///�����Ŷ���
			PointSwith(M_Register, Search_pos, M_pos1);
		}
		else
		{
			qDebug() << "Final Meandistance: " << old_dis << endl;
			is_Search = false;/////////����������ֹͣ����
		}
		Search_Num++;
	}
	//���ģ������Ⱦ
	//progressDlg->setValue(100);
	//progressDlg->close();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "333maxt_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	Matrix_boneToFarDRO->DeepCopy(M_Register);
	patient_Actor->SetUserMatrix(M_Register);
	patient_Actor->SetVisibility(true);
	patient_Actor->GetProperty()->SetColor(1.0, 1.0, 0);
	patient_Actor->GetProperty()->SetOpacity(1.0);
	patientRender->AddActor(patient_Actor);
	patientRender->RemoveActor(CoarseBone_Actor);
	//patientRender->AddActor(pointsActor);
	patientRender->ResetCamera();
	//patientRender->GetRenderWindow()->Render();
	QString text = "Final iteration distance: ";
	QString str = QString::number(old_dis, 'f', 3);
	QString text2 = "mm";
	text = text + str;
	text = text + text2;
	//box.setMessage(text, true);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qDebug() << "M_Register_is" << M_Register->GetElement(i, j) << "    " << endl;
		}
	}
	isICPReg = true;
	qDebug() << QStringLiteral("�����ICP��׼��");


}
