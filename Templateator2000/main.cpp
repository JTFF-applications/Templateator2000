#include "MainWindow.h"

int main(int argc, char* argv[])
{
	try
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		QApplication::exec();

		delete Log::Get();
		return EXIT_SUCCESS;
	} catch (const std::exception& except)
	{
		LOG_CRITICAL(except.what());

		delete Log::Get();
		return EXIT_FAILURE;
	}
}
