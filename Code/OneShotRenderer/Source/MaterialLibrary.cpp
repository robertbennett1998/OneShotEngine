#include "pch.h"
#include "MaterialLibrary.h"
#include "MaterialLoader.h"

using namespace OneShotRenderer;

CMaterialLibrary* CMaterialLibrary::sm_pInstance = nullptr;

CMaterialLibrary::CMaterialLibrary() :
m_p3DRenderer(nullptr)
{
}

CMaterialLibrary::CMaterialLibrary(IRenderer3D* p3DRenderer) :
m_p3DRenderer(p3DRenderer)
{
}

CMaterialLibrary::~CMaterialLibrary()
{
}

bool CMaterialLibrary::RegisterMaterials(std::string sMtlFilePath)
{
	CMaterialLoader materialLoader(m_p3DRenderer);
	if (!materialLoader.Load(sMtlFilePath))
		return false;

	std::vector<CMaterial*> materials = materialLoader.GetMaterials();
	for (auto& material : materials)
	{
		if (m_Materials.find(material->GetMaterialName()) == m_Materials.end())
			m_Materials.try_emplace(material->GetMaterialName(), material);
	}

	return true;
}

const CMaterial* CMaterialLibrary::GetMaterial(std::string sMtlName) const
{
	auto geom = m_Materials.find(sMtlName);
	if (geom == m_Materials.end())
		return nullptr;

	return geom->second;
}

void CMaterialLibrary::Shutdown()
{
	for (auto& i : m_Materials)
		OSE_SAFE_SHUTDOWN(i.second);
}