#include "pch.h"
#include "FileUtillities/XmlReader.h"
#include "Logging/Sinks/VSDebugSink.h"

using namespace std;

class BaseConverter
{
	public:
		virtual void operator() (void* addressToConvert) = 0;
};

template<class T>
class 

int main()
{
	auto*(*p)(char*) = Test;
	auto ptr = (Test);
	int* a = new int;
	*a = 12;

	int b = *ptr((char*)a);

	system("PAUSE");
	return 0;
}