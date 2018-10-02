#include "GBSipClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{	
	QApplication a(argc, argv);
	GBSipClient w;
	w.show();
	return a.exec();
}