#pragma once
#ifndef ONE_SHOT_RENDERER_I_RENDERABLE_H
#define ONE_SHOT_RENDERER_I_RENDERABLE_H

#include "Geometry.h"
#include "Shaders/Interfaces/IShaderProgram.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL IRenderable
	{
		public:
			virtual bool Initialize() = 0;
			virtual void Bind() = 0;
			virtual void Shutdown() = 0;

			virtual void Hide() = 0;
			virtual void Show() = 0;
			virtual void ToggleVisible() = 0;

			virtual const CGeometry* GetGeometry() const = 0;
			virtual const bool GetShouldRender() const = 0;
			virtual const IShaderProgram* GetShaderProgram() const = 0;
	};
};

#endif