#include "ScriptEngine.hpp"

#include "glash/Core/Log.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace Cine
{
	ScriptEngine ScriptEngine::s_ScriptEngine;

	bool ScriptEngine::IsKeyPressedFocused(KeyCode key)
	{
		return s_ScriptEngine.m_ActiveScene->IsViewportFocused() ? Internal::Input::IsKeyPressed(key) : false;
	}
	bool ScriptEngine::IsKeyDownFocused(KeyCode key)
	{
		return s_ScriptEngine.m_ActiveScene->IsViewportFocused() ? Internal::Input::IsKeyDown(key) : false;
	}
	bool ScriptEngine::IsKeyUpFocused(KeyCode key)
	{
		//Doesn't make sense to return neither false nor true but it is what it is.
		return s_ScriptEngine.m_ActiveScene->IsViewportFocused() ? Internal::Input::IsKeyUp(key) : true;
	}
	bool ScriptEngine::IsMouseButtonPressedFocused(MouseCode button)
	{
		return s_ScriptEngine.m_ActiveScene->IsViewportHoveredAndFocused() ? Internal::Input::IsMouseButtonPressed(button) : false;
	}
	bool ScriptEngine::IsMouseButtonDownFocused(MouseCode button)
	{
		return s_ScriptEngine.m_ActiveScene->IsViewportHoveredAndFocused() ? Internal::Input::IsMouseButtonDown(button) : false;
	}
	bool ScriptEngine::IsMouseButtonUpFocused(MouseCode button)
	{
		//Same as IsKeyUpFocused
		return s_ScriptEngine.m_ActiveScene->IsViewportHoveredAndFocused() ? Internal::Input::IsMouseButtonDown(button) : true;
	}
	glm::vec2 ScriptEngine::GetMouseViewportPosition()
	{
		static glm::vec2 s_LastViewportMousePosition = glm::vec2(0.0f);
		if (s_ScriptEngine.m_ActiveScene->IsViewportHoveredAndFocused())
		{
			glm::vec2 screenMousePosition = Internal::Input::GetMousePosition();
			auto& viewport = s_ScriptEngine.GetActiveScene()->GetViewportData();
			s_LastViewportMousePosition = { screenMousePosition.x - viewport.x, screenMousePosition.y - viewport.y };
		}

		return s_LastViewportMousePosition;
	}
	glm::vec3 ScriptEngine::ToWorldSpace(const glm::vec2& screenSpace)
	{
		return s_ScriptEngine.m_ActiveScene->ScreenToWorldSpace(screenSpace);
	}
	glm::vec2 ScriptEngine::ToScreenSpace(const glm::vec3& worldSpace)
	{
		return s_ScriptEngine.m_ActiveScene->WorldToScreenSpace(worldSpace);
	}


	ScriptEngine& ScriptEngine::Get()
	{
		return s_ScriptEngine;
	}


	void ScriptEngine::LoadLibary(const std::filesystem::path& libraryPath)
	{
		UnloadLibrary();

		std::filesystem::path exeDir = std::filesystem::current_path();

		std::filesystem::path tempLibraryPath = exeDir / libraryPath.filename();
		tempLibraryPath.replace_extension(".dll.temp");

		try {
			std::filesystem::copy_file(libraryPath, tempLibraryPath, std::filesystem::copy_options::overwrite_existing);
		}
		catch (const std::filesystem::filesystem_error& e) {
			CINE_CORE_ASSERT(false, "Failed to copy library {0} to {1}: {2}", libraryPath.string(), tempLibraryPath.string(), e.what());
			return;
		}

		if (!m_Library.Load(tempLibraryPath)) {
			CINE_CORE_ASSERT(false, "Failed to load library {0}", tempLibraryPath.string());
			return;
		}

		UpdateFunctionCalls();
		m_ComponentsData.clear();
	}
	void ScriptEngine::UnloadLibrary()
	{
		m_Library.Unload();
	}
	void ScriptEngine::InitializeComponents(entt::registry& registry)
	{
		m_LibraryCalls.InitializeComponents(registry);
		m_LibraryCalls.SetImGuiContext(Application::Get().GetImGuiContext());
		m_LibraryCalls.SetLoggers(&Log::GetCoreLogger(), &Log::GetClientLogger());
		m_LibraryCalls.InitializeInput
		(
			IsKeyPressedFocused,
			IsKeyDownFocused,
			IsKeyUpFocused,
			IsMouseButtonPressedFocused,
			IsMouseButtonDownFocused,
			IsMouseButtonUpFocused,
			GetMouseViewportPosition,
			ToWorldSpace,
			ToScreenSpace
		);
		UpdateComponentsData();
	}

	void ScriptEngine::CreateComponent(entt::entity entity, const std::string& componentName)
	{
		m_LibraryCalls.CreateComponent(entity, componentName);
	}

	void ScriptEngine::RemoveComponent(entt::entity entity, const std::string& componentName)
	{
		m_LibraryCalls.RemoveComponent(entity, componentName);
	}

	void ScriptEngine::UpdateScripts(Timestep ts)
	{
		m_LibraryCalls.UpdateScripts(ts);
	}

	YAML::Node ScriptEngine::SerializeComponent(entt::entity entity, const std::string& componentName)
	{
		return m_LibraryCalls.SerializeComponent(entity, componentName);
	}

	void ScriptEngine::DeserializeComponent(entt::entity entity, YAML::Node& node, const std::string& componentName)
	{
		m_LibraryCalls.DeserializeComponent(entity, componentName, node);
	}

	//TODO: 
	void ScriptEngine::SetActiveRegistry(entt::registry& registry)
	{
		m_LibraryCalls.SetActiveRegistry(registry);
	}

	const std::vector<ComponentData>& ScriptEngine::GetComponentsData() const
	{
		return m_ComponentsData;
	}

	bool ScriptEngine::UpdateFunctionCalls()
	{
		m_LibraryCalls.InitializeComponents =	m_Library.GetFunction<InitializeComponentsCall>("InitializeComponents");
		m_LibraryCalls.CreateComponent =		m_Library.GetFunction<CreateComponentCall>("CreateComponent");
		m_LibraryCalls.RemoveComponent =		m_Library.GetFunction<CreateComponentCall>("RemoveComponent");
		m_LibraryCalls.UpdateScripts =			m_Library.GetFunction<UpdateAllScriptsCall>("UpdateScripts");
		m_LibraryCalls.SerializeComponent =		m_Library.GetFunction<SerializeComponentCall>("SerializeComponent");
		m_LibraryCalls.DeserializeComponent =	m_Library.GetFunction<DeserializeComponentCall>("DeserializeComponent");
		m_LibraryCalls.GetComponentsData =		m_Library.GetFunction<GetComponentsDataCall>("GetComponentsData");
		m_LibraryCalls.SetActiveRegistry =		m_Library.GetFunction<SetActiveRegistryCall>("SetActiveRegistry");
		m_LibraryCalls.InitializeInput =		m_Library.GetFunction<InitializeInputCall>("InitializeInput");
		m_LibraryCalls.SetLoggers =				m_Library.GetFunction<SetLoggersCall>("SetLoggers");
		m_LibraryCalls.SetImGuiContext =		m_Library.GetFunction<SetImGuiContextCall>("SetImGuiContext");
		m_LibraryCalls.DrawImGui =				m_Library.GetFunction<DrawImGuiCall>("DrawImGui");

		bool allSet = m_LibraryCalls.InitializeComponents
			&& m_LibraryCalls.CreateComponent
			&& m_LibraryCalls.RemoveComponent
			&& m_LibraryCalls.UpdateScripts
			&& m_LibraryCalls.SerializeComponent
			&& m_LibraryCalls.DeserializeComponent
			&& m_LibraryCalls.SetActiveRegistry
			&& m_LibraryCalls.GetComponentsData
			&& m_LibraryCalls.InitializeInput
			&& m_LibraryCalls.SetLoggers
			&& m_LibraryCalls.SetImGuiContext
			&& m_LibraryCalls.DrawImGui;

		CINE_CORE_ASSERT(allSet, "Some of the library function weren't initialized!");

		return allSet;
	}

	void ScriptEngine::UpdateComponentsData()
	{
		m_ComponentsData.clear();

		ComponentsDataC data = m_LibraryCalls.GetComponentsData();
		for (size_t i = 0; i < data.Count; ++i)
		{
			m_ComponentsData.push_back({ data.Names[i], data.IsScript[i] });
			delete data.Names[i];
		}

		delete data.Names;
		delete data.IsScript;
	}

	void ScriptEngine::SetActiveScene(Scene* scene)
	{

		s_ScriptEngine.m_ActiveScene = scene;
	}

	Scene* ScriptEngine::GetActiveScene()
	{
		return s_ScriptEngine.m_ActiveScene;
	}

	void ScriptEngine::SetImGuiContext(ImGuiContext* context)
	{
		m_LibraryCalls.SetImGuiContext(context);
	}

	void ScriptEngine::DrawImGui()
	{
		return  m_LibraryCalls.DrawImGui();
	}

}