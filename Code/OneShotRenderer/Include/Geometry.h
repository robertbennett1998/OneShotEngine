#pragma once
#ifndef ONE_SHOT_RENDERER_GEOMETRY_H
#define ONE_SHOT_RENDERER_GEOMETRY_H

#include <DirectXMath.h>
#include <d3d11.h>
#include <stdlib.h>
#include <vector>

#include "Buffers/Interfaces/IVertexBuffer.h"
#include "Buffers/Interfaces/IIndexBuffer.h"
#include "Material.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CGeometry
	{
		friend class CGeometryManager;
		public:
			struct SubGeometry
			{
				const CMaterial* pMaterial;
				IIndexBuffer* pIndexBuffer;
				uint64_t ullNumberOfIndicies;

				void Bind()
				{
					if (pIndexBuffer != nullptr)
						pIndexBuffer->Bind();

					if (pMaterial != nullptr)
						pMaterial->Bind(0);
				}

				~SubGeometry()
				{
					OSE_SAFE_SHUTDOWN(pIndexBuffer);
				}
			};

		public:
			struct Vertex
			{
				DirectX::XMFLOAT3 xmf3VertexPosition;
				DirectX::XMFLOAT3 xmf3VertexNormal;
				DirectX::XMFLOAT2 xmf2VertexTextureCoordinate;
			};

			void Bind() const;

			const IVertexBuffer* const GetVertexBuffer() const
			{
				return m_pVertexBuffer;
			}

			const std::vector<SubGeometry*> GetSubGeometries() const
			{
				return m_pSubGeometries;
			}

			const uint64_t GetNumberOfIndicies() const
			{
				return m_ullIndicieCount;
			}

			const uint64_t GetNumberOfIVerticies() const
			{
				return m_ullVertexCount;
			}


		private:
			CGeometry();
			~CGeometry();

			bool Create(IVertexBuffer* pVertexBuffer, std::vector<SubGeometry*> pSubGeometries, uint64_t ullVertexCount, uint64_t ullIndicieCount, std::string sGeomName);
			void Shutdown();

			uint64_t m_ullVertexCount, m_ullIndicieCount;
			std::string m_sGeomName;
			std::vector<SubGeometry*> m_pSubGeometries;
			IVertexBuffer* m_pVertexBuffer;
	};
};

#endif