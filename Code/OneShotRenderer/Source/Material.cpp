#include "pch.h"
#include "Material.h"

using namespace OneShotRenderer;

CMaterial::CMaterial()
{
}

CMaterial::~CMaterial()
{
}

bool CMaterial::AddTexture(TextureType textureType, ITexture2D* pTexture)
{
	if (m_Textures.find(textureType) == m_Textures.end())
	{
		m_Textures.try_emplace(textureType, pTexture);
		return true;
	}

	return false;
}

void CMaterial::Bind()
{
	for (auto& i : m_Textures)
		i.second->BindToPixelShader();
}

void CMaterial::Shutdown()
{
	for (auto& i : m_Textures)
		OSE_SAFE_SHUTDOWN(i.second);
}