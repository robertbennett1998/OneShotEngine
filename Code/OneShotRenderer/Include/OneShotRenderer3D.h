#pragma once
#ifndef ONE_SHOT_RENDERER_3D_H
#define ONE_SHOT_RENDERER_3D_H

#include "Interfaces/IRenderer3D.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL COneShotRenderer3D
	{
		public:
			COneShotRenderer3D(IRenderer3D* pRenderer3D);
			~COneShotRenderer3D();

			bool Initialize(HWND hWnd);
			void Update(const double dDeltaTime);
			void Resize();
			void Shutdown();

			IRenderer3D* Get3DRenderer() const { return m_pRenderer3D; }

		private:
			bool m_bInitialized;

			IRenderer3D* m_pRenderer3D;
	};
};

#endif