#pragma once
#ifndef ONE_SHOT_CORE_CORE_HEADERS_H
#define ONE_SHOT_CORE_CORE_HEADERS_H

#include <Windows.h>
#include <math.h>

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <fstream>

#ifdef _ONE_SHOT_CORE_DLL
	#define ONE_SHOT_CORE_DLL __declspec(dllexport)
#else
	#define ONE_SHOT_CORE_DLL __declspec(dllimport)
#endif

#endif