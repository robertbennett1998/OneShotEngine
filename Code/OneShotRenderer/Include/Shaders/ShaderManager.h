#pragma once
#ifndef ONE_SHOT_RENDERER_SHADER_MANAGER_H
#define ONE_SHOT_RENDERER_SHADER_MANAGER_H

#include "Interfaces/IRenderer3D.h"
#include "Shaders/Interfaces/IShaderProgram.h"

namespace OneShotRenderer
{
	class ONE_SHOT_RENDERER_DLL CShaderManager
	{
		public:
			static CShaderManager* GetInstance()
			{
				return sm_pInstance;
			}

			static CShaderManager* CreateInstance(IRenderer3D* p3DRenderer)
			{
				if (sm_pInstance == nullptr)
					sm_pInstance = OSE_NEW_LIFETIME(CShaderManager(p3DRenderer));

				return sm_pInstance;
			}

			template<class T>
			bool RegisterShaderType(std::string sShaderName)
			{
				if (m_Shaders.find(sShaderName) == m_Shaders.end())
				{
					IShaderProgram* pShaderProgram = OSE_NEW(T(m_p3DRenderer));
					if (!pShaderProgram->Initialize())
					{
						OSE_SAFE_SHUTDOWN(pShaderProgram);
						return false;
					}

					m_Shaders.try_emplace(sShaderName, pShaderProgram);
					return true;
				}

				return false;
			}

			template<class R>
			R* GetShaderInstance(std::string sShaderName) const
			{
				auto shader = m_Shaders.find(sShaderName);
				if (shader == m_Shaders.end())
					return nullptr;

				return (R*)shader->second;
			}

			void Resize()
			{
				for (auto& i : m_Shaders)
				{
					i.second->Resize();
				}
			}

			void Update()
			{
				for (auto& i : m_Shaders)
					i.second->Update();
			}

			void Shutdown()
			{
				for (auto& i : m_Shaders)
					OSE_SAFE_SHUTDOWN(i.second);
			}

		private:
			CShaderManager(IRenderer3D* p3DRenderer);
			~CShaderManager();

			IRenderer3D* m_p3DRenderer;
			std::map<std::string, IShaderProgram*> m_Shaders;
			static CShaderManager* sm_pInstance;
	};
};

#endif