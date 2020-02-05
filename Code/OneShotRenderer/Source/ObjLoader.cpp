#include "pch.h"
#include "ObjLoader.h"

using namespace DirectX;
using namespace OneShotRenderer;

static std::string GetFileNameFromFilepath(std::string sFilePath)
{
	uint64_t ullLastSlashPos = sFilePath.find_last_of('/');
	if (ullLastSlashPos == std::string::npos)
	{
		ullLastSlashPos = sFilePath.find_last_of('\\');

		if (ullLastSlashPos == std::string::npos)
			return false;
	}
	ullLastSlashPos++;
	std::string sFileName;
	sFileName = sFilePath.substr(ullLastSlashPos, sFilePath.length() - ullLastSlashPos);
	size_t szFileExtenstionPos = sFileName.find_first_of('.');
	sFileName = sFileName.erase(szFileExtenstionPos, sFileName.size() - szFileExtenstionPos);

	return sFileName;
}

static std::string ConsumeUptoAndIncluding(std::string& in_string, char c, bool remCharacter = false)
{
	size_t cPos = in_string.find(c, 0);
	if (cPos == std::string::npos)
		return "";

	std::string res = in_string.substr(0, cPos + 1);
	in_string = in_string.erase(0, cPos + 1);

	if (remCharacter)
		res.pop_back();

	return res;
}

CObjLoader::CObjLoader() :
m_pObjModel(nullptr)
{
}

