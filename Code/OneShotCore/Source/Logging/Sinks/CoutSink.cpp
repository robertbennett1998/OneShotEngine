#include "CoreHeaders.h"
#include "Logging/Sinks/CoutSink.h"

#include <stdlib.h>
#include <iostream>

CCoutSink::CCoutSink()
{
}

CCoutSink::~CCoutSink()
{
}

void CCoutSink::Output(std::string sOut)
{
	std::cout << sOut.c_str();
}