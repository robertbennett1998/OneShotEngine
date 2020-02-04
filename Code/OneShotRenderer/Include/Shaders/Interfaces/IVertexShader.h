#pragma once
#ifndef ONE_SHOT_RENDERER_I_VERTEX_SHADER_H
#define ONE_SHOT_RENDERER_I_VERTEX_SHADER_H

class ONE_SHOT_RENDERER_DLL IVertexShader
{
	public:
		virtual bool Initialize(std::string sVSFile, void* pInputLayout, uint8_t ubyNumInputElems, std::string sVSEntryPoint) = 0;
		virtual void Bind() = 0;
		virtual void Shutdown() = 0;
};

#endif