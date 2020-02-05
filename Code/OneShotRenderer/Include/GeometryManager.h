#pragma once
#ifndef ONE_SHOT_RENDERER_GEOMETRY_MANAGER_H
#define ONE_SHOT_RENDERER_GEOMETRY_MANAGER_H

#include "Interfaces/IRenderer3D.h"
#include "Geometry.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CGeometryManager
	{
		public:
			static CGeometryManager* GetInstance()
			{
				return sm_pInstance;
			}

			static CGeometryManager* CreateInstance(IRenderer3D* p3DRenderer)
			{
				if (sm_pInstance == nullptr)
					sm_pInstance = OSE_NEW_LIFETIME(CGeometryManager(p3DRenderer));

				return sm_pInstance;
			}

			bool RegisterGeometry(std::string sFilePath);
			const CGeometry* GetGeomInstance(std::string sGeom);
			void Shutdown();

		private:
			CGeometryManager();
			CGeometryManager(IRenderer3D* p3DRenderer);
			~CGeometryManager();


			IRenderer3D* m_p3DRenderer;
			static CGeometryManager* sm_pInstance;
			std::map<std::string, CGeometry*> m_Geometry;
	};
}

#endif