bool CObjLoader::LoadModel(std::string sObjFilePath)
{
	//Read obj file
	std::ifstream objFileStream;
	objFileStream.open(sObjFilePath);

	if (!CVirtualFileSystem::GetInstance()->CreateFileStream(sObjFilePath, objFileStream))
		return false;

	objFileStream.seekg(0, std::ifstream::end);
	const uint64_t ulFileLength = objFileStream.tellg();
	objFileStream.seekg(0, std::ifstream::beg);

	char* pFileBuffer = OSE_NEW_ARRAY(char, ulFileLength);

	objFileStream.read(pFileBuffer, ulFileLength);

	objFileStream.close();

	//Get and store obj file name
	m_pObjModel = OSE_NEW(ObjModel);
	m_pObjModel->sModelName = GetFileNameFromFilepath(sObjFilePath);

	OutputDebugStringA("Reading OBJ File: "); OutputDebugStringA(m_pObjModel->sModelName.c_str()); OutputDebugStringA("\n");

	{
		std::vector<XMFLOAT3> vertPositions;
		std::vector<XMFLOAT3> vertNormals;
		std::vector<XMFLOAT2> vertTextureCoordinates;
		std::vector<ObjObjectUnparsed> upos;
		//extract the data out of the files
		{
			std::vector<char*> vertPosLines;
			std::vector<char*> vertNormLines;
			std::vector<char*> vertTexLines;
			//find vertex information
			{
				for (uint64_t i = 0; i < ulFileLength - 3; i++)
				{
					if (pFileBuffer[i] == '\n' && pFileBuffer[i + 1] == 'v' && pFileBuffer[i + 2] == ' '&& pFileBuffer[i + 3] == ' ')
					{
						i = i + 1;
						uint64_t u = i;
						while (u < ulFileLength && pFileBuffer[u] != '\n')
							u++;

						//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
						if (pFileBuffer[u - 1] == '\r')
							break;

						uint64_t ullCount = u - i;
						char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
						pStr[ullCount] = '\0';
						memcpy_s(pStr, ullCount, pFileBuffer + i, ullCount);
						vertPosLines.push_back(pStr);

						i++;
					}
				}

				for (uint64_t i = 0; i < ulFileLength - 3; i++)
				{
					if (pFileBuffer[i] == '\n' && pFileBuffer[i + 1] == 'v' && pFileBuffer[i + 2] == 'n' && pFileBuffer[i + 3] == ' ')
					{
						i = i + 1;
						uint64_t u = i;
						while (u < ulFileLength && pFileBuffer[u] != '\n')
							u++;

						//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
						if (pFileBuffer[u - 1] == '\r')
							break;

						uint64_t ullCount = u - i;
						char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
						pStr[ullCount] = '\0';
						memcpy_s(pStr, ullCount, pFileBuffer + i, ullCount);
						vertNormLines.push_back(pStr);

						i++;
					}
				}


				for (uint64_t i = 0; i < ulFileLength - 3; i++)
				{
					if (pFileBuffer[i] == '\n' && pFileBuffer[i + 1] == 'v' && pFileBuffer[i + 2] == 't' && pFileBuffer[i + 3] == ' ')
					{
						i = i + 1;
						uint64_t u = i;
						while (u < ulFileLength && pFileBuffer[u] != '\n')
							u++;

						//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
						if (pFileBuffer[u - 1] == '\r')
							break;

						uint64_t ullCount = u - i;
						char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
						pStr[ullCount] = '\0';
						memcpy_s(pStr, ullCount, pFileBuffer + i, ullCount);
						vertTexLines.push_back(pStr);

						i++;
					}
				}
			}
			//find objects, groups and faces
			{
				for (uint64_t i = 0; i < ulFileLength - 2; i++)
				{
					//find object tag
					if (pFileBuffer[i] == '\n' && pFileBuffer[i + 1] == 'o' && pFileBuffer[i + 2] == ' ')
					{
						ObjObjectUnparsed upo;
						{
							//find end of line
							i = i + 1;
							uint64_t u = i;
							while (u < ulFileLength && pFileBuffer[u] != '\n')
								u++;

							//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
							if (pFileBuffer[u - 1] == '\r')
								break;

							//get object tag
							uint64_t ullCount = u - i;
							char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
							pStr[ullCount] = '\0';
							memcpy_s(pStr, ullCount, pFileBuffer + i, ullCount);

							//create new unparsed object
							upo.sObjectName = std::string(pStr);
							OSE_SAFE_DELETE_ARRAY(pStr);
							i = u;
						}

						//find all groups and faces until another object is hit
						{
							uint64_t v = i;
							while (v < ulFileLength - 2 && !(pFileBuffer[v] == '\n' && pFileBuffer[v + 1] == 'o' && pFileBuffer[v + 2] == ' '))
							{
								if (pFileBuffer[v] == '\n' && pFileBuffer[v + 1] == 'g' && pFileBuffer[v + 2] == ' ')
								{
									v++;
									uint64_t u = v;
									while (u < ulFileLength && pFileBuffer[u] != '\n')
										u++;
									//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
									if (pFileBuffer[u - 1] == '\r')
										break;

									//get group tag
									uint64_t ullCount = u - v;
									char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
									pStr[ullCount] = '\0';
									memcpy_s(pStr, ullCount, pFileBuffer + v, ullCount);

									//create new unparsed group
									ObjGroupUnparsed upg;
									upg.sGroupName = std::string(pStr);
									OSE_SAFE_DELETE_ARRAY(pStr);

									//attempt to find a usemtl statement
									uint64_t z = u + 1;
									if(pFileBuffer[z] == 'u' && pFileBuffer[z + 1] == 's' && pFileBuffer[z + 2] == 'e' && pFileBuffer[z + 3] == 'm' && pFileBuffer[z + 4] == 't'  && pFileBuffer[z + 5] == 'l'  && pFileBuffer[z + 6] == ' ')
									{
										while (z < ulFileLength && pFileBuffer[z] != '\n')
											z++;

										ullCount = z - (u + 7);
										pStr = OSE_NEW_ARRAY(char, ullCount + 1);
										pStr[ullCount] = '\0';
										memcpy_s(pStr, ullCount, pFileBuffer + (u + 7), ullCount);
										upg.sMaterialName = std::string(pStr);
										OSE_SAFE_DELETE_ARRAY(pStr);
									}

									v = u;
									//find all faces...
									{
										while (v < ulFileLength - 2 && !(pFileBuffer[v] == '\n' && pFileBuffer[v + 1] == 'o' && pFileBuffer[v + 2] == ' '))
										{
											if (pFileBuffer[v] == '\n' && pFileBuffer[v + 1] == 'f' && pFileBuffer[v + 2] == ' ')
											{
												v++;
												uint64_t w = v;
												while (w < ulFileLength && pFileBuffer[w] != '\n')
													w++;

												//TODO: Solve very strange bug where extra verts, faces and normals seem to apear, they all have \n at the end
												if (pFileBuffer[w - 1] == '\r')
													break;

												ullCount = w - v;
												char* pStr = OSE_NEW_ARRAY(char, ullCount + 1);
												pStr[ullCount] = '\0';
												memcpy_s(pStr, ullCount, pFileBuffer + v, ullCount);
												upg.Indicies.push_back(pStr);
											}
											v++;
										}
									}


									upo.Groups.push_back(upg);
									v = u;
								}

								v++;
							}
						}

						upos.push_back(upo);
					}
				}
			}

			//clean up file buffer as we now have all data
			OSE_SAFE_DELETE_ARRAY(pFileBuffer);

			//Parse the strings into xmfloat3s
			{
				for (size_t i = 0; i < vertPosLines.size(); i++)
				{
					char* pLine = vertPosLines[i];
					uint64_t j = 3, k = 3, l = 0;
					XMFLOAT3 v = XMFLOAT3(0.0f, 0.0f, 0.0f);
					while (true)
					{
						if (pLine[j] == ' ' || pLine[j] == '\n')
						{
							char* pA = OSE_NEW_ARRAY(char, j - k + 1);
							pA[j - k] = '\0';
							memcpy_s(pA, j - k, pLine + k, j - k);

							if (l == 0)
							{
								v = XMFLOAT3(atof(pA), v.y, v.x);
							}
							else if (l == 1)
							{
								v = XMFLOAT3(v.x, atof(pA), v.x);
							}
							else if (l == 2)
							{
								v = XMFLOAT3(v.x, v.y, atof(pA));
								vertPositions.push_back(v);
								OSE_SAFE_DELETE_ARRAY(pA);
								break;
							}

							OSE_SAFE_DELETE_ARRAY(pA);
							k = j;
							l++;
						}
						j++;
					}
				}

				for (size_t i = 0; i < vertNormLines.size(); i++)
				{
					char* pLine = vertNormLines[i];
					uint64_t j = 3, k = 3, l = 0;
					XMFLOAT3 v = XMFLOAT3(0.0f, 0.0f, 0.0f);
					while (true)
					{
						if (pLine[j] == ' ' || pLine[j] == '\n')
						{
							char* pA = OSE_NEW_ARRAY(char, j - k + 1);
							pA[j - k] = '\0';
							memcpy_s(pA, j - k, pLine + k, j - k);

							if (l == 0)
							{
								v = XMFLOAT3(atof(pA), v.y, v.x);
							}
							else if (l == 1)
							{
								v = XMFLOAT3(v.x, atof(pA), v.x);
							}
							else if (l == 2)
							{
								v = XMFLOAT3(v.x, v.y, atof(pA));
								vertNormals.push_back(v);
								OSE_SAFE_DELETE_ARRAY(pA);
								break;
							}

							OSE_SAFE_DELETE_ARRAY(pA);
							k = j;
							l++;
						}
						j++;
					}
				}

				for (size_t i = 0; i < vertTexLines.size(); i++)
				{
					char* pLine = vertTexLines[i];
					uint64_t j = 3, k = 3, l = 0;
					XMFLOAT2 v = XMFLOAT2(0.0f, 0.0f);
					while (true)
					{
						if (pLine[j] == ' ' || pLine[j] == '\n')
						{
							char* pA = OSE_NEW_ARRAY(char, j - k + 1);
							pA[j - k] = '\0';
							memcpy_s(pA, j - k, pLine + k, j - k);

							if (l == 0)
							{
								v = XMFLOAT2(atof(pA), v.y);
							}
							else if (l == 1)
							{
								v = XMFLOAT2(v.x, 1.0f - atof(pA));
								vertTextureCoordinates.push_back(v);
								OSE_SAFE_DELETE_ARRAY(pA);
								break;
							}
							/*else if (l == 2)
							{
								v = XMFLOAT3(v.x, v.y, 0);
								vertTextureCoordinates.push_back(v);
								OSE_SAFE_DELETE_ARRAY(pA);
								break;
							}*/

							OSE_SAFE_DELETE_ARRAY(pA);
							k = j;
							l++;
						}
						j++;
					}
				}
			}

			//clean up unparsed vs vns and vts
			{
				for (size_t i = 0; i < vertPosLines.size(); i++)
				{
					OSE_SAFE_DELETE_ARRAY(vertPosLines[i]);
				}

				for (size_t i = 0; i < vertNormLines.size(); i++)
				{
					OSE_SAFE_DELETE_ARRAY(vertNormLines[i]);
				}

				for (size_t i = 0; i < vertTexLines.size(); i++)
				{
					OSE_SAFE_DELETE_ARRAY(vertTexLines[i]);
				}
			}
		}

		//extract faces and groups out of upos to create ObjObjects and ObjVerts
		//clean up a long the way
		//Format: f v/vt/vn v/vt/vn v/vt/vn 
		{
			for (size_t i = 0; i < upos.size(); i++)
			{
				ObjObject obj;
				obj.sObjectName = upos[i].sObjectName.erase(0, 2);
				for (size_t j = 0; j < upos[i].Groups.size(); j++)
				{
					ObjGroup group;
					group.sGroupName = upos[i].Groups[j].sGroupName.erase(0, 2);
					group.sMaterialName = upos[i].Groups[j].sMaterialName.erase(0, 1);
					for (size_t k = 0; k < upos[i].Groups[j].Indicies.size(); k++)
					{
						std::string indicieLine = std::string(upos[i].Groups[j].Indicies[k]).erase(0, 2);
						std::array<unsigned long, 3> tri;
						for (uint8_t n = 0; n < 3; n++)
						{
							//construct the vertex group
							ObjVertex vert;
							std::string vInd = ConsumeUptoAndIncluding(indicieLine, '/', true);
							std::string vtInd = ConsumeUptoAndIncluding(indicieLine, '/', true);
							std::string vnInd = ConsumeUptoAndIncluding(indicieLine, ' ', true);
							size_t a = atoi(vInd.c_str()) - 1, b = atoi(vtInd.c_str()) - 1, c = atoi(vnInd.c_str()) - 1;
							vert.xmf3Pos = vertPositions[a];

							if (vtInd == "")
								vert.xmf2Tex = XMFLOAT2(0.0f, 0.0f);
							else
								vert.xmf2Tex = vertTextureCoordinates[b];

							if (vnInd == "")
								vert.xmf3Norm = XMFLOAT3(0.0f, 0.0f, 0.0f);
							else
								vert.xmf3Norm = vertNormals[c];
							
							size_t szIndex = -1;
							for (size_t v = 0; v < m_pObjModel->Verticies.size(); v++)
							{
								if (m_pObjModel->Verticies[v].Equals(vert))
								{
									szIndex = v;
								}
							}

							if (szIndex == -1)
							{
								m_pObjModel->Verticies.push_back(vert);
								szIndex = m_pObjModel->Verticies.size() - 1;
								m_pObjModel->ullNumberOfVerticies++;
							}

							m_pObjModel->ullNumberOfIndicies++;
							tri[n] = szIndex;
						}

						group.Faces.push_back(tri);

						OSE_SAFE_DELETE_ARRAY(upos[i].Groups[j].Indicies[k]);
					}
					obj.Groups.push_back(group);
				}
				m_pObjModel->Objects.push_back(obj);
			}
		}
	}

	return true;
}

void CObjLoader::Shutdown()
{
	OSE_SAFE_DELETE(m_pObjModel);
}