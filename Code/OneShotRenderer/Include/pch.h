#ifndef ONE_SHOT_RENDERER_PCH_H
#define ONE_SHOT_RENDERER_PCH_H

#include <Windows.h>

#include <stdlib.h>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <array>
#include <DirectXMath.h>
#include <map>
#include <Algorithm>

#ifdef _ONE_SHOT_RENDERER_DLL
#define ONE_SHOT_RENDERER_DLL __declspec(dllexport)
#else
#define ONE_SHOT_RENDERER_DLL __declspec(dllimport)
#endif

#ifdef _ONE_SHOT_CORE_DLL
#define ONE_SHOT_CORE_DLL __declspec(dllexport)
#else
#define ONE_SHOT_CORE_DLL __declspec(dllimport)
#endif

//OneShotCore
#include <OneShotCore/Include/MemoryManagement/MemoryManager.h>
#include <OneShotCore/Include/Logging/LoggingManager.h>
#include <OneShotCore/Include/Logging/Logger.h>
#include <OneShotCore/Include/FileUtillities/VirtualFileSystem.h>
#include <OneShotCore/Include/Utillities/GlobalVariables.h>

#endif