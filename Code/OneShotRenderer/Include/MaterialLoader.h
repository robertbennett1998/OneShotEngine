#pragma once
#ifndef ONE_SHOT_RENDERER_MATERIAL_LOADER_H
#define ONE_SHOT_RENDERER_MATERIAL_LOADER_H

#include "Material.h"
#include "Interfaces/IRenderer3D.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CMaterialLoader
	{
		public:
			CMaterialLoader(IRenderer3D* p3DRenderer);

			bool Load(std::string sMtlFilePath);
			std::vector<CMaterial*> GetMaterials() const;

		private:
			CMaterialLoader() = default;

			std::vector<CMaterial*> m_pMaterials;
			IRenderer3D* m_p3DRenderer;
	};
};

#endif