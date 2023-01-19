#include "App.hpp"

#include <iostream>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Log.hpp"

#include "Binding.hpp"

#include "Credit.hpp"
#include "Setting.hpp"

namespace Core
{
	Resources::NextScene App::nextScene = {"Menu", Resources::SceneType::ST_Menu};
	bool ThreadsManager::multithread = true;
	GLFWwindow* App::window = nullptr;
	bool App::stopGame = false;

	App::App()
		: width(0)
		, height(0)
		, inputsManager(window)
		, threadsManager(5)
		, currentScene(nullptr)
		, timer(width, height)
	{
	}

	App::~App()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// glfw: terminate, clearing all previously allocated GLFW resources.
		glfwTerminate();
	}	

	bool App::Init(const AppInit& p_appInit)
	{
		width = p_appInit.width;
		height = p_appInit.height;

		// glfw: initialize and configure
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_appInit.major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_appInit.minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// glfw window creation
		window = glfwCreateWindow(p_appInit.width, p_appInit.height, p_appInit.name, NULL, NULL);
		if (window == NULL)
		{
			DEBUG_LOG("Failed to create GLFW window");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, p_appInit.framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DEBUG_LOG("Failed to initialize GLAD");
			return false;
		}

		GLint flags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(p_appInit.glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// Setup Dear ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		ImportScript();

		CreateScenes();
		
		currentScene = resources.GetResource<Resources::Scene>("Menu");

		inputsManager.SetWindow(window);

		return true;
	}

	void App::InitRenderer()
	{
		m_renderer.CreatePrimitifsMeshs();
	}

	void App::Update()
	{
		Core::Inputs inputs;
		while (!stopGame)
		{
			// input
			glfwPollEvents();
			inputs = inputsManager.Update();

			// render
			glClearColor(0.06f, 0.01f, 0.34f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			threadsManager.Update();

			ReloadScript(inputs);
			ChangeScene();

			timer.Update();
			currentScene->Update(inputs,timer.GetDeltaTime());
			currentScene->Draw(inputs, timer.elapsedMono, timer.elapsedMulti);

			if (resources.CheckAllResourcesLoaded() && !timer.timerDone && timer.begin && threadsManager.multithread)
				timer.ChronoEnd(timer.elapsedMulti);
			else if (resources.CheckAllResourcesLoaded() && !timer.timerDone && timer.begin && !threadsManager.multithread)
				timer.ChronoEnd(timer.elapsedMono);

			glfwSwapBuffers(window);
		}
	}

	LowRenderer::Model App::CreateModel(const std::string& p_mesh, const std::string& p_shader, const std::string& p_texture)
	{
		return LowRenderer::Model(
			resources.GetResource<Resources::Mesh>(p_mesh),
			resources.GetResource<Resources::Shader>(p_shader),
			resources.GetResource<Resources::Texture>(p_texture));
	}

	void App::ImportScript()
	{
		scripts.push_back(pybind11::module::import("InitScene"));
		scripts.push_back(pybind11::module::import("PlayerMove"));
		scripts.push_back(pybind11::module::import("PlayerJump"));
		scripts.push_back(pybind11::module::import("DeathZone"));
		scripts.push_back(pybind11::module::import("Camera"));
	}

	void App::ReloadScript(Core::Inputs p_inputs)
	{
		if (p_inputs.reloadScript)
		{
			for (pybind11::module script : scripts)
				script.reload();
		}
	}

	void App::PlayInitScript()
	{
		for (const pybind11::module script : scripts)
			script.attr("Init")();
	}

	void App::ChangeScene()
	{
		if (nextScene.name == currentScene->GetName())
			return;

		switch (nextScene.sceneType)
		{
		case Resources::SceneType::ST_Game:
			timer.begin = true;
			timer.ChronoBegin();
			CreateResource();
			currentScene = resources.GetResource<Resources::Scene>(nextScene.name);
			currentScene->scripts = scripts;
			Debug::Log::Print("Change scene " + nextScene.name + "\n", Debug::LogLevel::Notification);
			PlayInitScript();
			threadsManager.Init();
			break;

		case Resources::SceneType::ST_Menu:

			threadsManager.DeleteThreads();
			if (currentScene->GetName() == "Scene1" && !resources.CheckAllResourcesLoaded())
				break;

			currentScene = resources.GetResource<Resources::Menu>(nextScene.name);
			Debug::Log::Print("Change scene " + nextScene.name + "\n", Debug::LogLevel::Notification);
			resources.DeleteResources();
			timer.begin = false;
			timer.timerDone = false;

			break;

		case Resources::SceneType::ST_Setting:
			currentScene = resources.GetResource<Resources::Setting>(nextScene.name);
			Debug::Log::Print("Change scene " + nextScene.name + "\n", Debug::LogLevel::Notification);
			break;

		case Resources::SceneType::ST_Credit:
			currentScene = resources.GetResource<Resources::Credit>(nextScene.name);
			Debug::Log::Print("Change scene " + nextScene.name + "\n", Debug::LogLevel::Notification);
			break;

		default:
			break;
		}

	}

	void App::CreateResource()
	{
		Core::Debug::Log::Print("---------\n", Core::Debug::LogLevel::None);
		Core::Debug::Log::Print("Init resource\n", Core::Debug::LogLevel::Notification);

		std::string name = "BasicShader";
		std::string pathVert = "Resources/Shaders/VertexShaderSource.vert";
		std::string pathFrag = "Resources/Shaders/FragmentShaderSource.frag";
		threadsManager.AddResourceToInit(resources.Create<Resources::Shader>(name, pathVert, pathFrag));

		// Patrick
		name = "Patrick";
		std::string path = "Resources/Obj/patrick.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "PatrickText";
		path = "Resources/Textures/patrick.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		name = "ColliderShader";
		pathVert = "Resources/Shaders/VertexShaderSource.vert";
		pathFrag = "Resources/Shaders/ColliderFrag.frag";
		threadsManager.AddResourceToInit(resources.Create<Resources::Shader>(name, pathVert, pathFrag));

		path = "Resources/Obj/cube.obj";
		name = "Cube";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "Sphere";
		path = "Resources/Obj/Sphere.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "Wall";
		path = "Resources/Textures/wall.jpg";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		name = "Sample";
		path = "Resources/Textures/sample.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Slime
		name = "Slime";
		path = "Resources/Obj/Slime.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "SlimeText";
		path = "Resources/Textures/Slime.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Frying_pan
		name = "FryingPan";
		path = "Resources/Obj/frying_pan.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "FryingPanText";
		path = "Resources/Textures/frying_pan.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Pistol
		name = "Pistol";
		path = "Resources/Obj/c_pistol.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "PistolText";
		path = "Resources/Textures/c_pistol.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Companion
		name = "Companion";
		path = "Resources/Obj/companion.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "CompanionText";
		path = "Resources/Textures/companion.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// PotatOS
		name = "PotatOS";
		path = "Resources/Obj/potatOS.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "PotatOSText";
		path = "Resources/Textures/potatOS.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Chocobo
		name = "Chocobo";
		path = "Resources/Obj/chocobo.obj";
		threadsManager.AddResourceToInit(resources.Create<Resources::Mesh>(name, path));

		name = "ChocoboText";
		path = "Resources/Textures/chocobo.png";
		threadsManager.AddResourceToInit(resources.Create<Resources::Texture>(name, path));

		// Scene1
		name = "Scene1";
		resources.Create<Resources::Scene>(name, "")->Init(width, height);

		InitRenderer();
		name = "BoxCollider";
		Resources::Mesh* cube = resources.Create<Resources::Mesh>(name, "");
		*cube = m_renderer.CreateCubePrimitif();
		cube->LoadMesh();
		cube->SetStat(Resources::StatResource::LOADED);

		name = "SphereCollider";
		Resources::Mesh* sphere = resources.Create<Resources::Mesh>(name, "");
		*sphere = m_renderer.CreateSpherePrimitif();
		sphere->LoadMesh();
		sphere->SetStat(Resources::StatResource::LOADED);

		name = "CapsuleCollider";
		Resources::Mesh* capsule = resources.Create<Resources::Mesh>(name, "");
		*capsule = m_renderer.CreateCapsulePrimitif();
		capsule->LoadMesh();
		capsule->SetStat(Resources::StatResource::LOADED);
	}
	void App::CreateScenes()
	{
		// Menu
		std::string name = "Menu";
		resources.Create<Resources::Menu>(name, "")->Init(width, height);

		// Credit
		name = "Credit";
		resources.Create<Resources::Credit>(name, "")->Init(width, height);

		// Setting
		name = "Setting";
		resources.Create<Resources::Setting>(name, "")->Init(width, height, &inputsManager);
	}
}