#pragma once
#ifndef ONE_SHOT_RENDERER_IMAGE_LOADER_FACTORY_H
#define ONE_SHOT_RENDERER_IMAGE_LOADER_FACTORY_H
#include "Interfaces/IImageLoader.h"
#include "TgaLoader.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CImageLoaderFactory
	{
		public:
			static CImageLoaderFactory* GetInstance()
			{
				if (sm_pInstance == nullptr)
				{
					sm_pInstance = OSE_NEW_LIFETIME(CImageLoaderFactory());
				}

				return sm_pInstance;
			}

			IImageLoader* GetImageLoader(std::string sFileExtension)
			{
				sFileExtension.erase(std::remove(sFileExtension.begin(), sFileExtension.end(), '\0'), sFileExtension.end());
				if (sFileExtension == "tga")
				{
					return OSE_NEW(CTgaLoader());
				}

				return nullptr;// OSE_NEW(CTgaLoader());
			}

		private:
			CImageLoaderFactory() { return; }
			~CImageLoaderFactory() { return; }

			static CImageLoaderFactory* sm_pInstance;
	};
};

#endif