#include "Calibration.h"
#include <iostream>
#include <Eigen/Dense>
#include <chrono>

Calibration::Calibration(QObject *parent)
	: QObject{ parent }
{
	connectTimer = new QTimer(this);
	Ndi_thread = new QThread();
	NDI_Connect_init();
	this->moveToThread(Ndi_thread);
	Ndi_thread->start();



	pts_src.push_back(Vector3d(64.29168, -291.0937, -99.44607));//点0
	pts_src.push_back(Vector3d(69.95719, -111.3727, -104.1074));//点1
	pts_src.push_back(Vector3d(24.31963, -129.4461, -116.0865));//点2
	pts_src.push_back(Vector3d(-5.644194, -25.47674, 22.74995));//点7
	pts_src.push_back(Vector3d(-4.74547, -34.1056, 51.6211));//点9
	pts_src.push_back(Vector3d(-3.807946, 10.49349, 53.42791));//点8

	//猪骨的
/*
	mat_Registion_760 <<
		-0.312722, 0.949659, -0.0187918, 32.1532,
		0.104273, 0.0146592, -0.994441, 38.7258,
		-0.944104, -0.312943, -0.103608, -124.78,
		0, 0, 0, 1;
	mat_Registion_449 <<
		0.139021, 0.0689864, - 0.987884, 37.0064,
		0.399265, 0.908993, 0.119664, - 31.759,
		0.906234, - 0.411063, 0.0988252, - 156.372,
		0 ,0 ,0, 1;
	*/
	//裸骨的
	mat_Registion_449 <<
		0.0082344, 0.0728174, -0.997311, 56.6345,
		0.409656, 0.909566, 0.0697932, -32.6058,
		0.912203, -0.409129, -0.0223403, -155.281,
		0, 0, 0, 1;


	mat_Registion_760 <<
		-0.999761, -0.000798674, 0.0218353, 50.6689,
		-0.0218254, -0.0108516, -0.999703, -189.8,
		0.00103538, -0.999941, 0.0108316, -128.637,
		0, 0, 0, 1;

	mat_Registion <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	mat_Registion = mat_Registion_760;


}

Calibration::~Calibration()
{
}


void Calibration::NDI_Connect_init()
{
	connect(connectTimer, &QTimer::timeout, this, &Calibration::getConnectData);
}


void Calibration::NDI_Connect()
{
	if (isConnect)
	{
		emit setlabel(4);
		return;
	}
	if (ndi.registerTools() == -1)
		emit setlabel(5);
	else
	{
		emit setlabel(6);
		isConnect = true;
		startConnectTimer();
	}
}

void  Calibration::startConnectTimer()
{
	std::cout << std::endl << "Entering tracking mode and collecting data..." << std::endl;
	ndi.onErrorPrintDebugMessage("capi.startTracking()", ndi.capi.startTracking());
	connectTimer->setTimerType(Qt::PreciseTimer);
	connectTimer->start(50);
}
//结束connectTimer
void  Calibration::stopConnectTimer()
{
	connectTimer->stop();
	std::cout << std::endl << "Leaving tracking mode and returning to configuration mode..." << std::endl;
	ndi.onErrorPrintDebugMessage("capi.stopTracking()", ndi.capi.stopTracking());
}


void  Calibration::getConnectData()
{
	auto startTime = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds startms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);
	ndi.capi.getTrackingDataTX();

	if (ndi.apiSupportsBX2) {
		toolData = ndi.capi.getTrackingDataBX2();
	}
	else {
		toolData = ndi.capi.getTrackingDataBX();
	}

	//toolData = ndi.apiSupportsBX2 ? ndi.capi.getTrackingDataBX2() : ndi.capi.getTrackingDataBX();

	std::chrono::milliseconds endms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);

	auto endTime0 = std::chrono::high_resolution_clock::now();




	getPoint();


	getThreePoint();
	GetTmpDate();

	auto endTime1 = std::chrono::high_resolution_clock::now();
	auto duration0 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime0 - startTime);
	auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime1 - endTime0);
	float time = endms.count() - startms.count();
	//qDebug() << "onetime" << startms.count() << "twotime" << endms.count()<<"time" << duration0.count();
}

