#pragma once
#ifndef GAME_ENGINE_RENDERER_SHADER_PARAMS_H
#define GAME_ENGINE_RENDERER_SHADER_PARAMS_H

#include "Interfaces/IRenderer3D.h"
#include "Buffers/Interfaces/IConstantBuffer.h"

#include <d3d11.h>
//#include <DirectXMath.h>
//#include <stdlib.h>
//#include <sstream>

namespace OneShotRenderer
{
	template<typename... shaderParams>
	class ONE_SHOT_RENDERER_DLL CShaderParams
	{
		public:
			CShaderParams(IRenderer3D* p3DRenderer, UINT uiSlotNumber, CONSTANT_BUFFER_USAGE paramUsage) :
				m_bInitialized(false),
				m_uiSlotNumber(uiSlotNumber),
				m_ConstantBufferUsage(paramUsage),
				m_p3DRenderer(p3DRenderer),
				m_pConstantBuffer(nullptr),
				m_pBufferMemory(nullptr)
			{
				return;
			}

			~CShaderParams()
			{

			}

			inline bool Initiailze(shaderParams... values)
			{
				if (!m_bInitialized)
				{
					m_pConstantBuffer = m_p3DRenderer->CreateConstantBuffer();

					constexpr size_t szVarSize = (sizeof(values) + ...);

					size_t szBuffSize = szVarSize;
					size_t szRemainder = (szVarSize % m_pConstantBuffer->GetByteAlignment());
					if (szRemainder > 0)
						szBuffSize += m_pConstantBuffer->GetByteAlignment() - szRemainder;

					m_pBufferMemory = malloc(szBuffSize);

					size_t szOffset = 0;
					//TODO: Probs not going to work for certain types... didn't like the r value I believe so pass by value and assign isntead
					(CopyToMemBuffer(values, szOffset), ...);

					D3D11_BUFFER_DESC bd; ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
					bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					bd.ByteWidth = szBuffSize;
					bd.Usage = D3D11_USAGE_DYNAMIC;
					bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
					bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

					if (!m_pConstantBuffer->Initialize(m_pBufferMemory, szBuffSize))
						return false;

					m_bInitialized = true;
					return true;
				}

				return false;
			}

			inline bool SetValues(shaderParams... values)
			{
				if (m_bInitialized && m_pBufferMemory != nullptr)
				{
					size_t szOffset = 0;
					//TODO: Probs not going to work for certain types...
					(CopyToMemBuffer(values, szOffset), ...);

					m_pConstantBuffer->UpdateBuffer();

					return true;
				}

				return false;
			}

			inline void Bind()
			{
				m_pConstantBuffer->Bind(m_uiSlotNumber, 1, m_ConstantBufferUsage);
			}

			inline void Shutdown()
			{
				if (m_bInitialized)
				{
					OSE_SAFE_SHUTDOWN(m_pConstantBuffer);
					free(m_pBufferMemory);
					m_pBufferMemory = nullptr;

					m_bInitialized = false;
				}
			}

			inline ID3D11Buffer* const GetParamBuffer() const
			{
				return m_pConstantBuffer;
			}

		private:
			template<typename T>
			inline constexpr void CopyToMemBuffer(T val, size_t& szOffset)
			{
				if (m_pBufferMemory != nullptr)
				{
					T* pVar = (T*)(((char*)m_pBufferMemory) + szOffset);
					*pVar = val;
					szOffset += sizeof(T);
				}
			}

			bool m_bInitialized;
			UINT m_uiSlotNumber;
			CONSTANT_BUFFER_USAGE m_ConstantBufferUsage;
			IConstantBuffer* m_pConstantBuffer;
			void* m_pBufferMemory;
			IRenderer3D* m_p3DRenderer;
	};
};
#endif