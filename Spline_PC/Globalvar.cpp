#include "Globalvar.h"

float ProTransf[35];

float unityTrans[7] = { 0.1442,0.32445,0.5442,0.1741,0.142,0.11014,0.296441 };
bool isregistion = false;
vector<Vector3d> mulUnityTrans;
vector<Vector3d> mulNDITrans;
//float unityTrans[7];

float GuideIndex = 0;
float flagDatas[5];
bool temp0Flag;
bool temp1Flag;
bool temp2Flag;
bool temp3Flag;//339Guide
bool temp4Flag;//449Guide
bool temp5Flag;//684760Drill
bool temp6Flag;//tibiaDRO

bool isConnect;
vector<double> temp0;
vector<double> temp1;
vector<double> temp2;
vector<double> temp3;
vector<double> temp4;
vector<double> temp5;
vector<double> temp6;

Matrix4d mat_between_ULeft_NDIRight;
//std::vector<Vector3d> pts_src;

Matrix4d mat_caliTrans;
Matrix4d mat_between;

Matrix4d mat_registTrans;
