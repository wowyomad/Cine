#include "ScriptEngine.hpp"

#include "glash/Core/Log.hpp"

namespace Cine
{
	ScriptEngine ScriptEngine::s_ScriptEngine;

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
		m_LibraryCalls.InitializeApplicationContext(&Application::Get());
		m_LibraryCalls.InitializeComponents(registry);
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

	const std::vector<ComponentData>& ScriptEngine::GetComponentsData() const
	{
		return m_ComponentsData;
	}

	bool ScriptEngine::UpdateFunctionCalls()
	{
		m_LibraryCalls.InitializeApplicationContext = m_Library.GetFunction<InitializeApplicationContextCall>("InitializeApplicationContext");
		m_LibraryCalls.InitializeComponents = m_Library.GetFunction<InitializeComponentsCall>("InitializeComponents");
		m_LibraryCalls.CreateComponent = m_Library.GetFunction<CreateComponentCall>("CreateComponent");
		m_LibraryCalls.RemoveComponent = m_Library.GetFunction<CreateComponentCall>("RemoveComponent");
		m_LibraryCalls.UpdateScripts = m_Library.GetFunction<UpdateAllScriptsCall>("UpdateScripts");
		m_LibraryCalls.SerializeComponent = m_Library.GetFunction<SerializeComponentCall>("SerializeComponent");
		m_LibraryCalls.DeserializeComponent = m_Library.GetFunction<DeserializeComponentCall>("DeserializeComponent");
		m_LibraryCalls.GetComponentsData = m_Library.GetFunction<GetComponentsDataCall>("GetComponentsData");
		
		return m_LibraryCalls.InitializeComponents
			&& m_LibraryCalls.CreateComponent
			&& m_LibraryCalls.RemoveComponent
			&& m_LibraryCalls.UpdateScripts
			&& m_LibraryCalls.GetComponentsData;
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
}