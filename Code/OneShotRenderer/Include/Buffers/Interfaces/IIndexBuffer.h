#pragma once
#ifndef ONE_SHOT_RENDERER_I_INDEX_BUFFER_H
#define ONE_SHOT_RENDERER_I_INDEX_BUFFER_H

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IIndexBuffer
	{
		public:
			virtual bool Initialize(void* pIndexData, size_t szIndexByteWidth, UINT uiNumIndicies) = 0;
			virtual void Bind() = 0;
			virtual void Shutdown() = 0;
	};
};

#endif