#pragma once
#include <math.h>
#include <iostream>

using namespace std;
class MyVector3
{
public:
	MyVector3(double x, double y, double z);
	MyVector3(double arr[3]);
	MyVector3();
	~MyVector3();

	double distanceTo(MyVector3);

	double length();

	void normalize();

	double dot(MyVector3);

	void set(double x, double y, double z);

	void setX(double x)
	{
		this->X = x;
	}

	void setY(double y)
	{
		this->Y = y;
	}

	void setZ(double z)
	{
		this->Z = z;
	}

	void getCenterTo(MyVector3 target, MyVector3 &center);

	void cross(MyVector3 target, MyVector3 &result);
	void cross(MyVector3 target);

	double getX()
	{
		return this->X;
	}

	double getY()
	{
		return this->Y;
	}

	double getZ()
	{
		return this->Z;
	}

	void toArray(double arr[3]);

	void setFromArray(double arr[3]);

	void sub(double vec[3]);

	void sub(MyVector3 vec);

	void sub(MyVector3 vec, MyVector3 &vec1);

	static double getAngleInDegree(MyVector3 a, MyVector3 b);

	double X;
	double Y;
	double Z;
private:

};