void  Calibration::getPoint() //采集单点（配准点）
{
	vector<double> temp0 = getTemp(0);
	vector<double> temp2 = getTemp(2);
	if (isrecord)
	{
		if (temp0Flag && temp2Flag)
		{
			Vector4d point0(temp0[4], temp0[5], temp0[6], 1);
			Quaterniond q_odom_curr_tmp;//声明一个Eigen类的四元数旋转
			//此处进行赋值
			q_odom_curr_tmp.x() = temp2[1];
			q_odom_curr_tmp.y() = temp2[2];
			q_odom_curr_tmp.z() = temp2[3];
			q_odom_curr_tmp.w() = temp2[0];
			Matrix3d R_odom_curr_tmp;//声明一个Eigen类的3*3的旋转矩阵
			//四元数转为旋转矩阵--先归一化再转为旋转矩阵
			R_odom_curr_tmp = q_odom_curr_tmp.normalized().toRotationMatrix();
			//三个点的世界坐标转换矩阵
			Matrix4d mat_temp2;
			mat_temp2 <<
				R_odom_curr_tmp(0, 0), R_odom_curr_tmp(0, 1), R_odom_curr_tmp(0, 2), temp2[4],
				R_odom_curr_tmp(1, 0), R_odom_curr_tmp(1, 1), R_odom_curr_tmp(1, 2), temp2[5],
				R_odom_curr_tmp(2, 0), R_odom_curr_tmp(2, 1), R_odom_curr_tmp(2, 2), temp2[6],
				0, 0, 0, 1;
			Vector4d point1 = mat_temp2.inverse() * point0;
			pts_drt.push_back(Vector3d(point1(0), point1(1), point1(2)));
			isrecord = false;
			emit setlabel(3);
		}
	}
}
void Calibration::record()
{
	isrecord = true;
}

Matrix4d Calibration::Get3DR_TransMatrix(vector<Vector3d> srcPoints, vector<Vector3d> dstPoints)
{
	double srcSumX = 0.0f;
	double srcSumY = 0.0f;
	double srcSumZ = 0.0f;

	double dstSumX = 0.0f;
	double dstSumY = 0.0f;
	double dstSumZ = 0.0f;

	////至少三组点
	//if (srcPoints.size() != dstPoints.size() || srcPoints.size() < 3)
	//{
	//	return;
	//}

	int pointsNum = dstPoints.size();
	for (int i = 0; i < pointsNum; ++i)
	{
		srcSumX += srcPoints[i](0);
		srcSumY += srcPoints[i](1);
		srcSumZ += srcPoints[i](2);

		dstSumX += dstPoints[i](0);
		dstSumY += dstPoints[i](1);
		dstSumZ += dstPoints[i](2);
	}

	cv::Point3d centerSrc, centerDst;

	centerSrc.x = double(srcSumX / pointsNum);
	centerSrc.y = double(srcSumY / pointsNum);
	centerSrc.z = double(srcSumZ / pointsNum);

	centerDst.x = double(dstSumX / pointsNum);
	centerDst.y = double(dstSumY / pointsNum);
	centerDst.z = double(dstSumZ / pointsNum);

	//Mat::Mat(int rows, int cols, int type)
	cv::Mat srcMat(3, pointsNum, CV_64FC1);
	cv::Mat dstMat(3, pointsNum, CV_64FC1);

	for (int i = 0; i < pointsNum; ++i)//N组点
	{
		//三行
		srcMat.at<double>(0, i) = srcPoints[i](0) - centerSrc.x;
		srcMat.at<double>(1, i) = srcPoints[i](1) - centerSrc.y;
		srcMat.at<double>(2, i) = srcPoints[i](2) - centerSrc.z;

		dstMat.at<double>(0, i) = dstPoints[i](0) - centerDst.x;
		dstMat.at<double>(1, i) = dstPoints[i](1) - centerDst.y;
		dstMat.at<double>(2, i) = dstPoints[i](2) - centerDst.z;

	}

	cv::Mat matS = srcMat * dstMat.t();

	cv::Mat matU, matW, matV;
	cv::SVDecomp(matS, matW, matU, matV);

	cv::Mat matTemp = matU * matV;
	double det = cv::determinant(matTemp);//行列式的值

	double datM[] = { 1, 0, 0, 0, 1, 0, 0, 0, det };
	cv::Mat matM(3, 3, CV_64FC1, datM);

	cv::Mat matR = matV.t() * matM * matU.t();

	double* datR = (double*)(matR.data);
	double delta_X = centerDst.x - (centerSrc.x * datR[0] + centerSrc.y * datR[1] + centerSrc.z * datR[2]);
	double delta_Y = centerDst.y - (centerSrc.x * datR[3] + centerSrc.y * datR[4] + centerSrc.z * datR[5]);
	double delta_Z = centerDst.z - (centerSrc.x * datR[6] + centerSrc.y * datR[7] + centerSrc.z * datR[8]);

	//生成RT齐次矩阵(4*4)
	Matrix4d mat_rt;
	mat_rt <<
		matR.at<double>(0, 0), matR.at<double>(0, 1), matR.at<double>(0, 2), delta_X,
		matR.at<double>(1, 0), matR.at<double>(1, 1), matR.at<double>(1, 2), delta_Y,
		matR.at<double>(2, 0), matR.at<double>(2, 1), matR.at<double>(2, 2), delta_Z,
		0, 0, 0, 1;
	return mat_rt;
}

