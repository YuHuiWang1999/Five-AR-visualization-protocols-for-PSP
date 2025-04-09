#include "mygroup.h"

vtkStandardNewMacro(MyGroup);


MyGroup::MyGroup()
{
	this->matrix = vtkSmartPointer<vtkMatrix4x4>::New();
	this->axes = vtkSmartPointer<vtkAxesActor>::New();
	double axesSize[3] = { 100,100,100 };
	axes->SetTotalLength(axesSize);
	axes->SetConeRadius(0.005);
	axes->SetShaftTypeToLine();
	axes->SetAxisLabels(false);
	axes->SetVisibility(false);
}

MyGroup::~MyGroup()
{

}

void MyGroup::addChild(vtkSmartPointer<vtkActor> actor)
{
	int size = this->child.size();
	this->child.push_back(actor);
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	M->DeepCopy(actor->GetMatrix());
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	M1->DeepCopy(this->matrix);
	M1->Invert();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M1, M, M2);
	childInGroupM.push_back(M2);

}

vtkSmartPointer<vtkActor> MyGroup::getChild(int index)
{
	int size = this->child.size();
	if (index < size)
	{
		return this->child[index];
	}
	else
	{
		return nullptr;
	}
}

void MyGroup::addGroup(vtkSmartPointer<MyGroup> group)
{
	this->groupChild.push_back(group);
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	M->DeepCopy(group->getMatrix());
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	M1->DeepCopy(this->matrix);
	M1->Invert();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M1, M, M2);
	groupInGroupM.push_back(M2);
}

vtkSmartPointer<vtkMatrix4x4> MyGroup::getMatrix()
{
	return this->matrix;
}

vtkSmartPointer<vtkMatrix4x4> MyGroup::getAxesLocalM()
{
	return this->axesInGroupM;
}

vtkSmartPointer<vtkMatrix4x4> MyGroup::getAxesMatrix()
{
	return this->axes->GetMatrix();
}

void MyGroup::setMatrix(vtkSmartPointer<vtkMatrix4x4> M)
{
	//更新组坐标
	this->matrix->DeepCopy(M);
	int size = this->child.size();

	//更新子对象的坐标
	for (int i = 0; i < size; ++i)
	{
		vtkSmartPointer<vtkMatrix4x4>M1 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(M, childInGroupM[i], M1);
		this->child[i]->SetUserMatrix(M1);
	}
	size = this->groupChild.size();
	for (int i = 0; i < size; ++i)
	{
		vtkSmartPointer<vtkMatrix4x4>M1 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(M, groupInGroupM[i], M1);
		this->groupChild[i]->setMatrix(M1);
	}

	{
		vtkSmartPointer<vtkMatrix4x4>M1 = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(M, axesInGroupM, M1);
		this->axes->SetUserMatrix(M1);
	}

}

void MyGroup::setAxesMatrix(vtkSmartPointer<vtkMatrix4x4> matrix)
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	M1->DeepCopy(this->matrix);
	M1->Invert();
	vtkSmartPointer<vtkMatrix4x4> M2 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M1, matrix, M2);
	axesInGroupM->DeepCopy(M2);
	this->axes->SetUserMatrix(matrix);
}

void MyGroup::leftApplyMatrix(vtkSmartPointer<vtkMatrix4x4> M)
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Multiply4x4(M, this->matrix, M);

	this->setMatrix(M);
}

void MyGroup::rotateX(double angleInDegree)
{
	double axis[3] = { 1.0,0.0,0.0 };
	double pos[3];
	this->getPosition(pos);
	rotateOnAxis(angleInDegree, axis, pos);
}

void MyGroup::rotateY(double angleInDegree)
{
	double axis[3] = { 0.0,1.0,0.0 };
	double pos[3];
	this->getPosition(pos);
	rotateOnAxis(angleInDegree, axis, pos);
}

void MyGroup::rotateZ(double angleInDegree)
{
	double axis[3] = { 0.0,0.0,1.0 };
	double pos[3];
	this->getPosition(pos);
	rotateOnAxis(angleInDegree, axis, pos);
}

void MyGroup::rotateOnAxis(double angleInDegree, double axis[3], double position[3])
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


	this->leftApplyMatrix(MT2);
}

void MyGroup::rotateOnAxis(double angleInDegree, MyVector3 axis, double position[3])
{
	vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
	translation->RotateWXYZ(angleInDegree, axis.X, axis.Y, axis.Z);

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


	this->leftApplyMatrix(MT2);
}

void MyGroup::translateOnAxis(double distance, double axis[3])
{
	vtkSmartPointer<vtkMatrix4x4> M1 = vtkSmartPointer<vtkMatrix4x4>::New();
	double x = distance * axis[0];
	double y = distance * axis[1];
	double z = distance * axis[2];
	M1->SetElement(0, 3, x);
	M1->SetElement(1, 3, y);
	M1->SetElement(2, 3, z);

	this->leftApplyMatrix(M1);
}

void MyGroup::addChildInRender(vtkSmartPointer<vtkRenderer> render)
{
	int size = this->child.size();

	for (int i = 0; i < size; ++i)
	{
		render->AddActor(this->child[i]);
	}

	size = this->groupChild.size();
	for (int i = 0; i < size; ++i)
	{
		this->groupChild[i]->addChildInRender(render);
	}
	render->AddActor(this->axes);

}

void MyGroup::setAxesVisibilty(bool visible)
{
	this->axes->SetVisibility(visible);
}

vtkSmartPointer<vtkAxesActor> MyGroup::getAxes()
{
	return this->axes;
}

void MyGroup::setVisibilty(bool visible)
{
	int size = this->child.size();

	for (int i = 0; i < size; ++i)
	{
		this->child[i]->SetVisibility(visible);
	}
	size = this->groupChild.size();
	for (int i = 0; i < size; ++i)
	{
		this->groupChild[i]->setVisibilty(visible);
	}
}

void MyGroup::setPosition(double x, double y, double z)
{
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	M->SetElement(0, 3, x);
	M->SetElement(1, 3, y);
	M->SetElement(2, 3, z);
	this->setMatrix(M);
}

void MyGroup::setPositionForHelper(double x, double y, double z)
{
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	M->SetElement(0, 3, x);
	M->SetElement(1, 3, y);
	M->SetElement(2, 3, z);
	this->matrix->DeepCopy(M);
	int size = this->child.size();

	//更新子对象的坐标
	for (int i = 0; i < size; ++i)
	{
		vtkSmartPointer<vtkMatrix4x4>M1 = this->child[i]->GetMatrix();
		M1->SetElement(0, 3, x);
		M1->SetElement(1, 3, y);
		M1->SetElement(2, 3, z);
		this->child[i]->SetUserMatrix(M1);
	}

}

void MyGroup::getPosition(double position[3])
{
	position[0] = this->matrix->GetElement(0, 3);
	position[1] = this->matrix->GetElement(1, 3);
	position[2] = this->matrix->GetElement(2, 3);
}

void MyGroup::clear()
{
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	this->matrix = M;
	this->child.clear();
	this->groupChild.clear();
	double axesSize[3] = { 60,60,60 };
	axes->SetTotalLength(axesSize);
	axes->SetConeRadius(0.05);
	axes->SetShaftTypeToLine();
	axes->SetAxisLabels(false);
	axes->SetVisibility(false);
	this->axes = axes;
}

void MyGroup::backToOrigin()
{
	vtkSmartPointer<vtkMatrix4x4> M = vtkSmartPointer<vtkMatrix4x4>::New();
	M->DeepCopy(this->matrix);
	M->Invert();
	this->leftApplyMatrix(M);
}