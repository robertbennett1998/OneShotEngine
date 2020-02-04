#pragma once
#ifndef ONE_SHOT_RENDERER_D3D11_VERTEX_BUFFER_H
#define ONE_SHOT_RENDERER_D3D11_VERTEX_BUFFER_H

#include <d3d11.h>

#include "Buffers/Interfaces/IVertexBuffer.h"

namespace OneShotRenderer
{
	class CDirect3D;
	class CD3D11VertexBuffer : public IVertexBuffer
	{
		public:
			CD3D11VertexBuffer(CDirect3D* pDirect3D);
			~CD3D11VertexBuffer();

			virtual bool Initialize(void* pVertexData, size_t szVertexByteWidth, UINT uiNumVerts);
			virtual void Bind();
			virtual void Shutdown();

		private:
			CD3D11VertexBuffer();

			bool m_bInitialized;
			UINT m_uiVertexSize;
			CDirect3D* m_pDirect3D;
			ID3D11Buffer* m_pD3D11VertexBuffer;
	};
};
#endif