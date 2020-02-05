#include "pch.h"
#include "Shaders/ShaderManager.h"

using namespace OneShotRenderer;

CShaderManager* CShaderManager::sm_pInstance = nullptr;

CShaderManager::CShaderManager(IRenderer3D* p3DRenderer) :
m_p3DRenderer(p3DRenderer)
{
}

CShaderManager::~CShaderManager()
{
	for (auto& i : m_Shaders)
		OSE_SAFE_SHUTDOWN(i.second);
}