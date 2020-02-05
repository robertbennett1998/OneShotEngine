#include "pch.h"
#include "GeometryManager.h"
#include "ObjLoader.h"
#include <thread>
#include <sstream>
#include "MaterialLibrary.h"

using namespace OneShotRenderer;

static std::string GetFileNameFromFilepath(std::string sFilePath)
{
	uint64_t ullLastSlashPos = sFilePath.find_last_of('/');
	if (ullLastSlashPos == std::string::npos)
	{
		ullLastSlashPos = sFilePath.find_last_of('\\');

		if (ullLastSlashPos == std::string::npos)
			return std::string();
	}
	ullLastSlashPos++;
	std::string sFileName;
	sFileName = sFilePath.substr(ullLastSlashPos, sFilePath.length() - ullLastSlashPos);
	size_t szFileExtenstionPos = sFileName.find_first_of('.');
	sFileName = sFileName.erase(szFileExtenstionPos, sFileName.size() - szFileExtenstionPos);

	return sFileName;
}

CGeometryManager* CGeometryManager::sm_pInstance = nullptr;

CGeometryManager::CGeometryManager(IRenderer3D* p3DRenderer) :
m_p3DRenderer(p3DRenderer)
{
}

CGeometryManager::~CGeometryManager()
{
}

bool CGeometryManager::RegisterGeometry(std::string sFilePath)
{
	if(m_Geometry.find(GetFileNameFromFilepath(sFilePath)) == m_Geometry.end())
	{
		CObjLoader objLoader;
		if (!objLoader.LoadModel(sFilePath))
			return false;

		const CObjLoader::ObjModel* const pModel = objLoader.GetModel();

		CGeometry::Vertex* pVerticies = OSE_NEW_ARRAY(CGeometry::Vertex, pModel->ullNumberOfVerticies);
		for (size_t i = 0; i < pModel->Verticies.size(); i++)
		{
			pVerticies[i].xmf3VertexPosition = pModel->Verticies[i].xmf3Pos;
			pVerticies[i].xmf3VertexNormal = pModel->Verticies[i].xmf3Norm;
			pVerticies[i].xmf2VertexTextureCoordinate = pModel->Verticies[i].xmf2Tex;
		}

		IVertexBuffer* pVertexBuffer = m_p3DRenderer->CreateVertexBuffer();
		if (!pVertexBuffer->Initialize(pVerticies, sizeof(CGeometry::Vertex), pModel->ullNumberOfVerticies))
		{
			OSE_SAFE_DELETE_ARRAY(pVerticies);
			OSE_SAFE_SHUTDOWN(pVertexBuffer);
			objLoader.Shutdown();
			return false;
		}
		std::vector<CGeometry::SubGeometry*> pSubGeometries;
		uint64_t ullIndPos = 0;
		for (size_t i = 0; i < pModel->Objects.size(); i++)
		{
			for (size_t j = 0; j < pModel->Objects[i].Groups.size(); j++)
			{
				CGeometry::SubGeometry* pSubGeom = OSE_NEW(CGeometry::SubGeometry());
				pSubGeom->pMaterial = CMaterialLibrary::GetInstance()->GetMaterial(pModel->Objects[i].Groups[j].sMaterialName);

				pSubGeom->ullNumberOfIndicies = pModel->Objects[i].Groups[j].Faces.size() * 3;
				unsigned long* pIndicies = OSE_NEW_ARRAY(unsigned long, pSubGeom->ullNumberOfIndicies);
				//std::stringstream ss;
				ullIndPos = 0;
				for (size_t k = 0; k < pModel->Objects[i].Groups[j].Faces.size(); k++)
				{
					for (uint8_t l = 0; l < 3; l++)
					{
						pIndicies[ullIndPos] = pModel->Objects[i].Groups[j].Faces[k][l];
						//ss << (ullIndPos) + l << ": " << pIndicies[(k * 3) + l] << "\n";
						ullIndPos++;
					}
				}
				//OutputDebugStringA(ss.str().c_str());

				pSubGeom->pIndexBuffer = m_p3DRenderer->CreateIndexBuffer();
				if (!pSubGeom->pIndexBuffer->Initialize(pIndicies, sizeof(unsigned long), pSubGeom->ullNumberOfIndicies))
				{
					OSE_SAFE_DELETE_ARRAY(pVerticies);
					OSE_SAFE_SHUTDOWN(pVertexBuffer);
					OSE_SAFE_DELETE(pSubGeom);
					objLoader.Shutdown();
					return false;
				}

				OSE_SAFE_DELETE_ARRAY(pIndicies);

				pSubGeometries.push_back(pSubGeom);
			}
		}

		CGeometry* pGeom = OSE_NEW(CGeometry());
		if (!pGeom->Create(pVertexBuffer, pSubGeometries, pModel->ullNumberOfVerticies, pModel->ullNumberOfIndicies, pModel->sModelName))
		{
			OSE_SAFE_DELETE_ARRAY(pVerticies);
			OSE_SAFE_SHUTDOWN(pVertexBuffer);
			objLoader.Shutdown();
			return false;
		}

		m_Geometry.try_emplace(pModel->sModelName, pGeom);

		OSE_SAFE_DELETE_ARRAY(pVerticies);
		objLoader.Shutdown();
		return true;
	}

	return false;
}

const CGeometry* CGeometryManager::GetGeomInstance(std::string sGeomName)
{
	auto geom = m_Geometry.find(sGeomName);
	if (geom == m_Geometry.end())
		return nullptr;
	
	return geom->second;
}

void CGeometryManager::Shutdown()
{
	for (auto &p : m_Geometry) 
	{
		OSE_SAFE_SHUTDOWN(p.second);
	}
}
