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
	//��ȡ����
	void getConnectData();
	//����NDI

	//��ʼconnectTimer
	void startConnectTimer();
	//����connectTimer
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

	////���ټ���339&449
	//void tracker339and449();


	////���ټ���760
	//void tracker760();

	////���ٱ궨��072
	//void tracker072();

	////���ٹ���684760
	//void tracker684760();
	////���ټ���760
	//void trackerprobe();

	//void changeRegisMat();



	////���ٹ���684760
	//void file_record_slotP();
	//void file_record_slotD();

	//	//���ٹ���684760
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
	bool fileFlag = false; //�ļ��Ƿ�򿪱�־
	bool time_open = false;

	QThread* Ndi_thread;

	NdiApi ndi;
	//����NDI����Ķ�ʱ��
	QTimer* connectTimer;
	//�ж�NDI�Ƿ�����

	bool isrecord = false;
	bool isCali = false;

	bool isHande;


	bool isPosition = false;
	bool isDrection = false;

	vector<ToolData> toolData;

	vector<Vector3d> pts_drt;//�ɼ���
	vector<Vector3d> pts_src;//��֪�ֲ�����

	float proTransf_between_average[7];

	//�궨Hololes2�� ��ʵ��λ
	//vector<Vector3d> Cali_posR;

	Matrix4d mat;
	Matrix4d mat_Registion;
	Matrix4d mat_pre;



	//����ʵ��
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
