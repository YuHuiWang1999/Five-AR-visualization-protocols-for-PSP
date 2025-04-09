#include "UQICPRegister.h"



UQICPRegister::UQICPRegister()
{
	MaxOfIterations = 200;

	MinMeanError = 0.000001;
	Matrix_ICP->Identity();
	MeanError = 0.0;
	ICPDistance = 0.0;
	NumberOfIterations = 0;
	is_InputSourceData = 0;
	is_InputTargetData = 0;
	CalMeanNum = 10000000000;
}


UQICPRegister::~UQICPRegister()
{
}
void UQICPRegister::PointSwith(vtkMatrix4x4* Matrix, double in[3], double out[3])
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
void UQICPRegister::InputSourceData(vector<vector<double> > SourcePoints)
{
	if (SourcePoints.empty())
	{
		cout << "DataError: SourcePoints is empty!!\n";
		is_InputSourceData = 0;
		return;
	}
	if (SourcePoints[0].size() != 3)
	{
		cout << "DataError: SourcePoints dimension error!!\n";
		is_InputSourceData = 0;
		return;
	}
	this->SourcePoints.swap(SourcePoints);
	is_InputSourceData = 1;

}
void UQICPRegister::InputTargetData(vtkPolyData* TargetData)
{
	if (TargetData->GetNumberOfPoints() == 0)
	{
		cout << "DataError: SourcePoints is empty!!\n";
		is_InputTargetData = 0;
		return;
	}
	this->TargetData->DeepCopy(TargetData);
	is_InputTargetData = 1;
}
void UQICPRegister::SetMaxOfIterations(int max)
{
	this->MaxOfIterations = max;
}
void UQICPRegister::SetMinMeanError(double MeanError)
{
	this->MinMeanError = MeanError;
}
void UQICPRegister::SetCalMeanNum(int CalMeanNum)
{
	this->CalMeanNum = CalMeanNum;
}
void  UQICPRegister::UQRegister(vector<vector<double> > source, vector<vector<double> > target, vtkMatrix4x4* Matrix)
{
	double norm_s[3] = { 0 };
	double norm_t[3] = { 0 };
	double r[9];
	double t[3];
	for (int i = 0; i < source.size(); i++)
	{
		norm_s[0] = norm_s[0] + source[i][0];
		norm_s[1] = norm_s[1] + source[i][1];
		norm_s[2] = norm_s[2] + source[i][2];
		norm_t[0] = norm_t[0] + target[i][0];
		norm_t[1] = norm_t[1] + target[i][1];
		norm_t[2] = norm_t[2] + target[i][2];
	}
	double center_s[3], center_t[3];
	center_s[0] = norm_s[0] / source.size();
	center_s[1] = norm_s[1] / source.size();
	center_s[2] = norm_s[2] / source.size();
	center_t[0] = norm_t[0] / target.size();
	center_t[1] = norm_t[1] / target.size();
	center_t[2] = norm_t[2] / target.size();
	double sxx = 0;
	double sxy = 0;
	double sxz = 0;
	double syx = 0;
	double syy = 0;
	double syz = 0;
	double szx = 0;
	double szy = 0;
	double szz = 0;
	for (int i = 0; i < source.size(); i++)
	{
		double x_s = source[i][0] - center_s[0];
		double y_s = source[i][1] - center_s[1];
		double z_s = source[i][2] - center_s[2];
		double x_t = target[i][0] - center_t[0];
		double y_t = target[i][1] - center_t[1];
		double z_t = target[i][2] - center_t[2];
		sxx = sxx + x_s * x_t;
		sxy = sxy + x_s * y_t;
		sxz = sxz + x_s * z_t;
		syx = syx + y_s * x_t;
		syy = syy + y_s * y_t;
		syz = syz + y_s * z_t;
		szx = szx + z_s * x_t;
		szy = szy + z_s * y_t;
		szz = szz + z_s * z_t;
	}
	MatrixXd N(4, 4);
	N << sxx + syy + szz, syz - szy, szx - sxz, sxy - syx,
		syz - szy, sxx - syy - szz, sxy + syx, szx + sxz,
		szx - sxz, sxy + syx, -sxx + syy - szz, syz + szy,
		sxy - syx, szx + sxz, syz + szy, -sxx - syy + szz;
	EigenSolver<Matrix4d> es(N);
	MatrixXcd evecs = es.eigenvectors();//获取矩阵特征向量4*4，这里定义的MatrixXcd必须有c，表示获得的是complex复数矩阵
	MatrixXcd evals = es.eigenvalues();//获取矩阵特征值 4*1
	MatrixXd evalsReal;//注意这里定义的MatrixXd里没有c
	evalsReal = evals.real();//获取特征值实数部分
	MatrixXf::Index evalsMax;
	evalsReal.rowwise().sum().maxCoeff(&evalsMax);//得到最大特征值的位置
	double q0 = evecs.real()(0, evalsMax);
	double qx = evecs.real()(1, evalsMax);
	double qy = evecs.real()(2, evalsMax);
	double qz = evecs.real()(3, evalsMax);
	r[0] = (q0 * q0) + (qx * qx) - (qy * qy) - (qz * qz);
	r[1] = 2 * ((qx * qy) - (q0 * qz));
	r[2] = 2 * ((qx * qz) + (q0 * qy));
	r[3] = 2 * ((qx * qy) + (q0 * qz));
	r[4] = (q0 * q0) - (qx * qx) + (qy * qy) - (qz * qz);
	r[5] = 2 * ((qy * qz) - (q0 * qx));
	r[6] = 2 * ((qx * qz) - (q0 * qy));
	r[7] = 2 * ((qy * qz) + (q0 * qx));
	r[8] = (q0 * q0) - (qx * qx) - (qy * qy) + (qz * qz);
	MatrixXd R = Map<MatrixXd>(r, 3, 3);
	Vector3d P1, P2, T;
	P1 << center_s[0], center_s[1], center_s[2];
	P2 << center_t[0], center_t[1], center_t[2];
	T = P2 - (R.transpose()) * P1;
	Matrix->SetElement(0, 0, r[0]);
	Matrix->SetElement(0, 1, r[1]);
	Matrix->SetElement(0, 2, r[2]);
	Matrix->SetElement(0, 3, T(0));
	Matrix->SetElement(1, 0, r[3]);
	Matrix->SetElement(1, 1, r[4]);
	Matrix->SetElement(1, 2, r[5]);
	Matrix->SetElement(1, 3, T(1));
	Matrix->SetElement(2, 0, r[6]);
	Matrix->SetElement(2, 1, r[7]);
	Matrix->SetElement(2, 2, r[8]);
	Matrix->SetElement(2, 3, T(2));
	Matrix->SetElement(3, 0, 0);
	Matrix->SetElement(3, 1, 0);
	Matrix->SetElement(3, 2, 0);
	Matrix->SetElement(3, 3, 1);
}
void UQICPRegister::DoICP()
{
	int NumberOfIterations = 1;
	bool is_Iterate = true;
	double MeanDistance;
	double oldMeanDistance = 100.0;

	vtkSmartPointer<vtkOctreePointLocator> targetLocator = vtkSmartPointer<vtkOctreePointLocator>::New();
	targetLocator->SetDataSet(this->TargetData);
	targetLocator->BuildLocator();
	vtkSmartPointer<vtkMatrix4x4> Matrix_oldICP = vtkSmartPointer<vtkMatrix4x4>::New();
	vector<vector<double> > target;
	while (is_Iterate && NumberOfIterations <= MaxOfIterations)
	{
		target.clear();
		vector<double> sp(3);
		double distant = 0;
		for (int i = 0; i < SourcePoints.size(); i = i++)
		{
			double dis;
			double ori_p[3], change_p[3];
			ori_p[0] = SourcePoints[i][0];
			ori_p[1] = SourcePoints[i][1];
			ori_p[2] = SourcePoints[i][2];
			PointSwith(Matrix_ICP, ori_p, change_p);
			double point[3];
			int index = targetLocator->FindClosestPoint(change_p[0], change_p[1], change_p[2], dis);
			//std::cout << "index " << index << " dis." << dis  << std::endl;
			dis = sqrt(dis);
			if (CalMeanNum >= SourcePoints.size())
			{
				distant = distant + dis;
			}
			else
			{
				if (i < CalMeanNum)
				{
					distant = distant + dis;
				}

			}

			targetLocator->GetDataSet()->GetPoint(index, point);
			sp[0] = point[0];
			sp[1] = point[1];
			sp[2] = point[2];
			target.push_back(sp);
		}
		UQRegister(SourcePoints, target, Matrix_ICP);
		if (CalMeanNum >= SourcePoints.size())
		{
			MeanDistance = distant / SourcePoints.size();
		}
		else
		{
			MeanDistance = distant / CalMeanNum;
		}

		if (((oldMeanDistance - MeanDistance) <= this->MinMeanError) || (NumberOfIterations == 200))
		{
			this->ICPDistance = oldMeanDistance;
			this->NumberOfIterations = NumberOfIterations - 1;
			this->MeanError = oldMeanDistance - MeanDistance;
			Matrix_ICP->DeepCopy(Matrix_oldICP);//////////////////记录上一次的配准矩阵为最终转换
			is_Iterate = false;
		}
		oldMeanDistance = MeanDistance;
		Matrix_oldICP->DeepCopy(Matrix_ICP);
		NumberOfIterations++;
	}
}
void UQICPRegister::GetICPResult(vtkMatrix4x4* Matrix)
{
	if (!is_InputSourceData || !is_InputTargetData)
	{
		cout << "DataError: Registration data missing!!\n";
		abort();
	}
	if ((int)this->TargetData->GetNumberOfPoints() < (int)this->SourcePoints.size())
	{
		cout << "DataError: Source is greater than Target!!\n";
		abort();
	}
	DoICP();
	Matrix->DeepCopy(this->Matrix_ICP);
}
double UQICPRegister::GetICPDistance()
{
	return this->ICPDistance;
}
double UQICPRegister::GetICPMeanError()
{
	return this->MeanError;
}
int UQICPRegister::GetNumberOfIterations()
{
	return this->NumberOfIterations;
}