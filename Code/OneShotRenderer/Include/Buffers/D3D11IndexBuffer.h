#pragma once
#ifndef ONE_SHOT_RENDERER_D3D11_INDEX_BUFFER_H
#define ONE_SHOT_RENDERER_D3D11_INDEX_BUFFER_H

#include <d3d11.h>

#include "Buffers/Interfaces/IIndexBuffer.h"

namespace OneShotRenderer
{
	class CDirect3D;
	class CD3D11IndexBuffer : public IIndexBuffer
	{
		public:
			CD3D11IndexBuffer(CDirect3D* pDirect3D);
			~CD3D11IndexBuffer();

			virtual bool Initialize(void* pIndexData, size_t szIndexByteWidth, UINT uiNumIndicies);
			virtual void Bind();
			virtual void Shutdown();

		private:
			CD3D11IndexBuffer();

			bool m_bInitialized;
			CDirect3D* m_pDirect3D;
			ID3D11Buffer* m_pD3D11IndexBuffer;
	};
};

#endif