void Calibration::registion() {
	Matrix4d S_z;
	S_z <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1;
	if (pts_drt.size() >= 3 && pts_drt.size() <= 6) {
		mat_Registion = S_z * Get3DR_TransMatrix(pts_src, pts_drt) * S_z;
	}
	else {
		emit setlabel(7);
	}


}

//void Calibration::Position()
//{
//	isPosition = true;
//	bd449position();
//	isPosition = false;
//}
void Calibration::Driection()
{
	isDrection = true;
}

void Calibration::getThreePoint()//获得标定块点
{
	vector<double> temp1 = getTemp(1);

	if (isCali)
	{
		if (temp1Flag)//修改：是否看得见标定块的反光球
		{
			////-------------------点标定
			//Quaterniond q_odom_curr_tmp;//声明一个Eigen类的四元数
			////此处进行赋值
			//q_odom_curr_tmp.x() = temp1[1];
			//q_odom_curr_tmp.y() = temp1[2];
			//q_odom_curr_tmp.z() = temp1[3];
			//q_odom_curr_tmp.w() = temp1[0];
			//Matrix3d R_odom_curr_tmp;//声明一个Eigen类的3*3的旋转矩阵
			////四元数转为旋转矩阵--先归一化再转为旋转矩阵
			//R_odom_curr_tmp = q_odom_curr_tmp.normalized().toRotationMatrix();
			////三个点的世界坐标转换矩阵
			//mat_caliTrans <<
			//	R_odom_curr_tmp(0, 0), R_odom_curr_tmp(0, 1), R_odom_curr_tmp(0, 2), temp1[4],
			//	R_odom_curr_tmp(1, 0), R_odom_curr_tmp(1, 1), R_odom_curr_tmp(1, 2), temp1[5],
			//	R_odom_curr_tmp(2, 0), R_odom_curr_tmp(2, 1), R_odom_curr_tmp(2, 2), temp1[6],
			//	0, 0, 0, 1;

			//Vector4d pointOne(-5, 55, 8.13, 1);
			//Vector4d pointTwo(-5, 55, 68.13, 1);
			//Vector4d pointOne2 = mat_caliTrans * pointOne;//在NDI的坐标
			//Vector4d pointTwo2 = mat_caliTrans * pointTwo;//在NDI的坐标
			//mulNDITrans.push_back(Vector3d(pointOne2(0), pointOne2(1), pointOne2(2)));
			//mulNDITrans.push_back(Vector3d(pointTwo2(0), pointTwo2(1), pointTwo2(2)));

			//--------------位姿标定
			Quaterniond q_odom_curr_tmp;//声明一个Eigen类的四元数
			q_odom_curr_tmp.x() = temp1[1];
			q_odom_curr_tmp.y() = temp1[2];
			q_odom_curr_tmp.z() = temp1[3];
			q_odom_curr_tmp.w() = temp1[0];
			Matrix3d R_odom_curr_tmp;//声明一个Eigen类的3*3的旋转矩阵
			//四元数转为旋转矩阵--先归一化再转为旋转矩阵
			R_odom_curr_tmp = q_odom_curr_tmp.normalized().toRotationMatrix();
			//三个点的世界坐标转换矩阵
			mat_caliTrans <<
				R_odom_curr_tmp(0, 0), R_odom_curr_tmp(0, 1), R_odom_curr_tmp(0, 2), temp1[4],
				R_odom_curr_tmp(1, 0), R_odom_curr_tmp(1, 1), R_odom_curr_tmp(1, 2), temp1[5],
				R_odom_curr_tmp(2, 0), R_odom_curr_tmp(2, 1), R_odom_curr_tmp(2, 2), temp1[6],
				0, 0, 0, 1;
			qDebug() << "temp1[4]:" << temp1[4] << "temp1[5]:" << temp1[5] << "temp1[6]:" << temp1[6];
			//--------------位姿标定



			isCali = false;
		}
	}
}

void Calibration::cali()
{
	isCali = true;
}

