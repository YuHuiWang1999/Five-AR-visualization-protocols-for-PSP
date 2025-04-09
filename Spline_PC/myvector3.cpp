#include "myvector3.h"

MyVector3::MyVector3(double x = 0.0, double y = 0.0, double z = 0.0)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

MyVector3::MyVector3(double arr[3])
{
	this->X = arr[0];
	this->Y = arr[1];
	this->Z = arr[2];
}

MyVector3::MyVector3(void)
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

MyVector3::~MyVector3()
{

}

double MyVector3::distanceTo(MyVector3 vec)
{
	double distance = sqrt((X - vec.X)*(X - vec.X) + (Y - vec.Y)*(Y - vec.Y) + (Z - vec.Z)*(Z - vec.Z));
	return distance;
}

double MyVector3::length()
{
	return sqrt(X * X + Y * Y + Z * Z);
}

void MyVector3::normalize()
{
	if (this->length() == 0.0) return;
	double x = this->X / this->length();
	double y = this->Y / this->length();
	double z = this->Z / this->length();
	this->X = x;
	this->Y = y;
	this->Z = z;
}

void MyVector3::getCenterTo(MyVector3 target, MyVector3 &center)
{
	center.setX((this->X + target.X) / 2);
	center.setY((this->Y + target.Y) / 2);
	center.setZ((this->Z + target.Z) / 2);
}

void MyVector3::cross(MyVector3 target, MyVector3 &result)
{
	double x = this->Y*target.Z - this->Z*target.Y;
	double y = this->Z*target.X - this->X*target.Z;
	double z = this->X*target.Y - this->Y*target.X;
	result.setX(x);
	result.setY(y);
	result.setZ(z);
}

void MyVector3::cross(MyVector3 target)
{
	double x = this->Y*target.Z - this->Z*target.Y;
	double y = this->Z*target.X - this->X*target.Z;
	double z = this->X*target.Y - this->Y*target.X;
	this->X = x;
	this->Y = y;
	this->Z = z;
}

double MyVector3::dot(MyVector3 vec)
{
	return this->X*vec.X + this->Y*vec.Y + this->Z*vec.Z;
}

void MyVector3::set(double x, double y, double z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

double MyVector3::getAngleInDegree(MyVector3 a, MyVector3 b)
{
	double tempCos = (a.X * b.X + a.Y * b.Y + a.Z * b.Z) /
		(sqrt(a.X * a.X + a.Y * a.Y + a.Z * a.Z)*
			sqrt(b.X * b.X + b.Y * b.Y + b.Z * b.Z));
	double angleInDegree = acos(tempCos) * 180 / 3.141592654;
	return angleInDegree;
}

void MyVector3::toArray(double arr[3])
{
	arr[0] = this->X;
	arr[1] = this->Y;
	arr[2] = this->Z;
}

void MyVector3::setFromArray(double arr[3])
{
	this->X = arr[0];
	this->Y = arr[1];
	this->Z = arr[2];
}

void MyVector3::sub(double vec[3])
{
	this->X = this->X - vec[0];
	this->Y = this->Y - vec[1];
	this->Z = this->Z - vec[2];
}

void MyVector3::sub(MyVector3 vec)
{
	this->X = this->X - vec.X;
	this->Y = this->Y - vec.Y;
	this->Z = this->Z - vec.Z;
}

void MyVector3::sub(MyVector3 vec, MyVector3 &vec1)
{
	vec1.X = this->X - vec.X;
	vec1.Y = this->Y - vec.Y;
	vec1.Z = this->Z - vec.Z;
}