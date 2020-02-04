#pragma once
#ifndef ONE_SHOT_RENDERER_D3D11_CONSTANT_BUFFER_H
#define ONE_SHOT_RENDERER_D3D11_CONSTANT_BUFFER_H

#include <d3d11.h>

#include "Buffers/Interfaces/IConstantBuffer.h"

#define SafeComRelease(x) { if (x != nullptr) { x->Release(); x = nullptr; } }

namespace OneShotRenderer
{
	class CDirect3D;
	class CD3D11ConstantBuffer : public IConstantBuffer
	{
		public:
			CD3D11ConstantBuffer(CDirect3D* pDirect3D);
			~CD3D11ConstantBuffer();

			// Inherited via IConstantBuffer
			virtual bool Initialize(void* pData, size_t szDataSize);
			virtual void UpdateBuffer();
			virtual void Bind(UINT uiSlot, UINT uiNumberOfBuffers, CONSTANT_BUFFER_USAGE buffUsage);
			virtual void Shutdown();
			virtual uint8_t GetByteAlignment() const { return 16; }

		private:
			CD3D11ConstantBuffer();

			bool m_bInitialized;
			void* m_pData;
			size_t m_szDataSize;
			CDirect3D* m_pDirect3D;
			ID3D11Buffer* m_pD3D11Buffer;
	};
};

#endif