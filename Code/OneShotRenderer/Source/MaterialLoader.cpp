#include "pch.h"
#include "MaterialLoader.h"

using namespace OneShotRenderer;

static std::string GetDirectoryFromFilepath(std::string sFilePath)
{
	size_t szBackwardSlashPos = sFilePath.find_first_of('\\');
	while (szBackwardSlashPos != std::string::npos)
	{
		sFilePath[szBackwardSlashPos] = '/';

		szBackwardSlashPos = sFilePath.find_first_of('\\', szBackwardSlashPos);
	}

	size_t szLastSlashPos = sFilePath.find_last_of('/');

	std::string sDirectory = sFilePath.substr(0, szLastSlashPos + 1);

	return sDirectory;
}

CMaterialLoader::CMaterialLoader(IRenderer3D* p3DRenderer) :
m_p3DRenderer(p3DRenderer)
{
}

bool CMaterialLoader::Load(std::string sMtlFilePath)
{
	char* pFileBuffer = nullptr;

	std::ifstream filestream;
	CVirtualFileSystem::GetInstance()->CreateFileStream(sMtlFilePath, filestream);
	if(filestream.is_open())
	{
		filestream.seekg(0, std::ifstream::end);
		const size_t szFileLength = filestream.tellg();
		filestream.seekg(0, std::ifstream::beg);

		pFileBuffer = OSE_NEW_ARRAY(char, szFileLength);

		filestream.read(pFileBuffer, szFileLength);

		std::string sFile(pFileBuffer);

		OSE_DELETE_ARRAY(pFileBuffer);
		pFileBuffer = nullptr;

		size_t szNextMtlPos = sFile.find("newmtl");
		size_t szEoLPos = sFile.find('\n', szNextMtlPos);
		size_t szDiffPos = std::string::npos;
		size_t szSpecPos = std::string::npos;
		size_t szNormPos = std::string::npos;
		while(szNextMtlPos != std::string::npos)
		{
			szDiffPos = std::string::npos;
			szSpecPos = std::string::npos;
			szNormPos = std::string::npos;

			{
				CMaterial* pMaterial = OSE_NEW(CMaterial);

				//mtl name
				szEoLPos = sFile.find('\n', szNextMtlPos);
				pMaterial->m_sMaterialName = sFile.substr(szNextMtlPos + 7, szEoLPos - (szNextMtlPos + 7));

				szDiffPos = sFile.find("map_Kd", szNextMtlPos) + 7;
				//szSpecPos = sFile.find("map_Ks", szNextMtlPos);
				//szNormPos = sFile.find("map_Kn", szNextMtlPos);

				szNextMtlPos = sFile.find("newmtl", szEoLPos);
				if(szNextMtlPos != std::string::npos)
				{
					if (szDiffPos > szNextMtlPos)
						continue;

					if (szSpecPos > szNextMtlPos)
						continue;

					if (szNormPos > szNextMtlPos)
						continue;
				}

				//diffuse texture
				{
					szEoLPos = sFile.find('\n', szDiffPos);
					std::string diffPath = GetDirectoryFromFilepath(sMtlFilePath) + sFile.substr(szDiffPos, szEoLPos - szDiffPos) + '\0';

					//TODO: Redo all the file path code in next iteration
					size_t szBackwardSlashPos = diffPath.find_first_of('\\');
					while (szBackwardSlashPos != std::string::npos)
					{
						diffPath[szBackwardSlashPos] = '/';

						szBackwardSlashPos = diffPath.find_first_of('\\', szBackwardSlashPos);
					}

					//load into texture
					ITexture2D* pTexture2D = m_p3DRenderer->CreateTexture2D();
					if (!pTexture2D->Initialize(diffPath))
						return false;

					if (!pMaterial->AddTexture(CMaterial::TextureType::DIFFUSE, pTexture2D))
						return false;
				}

				//specular texture
				{
					//szEoLPos = sFile.find('\n', szNextMtlPos);
					//std::string specPath = sFile.substr(szDiffPos, szEoLPos - szDiffPos);
				}

				//normal texture
				{
					//szEoLPos = sFile.find('\n', szNextMtlPos);
					//std::string normPath = sFile.substr(szDiffPos, szEoLPos - szDiffPos);
				}

				m_pMaterials.push_back(pMaterial);
			}

			szEoLPos = sFile.find('\n', szNextMtlPos);
		}

		filestream.close();
		return true;
	}

	return false;
}

std::vector<CMaterial*> CMaterialLoader::GetMaterials() const
{
	return m_pMaterials;
}