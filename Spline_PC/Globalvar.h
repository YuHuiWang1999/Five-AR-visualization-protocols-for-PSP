#pragma once

#include <vector>
#include <iostream>
#include <math.h>
//#include <cmath>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;


extern Matrix4d mat_caliTrans;
extern Matrix4d mat_registTrans;//Åä×¼¾ØÕó
extern float ProTransf[35];
extern bool isregistion;
extern float flagDatas[5];
extern float unityTrans[7];

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
extern vector<Vector3d> mulUnityTrans;
extern vector<Vector3d> mulNDITrans;
extern float GuideIndex;
extern Matrix4d mat_between_ULeft_NDIRight;
extern Matrix4d mat_between;
extern bool temp0Flag;//Probe
extern bool temp1Flag;//Cube
extern bool temp2Flag;//Bone
extern bool temp3Flag;//339Guide
extern bool temp4Flag;//449Guide
extern bool temp5Flag;//684760Drill
extern bool temp6Flag;//tibiaDRO
extern bool isConnect;

extern vector<double> temp0;
extern vector<double> temp1;
extern vector<double> temp2;
extern vector<double> temp3;
extern vector<double> temp4;
extern vector<double> temp5;
extern vector<double> temp6;

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//extern std::vector<Vector3d> pts_src;



class Globalvar
{
};

