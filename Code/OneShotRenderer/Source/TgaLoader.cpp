#include "pch.h"
#include "TgaLoader.h"
#include "Image.h"

using namespace OneShotRenderer;

CTgaLoader::CTgaLoader()
{
	return;
}

CTgaLoader::~CTgaLoader()
{
	return;
}

IImage* CTgaLoader::Load(std::string sFilePath)
{
	char* pFileBuffer = nullptr;
	uint32_t ulFileLength = 0;
	TGA_File tgaFile; ZeroMemory(&tgaFile, sizeof(TGA_File));

	//Read TGA file
	{
		std::ifstream ifTgaFile;
		CVirtualFileSystem::GetInstance()->CreateFileStream(sFilePath, ifTgaFile, std::ifstream::in | std::ifstream::binary);

		if (!ifTgaFile.is_open())
		{
			DWORD err = GetLastError();
			OSE_LOG_WARNING("General", "Couldn't open TGA file (%) Error Code: %", sFilePath, err);
			return nullptr;
		}

		ifTgaFile.seekg(0, std::ifstream::end);
		ulFileLength = ifTgaFile.tellg();
		ifTgaFile.seekg(0, std::ifstream::beg);

		pFileBuffer = OSE_NEW_ARRAY(char, ulFileLength);

		ifTgaFile.read(pFileBuffer, ulFileLength);

		ifTgaFile.close();
	}

	LoadFooter(pFileBuffer, ulFileLength, tgaFile);
	LoadHeader(pFileBuffer, ulFileLength, tgaFile);

	if (tgaFile.header.ubyIdLength != 0)
	{
		if (!LoadImageId(pFileBuffer, ulFileLength, tgaFile))
			return nullptr;
	}

	switch (tgaFile.header.ubyImageType)
	{
		default: OSE_DELETE_ARRAY(pFileBuffer); pFileBuffer = nullptr; return nullptr;	
		//case ImageType::UncompressedColourMappedImage: break;
		//case ImageType::UncompressedTrueColourImage: break;
		//case ImageType::UncompressedBlackAndWhiteImage: break;
		//case ImageType::RunLengthEncodedColourMappedImage: break;
		case ImageType::RunLengthEncodedTrueColourImage:
		{
			if (tgaFile.header.ubyColourMapType != ColourMapType::NoColourMapIncluded)
			{
				OSE_DELETE_ARRAY(pFileBuffer);
				pFileBuffer = nullptr;
				return nullptr;
			}

			LoadRunLengthEncodedImage(pFileBuffer, ulFileLength, tgaFile);
			break;
		}
		//case ImageType::RunLengthEncodedBlackAndWhiteImage: break;
	}

	//if its the new format then read expansion data
	if(std::string((char*)tgaFile.footer.aubySignature) == "TRUEVISION-XFILE.")
	{
		
	}

	OSE_DELETE_ARRAY(pFileBuffer);
	pFileBuffer = nullptr;
	return CreateImage(tgaFile);
}

void CTgaLoader::Destroy()
{
	return;
}

void CTgaLoader::LoadFooter(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile)
{
	memcpy_s(&tgaFile.footer, sizeof(TGA_Footer), pFileBuffer + (ulFileLength - sizeof(TGA_Footer)), sizeof(TGA_Footer));
	return;
}

void CTgaLoader::LoadHeader(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile)
{
	memcpy_s(&tgaFile.header, sizeof(TGA_Header), pFileBuffer, sizeof(TGA_Header));

	return;
}

void CTgaLoader::LoadRunLengthEncodedImage(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile)
{
	uint64_t ullPixelCount = 0;
	uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;
	const char* pRawPixelData = pFileBuffer + sizeof(TGA_Header) + tgaFile.header.ubyIdLength + (tgaFile.header.colourMapSpecification.uiColourMapLength * tgaFile.header.colourMapSpecification.ubyColourMapEntrySize);
	tgaFile.pImageData = OSE_NEW_ARRAY(char, tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight * ubyPixelLength);
	char* pTargetPixel = (char*)tgaFile.pImageData;


	while (ullPixelCount < (tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight))
	{
		bool bIsRLE = *pRawPixelData & 0x80;
		uint8_t ubyRepCount = (*pRawPixelData & 0x7F) + 1;
		ullPixelCount += ubyRepCount;
		pRawPixelData++;
		if (bIsRLE)
		{
			for (uint8_t i = 0; i < ubyRepCount; i++)
			{
				memcpy_s(pTargetPixel, ubyPixelLength, pRawPixelData, ubyPixelLength);
				pTargetPixel += ubyPixelLength;
			}
			pRawPixelData += ubyPixelLength;
		}
		else
		{
			for (uint8_t i = 0; i < ubyRepCount; i++)
			{
				memcpy_s(pTargetPixel, ubyPixelLength, pRawPixelData, ubyPixelLength);
				pTargetPixel += ubyPixelLength;
				pRawPixelData += ubyPixelLength;
			}
		}
	}
}

