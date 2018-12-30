#include "CoreHeaders.h"
#include "Logging/Sinks/VSDebugSink.h"

CVSDebugSink::CVSDebugSink()
{
}

CVSDebugSink::~CVSDebugSink()
{
}

void CVSDebugSink::Output(std::string sOut)
{
	OutputDebugStringA(sOut.c_str());
}