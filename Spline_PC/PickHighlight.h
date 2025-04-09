#pragma once

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1


#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkIdFilter.h>
#include <vtkIdTypeArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPlanes.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVersion.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>


//定义类-继承至vtkInteractorStyleRubberBandPick类
class PickHighlight : public vtkInteractorStyleRubberBandPick
{
public:
	static PickHighlight* New();
	vtkTypeMacro(PickHighlight, vtkInteractorStyleRubberBandPick);
	void SetPolyData(vtkSmartPointer<vtkPolyData> polyData);
	void GetRender(vtkSmartPointer<vtkRenderer> render, vtkSmartPointer<vtkInteractorStyleRubberBandPick> style);
	vtkSmartPointer<vtkActor> SelectedActor;
	vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
	bool isSelected = false;

protected:
	PickHighlight();
	~PickHighlight() override;

	virtual void OnLeftButtonUp() override;
	virtual void OnChar() override;

private:
	vtkSmartPointer<vtkPolyData> PolyData;
	vtkSmartPointer<vtkRenderer> render;
	vtkSmartPointer<vtkInteractorStyleRubberBandPick> style;
	vtkSmartPointer<vtkPolyData> ReadPolyData(const char* fileName);
};


