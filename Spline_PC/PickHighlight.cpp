#include "PickHighlight.h"
#include <vtkNamedColors.h>
#include <vtkAreaPicker.h>
#include <vtkExtractPolyDataGeometry.h>

vtkStandardNewMacro(PickHighlight);

// 构造函数
PickHighlight::PickHighlight(): vtkInteractorStyleRubberBandPick()
{
	this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
	this->SelectedActor = vtkSmartPointer<vtkActor>::New();
	this->SelectedActor->SetMapper(this->SelectedMapper);
}

// 析构函数
PickHighlight::~PickHighlight() = default;

// OnLeftButtonUp方法实现
void PickHighlight::OnLeftButtonUp()
{
		// Forward events
	if (this->CurrentMode != VTKISRBP_SELECT)
	{
		// if not in rubber band mode,  let the parent class handle it
		this->Superclass::OnLeftButtonUp();
		return;
	}
	if (!this->Interactor || !this->Moving)
	{
		return;
	}

	// otherwise record the rubber band end coordinate and then fire off a pick
	if ((this->StartPosition[0] != this->EndPosition[0]) ||
		(this->StartPosition[1] != this->EndPosition[1]))
	{
		this->Pick();

		vtkNew<vtkNamedColors> colors;

		vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

		vtkNew<vtkExtractPolyDataGeometry> extractPolyDataGeometry;
		extractPolyDataGeometry->SetInputData(this->PolyData);
		extractPolyDataGeometry->SetImplicitFunction(frustum);
		extractPolyDataGeometry->Update();
		if (extractPolyDataGeometry->GetOutput()->GetNumberOfCells() == 0) { 
			
			isSelected = false;		
			this->SelectedActor->SetVisibility(false);
		}
		else
		{ isSelected = true;	
		this->SelectedActor->SetVisibility(true);
		}
		std::cout << "Extracted "
			<< extractPolyDataGeometry->GetOutput()->GetNumberOfCells()
			<< " cells." <<"isSelected是"<< isSelected << std::endl;

		this->SelectedMapper->SetInputData(extractPolyDataGeometry->GetOutput());
		this->SelectedMapper->ScalarVisibilityOff();

		this->SelectedActor->GetProperty()->SetColor(
			colors->GetColor3d("Tomato").GetData());
		this->SelectedActor->GetProperty()->SetPointSize(1);
		this->SelectedActor->GetProperty()->SetRepresentationToWireframe();

		this->GetInteractor()
			->GetRenderWindow()
			->GetRenderers()
			->GetFirstRenderer()
			->AddActor(SelectedActor);
		this->GetInteractor()->GetRenderWindow()->Render();
		this->HighlightProp(NULL);
	}
	this->Moving = 0;
	// this->CurrentMode = VTKISRBP_ORIENT;

		//vtkInteractorStyleRubberBandPick::OnLeftButtonUp();



}
// OnChar方法重写
void PickHighlight::OnChar()
{
	char* cKeySym = this->Interactor->GetKeySym();
	std::string keySym = cKeySym != nullptr ? cKeySym : "";
	std::transform(keySym.begin(), keySym.end(), keySym.begin(), ::toupper);
	if (keySym == "S")
	{
		// s toggles the rubber band selection mode for mouse button 1
		if (this->CurrentMode == VTKISRBP_ORIENT)
		{
			this->CurrentMode = VTKISRBP_SELECT;
		}
		else
		{
			this->CurrentMode = VTKISRBP_ORIENT;
		}
	}
	else if (keySym == "P")
	{
		vtkRenderWindowInteractor* rwi = this->Interactor;
		int* eventPos = rwi->GetEventPosition();
		this->FindPokedRenderer(eventPos[0], eventPos[1]);
		this->StartPosition[0] = eventPos[0];
		this->StartPosition[1] = eventPos[1];
		this->EndPosition[0] = eventPos[0];
		this->EndPosition[1] = eventPos[1];
		this->Pick();
	}
	//else
	//{
	//	return;
	//}
}

// SetPolyData方法实现
void PickHighlight::SetPolyData(vtkSmartPointer<vtkPolyData> polyData)
{
	this->PolyData = polyData;
}

void PickHighlight::GetRender(vtkSmartPointer<vtkRenderer> render, vtkSmartPointer<vtkInteractorStyleRubberBandPick> style)
{
	this->render = render;
	this->style = style;
}

//// ReadPolyData方法实现
//vtkSmartPointer<vtkPolyData> PickHighlight::ReadPolyData(const char* fileName)
//{
//	// 读取文件并返回vtkPolyData对象...
//}
