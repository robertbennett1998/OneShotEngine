#pragma once
#ifndef ONE_SHOT_RENDERER_OBJ_LOADER_H
#define ONE_SHOT_RENDERER_OBJ_LOADER_H

#include "DirectXMath.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CObjLoader
	{
		public:
			struct ObjModel;
			struct ObjObject;
			struct ObjObjectUnparsed;
			struct ObjGroup;
			struct ObjGroupUnparsed;
			struct ObjVertex;

			struct ObjModel
			{
				std::string sModelName = "UNDEFINED";
				std::vector<ObjObject> Objects;
				std::vector<ObjVertex> Verticies;
				uint64_t ullNumberOfVerticies = 0;
				uint64_t ullNumberOfIndicies = 0;
			};

			struct ObjObject
			{
				std::string sObjectName;
				std::vector<ObjGroup> Groups;
			};

			struct ObjObjectUnparsed
			{
				std::string sObjectName;
				std::vector<ObjGroupUnparsed> Groups;
			};

			struct ObjGroup
			{
				std::string sGroupName;
				std::string sMaterialName;
				std::vector<std::array<unsigned long, 3>> Faces;
			};

			struct ObjGroupUnparsed
			{
				std::string sGroupName;
				std::string sMaterialName;
				std::vector<char*> Indicies;
			};

			struct ObjVertex
			{
				DirectX::XMFLOAT3 xmf3Pos;
				DirectX::XMFLOAT3 xmf3Norm;
				DirectX::XMFLOAT2 xmf2Tex;
				bool bUsingWCoord = false;

				bool Equals(ObjVertex& vert)
				{
					return	bUsingWCoord == vert.bUsingWCoord &&
						xmf3Norm.x == vert.xmf3Norm.x &&
						xmf3Norm.y == vert.xmf3Norm.y &&
						xmf3Norm.z == vert.xmf3Norm.z &&
						xmf3Pos.x == vert.xmf3Pos.x &&
						xmf3Pos.y == vert.xmf3Pos.y &&
						xmf3Pos.z == vert.xmf3Pos.z &&
						xmf2Tex.x == vert.xmf2Tex.x &&
						xmf2Tex.y == vert.xmf2Tex.y;
				}
			};

		public:
			CObjLoader();

			bool LoadModel(std::string sObjFilePath);
			void Shutdown();

			const ObjModel* const GetModel() const
			{
				return m_pObjModel;
			}

		private:
			ObjModel* m_pObjModel;
	};
};

#endif