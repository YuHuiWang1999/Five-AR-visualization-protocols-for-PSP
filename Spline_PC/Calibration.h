#pragma once

#include "NdiApi.h"
#include <QObject>
#include <QThread>
#include "Globalvar.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <cmath>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>
#include <fstream>
#include <QMessageBox>

#include <QTimer>
#include <QTime>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
using namespace Eigen;
using namespace cv;
using namespace std;



class Calibration : public QObject
{
	Q_OBJECT
public:

	Calibration(QObject *parent = nullptr);
	~Calibration();

	void NDI_Connect_init();
	//获取数据
	void getConnectData();
	//连接NDI

	//开始connectTimer
	void startConnectTimer();
	//结束connectTimer
	void stopConnectTimer();

	void record();
	void cali();
	void Driection();


	void getThreePoint();
	void getPoint();
	void transmat();
	//void getQuaternion();
	vector<double> getTemp(int tooldata);
	void pts_drt_clear();

	void caliClear();

	void registion();

	void GetTmpDate();

	Matrix4d Get3DR_TransMatrix(vector<Vector3d> srcPoints, vector<Vector3d> dstPoints);

	//void bd449position();

	////跟踪架子339&449
	//void tracker339and449();


	////跟踪架子760
	//void tracker760();

	////跟踪标定块072
	//void tracker072();

	////跟踪骨钻684760
	//void tracker684760();
	////跟踪架子760
	//void trackerprobe();

	//void changeRegisMat();



	////跟踪骨钻684760
	//void file_record_slotP();
	//void file_record_slotD();

	//	//跟踪骨钻684760
	//void file_stop_slotP();
	//void file_stop_slotD();

	//void updataForceFileP();
	//void updataForceFileD();

public slots:


	void NDI_Connect();
signals:
	void setlabel(int label);

private:

	QTextStream *stream1;
	QFile* file;
	QTimer* time_force;
	bool fileFlag = false; //文件是否打开标志
	bool time_open = false;

	QThread* Ndi_thread;

	NdiApi ndi;
	//连接NDI界面的定时器
	QTimer* connectTimer;
	//判断NDI是否连接

	bool isrecord = false;
	bool isCali = false;

	bool isHande;


	bool isPosition = false;
	bool isDrection = false;

	vector<ToolData> toolData;

	vector<Vector3d> pts_drt;//采集得
	vector<Vector3d> pts_src;//已知局部坐标

	float proTransf_between_average[7];

	//标定Hololes2的 现实点位
	//vector<Vector3d> Cali_posR;

	Matrix4d mat;
	Matrix4d mat_Registion;
	Matrix4d mat_pre;



	//动物实验
	Matrix4d mat_Registion_760;
	Matrix4d mat_Registion_449;


	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	int erroPointIndex = 0;

	//test
	int recordFlag = -1;

	int tempindex = 4;

	bool ttttttttt;
	//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
};
