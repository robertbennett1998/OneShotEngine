#include "pch.h"
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

	void TestTwoChanged()
	{
		std::cout << "Changed" << std::endl;
	}

	int m_i;
};

int main()
{
	CConsoleVariables cvars;
	Test t(12);
	std::function<void()> callback = std::bind(&Test::TestTwoChanged, t);
	std::function<void()> callback2 = std::bind([]() {std::cout << "Test" << std::endl;  });
	cvars.RegisterVariable<int>("Test", 12, callback);
	cvars.AddValueChangedCallback("Test", callback2);

	int value = 0;
	cvars.GetValue("Test", value);
	std::cout << "Test = " << value << std::endl;

	cvars.SetValue("Test", 14);

	cvars.GetValue("Test", value);
	std::cout << "Test = " << value << std::endl;

	system("PAUSE");
	return 0;
}