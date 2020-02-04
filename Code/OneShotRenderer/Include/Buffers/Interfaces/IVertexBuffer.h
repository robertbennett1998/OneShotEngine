#pragma once
#ifndef ONE_SHOT_RENDERER_I_VERTEX_BUFFER_H
#define ONE_SHOT_RENDERER_I_VERTEX_BUFFER_H

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IVertexBuffer
	{
	public:
		virtual bool Initialize(void* pVertexData, size_t szVertexByteWidth, UINT uiNumVerts) = 0;
		virtual void Bind() = 0;
		virtual void Shutdown() = 0;
	};
};

#endif