#include "AR.h"
#include "Calibration.h"
#include <QtWidgets/QApplication>
#include <vtkOutputWindow.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AR w;
	w.show();
	return a.exec();
}
