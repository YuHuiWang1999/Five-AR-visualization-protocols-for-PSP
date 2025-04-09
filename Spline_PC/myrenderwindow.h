#pragma once

#include <QObject>
#include <QDebug>
#include <QKeyEvent>
#include <QThread>
#include <QVTKOpenGLWidget.h>
#include <QPoint.h>
#include <vtkTextActor.h>
#include <vtkTextWidget.h>
#include <vtkTextRepresentation.h>
#include <vtkTextProperty.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkAxesActor.h>


class MyRenderWindow : public QVTKOpenGLWidget
{
	Q_OBJECT

public:
	MyRenderWindow(QWidget *parent);
	~MyRenderWindow();
	void keyPressEvent(QKeyEvent *event);
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *ev);
	vtkSmartPointer<vtkTextActor> textactor;
	vtkSmartPointer<vtkTextWidget> textwidget;
	vtkSmartPointer<vtkTextRepresentation> textrepresentation;
	vtkSmartPointer<vtkTextActor> textactor2;
	vtkSmartPointer<vtkTextWidget> textwidget2;
	vtkSmartPointer<vtkTextRepresentation> textrepresentation2;
	vtkSmartPointer<vtkTextActor> textactor3;
	vtkSmartPointer<vtkTextWidget> textwidget3;
	vtkSmartPointer<vtkTextRepresentation> textrepresentation3;
	vtkSmartPointer<vtkTextActor> textactor_num;
	vtkSmartPointer<vtkTextWidget> textwidget_num;
	vtkSmartPointer<vtkTextRepresentation> textrepresentation_num;
	void SetText(const char* text, double p1, double p2, double p3, double p4);
	void SetText1(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render);
	void SetText2(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render);
	void SetText3(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render);
	void SetNumText(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render);
	bool fullScreenFlag = false;



signals:
	void fullScreenSignal();
	void cursorChangeSignal(int);
	void rightSignal(QPoint point, QPoint pointLocal);
	void middlePickSignal(int x, int y);
	void leftUpSignal(QPoint point, int index);
	void mouseMoveSignal(int x, int y, bool isMousePressed);
	void leftMouseDownSignal(int x, int y);
	void leftMouseUpSignal(int x, int y);
	void KeyPressSignal(QKeyEvent *ev);
	void TranslatePressSignal(QKeyEvent *ev);
	void RoatePressSignal(QKeyEvent *ev);
	void PickPressSignal(QKeyEvent *ev);
private:

	bool isMousePressed = false;
};
