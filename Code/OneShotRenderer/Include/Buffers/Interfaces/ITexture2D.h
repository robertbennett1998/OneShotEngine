#pragma once
#ifndef ONE_SHOT_RENDERER_I_TEXTURE_2D_H
#define ONE_SHOT_RENDERER_I_TEXTURE_2D_H

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL ITexture2D
	{
	public:
		virtual bool Initialize(std::string sTexturePath) = 0;
		virtual void BindToPixelShader(uint8_t iSlot = 0) const = 0;
		virtual void Shutdown() = 0;
	};
}

#endif