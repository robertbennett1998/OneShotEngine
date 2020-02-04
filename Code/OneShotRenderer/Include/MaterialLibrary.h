#pragma once
#ifndef ONE_SHOT_RENDERER_MATERIAL_LIBRARY_H
#define ONE_SHOT_RENDERER_MATERIAL_LIBRARY_H

#include "Interfaces/IRenderer3D.h"
#include "Material.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CMaterialLibrary
	{
		public:
			static CMaterialLibrary* GetInstance()
			{
				return sm_pInstance;
			}

			static CMaterialLibrary* CreateInstance(IRenderer3D* p3DRenderer)
			{
				if (sm_pInstance == nullptr)
				{
					sm_pInstance = new CMaterialLibrary(p3DRenderer);
				}

				return sm_pInstance;
			}

			bool RegisterMaterials(std::string sMtlFilePath);
			const CMaterial* GetMaterial(std::string sMtlName) const;
			void Shutdown();

		private:
			CMaterialLibrary();
			CMaterialLibrary(IRenderer3D* p3DRenderer);
			~CMaterialLibrary();

			IRenderer3D* m_p3DRenderer;
			std::map<std::string, CMaterial*> m_Materials;
			static CMaterialLibrary* sm_pInstance;
	};
}

#endif