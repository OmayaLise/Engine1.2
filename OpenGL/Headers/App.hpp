#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <pybind11/pybind11.h>

#include <map>

// Resources
#include "ResourcesManager.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
// LowRenderer
#include "Camera.hpp"
#include "GameObject.hpp"
#include "LightManager.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
// Core
#include "InputsManager.hpp"
#include "Timer.hpp"
#include "ThreadsManager.hpp"

namespace Core
{
	struct AppInit
	{
		unsigned int width;
		unsigned int height;
		unsigned int major;
		unsigned int minor;
		const char* name;
		void (*framebuffer_size_callback) (GLFWwindow* window, int width, int height);
		void (*glDebugOutput) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei lenght, const GLchar* message, const void* userParam);
	};

	class App
	{
		// Attribute
	private:

		static GLFWwindow* window;
		unsigned int width;
		unsigned int height;

		Time::Timer timer;
		InputsManager inputsManager;
		LowRenderer::Renderer m_renderer;
		ThreadsManager threadsManager;

		static bool stopGame;

		std::vector<pybind11::module> scripts;

	public:
		Resources::ResourceManager resources;
		Resources::Scene* currentScene;
		static Resources::NextScene nextScene;
		
		// Methode
	public:
		App();
		~App();
		static App* GetInstance() 
		{
			static App* singleton = nullptr;
			if (singleton == nullptr)
				singleton = new App();
			return singleton;
		}

		bool Init(const AppInit& p_appInit);
		void Update();

		// Binding
		LowRenderer::Model CreateModel(const std::string& p_mesh, const std::string& p_shader, const std::string& p_texture);
		void AddGameObject(LowRenderer::GameObject* p_gameObject, LowRenderer::GOType p_type = LowRenderer::GOType::None) { currentScene->AddGameObject(p_gameObject, p_type); };

		// Get and Set
		GLFWwindow* GetWindow() { return window; };
		static void StopGame() { stopGame = true; };

	private:
		void ChangeScene();
		void InitRenderer();
		void CreateResource();
		void CreateScenes();

		void ImportScript();
		void ReloadScript(Core::Inputs p_inputs);
		void PlayInitScript();
	};
}