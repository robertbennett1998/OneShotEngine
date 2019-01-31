#pragma once
#ifndef ONE_SHOT_CORE_CORE_HEADERS_H
#define ONE_SHOT_CORE_CORE_HEADERS_H

#include <Windows.h>

//stdlib
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <fstream>
#include <thread>
#include <tuple>

#ifdef _ONE_SHOT_CORE_DLL
	#define ONE_SHOT_CORE_DLL __declspec(dllexport)
#else
	#define ONE_SHOT_CORE_DLL __declspec(dllimport)
#endif

#include <MemoryManagement/MemoryManager.h>
#include <Logging/LoggingManager.h>
#include <Logging/Logger.h>
#include <FileUtillities/VirtualFileSystem.h>
#include "Utillities/GlobalVariables.h"

#endif