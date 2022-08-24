#include "MainWindow.h"

int main(int argc, char* argv[])
{
	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		return QApplication::exec();
	} catch (const std::exception& except)
	{
		LOG_ERROR(except.what());
		return EXIT_FAILURE;
	}
}
