#pragma once
#ifndef ONE_SHOT_RENDERER_I_SHADER_PROGRAM_H
#define ONE_SHOT_RENDERER_I_SHADER_PROGRAM_H

class IShaderProgram
{
	public:
		virtual bool Initialize() = 0;
		virtual void Update() = 0;
		virtual void Bind() = 0;
		virtual void Resize() = 0;
		virtual void Shutdown() = 0;
};

#endif