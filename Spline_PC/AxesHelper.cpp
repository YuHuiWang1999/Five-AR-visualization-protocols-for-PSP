#include "AxesHelper.h"

vtkStandardNewMacro(AxesHelper);

AxesHelper::AxesHelper()
{
	scaleM->SetElement(0, 0, 150);
	scaleM->SetElement(1, 1, 150);
	scaleM->SetElement(2, 2, 150);
	double length = 100.0;

	//三个虚拟坐标轴,用于射线检测
	//index 0 1 2
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
		cylinder->SetHeight(2 * length);
		cylinder->SetRadius(2.0);
		cylinder->SetResolution(5);
		cylinder->Update();
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(cylinder->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		if (i == 0)
		{
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateZ(-90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();
			actor->SetUserMatrix(M1);
		}
		else if (i == 2)
		{
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateX(90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();
			actor->SetUserMatrix(M1);
		}
		actor->GetProperty()->SetOpacity(0.01);
		this->group->addChild(actor);
	}

	//三个平面，用于射线检测
 //   //index 3 4 5
	//for (int i = 0; i < 3; ++i)
	//{
	//	vtkSmartPointer<vtkPlaneSource>planeSource = vtkSmartPointer<vtkPlaneSource>::New();
	//	planeSource->SetCenter(0, 0, 0);
	//	if (i == 0)
	//	{
	//		planeSource->SetNormal(1, 0, 0);
	//	}
	//	else if (i == 1)
	//	{
	//		planeSource->SetNormal(0, 1, 0);
	//	}
	//	else if (i == 2)
	//	{
	//		planeSource->SetNormal(0, 0, 1);
	//	}
	//	planeSource->Update();
	//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//	mapper->SetInputConnection(planeSource->GetOutputPort());
	//	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//	actor->SetMapper(mapper);
	//	actor->SetUserMatrix(scaleM);
	//	actor->GetProperty()->SetOpacity(0.001);
	//	actor->SetPickable(false);
	//	this->group->addChild(actor);
	//}

	////两个虚拟坐标轴,用于射线检测 x y z
	////index 0 1 2
	//for (int i = 0; i < 3; ++i)
	//{
	//	vtkSmartPointer<vtkSTLReader>reader = vtkSmartPointer<vtkSTLReader>::New();
	//	double axicolor[3];
	//	if (i == 0)
	//	{
	//		reader->SetFileName("./Modle/m/axi-x.STL");
	//	}
	//	else if (i == 1) {
	//		reader->SetFileName("./Modle/m/axi-z.STL");
	//	}
	//	else if (i == 2) {
	//		reader->SetFileName("./Modle/m/axi-x.STL");
	//	}
	//	reader->Update();
	//	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//	mapper->SetInputConnection(reader->GetOutputPort());
	//	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//	actor->SetMapper(mapper);
	//	actor->SetPickable(true);
	//	actor->GetProperty()->SetOpacity(0.001);
	//	this->group->addChild(actor);
	//}

	//三条直线，用于辅助显示坐标系的方向
	//index 6 7 8
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkLineSource>lineSource = vtkSmartPointer<vtkLineSource>::New();
		double length = 10000;
		if (i == 0)
		{
			lineSource->SetPoint1(length, 0, 0);
			lineSource->SetPoint2(-length, 0, 0);
		}
		else if (i == 1)
		{
			lineSource->SetPoint1(0, length, 0);
			lineSource->SetPoint2(0, -length, 0);
		}
		else if (i == 2)
		{
			lineSource->SetPoint1(0, 0, length);
			lineSource->SetPoint2(0, 0, -length);
		}
		lineSource->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(lineSource->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		actor->SetVisibility(false);
		actor->GetProperty()->SetColor(0.5, 0.5, 0.5);
		actor->GetProperty()->SetRenderLinesAsTubes(true);
		actor->GetProperty()->SetLineWidth(1.0);
		actor->SetPickable(false);

		this->group->addChild(actor);
	}
	   	  
	//三个真实坐标轴
	//index 9 10 11
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkLineSource>lineSource = vtkSmartPointer<vtkLineSource>::New();
		double length = 100.0;
		lineSource->SetPoint1(0, 0, 0);
		if (i == 0)
			lineSource->SetPoint2(length, 0, 0);
		else if (i == 1)
			lineSource->SetPoint2(0, length, 0);
		else if (i == 2)
			lineSource->SetPoint2(0, 0, length);
		lineSource->Update();

		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(lineSource->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		if (i == 0)
			actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
		else if (i == 1)
			actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
		else if (i == 2)
			actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
		actor->SetPickable(false);
		actor->GetProperty()->SetRenderLinesAsTubes(true);
		actor->GetProperty()->SetLineWidth(2.0);
		actor->GetProperty()->SetOpacity(1.0);
		this->group->addChild(actor);
	}

	//三个箭头
	//index 12 13 14
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkConeSource>cone = vtkSmartPointer<vtkConeSource>::New();
		cone->SetHeight(6.0);
		cone->SetRadius(1.0);
		cone->SetResolution(10);

		vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		cylinderMapper->SetInputConnection(cone->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(cylinderMapper);
		if (i == 0)
		{
			actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

			vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
			M1->SetElement(0, 3, 100.0);
			actor->SetUserMatrix(M1);
		}
		else if (i == 1)
		{
			actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateZ(90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();
			M1->SetElement(1, 3, 100.0);
			actor->SetUserMatrix(M1);
		}
		else if (i == 2)
		{
			actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateY(-90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();
			M1->SetElement(2, 3, 100.0);
			actor->SetUserMatrix(M1);
			//actor->RotateY(-90.0);
			//actor->SetPosition(0, 0, 35);
		}
		actor->SetPickable(false);

		this->group->addChild(actor);
	}



	//三个真实圆环，用于显示
	//index 15 16 17
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkCylinderSource>cylinder = vtkSmartPointer<vtkCylinderSource>::New();
		cylinder->SetHeight(0.0);
		cylinder->SetRadius(25.0);
		cylinder->SetResolution(40);
		vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		cylinderMapper->SetInputConnection(cylinder->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(cylinderMapper);
		vtkSmartPointer<vtkProperty> backfaceProperty = vtkSmartPointer<vtkProperty>::New();
		if (i == 0)
		{
			actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateZ(-90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();

			actor->SetUserMatrix(M1);
			//actor->RotateZ(-90.0);
			backfaceProperty->SetColor(1.0, 0.0, 0.0);
		}
		else if (i == 1)
		{
			actor->GetProperty()->SetColor(0.0, 1.0, 0.0);
			backfaceProperty->SetColor(0.0, 1.0, 0.0);
		}
		else if (i == 2)
		{
			actor->GetProperty()->SetColor(0.0, 0.0, 1.0);
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateX(90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();

			actor->SetUserMatrix(M1);

			//actor->RotateX(90.0);
			backfaceProperty->SetColor(0.0, 0.0, 1.0);
		}
		actor->SetBackfaceProperty(backfaceProperty);
		actor->GetProperty()->SetRepresentationToWireframe();
		actor->GetProperty()->SetEdgeColor(1.0, 0.0, 0.0);
		actor->GetProperty()->SetEdgeVisibility(true);
		actor->GetProperty()->SetLineWidth(1.2);
		actor->GetProperty()->SetRenderLinesAsTubes(true);
		actor->SetPickable(false);
		actor->SetVisibility(false);

		this->group->addChild(actor);
	}

	//三个虚拟环，用于射线拾取
	//index 18 19 20
	for (int i = 0; i < 3; ++i)
	{
		vtkSmartPointer<vtkSTLReader>reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName("./Modle/m/rot-z.stl");
		reader->Update();
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputConnection(reader->GetOutputPort());
		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		if (i == 0)
		{
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateZ(90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();

			actor->SetUserMatrix(M1);

			//actor->RotateZ(90);
		}
		else if (i == 2)
		{
			vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
			translation->RotateX(-90.0);
			vtkSmartPointer<vtkMatrix4x4> M1 = translation->GetMatrix();

			actor->SetUserMatrix(M1);

			//actor->RotateX(-90);
		}

		actor->GetProperty()->SetOpacity(0.01);
		actor->SetVisibility(false);

		this->group->addChild(actor);
	}

	initPoint();   
	//平移
	this->getChild(line_pick_x)->SetVisibility(false);
	this->getChild(line_pick_y)->SetVisibility(false);
	this->getChild(line_pick_z)->SetVisibility(false);
	//辅助线
	this->getChild(line_show_x)->SetVisibility(false);
	this->getChild(line_show_y)->SetVisibility(false);
	this->getChild(line_show_z)->SetVisibility(false);
	//箭头
	this->getChild(line_arrow_x)->SetVisibility(false);
	this->getChild(line_arrow_y)->SetVisibility(false);
	this->getChild(line_arrow_z)->SetVisibility(false);
	//旋转
	this->getChild(circle_show_x)->SetVisibility(false);
	this->getChild(circle_show_y)->SetVisibility(false);
	this->getChild(circle_show_z)->SetVisibility(false);
	this->getChild(circle_pick_z)->SetVisibility(false);
	this->getChild(circle_pick_x)->SetVisibility(false);
	this->getChild(circle_pick_y)->SetVisibility(false);
}

void AxesHelper::helperChildVisibility(bool isselect,bool flag, double size)
{
	if (isselect)
	{
		//平移
		this->getChild(line_pick_x)->SetVisibility(flag);
		this->getChild(line_pick_x)->SetScale(size, size, size);
		this->getChild(line_pick_y)->SetVisibility(flag);
		this->getChild(line_pick_y)->SetScale(size, size, size);
		this->getChild(line_pick_z)->SetVisibility(flag);
		this->getChild(line_pick_z)->SetScale(size, size, size);
		////辅助线
		this->getChild(line_show_x)->SetVisibility(flag);
		this->getChild(line_show_y)->SetVisibility(flag);
		this->getChild(line_show_z)->SetVisibility(flag);
		////箭头
		this->getChild(line_arrow_x)->SetVisibility(flag);
		this->getChild(line_arrow_x)->SetScale(size, size, size);
		this->getChild(line_arrow_y)->SetVisibility(flag);
		this->getChild(line_arrow_y)->SetScale(size, size, size);
		this->getChild(line_arrow_z)->SetVisibility(flag);
		this->getChild(line_arrow_z)->SetScale(size, size, size);
		//旋转
		this->getChild(circle_show_x)->SetVisibility(!flag);
		this->getChild(circle_show_x)->SetScale(size, size, size);
		this->getChild(circle_show_y)->SetVisibility(!flag);
		this->getChild(circle_show_y)->SetScale(size, size, size);
		this->getChild(circle_show_z)->SetVisibility(!flag);
		this->getChild(circle_show_z)->SetScale(size, size, size);
		this->getChild(circle_pick_z)->SetVisibility(!flag);
		this->getChild(circle_pick_z)->SetScale(size, size, size);
		this->getChild(circle_pick_x)->SetVisibility(!flag);
		this->getChild(circle_pick_x)->SetScale(size, size, size);
		this->getChild(circle_pick_y)->SetVisibility(!flag);
		this->getChild(circle_pick_y)->SetScale(size, size, size);
	}
	else 
	{
		//平移
		this->getChild(line_pick_x)->SetVisibility(false);
		this->getChild(line_pick_y)->SetVisibility(false);
		this->getChild(line_pick_z)->SetVisibility(false);
		//辅助线
		this->getChild(line_show_x)->SetVisibility(false);
		this->getChild(line_show_y)->SetVisibility(false);
		this->getChild(line_show_z)->SetVisibility(false);
		//箭头
		this->getChild(line_arrow_x)->SetVisibility(false);
		this->getChild(line_arrow_y)->SetVisibility(false);
		this->getChild(line_arrow_z)->SetVisibility(false);
		//旋转
		this->getChild(circle_show_x)->SetVisibility(false);
		this->getChild(circle_show_y)->SetVisibility(false);
		this->getChild(circle_show_z)->SetVisibility(false);
		this->getChild(circle_pick_z)->SetVisibility(false);
		this->getChild(circle_pick_x)->SetVisibility(false);
		this->getChild(circle_pick_y)->SetVisibility(false);	
	}

	//this->updatePlane();
}

void AxesHelper::updatePlane()
{
	//if (isLock) return;

	////相机投影方向
	//double project[3];
	//render->GetActiveCamera()->GetDirectionOfProjection(project);
	////直线或平面的初始方向
	//double a[3] = { 0,1,0 };
	////更新plane_pick、circle_show_eye、circle_pick_eye
	//{
	//	//相机投影方向和a的夹角
	//	double tempCos = (a[0] * project[0] + a[1] * project[1] + a[2] * project[2]) /
	//		(sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) *
	//			sqrt(project[0] * project[0] + project[1] * project[1] + project[2] * project[2]));
	//	double angleInDegree = acos(tempCos) * 180 / 3.141592654;
	//	//相机投影方向和a的叉乘
	//	double x = a[1] * project[2] - a[2] * project[1];
	//	double y = a[2] * project[0] - a[0] * project[2];
	//	double z = a[0] * project[1] - a[1] * project[0];

	//	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	//	translation->RotateWXYZ(angleInDegree, x, y, z);

	//	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	//	M1 = translation->GetMatrix();
	//	double pos[3];
	//	this->getPosition(pos);
	//	M1->SetElement(0, 3, pos[0]);
	//	M1->SetElement(1, 3, pos[1]);
	//	M1->SetElement(2, 3, pos[2]);
	//	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	//	vtkMatrix4x4::Multiply4x4(M1, scaleM, M);
	//	this->getChild(plane_pick)->SetUserMatrix(M);
	//	//this->getChild(circle_show_eye)->SetUserMatrix(M1);
	//	//this->getChild(circle_pick_eye)->SetUserMatrix(M1);
	//	//this->getChild(line_pick_z)->SetUserMatrix(M1);
	//	//this->getChild(line_assist_z)->SetUserMatrix(M1);
	//	//this->getChild(line_show_z)->SetUserMatrix(M1);
	//}
	//double up[3];
	//double left[3];
	//render->GetActiveCamera()->GetViewUp(up);
	////更新line_pick_y、line_assist_y、line_show_y
	//{
	//	double tempCos = (a[0] * up[0] + a[1] * up[1] + a[2] * up[2]) /
	//		(sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) *
	//			sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]));
	//	double angleInDegree = acos(tempCos) * 180 / 3.141592654;
	//	double x = a[1] * up[2] - a[2] * up[1];
	//	double y = a[2] * up[0] - a[0] * up[2];
	//	double z = a[0] * up[1] - a[1] * up[0];
	//	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	//	translation->RotateWXYZ(angleInDegree, x, y, z);
	//	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	//	M1 = translation->GetMatrix();
	//	double pos[3];
	//	this->getPosition(pos);
	//	M1->SetElement(0, 3, pos[0]);
	//	M1->SetElement(1, 3, pos[1]);
	//	M1->SetElement(2, 3, pos[2]);
	//	//this->getChild(line_pick_y)->SetUserMatrix(M1);
	//	//this->getChild(line_assist_y)->SetUserMatrix(M1);
	//	//this->getChild(line_show_y)->SetUserMatrix(M1);
	//	//double position[3];
	//	//this->getChild(line_show_y)->GetPosition(position);
	//	//double scale[3];
	//	//this->getChild(line_show_y)->GetScale(scale);
	//	////cout << "  M矩阵是=" << position[0] << position[1] << position[2] << "  line_show_y的Scale是=" << scale[0] << scale[1] << scale[2];

	//}
	////更新line_pick_x、line_assist_x、line_show_x
	//{
	//	left[0] = up[1] * project[2] - up[2] * project[1];
	//	left[1] = up[2] * project[0] - up[0] * project[2];
	//	left[2] = up[0] * project[1] - up[1] * project[0];
	//	double tempCos = (a[0] * left[0] + a[1] * left[1] + a[2] * left[2]) /
	//		(sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) *
	//			sqrt(left[0] * left[0] + left[1] * left[1] + left[2] * left[2]));
	//	double angleInDegree = acos(tempCos) * 180 / 3.141592654;
	//	double x = a[1] * left[2] - a[2] * left[1];
	//	double y = a[2] * left[0] - a[0] * left[2];
	//	double z = a[0] * left[1] - a[1] * left[0];
	//	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	//	translation->RotateWXYZ(angleInDegree, x, y, z);
	//	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	//	M1 = translation->GetMatrix();
	//	double pos[3];
	//	this->getPosition(pos);
	//	M1->SetElement(0, 3, pos[0]);
	//	M1->SetElement(1, 3, pos[1]);
	//	M1->SetElement(2, 3, pos[2]);
	//	//this->getChild(line_pick_x)->SetUserMatrix(M1);
	//	//this->getChild(line_assist_x)->SetUserMatrix(M1);
	//	//this->getChild(line_show_x)->SetUserMatrix(M1);
	//}
}

void AxesHelper::printM(vtkSmartPointer<vtkMatrix4x4> M)
{
	for (int i = 0; i < 4; ++i)
	{
		qDebug() << M->GetElement(i, 0) << " " << M->GetElement(i, 1) << " " << M->GetElement(i, 2) << " " << M->GetElement(i, 3);
	}
}
//那个中心点小球
void AxesHelper::initPoint()
{
	vtkSmartPointer<vtkSphereSource> pointSource = vtkSmartPointer<vtkSphereSource>::New();
	pointSource->SetCenter(0, 0, 0);
	pointSource->SetRadius(1.0);
	pointSource->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	mapper->SetInputData(pointSource->GetOutput());

	tempPoint->SetMapper(mapper);
	tempPoint->GetProperty()->SetColor(0.6, 0.6, 0.6);
	tempPoint->GetProperty()->SetPointSize(0.8);
	tempPoint->SetPickable(false);
}
//获取Actor的点的位置从Mat的t中获取--math工具
void AxesHelper::getPosition(double position[3])
{
	position[0] = this->getMatrix()->GetElement(0, 3);
	position[1] = this->getMatrix()->GetElement(1, 3);
	position[2] = this->getMatrix()->GetElement(2, 3);
}

void AxesHelper::backToOrigin()
{
	this->group->backToOrigin();
	this->object = vtkSmartPointer<vtkActor>::New();
}

void AxesHelper::rotateOnAxis(double angleInDegree, double axis[3], double position[3])
{
	this->group->rotateOnAxis(angleInDegree, axis, position);
}

void AxesHelper::setObject(vtkSmartPointer<vtkActor> object)
{
	this->object = object;
	updateObjectM();
}

void AxesHelper::setGroupObject(vtkSmartPointer<MyGroup> object)
{
	this->groupObject = object;
	updateGroupObjectM();
}

void AxesHelper::updateObjectM()
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();

	M1->DeepCopy(this->object->GetMatrix());
	M2->DeepCopy(this->group->getMatrix());
	M2->Invert();
	vtkMatrix4x4::Multiply4x4(M2, M1, objectInAxesM);
}

void AxesHelper::updateGroupObjectM()
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();

	M1->DeepCopy(this->groupObject->getMatrix());
	M2->DeepCopy(this->group->getMatrix());
	M2->Invert();
	vtkMatrix4x4::Multiply4x4(M2, M1, groupObjectInAxesM);
}

void AxesHelper::updateObject(double x, double y, double z)
{
	vtkSmartPointer<vtkMatrix4x4> M1 = this->object->GetMatrix();
	M1->SetElement(0, 3, x);
	M1->SetElement(1, 3, y);
	M1->SetElement(2, 3, z);
	//std::cout << "Matrix M1:" << std::endl;
	//for (int i = 0; i < 4; ++i) {
	//	for (int j = 0; j < 4; ++j) {
	//		std::cout << M1->GetElement(i, j) << " ";
	//	}
	//	std::cout << std::endl;
	//}
	this->object->SetUserMatrix(M1);
	this->group->setMatrix(M1);
	//更新子对象的坐标
}

void AxesHelper::updateGroupObject()
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	M1->DeepCopy(this->group->getMatrix());
	vtkMatrix4x4::Multiply4x4(M1, groupObjectInAxesM, M2);
	this->groupObject->setMatrix(M2);
}

void AxesHelper::rotateArrow(vtkSmartPointer<vtkActor> objectX, vtkSmartPointer<vtkActor> objectY, vtkSmartPointer<vtkActor> objectZ)
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M3 = vtkSmartPointer<vtkMatrix4x4>::New();
	double matrix1[16] = {
	1, 0, 0, 35,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	};
	double matrix2[16] = {
	1, 0, 0, 0,
	0, 1, 0, 35,
	0, 0, 1, 0,
	0, 0, 0, 1
	};
	double matrix3[16] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 35,
	0, 0, 0, 1
	};
	M1->DeepCopy(matrix1);
	M2->DeepCopy(matrix2);
	M3->DeepCopy(matrix3);
	vtkSmartPointer<vtkMatrix4x4> MTX = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> MTY = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> MTZ = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> MX = objectX->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> MY = objectY->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> MZ = objectZ->GetMatrix();
	vtkMatrix4x4::Multiply4x4(M1, MX, MTX);
	vtkMatrix4x4::Multiply4x4(M2, MY, MTY);
	vtkMatrix4x4::Multiply4x4(M3, MZ, MTZ);
	objectX->SetUserMatrix(MTX);
	objectY->SetUserMatrix(MTY);
	objectZ->SetUserMatrix(MTZ);
	//更新子对象的坐标
}

void AxesHelper::rotateGroupObject(double angleInDegree, double axis[3], double position[3])
{
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->RotateWXYZ(angleInDegree, axis[0], axis[1], axis[2]);

	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M3 = vtkSmartPointer<vtkMatrix4x4>::New();

	//旋转矩阵
	M2 = translation->GetMatrix();

	double matrix1[16] = {
	1, 0, 0, -position[0],
	0, 1, 0, -position[1],
	0, 0, 1, -position[2],
	0, 0, 0, 1
	};
	//将模型移动到坐标原点
	M1->DeepCopy(matrix1);

	double matrix3[16] = {
	1, 0, 0, position[0],
	0, 1, 0, position[1],
	0, 0, 1, position[2],
	0, 0, 0, 1
	};
	//将模型移动到原来的位置
	M3->DeepCopy(matrix3);


	vtkSmartPointer<vtkMatrix4x4> MT1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> MT2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M3, M2, MT1);
	vtkMatrix4x4::Multiply4x4(MT1, M1, MT2);

	{
		vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(MT2, this->groupObjectMatrix, MT2);
		this->groupObject->setMatrix(MT2);
	}
}

void AxesHelper::upDateModel()
{
	if (upGroup == nullptr && downGroup == nullptr) return;

	double temp[6] = { 0 };
	for (int i = 0; i < 6; ++i)
	{
		double point1[3];
		double point2[3];
		point1[0] = upGroup->getChild(i + 1)->GetMatrix()->GetElement(0, 3);
		point1[1] = upGroup->getChild(i + 1)->GetMatrix()->GetElement(1, 3);
		point1[2] = upGroup->getChild(i + 1)->GetMatrix()->GetElement(2, 3);

		point2[0] = downGroup->getChild(i + 1)->GetMatrix()->GetElement(0, 3);
		point2[1] = downGroup->getChild(i + 1)->GetMatrix()->GetElement(1, 3);
		point2[2] = downGroup->getChild(i + 1)->GetMatrix()->GetElement(2, 3);

		line[i]->SetPoint1(point1);

		line[i]->SetPoint2(point2);

		line[i]->Update();
		temp[i] = (point1[0] - point2[0]) * (point1[0] - point2[0]) +
			(point1[1] - point2[1]) * (point1[1] - point2[1]) +
			(point1[2] - point2[2]) * (point1[2] - point2[2]);
		temp[i] = sqrt(temp[i]);

	}
	//qDebug() << temp[0] << " " << temp[1] << " " << temp[2] << " " << temp[3] << " " << temp[4] << " " << temp[5];

}

void AxesHelper::rotateObject(double angleInDegree, double axis[3], double position[3])
{
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->RotateWXYZ(angleInDegree, axis[0], axis[1], axis[2]);
	translation->GetMatrix();
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> M3 = vtkSmartPointer<vtkMatrix4x4>::New();

	//旋转矩阵
	M2 = translation->GetMatrix();

	double matrix1[16] = {
	1, 0, 0, -position[0],
	0, 1, 0, -position[1],
	0, 0, 1, -position[2],
	0, 0, 0, 1
	};
	//将模型移动到坐标原点
	M1->DeepCopy(matrix1);

	double matrix3[16] = {
	1, 0, 0, position[0],
	0, 1, 0, position[1],
	0, 0, 1, position[2],
	0, 0, 0, 1
	};
	//将模型移动到原来的位置
	M3->DeepCopy(matrix3);

	vtkSmartPointer<vtkMatrix4x4> MT1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkSmartPointer<vtkMatrix4x4> MT2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M3, M2, MT1);
	vtkMatrix4x4::Multiply4x4(MT1, M1, MT2);

	{
		vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(MT2, this->objectMatrix, MT2);
		this->object->SetUserMatrix(MT2);
		this->group->setMatrix(MT2);
	}
}

