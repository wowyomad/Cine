#include "ScriptEngine.hpp"

#include "glash/Core/Log.hpp"
#include "Scene.hpp"
#include "Entity.hpp"

namespace Cine
{
	ScriptEngine ScriptEngine::s_ScriptEngine;

	bool ScriptEngine::IsKeyPressedFocused(KeyCode key)
	{
		if (s_ScriptEngine.m_ActiveScene->IsViewportFocused())
		{
			return Internal::Input::IsKeyPressed(key);
		}
		return false;

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
		m_LibraryCalls.SetLoggers(&Log::GetCoreLogger(), &Log::GetClientLogger());
		m_LibraryCalls.InitializeInput
		(
			IsKeyPressedFocused,
			Internal::Input::IsKeyDown,
			Internal::Input::IsKeyUp,
			Internal::Input::IsMouseButtonPressed,
			Internal::Input::IsMouseButtonDown,
			Internal::Input::IsMouseButtonUp,
			[]()->glm::vec2
			{
				glm::vec2 screenMousePosition = Internal::Input::GetMousePosition();
				auto data = s_ScriptEngine.GetActiveScene()->GetViewportData();

				glm::vec2 viewportMousePosition = { screenMousePosition.x - data.x, screenMousePosition.y - data.y };
				return viewportMousePosition;

			},
			[](const glm::vec2& screen) -> glm::vec3 //ScreenToWorld
			{
				auto data = s_ScriptEngine.GetActiveScene()->GetViewportData();
				Entity cameraEntity = s_ScriptEngine.GetActiveScene()->GetMainCameraEntity();
				if (!cameraEntity)
				{
					CINE_CORE_WARN("Getting position from camera without main camera");
					return {};
				}

				auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				auto& transformMatrix = cameraEntity.GetComponent<CachedTransform>().CachedMatrix;

				glm::mat4 projectionMatrix = camera.GetProjection();
				glm::mat4 viewMatrix = glm::inverse(transformMatrix);

				float x = (2.0f * screen.x) / data.Width - 1.0f;
				float y = 1.0f - (2.0f * screen.y) / data.Height;

				glm::vec4 ndc(x, y, 0.0f, 1.0f);

				glm::mat4 invProjView = glm::inverse(projectionMatrix * viewMatrix);

				glm::vec4 worldSpace = invProjView * ndc;
				worldSpace /= worldSpace.w;

				return glm::vec3(worldSpace);

			},
			[](const glm::vec3& world) -> glm::vec2
			{
				auto data = s_ScriptEngine.GetActiveScene()->GetViewportData();
				Entity cameraEntity = s_ScriptEngine.GetActiveScene()->GetMainCameraEntity();
				if (!cameraEntity)
				{
					CINE_CORE_WARN("Getting position from camera without main camera");
					return {};
				}

				auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				auto& transform = cameraEntity.GetComponent<CachedTransform>().CachedMatrix;

				glm::mat4 projectionMatrix = camera.GetProjection();
				glm::mat4 viewMatrix = glm::inverse(transform);

				glm::mat4 viewProjMatrix = projectionMatrix * viewMatrix;

				glm::vec4 clipSpacePosition = viewProjMatrix * glm::vec4(world, 1.0f);

				glm::vec2 ndc = glm::vec2(
					(clipSpacePosition.x / clipSpacePosition.w + 1.0f) * 0.5f,
					(1.0f - clipSpacePosition.y / clipSpacePosition.w) * 0.5f
				);

				glm::vec2 screen = glm::vec2(
					ndc.x * data.Width,
					ndc.y * data.Height
				);

				return screen;
			}
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
		m_LibraryCalls.InitializeComponents = m_Library.GetFunction<InitializeComponentsCall>("InitializeComponents");
		m_LibraryCalls.CreateComponent = m_Library.GetFunction<CreateComponentCall>("CreateComponent");
		m_LibraryCalls.RemoveComponent = m_Library.GetFunction<CreateComponentCall>("RemoveComponent");
		m_LibraryCalls.UpdateScripts = m_Library.GetFunction<UpdateAllScriptsCall>("UpdateScripts");
		m_LibraryCalls.SerializeComponent = m_Library.GetFunction<SerializeComponentCall>("SerializeComponent");
		m_LibraryCalls.DeserializeComponent = m_Library.GetFunction<DeserializeComponentCall>("DeserializeComponent");
		m_LibraryCalls.GetComponentsData = m_Library.GetFunction<GetComponentsDataCall>("GetComponentsData");
		m_LibraryCalls.SetActiveRegistry = m_Library.GetFunction<SetActiveRegistryCall>("SetActiveRegistry");
		m_LibraryCalls.InitializeInput = m_Library.GetFunction<InitializeInputCall>("InitializeInput");
		m_LibraryCalls.SetLoggers = m_Library.GetFunction<SetLoggersCall>("SetLoggers");

		bool allSet = m_LibraryCalls.InitializeComponents
			&& m_LibraryCalls.CreateComponent
			&& m_LibraryCalls.RemoveComponent
			&& m_LibraryCalls.UpdateScripts
			&& m_LibraryCalls.SerializeComponent
			&& m_LibraryCalls.DeserializeComponent
			&& m_LibraryCalls.SetActiveRegistry
			&& m_LibraryCalls.GetComponentsData
			&& m_LibraryCalls.InitializeInput
			&& m_LibraryCalls.SetLoggers;

		CINE_CORE_ASSERT(allSet, "Some of the library function weren't initialized");

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

}