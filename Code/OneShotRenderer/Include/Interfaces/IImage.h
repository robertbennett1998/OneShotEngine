#pragma once
#ifndef ONE_SHOT_RENDERER_I_IMAGE_H
#define ONE_SHOT_RENDERER_I_IMAGE_H

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IImage
	{
		public:
			enum class ImagePixelFormat
			{
				Undefined = 0,
				BLACKWHITE_FLOAT = 1,
				RGB_FLOAT = 3,
				RGBA_FLOAT = 4,
			};

		public:
			IImage() { return; }
			~IImage() { return; }
			virtual bool Initialize(uint16_t uiWidth, uint16_t uiHeight, ImagePixelFormat pixelFormat, const void* const pPixelData) = 0;
			virtual void Shutdown() = 0;
			virtual void* GetPixelData() const = 0;
			virtual uint16_t GetWidth() const = 0;
			virtual uint16_t GetHeight() const = 0;
			virtual ImagePixelFormat GetPixelFormat() const = 0;
			virtual uint32_t GetRowPitchInBytes() const = 0;
	};
};

#endif