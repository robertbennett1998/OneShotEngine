#pragma once
#ifndef ONE_SHOT_RENDERER_BIPED_H
#define ONE_SHOT_RENDERER_BIPED_H

#include "Interfaces/IRenderable.h"
#include "Shaders/Interfaces/IShaderProgram.h"
#include <DirectXMath.h>

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CBiped : public IRenderable
	{
		public:
			CBiped();
			~CBiped();

			bool Initialize() override;
			void Bind() override;
			void Shutdown() override;

			void Hide() override;
			void Show() override;
			void ToggleVisible() override;

			void SetWorldMatrix(DirectX::XMMATRIX xmmWorld);

			const CGeometry* GetGeometry() const override
			{
				return m_pGeometry;
			}
			const bool GetShouldRender() const override
			{
				return m_bShouldRender;
			}
			const IShaderProgram* GetShaderProgram() const override
			{
				return m_pShaderProgram;
			}

		private:
			DirectX::XMMATRIX m_xmmWorldMatrix;
			const CGeometry* m_pGeometry;
			bool m_bShouldRender;
			IShaderProgram* m_pShaderProgram;
	};
};

#endif