AxesHelper::~AxesHelper()
{
}

void AxesHelper::helperStyle(vtkSmartPointer<vtkRenderer> render)
{
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	//style->SetMotionFactor(0.1);
	render->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::RightButtonPressEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::LeftButtonPressEvent);


	/*render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::RightButtonPressEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::LeftButtonPressEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::MouseWheelForwardEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::MouseWheelBackwardEvent);*/
}

void AxesHelper::normalStyle(vtkSmartPointer<vtkRenderer> render)
{
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
	vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	//style->SetMotionFactor(0.1);
	render->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::RightButtonPressEvent);

	/*render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::RightButtonPressEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::LeftButtonPressEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::MouseWheelForwardEvent);
	render->GetRenderWindow()->GetInteractor()->RemoveObservers(vtkCommand::MouseWheelBackwardEvent);*/
}

void AxesHelper::middlePick(int x, int y, vtkSmartPointer<vtkRenderer> render, QSize size)
{
	//if (!this->getChild(line_show_x)->GetVisibility() && !this->getChild(circle_show_eye)->GetVisibility()) return;

	//double position[3];
	//vtkSmartPointer<vtkCellPicker> cellPicker =
	//	vtkSmartPointer<vtkCellPicker>::New();
	//cellPicker->SetTolerance(0.005);
	//cellPicker->Pick(x, size.height() - y - 1, 0, render);
	//if (cellPicker->GetPointId() < 0)
	//	return;
	//cellPicker->GetPickPosition(position);
	//this->setPosition(position[0], position[1], position[2]);
	//if (this->objectType == "actor")
	//	this->updateObjectM();
	//else
	//	this->updateGroupObjectM();
	//this->updatePlane();
	//render->GetRenderWindow()->Render();
}
//没什么用
void AxesHelper::test(double det)
{
	//this->getPosition(this->groupOldPosition);
	//
	//double vec4[3];

	//double eye[3];
	//vtkSmartPointer<vtkMatrix4x4> M = this->getChild(line_assist_z)->GetMatrix();

	//eye[0] = M->GetElement(0, 1);
	//eye[1] = M->GetElement(1, 1);
	//eye[2] = M->GetElement(2, 1);

	//vec4[0] = det * eye[0];
	//vec4[1] = det * eye[1];
	//vec4[2] = det * eye[2];

	////qDebug() << vec4[0] << " " << vec4[1] << " " << vec4[2];
	//double curPos[3] = { this->groupOldPosition[0] + vec4[0] ,
	//	this->groupOldPosition[1] + vec4[1], this->groupOldPosition[2] + vec4[2] };

	//this->group->setPositionForHelper(curPos[0], curPos[1], curPos[2]);

	//this->updateObject();

	//render->GetRenderWindow()->Render();
}

