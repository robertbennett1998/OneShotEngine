#pragma once
#ifndef ONE_SHOT_RENDERER_I_CONSTANT_BUFFER_H
#define ONE_SHOT_RENDERER_I_CONSTANT_BUFFER_H

namespace OneShotRenderer
{
	enum class CONSTANT_BUFFER_USAGE
	{
		VertextShader = 0,
		PixelShader = 1
	};

	class ONE_SHOT_RENDERER_DLL IConstantBuffer
	{
		public:
			virtual bool Initialize(void* pData, size_t szDataSize) = 0;
			virtual void UpdateBuffer() = 0;
			virtual void Bind(UINT uiSlot, UINT uiNumberOfBuffers, CONSTANT_BUFFER_USAGE buffUsage) = 0;
			virtual void Shutdown() = 0;
			virtual uint8_t GetByteAlignment() const = 0;
	};
};

#endif