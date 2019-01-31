#include "pch.h"
#include "FileUtillities/XmlReader.h"
#include "Logging/Sinks/VSDebugSink.h"

using namespace std;

class Test
{
public:
	Test(int i)
	{
		m_i = i;
	}

	void Print()
	{
		std::cout << m_i << std::endl;
	}

	int m_i;
};

int main()
{
	CConsoleVariables cvars;
	cvars.RegisterVariable<Test>("Test", 12);

	auto p = cvars.GetVariable<Test>("Test");

	p->Print();

	system("PAUSE");
	return 0;
}