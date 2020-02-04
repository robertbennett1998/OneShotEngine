#pragma once
#ifndef ONE_SHOT_RENDERER_MATERIAL_H
#define ONE_SHOT_RENDERER_MATERIAL_H

#include "Buffers/Interfaces/ITexture2D.h"
#include <stdlib.h>
#include <map>

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CMaterial
	{
		friend class CMaterialLibrary;
		friend class CMaterialLoader;
	public:
		enum class TextureType
		{
			UNDEFINED = 0,
			DIFFUSE = 1,
			NORMAL = 2,
			SPECULAR = 3
		};

		friend class CMaterialLoader;
	public:
		std::string GetMaterialName() const
		{
			return m_sMaterialName;
		}

		void Bind(uint8_t ubyDiffuseSlot) const
		{
			GetTexture2D(TextureType::DIFFUSE)->BindToPixelShader(ubyDiffuseSlot);
		}

		const ITexture2D* GetTexture2D(TextureType textureType) const
		{
			auto texture = m_Textures.find(textureType);
			if (texture == m_Textures.end())
				return nullptr;

			return texture->second;
		}

	private:
		CMaterial();
		~CMaterial();

		bool AddTexture(TextureType textureType, ITexture2D* pTexture);
		void Bind();
		void Shutdown();

		std::string m_sMaterialName;
		std::map<TextureType, ITexture2D*> m_Textures;
	};
};
#endif