//求得真实与虚拟转换矩阵mat_between
void  Calibration::transmat()
{
	////--------------------------点标定
	//Matrix4d S_z;
	//S_z <<
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, -1, 0,
	//	0, 0, 0, 1;
	//if (mulNDITrans.size() == mulUnityTrans.size() && mulNDITrans.size() > 3)
	//{
	//	mat_between_ULeft_NDIRight = S_z * Get3DR_TransMatrix(mulNDITrans, mulUnityTrans);
	//	//NDI右手相对于Unity左手
	//	emit setlabel(12);
	//}
	//else {
	//	emit setlabel(13);
	//}

	//---------------------------位姿标定
	Quaterniond q_odom_curr_tmp2;//声明一个Eigen类的四元数
	//此处进行赋值
	q_odom_curr_tmp2.x() = unityTrans[4];
	q_odom_curr_tmp2.y() = unityTrans[5];
	q_odom_curr_tmp2.z() = unityTrans[6];
	q_odom_curr_tmp2.w() = unityTrans[3];
	Matrix3d R_odom_curr_tmp2;//声明一个Eigen类的3*3的旋转矩阵

	R_odom_curr_tmp2 = q_odom_curr_tmp2.toRotationMatrix();
	//三个点的世界坐标转换矩阵
	Matrix4d mat_unityTrans;
	mat_unityTrans <<
		R_odom_curr_tmp2(0, 0), R_odom_curr_tmp2(0, 1), R_odom_curr_tmp2(0, 2), unityTrans[0],
		R_odom_curr_tmp2(1, 0), R_odom_curr_tmp2(1, 1), R_odom_curr_tmp2(1, 2), unityTrans[1],
		R_odom_curr_tmp2(2, 0), R_odom_curr_tmp2(2, 1), R_odom_curr_tmp2(2, 2), unityTrans[2],
		0, 0, 0, 1;

	Matrix4d S_z;
	S_z <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1;
	qDebug() << "unityTrans[0]:" << unityTrans[0] << "unityTrans[1]:" << unityTrans[1] << "unityTrans[2]:" << unityTrans[2];
	mat_between = mat_unityTrans * S_z * mat_caliTrans.inverse();//N右 相对于 U左 的变换
}

void Calibration::pts_drt_clear()
{
	mat_Registion <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
	pts_drt.clear();
	qDebug() << "配准点位已清除";
}

void Calibration::caliClear()
{
	mulNDITrans.clear();
	mulUnityTrans.clear();
	qDebug() << "------------buildingisclear";
}

vector<double>  Calibration::getTemp(int tooldata) {
	vector<double> temp;

	//跟踪

	temp.push_back(toolData[tooldata].transform.q0);
	temp.push_back(toolData[tooldata].transform.qx);
	temp.push_back(toolData[tooldata].transform.qy);
	temp.push_back(toolData[tooldata].transform.qz);

	temp.push_back(toolData[tooldata].transform.tx);
	temp.push_back(toolData[tooldata].transform.ty);
	temp.push_back(toolData[tooldata].transform.tz);
	temp.push_back(toolData[tooldata].transform.error);

	if (abs(temp[0]) > 1 || abs(temp[1]) > 1 || abs(temp[2]) > 1 || abs(temp[3]) > 1 ||
		abs(temp[4]) > 5000 || abs(temp[5]) > 5000 || abs(temp[6]) > 5000)
	{

		switch (tooldata)
		{
		case 0:
			emit setlabel(1);
			temp0Flag = false;
			break;
		case 1:
			emit setlabel(8);
			temp1Flag = false;
			break;
		case 2:
			emit setlabel(10);
			temp2Flag = false;
			break;
		case 3:
			emit setlabel(17);
			temp3Flag = false;
			break;
		case 4:
			temp4Flag = false;
			emit setlabel(19);
			break;
		case 5:
			temp5Flag = false;
			emit setlabel(21);
			break;
		case 6:
			temp6Flag = false;
			emit setlabel(23);
			break;
		}
	}
	else
	{
		switch (tooldata)
		{
		case 0:
			emit setlabel(2);
			temp0Flag = true;
			break;
		case 1:
			emit setlabel(9);
			temp1Flag = true;
			break;
		case 2:
			emit setlabel(11);
			temp2Flag = true;
			break;
		case 3:
			emit setlabel(18);
			temp3Flag = true;
			break;
		case 4:
			emit setlabel(20);
			temp4Flag = true;
			break;
		case 5:
			emit setlabel(22);
			temp5Flag = true;
			break;
		case 6:
			emit setlabel(24);
			temp6Flag = true;
			break;
		}
	}
	return temp;
}

void Calibration::GetTmpDate()
{
	temp0 = getTemp(0);
	temp1 = getTemp(1);
	temp2 = getTemp(2);
	temp3 = getTemp(3);
	temp4 = getTemp(4);
	temp5 = getTemp(5);
	temp6 = getTemp(6);
}


