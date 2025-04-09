#include "myrenderwindow.h"


MyRenderWindow::MyRenderWindow(QWidget *parent)
	: QVTKOpenGLWidget(parent)
{
	textactor = vtkSmartPointer<vtkTextActor>::New();
	textwidget = vtkSmartPointer<vtkTextWidget>::New();
	textrepresentation = vtkSmartPointer<vtkTextRepresentation>::New();
	textactor2 = vtkSmartPointer<vtkTextActor>::New();
	textwidget2 = vtkSmartPointer<vtkTextWidget>::New();
	textrepresentation2 = vtkSmartPointer<vtkTextRepresentation>::New();
	textactor3 = vtkSmartPointer<vtkTextActor>::New();
	textwidget3 = vtkSmartPointer<vtkTextWidget>::New();
	textrepresentation3 = vtkSmartPointer<vtkTextRepresentation>::New();
	textactor_num = vtkSmartPointer<vtkTextActor>::New();
	textwidget_num = vtkSmartPointer<vtkTextWidget>::New();
	textrepresentation_num = vtkSmartPointer<vtkTextRepresentation>::New();
}

MyRenderWindow::~MyRenderWindow()
{
}

void MyRenderWindow::enterEvent(QEvent *ev)
{
	this->fullScreenFlag = true;
	emit cursorChangeSignal(0);
}

void MyRenderWindow::leaveEvent(QEvent *ev)
{
	this->fullScreenFlag = false;
	emit cursorChangeSignal(1);
}

void MyRenderWindow::mouseDoubleClickEvent(QMouseEvent *ev)
{
	if (this->fullScreenFlag)
	{
		emit fullScreenSignal();
	}
}

void MyRenderWindow::keyPressEvent(QKeyEvent *event)
{
	if (this->fullScreenFlag)
	{
		if (event->isAutoRepeat()) return;
		if (event->key() == 32)//¿Õ¸ñ¼ü
		{
			emit fullScreenSignal();
		}
		if (event->key() == Qt::Key_F2)//F2¼ü
		{
			emit KeyPressSignal(event);
		}
		if (event->key() == Qt::Key_Q)//E¼ü
		{
			emit TranslatePressSignal(event);
		}
		if (event->key() == Qt::Key_W)//W¼ü
		{
			emit RoatePressSignal(event);
		}


		//qDebug() << "keyPressEvent" << event->key();
	}
}

void MyRenderWindow::mousePressEvent(QMouseEvent *event)
{
	if (Qt::RightButton == event->button())
	{
		QPoint *point = new	QPoint(event->screenPos().x(), event->screenPos().y());
		QPoint *pointLocal = new	QPoint(event->pos().x(), event->pos().y());
		emit rightSignal(*point, *pointLocal);
	}

	if (Qt::MidButton == event->button())
	{
		emit middlePickSignal(event->pos().x(), event->pos().y());
	}

	if (Qt::LeftButton == event->button())
	{
		isMousePressed = true;
		emit leftMouseDownSignal(event->pos().x(), event->pos().y());
		emit mouseMoveSignal(event->pos().x(), event->pos().y(), isMousePressed);
	}
}

void MyRenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button())
	{
		isMousePressed = false;
		emit mouseMoveSignal(event->pos().x(), event->pos().y(), isMousePressed);

		//ÓÒ¼ü²Ëµ¥
		QPoint *pointLocal = new QPoint(event->pos().x(), event->pos().y());
		emit leftUpSignal(*pointLocal, 3);
	}
}

void MyRenderWindow::mouseMoveEvent(QMouseEvent *ev)
{
	emit mouseMoveSignal(ev->pos().x(), ev->pos().y(), isMousePressed);
}



void MyRenderWindow::SetText(const char* text, double p1, double p2, double p3, double p4)
{
	textactor->SetInput(text);
	textactor->GetTextProperty()->SetColor(1, 0, 0);
	textactor->GetTextProperty()->SetFontFamilyToTimes();
	textrepresentation->GetPositionCoordinate()->SetValue(p1, p2);
	textrepresentation->GetPosition2Coordinate()->SetValue(p3, p4);
	textwidget->SetRepresentation(textrepresentation);
	textwidget->SetInteractor(this->GetInteractor());
	textwidget->SetTextActor(textactor);
	textwidget->SelectableOff();
	textwidget->On();
}
void MyRenderWindow::SetText1(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render)
{
	textactor->SetInput(text);
	textactor->GetTextProperty()->SetColor(0, 1, 0);
	textactor->GetTextProperty()->SetBold(1);
	textactor->GetTextProperty()->SetFontFamilyToTimes();
	textrepresentation->GetPositionCoordinate()->SetValue(p1, p2);
	textrepresentation->GetPosition2Coordinate()->SetValue(p3, p4);
	textwidget->SetRepresentation(textrepresentation);
	textwidget->SetInteractor(this->GetInteractor());
	textwidget->SetCurrentRenderer(render);
	textwidget->SetTextActor(textactor);
	textwidget->SelectableOff();
	textwidget->On();
}
void MyRenderWindow::SetText2(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render)
{
	textactor2->SetInput(text);
	textactor2->GetTextProperty()->SetColor(0, 1, 0);
	textactor2->GetTextProperty()->SetBold(1);
	textactor2->GetTextProperty()->SetFontFamilyToTimes();
	textrepresentation2->GetPositionCoordinate()->SetValue(p1, p2);///×óÏÂ½Çview×ø±ê
	textrepresentation2->GetPosition2Coordinate()->SetValue(p3, p4); ///¿í,¸ß
	textwidget2->SetRepresentation(textrepresentation2);
	textwidget2->SetInteractor(this->GetInteractor());
	textwidget2->SetCurrentRenderer(render);
	textwidget2->SetTextActor(textactor2);
	textwidget2->SelectableOff();
	textwidget2->On();
}
void MyRenderWindow::SetText3(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render)
{
	textactor3->SetInput(text);
	textactor3->GetTextProperty()->SetColor(0, 1, 0);
	textactor3->GetTextProperty()->SetFontFamilyToTimes();
	textactor3->GetTextProperty()->SetBold(1);
	textrepresentation3->GetPositionCoordinate()->SetValue(p1, p2);
	textrepresentation3->GetPosition2Coordinate()->SetValue(p3, p4);
	textwidget3->SetRepresentation(textrepresentation3);
	textwidget3->SetInteractor(this->GetInteractor());
	textwidget3->SetCurrentRenderer(render);
	textwidget3->SetTextActor(textactor3);
	textwidget3->SelectableOff();
	textwidget3->On();
}
void MyRenderWindow::SetNumText(const char* text, double p1, double p2, double p3, double p4, vtkRenderer* render)
{
	textactor_num->SetInput(text);
	textactor_num->GetTextProperty()->SetColor(160 / 255.0, 32 / 255.0, 240 / 255.0);
	textactor_num->GetTextProperty()->SetFontFamilyToArial();
	textactor_num->GetTextProperty()->SetFontSize(30);
	textactor_num->GetTextProperty()->SetBold(1);
	textrepresentation_num->GetPositionCoordinate()->SetValue(p1, p2);///×óÏÂ½Çview×ø±ê
	textrepresentation_num->GetPosition2Coordinate()->SetValue(p3, p4);///¿í,¸ß 
	textwidget_num->SetRepresentation(textrepresentation_num);
	textwidget_num->SetInteractor(this->GetInteractor());
	textwidget_num->SetCurrentRenderer(render);
	textwidget_num->SetTextActor(textactor_num);
	textwidget_num->SelectableOff();
	textwidget_num->On();
}