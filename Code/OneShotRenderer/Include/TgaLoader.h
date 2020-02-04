#pragma once
#ifndef ONE_SHOT_TGA_LOADER_H
#define ONE_SHOT_TGA_LOADER_H

#include "Interfaces/IImageLoader.h"
#include "Image.h"

namespace OneShotRenderer
{
	class CTgaLoader : public IImageLoader
	{
		#pragma region TGA_File_Definitions
		public:
		#pragma pack(push, 1)
			enum class ColourMapType : uint8_t
			{
				NoColourMapIncluded = 0,
				ColourMapIncluded = 1
			};

			enum class ImageType : uint8_t
			{
				NoImageData = 0,
				UncompressedColourMappedImage = 1,
				UncompressedTrueColourImage = 2,
				UncompressedBlackAndWhiteImage = 3,
				RunLengthEncodedColourMappedImage = 9,
				RunLengthEncodedTrueColourImage = 10,
				RunLengthEncodedBlackAndWhiteImage = 11
			};

			struct ImageDescriptor
			{
				uint8_t alphaChannelBits : 4;
				uint8_t imageOrigin : 2;
				uint8_t unused : 2;
			};

			struct ColourMapSpecification
			{
				uint16_t uiFirstEntryIndex;
				uint16_t uiColourMapLength;
				uint8_t ubyColourMapEntrySize;
			};

			struct ImageSpecification
			{
				uint16_t uiXOrigin;
				uint16_t uiYOrigin;
				uint16_t uiImageWidth;
				uint16_t uiImageHeight;
				uint8_t ubyPixelDepth;
				ImageDescriptor imageDescriptor;
			};

			struct TGA_Header
			{
				uint8_t ubyIdLength;
				ColourMapType ubyColourMapType;
				ImageType ubyImageType;
				ColourMapSpecification colourMapSpecification;
				ImageSpecification imageSpecification;
			};

			enum class AlphaChannelType : uint8_t
			{
				NoAlphaData = 0,
				UndefinedDataInAlphaChannelDiscard = 1,
				UndefinedDataInTheAlphaChannelRetain = 2,
				UsefulAlphaDataPresent = 3,
				PreMultipliedAlphaDataPresent = 4
			};

			struct TGA_ExtensionArea
			{
				uint16_t uiExtensionSize;
				uint8_t aubyAuthorName[41];
				uint8_t aubyAuthorComments[324];
				uint8_t aubyDateTimeStamp[12];
				uint8_t aubyJobName[41];
				uint8_t aubyJobTime[6];
				uint8_t aubySoftwareIdp[41];
				uint8_t aubySoftwareVersion[3];
				uint8_t aubyKeyColour[4];
				uint16_t aubyPixelAspectRatio[2];
				uint16_t aubyGammeValue[2];
				uint32_t ulColourCorrectionOffset;
				uint32_t ulPostageStampOffset;
				uint32_t ulScanLineOffset;
				AlphaChannelType ubyAttributesType;
			};

			struct TGA_Footer
			{
				uint32_t ulExtensionArea;
				uint32_t ulDeveloperArea;
				uint8_t aubySignature[18];
			};

			struct TGA_File
			{
				~TGA_File()
				{
					OSE_SAFE_DELETE_ARRAY(pImageId);
					OSE_SAFE_DELETE_ARRAY(pColourMapData);
					OSE_SAFE_DELETE_ARRAY(pImageData);
					OSE_SAFE_DELETE_ARRAY(pScanLineTable);
					OSE_SAFE_DELETE_ARRAY(pPostageStampImage);
					OSE_SAFE_DELETE_ARRAY(ColourCorectionTable);
				}

				TGA_Header header;
				char* pImageId;
				char* pColourMapData;
				char* pImageData;
				//Dev area
				TGA_ExtensionArea extensionArea;
				char* pScanLineTable;
				char* pPostageStampImage;
				char* ColourCorectionTable;
				TGA_Footer footer;
			};

			struct RLE_Packet_Header
			{
				uint8_t ubyPacketType : 1;
				uint8_t ubyRepititionCount : 7;
			};

		#pragma pack(pop)
		#pragma endregion
		public:
			CTgaLoader();
			~CTgaLoader();

			IImage* Load(std::string sFilePath) override;
			void Destroy() override;

		private:
			inline void LoadFooter(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile);
			inline void LoadHeader(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile);
			inline bool LoadExpansionArea(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline bool LoadDeveloperArea(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline bool LoadImageId(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline bool LoadColourMapData(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline void LoadRunLengthEncodedImage(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile);
			inline bool LoadScanlineData(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline bool LoadPostageImage(const char* pFileBuffer, const uint32_t ulFileLength, TGA_File& tgaFile) { return false; }
			inline bool LoadColourCorrectionTable(const char* pFileBuffer, const uint32_t ulFileLength) { return false; }
			inline IImage* CreateImage(TGA_File& tgaFile);
			inline DirectX::XMFLOAT4 LoadPixelDataToXMFloat4(const char* pPixel);
			inline DirectX::XMFLOAT4* FormatPixelData(TGA_File& tgaFile);
	};
};

#endif