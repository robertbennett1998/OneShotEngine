#pragma once
#ifndef ONE_SHOT_RENDERER_I_PIXEL_SHADER_H
#define ONE_SHOT_RENDERER_I_PIXEL_SHADER_H

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IPixelShader
	{
		public:
			virtual bool Initialize(std::string sPSFile, std::string sPSEntryPoint) = 0;
			virtual void Bind() = 0;
			virtual void Shutdown() = 0;
	};
};
#endif