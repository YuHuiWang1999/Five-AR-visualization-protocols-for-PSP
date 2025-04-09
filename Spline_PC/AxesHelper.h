#pragma once
#include <vtkObject.h>
#include <vtkObjectFactory.h>  
#include <vtkSTLReader.h>  
#include <vtkActor.h>  
#include <vtkPolyDataMapper.h>  
#include <vtkProperty.h>  
#include <vtkSmartPointer.h>  
#include <vtkRenderer.h>  
#include <vtkMatrix4x4.h>  
#include <vtkPlaneSource.h>  
#include <vtkLineSource.h>  
#include <vtkCylinderSource.h>  
#include <vtkPointSource.h>  
#include <vtkAxesActor.h> 
#include <vtkSphereSource.h> 
#include <vtkConeSource.h>
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkTransform.h>
#include "myrenderwindow.h"
#include "mygroup.h"
#include "myvector3.h"
#include <QDebug>
#include <string>
//类AxesHelper继承至vtkObject
class AxesHelper :public vtkObject
{
public:


	vtkSmartPointer<MyGroup> group = vtkSmartPointer<MyGroup>::New();
	//属性
	//鼠标按下时点击的位置
	double startPostion[3];
	//鼠标移动时点击的位置
	double endPositon[3];
	//鼠标按下时组坐标系的位置
	double groupOldPosition[3];
	vtkSmartPointer<vtkActor> tempPoint = vtkSmartPointer<vtkActor>::New();
	bool tempPointFlag = false;
	std::string axis = "";
	std::string mode = "";
	std::string objectType = "actor";
	vtkSmartPointer<MyGroup> plane = vtkSmartPointer<MyGroup>::New();
	vtkSmartPointer<vtkCylinderSource>planeSource = vtkSmartPointer<vtkCylinderSource>::New();
	bool colorChanged = false;
	bool dragging = false;
	vtkSmartPointer<vtkActor> object = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> object2 = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<MyGroup> groupObject = vtkSmartPointer<MyGroup>::New();
	vtkSmartPointer<vtkMatrix4x4> objectMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> groupObjectMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> objectInAxesM = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> groupObjectInAxesM = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderer> render2 = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<MyGroup> upGroup = nullptr;
	vtkSmartPointer<MyGroup> downGroup = nullptr;
	vtkSmartPointer<vtkLineSource> line[6];

private:
	bool isLock = false;
public:
	vtkTypeMacro(AxesHelper, vtkObject);
	static AxesHelper* New();
	void setLock(bool flag) {
		this->isLock = flag;
		updatePlane();
	};
	void upDateModel();
	void addInRender(vtkSmartPointer<vtkRenderer> render)
	{
		this->group->addChildInRender(render);
	}

	void backToOrigin();
	//转到MyGroup中运行getMatrix()
	vtkSmartPointer<vtkMatrix4x4> getMatrix()
	{
		return this->group->getMatrix();
	}

	void setPosition(double x, double y, double z)
	{
		this->group->setPositionForHelper(x, y, z);
	}

	void getPosition(double postion[3]);

	void setVisibilty(bool visible)
	{
		this->group->setVisibilty(visible);
	}

	vtkSmartPointer<vtkActor> getChild(int index)
	{
		return this->group->getChild(index);
	}

	void rotateOnAxis(double angleInDegree, double axis[3], double position[3]);

	void translateOnAxis(double distance, double axis[3])
	{
		this->group->translateOnAxis(distance, axis);
	}



	void updatePlane();


	void setObject(vtkSmartPointer<vtkActor> object);
	void setGroupObject(vtkSmartPointer<MyGroup> object);
	void updateObject(double x, double y, double z);
	void updateGroupObject();
	void rotateObject(double angleInDegree, double axis[3], double position[3]);
	void rotateGroupObject(double angleInDegree, double axis[3], double position[3]);
	void rotateArrow(vtkSmartPointer<vtkActor> objectX, vtkSmartPointer<vtkActor> objectY, vtkSmartPointer<vtkActor> objectZ);
	void updateObjectM();
	void updateGroupObjectM();



	void helperChildVisibility(bool isselect,bool flag, double size);
	void mouseMove(int x, int y, bool isMousePressed, vtkSmartPointer<vtkRenderer> render, QSize size);
	void test(double det);
	void middlePick(int x, int y, vtkSmartPointer<vtkRenderer> render, QSize size);
	void helperStyle(vtkSmartPointer<vtkRenderer> render);
	void normalStyle(vtkSmartPointer<vtkRenderer> render);
protected:

	AxesHelper();
	~AxesHelper();

private:
	vtkSmartPointer<vtkMatrix4x4> scaleM = vtkSmartPointer<vtkMatrix4x4>::New();
	//
	void initPoint();

	void printM(vtkSmartPointer<vtkMatrix4x4> M);

	enum INDEX
	{
		line_pick_x,//0
		line_pick_y,//1
		line_pick_z,//2
		//line_plane_x,//3
		//line_plane_y,//4
		//line_plane_z,//5
		line_assist_x,//6
		line_assist_y,//7
		line_assist_z,//8
		line_show_x,//9
		line_show_y,//10
		line_show_z,//11
		line_arrow_x,//12
		line_arrow_y,//13
		line_arrow_z,//14
		circle_show_x,//15
		circle_show_y,//16
		circle_show_z,//17
		circle_pick_x,//18
		circle_pick_y,//19
		circle_pick_z,//20
		//plane_pick,//15
	};
};

