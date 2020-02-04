#pragma once
#ifndef ONE_SHOT_RENDERER_IMAGE_H
#define ONE_SHOT_RENDERER_IMAGE_H

#include "Interfaces/IImage.h"

namespace OneShotRenderer
{
	template<class T>
	class CImage : public IImage
	{
		public:
			CImage() :
				m_bInitialized(false),
				m_pPixelData(nullptr),
				m_uiWidth(0),
				m_uiHeight(0),
				m_PixelFormat(ImagePixelFormat::Undefined)
			{
			}

			~CImage()
			{
			}

			bool Initialize(uint16_t uiWidth, uint16_t uiHeight, ImagePixelFormat pixelFormat, const void* const pPixelData) override
			{
				if (!m_bInitialized)
				{
					if (pPixelData == nullptr || pixelFormat == ImagePixelFormat::Undefined)
						return false;

					m_uiWidth = uiWidth;
					m_uiHeight = uiHeight;
					m_PixelFormat = pixelFormat;

					m_pPixelData = OSE_NEW_ARRAY(T, m_uiWidth * m_uiHeight);
					memcpy_s(m_pPixelData, sizeof(T) * m_uiWidth * m_uiHeight, pPixelData, sizeof(T) * m_uiWidth * m_uiHeight);

					m_bInitialized = true;
					return true;
				}

				return false;
			}

			void Shutdown() override
			{
				OSE_SAFE_DELETE_ARRAY(m_pPixelData);
				m_bInitialized = false;
			}

			void* GetPixelData() const override
			{
				return m_pPixelData;
			}

			uint16_t GetWidth() const override
			{
				return m_uiWidth;
			}

			uint16_t GetHeight() const override
			{
				return m_uiHeight;
			}

			ImagePixelFormat GetPixelFormat() const override
			{
				return m_PixelFormat;
			}

			uint32_t GetRowPitchInBytes() const override
			{
				return sizeof(T) * m_uiWidth;
			}

		private:
			bool m_bInitialized;
			T* m_pPixelData;
			uint16_t m_uiWidth, m_uiHeight;
			ImagePixelFormat m_PixelFormat;
	};
};

#endif