IImage* CTgaLoader::CreateImage(TGA_File& tgaFile)
{
	IImage* pImage = nullptr;
	uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;

	if (ubyPixelLength == 1)
	{
		//pImage = OSE_NEW(CImage<float>());
		//if(!pImage->Initialize(tgaFile.header.imageSpecification.uiImageWidth, tgaFile.header.imageSpecification.uiImageHeight, IImage::ImagePixelFormat::BLACKWHITE_FLOAT, tgaFile.pImageData))
		//{
		//	OSE_SAFE_SHUTDOWN(pImage);
		//	return nullptr;
		//}
		return nullptr;
	}
	else if (ubyPixelLength == 3)
	{
		//pImage = OSE_NEW(CImage<DirectX::XMFLOAT3>());
		//if (!pImage->Initialize(tgaFile.header.imageSpecification.uiImageWidth, tgaFile.header.imageSpecification.uiImageHeight, IImage::ImagePixelFormat::RGB_FLOAT, tgaFile.pImageData))
		//{
		//	OSE_SAFE_SHUTDOWN(pImage);
		//	return nullptr;
		//}
		return nullptr;
	}
	else if (ubyPixelLength == 4)
	{
		DirectX::XMFLOAT4* pPixelData = FormatPixelData(tgaFile);
		pImage = OSE_NEW(CImage<DirectX::XMFLOAT4>());
		if (!pImage->Initialize(tgaFile.header.imageSpecification.uiImageWidth, tgaFile.header.imageSpecification.uiImageHeight, IImage::ImagePixelFormat::RGBA_FLOAT, pPixelData))
		{
			OSE_SAFE_DELETE_ARRAY(pPixelData);
			OSE_SAFE_SHUTDOWN(pImage);
			return nullptr;
		}

		OSE_SAFE_DELETE_ARRAY(pPixelData);
	}

	return pImage;
}

DirectX::XMFLOAT4 CTgaLoader::LoadPixelDataToXMFloat4(const char* pPixel)
{
	return DirectX::XMFLOAT4((uint8_t)pPixel[2] / 255.0f, (uint8_t)pPixel[1] / 255.0f, (uint8_t)pPixel[0] / 255.0f, (uint8_t)pPixel[3] / 255.0f);
}

DirectX::XMFLOAT4* CTgaLoader::FormatPixelData(TGA_File& tgaFile)
{
	DirectX::XMFLOAT4* pPixelData = nullptr;
	if (tgaFile.header.imageSpecification.imageDescriptor.imageOrigin == 0)
	{
		pPixelData = OSE_NEW_ARRAY(DirectX::XMFLOAT4, tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight);
		uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;

		uint64_t ullCount = 0;
		for (uint32_t y = tgaFile.header.imageSpecification.uiImageHeight; y > 0; y--)
		{
			for (uint32_t x = 0; x < tgaFile.header.imageSpecification.uiImageWidth; x++)
			{
				((DirectX::XMFLOAT4*)pPixelData)[ullCount] = LoadPixelDataToXMFloat4((char*)tgaFile.pImageData + ((((y-1) * tgaFile.header.imageSpecification.uiImageWidth) + x) * ubyPixelLength));
				ullCount++;
			}
		}
	}
	else if (tgaFile.header.imageSpecification.imageDescriptor.imageOrigin == 1)
	{
		pPixelData = OSE_NEW_ARRAY(DirectX::XMFLOAT4, tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight);
		uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;

		uint64_t ullCount = 0;
		for (uint32_t y = tgaFile.header.imageSpecification.uiImageHeight; y > 0; y--)
		{
			for (uint32_t x = tgaFile.header.imageSpecification.uiImageWidth; x > 0; x--)
			{
				((DirectX::XMFLOAT4*)pPixelData)[ullCount] = LoadPixelDataToXMFloat4((char*)tgaFile.pImageData + ((((y - 1) * tgaFile.header.imageSpecification.uiImageWidth) + (x - 1)) * ubyPixelLength));
				ullCount++;
			}
		}
	}
	else if (tgaFile.header.imageSpecification.imageDescriptor.imageOrigin == 2)
	{
		pPixelData = OSE_NEW_ARRAY(DirectX::XMFLOAT4, tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight);
		uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;

		uint64_t ullCount = 0;
		for (uint32_t y = 0; y < tgaFile.header.imageSpecification.uiImageHeight; y++)
		{
			for (uint32_t x = 0; x < tgaFile.header.imageSpecification.uiImageWidth; x++)
			{
				((DirectX::XMFLOAT4*)pPixelData)[ullCount] = LoadPixelDataToXMFloat4((char*)tgaFile.pImageData + (((y * tgaFile.header.imageSpecification.uiImageWidth) + x) * ubyPixelLength));
				ullCount++;
			}
		}
	}
	else if (tgaFile.header.imageSpecification.imageDescriptor.imageOrigin == 3)
	{
		pPixelData = OSE_NEW_ARRAY(DirectX::XMFLOAT4, tgaFile.header.imageSpecification.uiImageWidth * tgaFile.header.imageSpecification.uiImageHeight);
		uint8_t ubyPixelLength = tgaFile.header.imageSpecification.ubyPixelDepth / 8;

		uint64_t ullCount = 0;
		for (uint32_t y = 0; y < tgaFile.header.imageSpecification.uiImageHeight; y++)
		{
			for (uint32_t x = tgaFile.header.imageSpecification.uiImageWidth; x > 0; x--)
			{
				((DirectX::XMFLOAT4*)pPixelData)[ullCount] = LoadPixelDataToXMFloat4((char*)tgaFile.pImageData + ((((y)* tgaFile.header.imageSpecification.uiImageWidth) + (x - 1)) * ubyPixelLength));
				ullCount++;
			}
		}
	}

	return pPixelData;
}