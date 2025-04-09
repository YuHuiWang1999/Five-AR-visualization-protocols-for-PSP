/**
 * @class   UQICPRegister
 * @brief   Do ICP based on UQ algorithm,suitable for source data volume less than target data.
 *
*/
#pragma once
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>
#include <vtkPolyData.h>
#include <vtkOctreePointLocator.h>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <algorithm>
#include <iostream>   
using namespace std;
using namespace Eigen;
class UQICPRegister
{
public:
	UQICPRegister();
	~UQICPRegister();
public:
	void InputSourceData(vector<vector<double> > source);//源数据输入为Vector三维点数据，小数据
	void InputTargetData(vtkPolyData* Target);//目标数据输入为vtkDataSet数据，大数据
	void SetMaxOfIterations(int);//设置最大迭代次数
	void SetMinMeanError(double);//设置迭代终止距离阈值
	void SetCalMeanNum(int);//设置迭代终止距离阈值
	void GetICPResult(vtkMatrix4x4* Matrix);
	double GetICPDistance();
	double GetICPMeanError();
	int GetNumberOfIterations();



private:
	void PointSwith(vtkMatrix4x4* Matrix, double in[3], double out[3]);
	void UQRegister(vector<vector<double> > source, vector<vector<double> > target, vtkMatrix4x4* Matrix);
	void DoICP();
	int MaxOfIterations;
	int NumberOfIterations;

	double MinMeanError;
	double MeanError;
	int CalMeanNum;

	double ICPDistance;
	vtkSmartPointer<vtkMatrix4x4> Matrix_ICP = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkPolyData> TargetData = vtkSmartPointer<vtkPolyData>::New();
	vector<vector<double> > SourcePoints;
	bool is_InputSourceData;
	bool is_InputTargetData;
};


