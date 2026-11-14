#include <app/App.h>

#include <exception>
#include <iostream>

int main() {
	try
	{
		App app;
		app.run();
		return 0;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}
}
