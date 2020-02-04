#pragma once
#ifndef ONE_SHOT_RENDERER_I_IMAGE_LOADER_H
#define ONE_SHOT_RENDERER_I_IMAGE_LOADER_H

#include "IImage.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IImageLoader
	{
		public:
			virtual IImage* Load(std::string sFilePath) = 0;
			virtual void Destroy() = 0;
	};
};

#endif