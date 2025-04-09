#pragma once

#include <vtkObject.h>
#include <vtkObjectFactory.h>  
#include <vtkSmartPointer.h>  
#include <vtkActor.h>  
#include <vtkMatrix4x4.h>  
#include <vtkRenderer.h>  
#include <vtkProp3D.h>  
#include <vtkTransform.h>  
#include <vtkAxesActor.h>  
#include <vector>
#include <QDebug>
#include "MyVector3.h"

using namespace std;
class MyGroup :public vtkObject
{
public:
	vtkTypeMacro(MyGroup, vtkObject);
	static MyGroup *New();

	void setMatrix(vtkSmartPointer<vtkMatrix4x4> M);
	//添加子对象
	void addChild(vtkSmartPointer<vtkActor> actor);

	vtkSmartPointer<vtkActor> getChild(int index);

	//获取矩阵
	vtkSmartPointer<vtkMatrix4x4> getMatrix();

	vtkSmartPointer<vtkMatrix4x4> getAxesMatrix();
	vtkSmartPointer<vtkMatrix4x4> getAxesLocalM();

	void leftApplyMatrix(vtkSmartPointer<vtkMatrix4x4> M);

	void rotateOnAxis(double angleInDegree, double axis[3], double position[3]);
	void rotateOnAxis(double angleInDegree, MyVector3 axis, double position[3]);

	void rotateX(double angleInDegree);
	void rotateY(double angleInDegree);
	void rotateZ(double angleInDegree);

	void translateOnAxis(double distance, double axis[3]);


	void addChildInRender(vtkSmartPointer<vtkRenderer> render);

	void setVisibilty(bool visible);

	void setAxesVisibilty(bool visible);

	void setPosition(double x, double y, double z);
	void setPositionForHelper(double x, double y, double z);
	void getPosition(double position[3]);
	void addGroup(vtkSmartPointer<MyGroup> group);
	void backToOrigin();
	void clear();
	void setAxesMatrix(vtkSmartPointer<vtkMatrix4x4> matrix);

	vtkSmartPointer<vtkAxesActor> getAxes();
	vector<vtkSmartPointer<vtkActor> > child;
protected:
	MyGroup();
	~MyGroup();

private:
	vtkSmartPointer<vtkAxesActor> axes;
	vtkSmartPointer<vtkMatrix4x4> matrix;
	vtkSmartPointer<vtkMatrix4x4> axesInGroupM = vtkSmartPointer<vtkMatrix4x4>::New();
	vector<vtkSmartPointer<MyGroup> > groupChild;
	vector<vtkSmartPointer<vtkMatrix4x4> > childInGroupM;
	vector<vtkSmartPointer<vtkMatrix4x4> > groupInGroupM;
};