///main
void AxesHelper::mouseMove(int x, int y, bool isMousePressed, vtkSmartPointer<vtkRenderer> render, QSize size)
{
	if (this->mode == "") return;

	//vtkCellPicker是获取点位置用的-全局坐标
	vtkSmartPointer<vtkCellPicker> cellPicker =vtkSmartPointer<vtkCellPicker>::New();
	cellPicker->Pick(x, size.height() - y - 1, 0, render);
	if (this->mode == "translate")
	{
		if (cellPicker->GetActor() == this->getChild(line_pick_x))
		{
			if (!isMousePressed)
			{
				this->colorChanged = true;
				this->axis = "X";
				this->getChild(line_assist_x)->SetVisibility(true);
				this->getChild(line_pick_y)->SetPickable(true);
				this->getChild(line_pick_z)->SetPickable(true);
				this->getChild(line_show_z)->GetProperty()->SetColor(0, 0, 1);
				this->getChild(line_show_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_arrow_z)->GetProperty()->SetColor(0, 0, 1);
				this->getChild(line_arrow_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_show_x)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				this->getChild(line_arrow_x)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				//this->getChild(line_plane_y)->SetPickable(true);
				//this->getChild(line_plane_z)->SetPickable(true);
				cellPicker->GetPickPosition(this->startPostion);
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
			{
				this->dragging = false;
				render->RemoveActor(this->tempPoint);
				this->tempPointFlag = false;
			}
		}
		else if (cellPicker->GetActor() == this->getChild(line_pick_y))
		{
			if (!isMousePressed)
			{
				this->colorChanged = true;
				this->axis = "Y";
				this->getChild(line_assist_y)->SetVisibility(true);
				this->getChild(line_pick_x)->SetPickable(true);
				this->getChild(line_pick_z)->SetPickable(true);
				this->getChild(line_show_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_show_x)->GetProperty()->SetColor(1, 0, 0);
				this->getChild(line_arrow_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_arrow_x)->GetProperty()->SetColor(1, 0, 0);
				this->getChild(line_show_y)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				this->getChild(line_arrow_y)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				//this->getChild(line_plane_x)->SetPickable(true);
				//this->getChild(line_plane_z)->SetPickable(true);
				cellPicker->GetPickPosition(this->startPostion);
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
			{
				this->dragging = false;
				render->RemoveActor(this->tempPoint);
				this->tempPointFlag = false;
			}
		}
		else if (cellPicker->GetActor() == this->getChild(line_pick_z))
		{
			if (!isMousePressed)
			{
				this->colorChanged = true;
				this->axis = "Z";
				this->getChild(line_assist_z)->SetVisibility(true);
				this->getChild(line_pick_x)->SetPickable(true);
				this->getChild(line_pick_y)->SetPickable(true);
				this->getChild(line_show_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_show_x)->GetProperty()->SetColor(1, 0, 0);
				this->getChild(line_arrow_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_arrow_x)->GetProperty()->SetColor(1, 0, 0);
				this->getChild(line_show_z)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				this->getChild(line_arrow_z)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				//this->getChild(line_plane_x)->SetPickable(true);
				//this->getChild(line_plane_y)->SetPickable(true);
				cellPicker->GetPickPosition(this->startPostion);
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
			{
				this->dragging = false;
				render->RemoveActor(this->tempPoint);
				this->tempPointFlag = false;
			}
		}
		else
		{
			// 恢复正常颜色  只有在颜色改变后且鼠标没按下的情况下执行一下代码
			if (!isMousePressed && this->colorChanged)
			{
				this->getChild(line_assist_x)->SetVisibility(false);
				this->getChild(line_assist_y)->SetVisibility(false);
				this->getChild(line_assist_z)->SetVisibility(false);
				this->getChild(line_show_z)->GetProperty()->SetColor(0, 0, 1);
				this->getChild(line_show_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_show_x)->GetProperty()->SetColor(1, 0, 0);
				this->getChild(line_arrow_z)->GetProperty()->SetColor(0, 0, 1);
				this->getChild(line_arrow_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(line_arrow_x)->GetProperty()->SetColor(1, 0, 0);
				render->RemoveActor(this->tempPoint);
				this->tempPointFlag = false;
				this->colorChanged = false;
				this->dragging = false;
				//恢复正常的交互方式
				normalStyle(render);
				render->GetRenderWindow()->Render();
			}
		}
		//如果被拖拽了的话
		if (this->dragging)
		{
			if (!this->tempPointFlag)
			{
				//拖动后-设置小球新的位置
				this->tempPoint->SetPosition(this->groupOldPosition);
				render->AddActor(this->tempPoint);
				this->tempPointFlag = true;
			}
			if (isLock)
			{
				if ((this->axis == "Y" && cellPicker->GetActor() == getChild(line_pick_y))|| (this->axis == "X" && cellPicker->GetActor() == getChild(line_pick_x))|| (this->axis == "Z" && cellPicker->GetActor() == getChild(line_pick_z)))
					cellPicker->GetPickPosition(this->endPositon);
				else
				{
					this->getChild(line_assist_x)->SetVisibility(false);
					this->getChild(line_assist_y)->SetVisibility(false);
					this->getChild(line_assist_z)->SetVisibility(false);
					render->RemoveActor(this->tempPoint);
					this->tempPointFlag = false;
					this->colorChanged = false;
					this->dragging = false;
					//恢复正常的交互方式
					normalStyle(render);
					render->GetRenderWindow()->Render();
				}
			}
			else
			{
				cellPicker->GetPickPosition(this->endPositon);
			}

			double vec1[3] = { this->startPostion[0] - this->groupOldPosition[0], this->startPostion[1] - this->groupOldPosition[1] ,this->startPostion[2] - this->groupOldPosition[2] };
			double vec2[3] = { this->endPositon[0] - this->groupOldPosition[0], this->endPositon[1] - this->groupOldPosition[1] ,this->endPositon[2] - this->groupOldPosition[2] };
			double vec3[3] = { vec2[0] - vec1[0],vec2[1] - vec1[1] ,vec2[2] - vec1[2] };
			double vec4[3];
			if (this->axis == "Y")
			{
				double up[3];
				vtkSmartPointer<vtkMatrix4x4> M = this->getChild(line_assist_y)->GetMatrix();
				up[0] = M->GetElement(0, 1);
				up[1] = M->GetElement(1, 1);
				up[2] = M->GetElement(2, 1);
				double temp = vec3[0] * up[0] + vec3[1] * up[1] + vec3[2] * up[2];
				vec4[0] = temp * up[0];
				vec4[1] = temp * up[1];
				vec4[2] = temp * up[2];
			}
			if (this->axis == "X")
			{
				double left[3];
				vtkSmartPointer<vtkMatrix4x4> M = this->getChild(line_assist_x)->GetMatrix();
				left[0] = M->GetElement(0, 0);
				left[1] = M->GetElement(1, 0);
				left[2] = M->GetElement(2, 0);
				double temp = vec3[0] * left[0] + vec3[1] * left[1] + vec3[2] * left[2];
				vec4[0] = temp * left[0];
				vec4[1] = temp * left[1];
				vec4[2] = temp * left[2];
				//std::cout << "temp:" << temp << "left:" << left[0] << " " << left[1] << " " << left[2] << std::endl;
				//std::cout << std::endl; // 每行结束后换行
			}
			if (this->axis == "Z")
			{
				double eye[3];
				vtkSmartPointer<vtkMatrix4x4> M = this->getChild(line_assist_z)->GetMatrix();

				eye[0] = M->GetElement(0, 2);
				eye[1] = M->GetElement(1, 2);
				eye[2] = M->GetElement(2, 2);
				double temp = vec3[0] * eye[0] + vec3[1] * eye[1] + vec3[2] * eye[2];
				vec4[0] = temp * eye[0];
				vec4[1] = temp * eye[1];
				vec4[2] = temp * eye[2];
			}
			//std::cout << "vec3:" << vec3[0] << " " << vec3[1] << " " << vec3[2] << std::endl;
			//std::cout << std::endl; // 每行结束后换行
			double curPos[3] = { this->groupOldPosition[0] + vec4[0] ,this->groupOldPosition[1] + vec4[1], this->groupOldPosition[2] + vec4[2] };
			//更新选择物体的位置
			if (this->objectType == "actor")
			{
				this->updateObject(curPos[0], curPos[1], curPos[2]);

			}
			else 
			{
				this->updateGroupObject(); 
			}
			render->GetRenderWindow()->Render();
		}
		else
		{
			this->getChild(line_pick_x)->SetPickable(true);
			this->getChild(line_pick_y)->SetPickable(true);
			this->getChild(line_pick_y)->SetPickable(true);
		}
		cellPicker->Pick(x, size.height() - y - 1, 0, render);
	}
	else if (this->mode == "rotate")
	{
		if (cellPicker->GetActor() == this->getChild(circle_pick_z))
		{
			//鼠标没有按下
			if (!isMousePressed)
			{
				//改变颜色
				this->colorChanged = true;
				this->axis = "Z";
				this->getChild(circle_pick_z)->SetVisibility(true);
				this->getChild(line_assist_z)->SetVisibility(true);
				//悬浮改变颜色
				this->getChild(circle_show_z)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			//鼠标按下&&未拖拽&&颜色改变 == 拖拽的初始时刻
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				//获取actor位置
				if (this->objectType == "actor")
					this->objectMatrix->DeepCopy(this->object->GetMatrix());
				else
					this->groupObjectMatrix->DeepCopy(this->groupObject->getMatrix());
				//获取鼠标按下时的位置
				cellPicker->GetPickPosition(this->startPostion);
				//切换交互方式
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				//允许拖拽
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
				this->dragging = false;
		}
		else if (cellPicker->GetActor() == this->getChild(circle_pick_y))
		{
			if (!isMousePressed)
			{
				this->colorChanged = true;
				this->axis = "Y";
				this->getChild(line_assist_y)->SetVisibility(true);
				this->getChild(circle_pick_y)->SetVisibility(true);
				this->getChild(circle_show_y)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				if (this->objectType == "actor")
					this->objectMatrix->DeepCopy(this->object->GetMatrix());
				else
					this->groupObjectMatrix->DeepCopy(this->groupObject->getMatrix());
				cellPicker->GetPickPosition(this->startPostion);
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
				this->dragging = false;
		}
		else if (cellPicker->GetActor() == this->getChild(circle_pick_x))
		{
			if (!isMousePressed)
			{
				this->colorChanged = true;
				this->axis = "X";
				this->getChild(circle_pick_x)->SetVisibility(true);
				this->getChild(line_assist_x)->SetVisibility(true);
				this->getChild(circle_show_x)->GetProperty()->SetColor(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0);
				render->GetRenderWindow()->Render();
			}
			if (isMousePressed && this->dragging == false && this->colorChanged)
			{
				if (this->objectType == "actor")
					this->objectMatrix->DeepCopy(this->object->GetMatrix());
				else
					this->groupObjectMatrix->DeepCopy(this->groupObject->getMatrix());
				cellPicker->GetPickPosition(this->startPostion);
				this->getPosition(this->groupOldPosition);
				helperStyle(render);
				this->dragging = true;
				render->GetRenderWindow()->Render();
			}
			if (!isMousePressed && this->dragging)
				this->dragging = false;
		}
		else
		{
			if (!isMousePressed && this->colorChanged)
			{
				this->getChild(line_assist_x)->SetVisibility(false);
				this->getChild(line_assist_y)->SetVisibility(false);
				this->getChild(line_assist_z)->SetVisibility(false);
				this->getChild(circle_show_z)->GetProperty()->SetColor(0, 0, 1);
				this->getChild(circle_show_y)->GetProperty()->SetColor(0, 1, 0);
				this->getChild(circle_show_x)->GetProperty()->SetColor(1, 0, 0);
				this->colorChanged = false;
				this->dragging = false;
				normalStyle(render);
				render->GetRenderWindow()->Render();
			}
		}
		if (this->dragging)
		{
			double cameraPos[3];
			render->GetActiveCamera()->GetPosition(cameraPos);
			MyVector3 point1(cameraPos);
			double thisPos[3];
			this->getPosition(thisPos);
			MyVector3 point2(thisPos);
			double rotateSpeed = 20 / point1.distanceTo(point2);

			if (isLock)
			{
				if ((this->axis == "Y" && cellPicker->GetActor() == getChild(circle_pick_y))
					|| (this->axis == "X" && cellPicker->GetActor() == getChild(circle_pick_x))
					|| (this->axis == "Z" && cellPicker->GetActor() == getChild(circle_pick_z)))
				{
					cellPicker->GetPickPosition(this->endPositon);
				}
				else
				{
					this->getChild(circle_pick_z)->GetProperty()->SetColor(0, 0, 1);
					this->getChild(circle_pick_y)->GetProperty()->SetColor(0, 1, 0);
					this->getChild(circle_pick_x)->GetProperty()->SetColor(1, 0, 0);
					this->colorChanged = false;
					this->dragging = false;
					//恢复正常的交互方式
					normalStyle(render);
					render->GetRenderWindow()->Render();
				}
			}
			else
			{
				cellPicker->GetPickPosition(this->endPositon);
			}
			//鼠标按下时的位置
			MyVector3 vec1(this->startPostion);
			vec1.sub(this->groupOldPosition);//sub减法
			//鼠标当前位置
			MyVector3 vec2(this->endPositon);
			vec2.sub(this->groupOldPosition);
			//鼠标滑过的向量
			MyVector3 vec3;
			vec2.sub(vec1, vec3);
			//轴线向量
			double axis[3] = { 1.0 ,1.0, 1.0 };
			double rotateAngle;
			{
				MyVector3 vec;
				vec2.cross(vec1, vec);
				vec.normalize();
				if (this->axis == "Z")
				{
					MyVector3 eye;
					vtkSmartPointer<vtkMatrix4x4> tempM = this->getChild(line_assist_z)->GetMatrix();
					eye.X = tempM->GetElement(0, 2);
					eye.Y = tempM->GetElement(1, 2);
					eye.Z = tempM->GetElement(2, 2);
					if (vec.X == 0.0 && vec.Y == 0.0 && vec.Z == 0.0)
						return;
					rotateAngle = MyVector3::getAngleInDegree(vec2, vec1);
					rotateAngle = rotateAngle * (vec.dot(eye) < 0 ? 1 : -1);
					axis[0] = eye.X;
					axis[1] = eye.Y;
					axis[2] = eye.Z;
				}
				if (this->axis == "X")
				{
					MyVector3 left;
					vtkSmartPointer<vtkMatrix4x4> tempM = this->getChild(line_assist_x)->GetMatrix();
					left.X = tempM->GetElement(0, 0);
					left.Y = tempM->GetElement(1, 0);
					left.Z = tempM->GetElement(2, 0);
					if (vec.X == 0.0 && vec.Y == 0.0 && vec.Z == 0.0)
						return;
					rotateAngle = MyVector3::getAngleInDegree(vec2, vec1);
					rotateAngle = rotateAngle * (vec.dot(left) < 0 ? 1 : -1);

					axis[0] = left.X;
					axis[1] = left.Y;
					axis[2] = left.Z;				
				}
				if (this->axis == "Y")
				{
					MyVector3 up;
					vtkSmartPointer<vtkMatrix4x4> tempM = this->getChild(line_assist_y)->GetMatrix();
					up.X = tempM->GetElement(0, 1);
					up.Y = tempM->GetElement(1, 1);
					up.Z = tempM->GetElement(2, 1);
					if (vec.X == 0.0 && vec.Y == 0.0 && vec.Z == 0.0)
						return;
					rotateAngle = MyVector3::getAngleInDegree(vec2, vec1);
					rotateAngle = rotateAngle * (vec.dot(up) < 0 ? 1 : -1);
					axis[0] = up.X;
					axis[1] = up.Y;
					axis[2] = up.Z;
				}
			}
			if (abs(rotateAngle) > 90.0)
			{
				this->dragging = false;
				return;
			}
			if (this->objectType == "actor")
			{
				this->rotateObject(rotateAngle / 2.0, axis, thisPos);
			}
			else
			{
				this->rotateGroupObject(rotateAngle / 2.0, axis, thisPos);
				this->updateGroupObjectM();
			}
			render->GetRenderWindow()->Render();
		}
		else
		{
			this->getChild(circle_pick_x)->SetPickable(true);
			this->getChild(circle_pick_y)->SetPickable(true);
			this->getChild(circle_pick_z)->SetPickable(true);
		}
	}
}


