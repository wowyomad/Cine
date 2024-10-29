#include "glash/glash_pch.hpp"
#include "ContentBrowserPanel.hpp"

#include "glash/Scene/AssetManager.hpp"
#include "glash/Scene/SceneSerializer.hpp"
#include "glash/Core/Timer.hpp"

#include "../Utils/Shell.hpp"

#include <filesystem>
#include <imgui.h>	

static std::atomic<bool> s_IsReloadingScripts(false);
static std::atomic<int> s_ScriptReloadTimeElapsed(0);
static std::thread s_ScriptThread;
static bool s_IsSceneRuntime = false;

namespace Cine
{
	void RunScriptAsync(const std::string& scriptPath, std::function<void()> onFinish) {
		if (s_ScriptThread.joinable()) {
			s_ScriptThread.join();
		}

		s_IsReloadingScripts = true;
		s_ScriptReloadTimeElapsed = 0;

		s_ScriptThread = std::thread([scriptPath, onFinish]() {
			auto startTime = std::chrono::steady_clock::now();

			std::thread timerThread([&]() {
				while (s_IsReloadingScripts) {
					std::this_thread::sleep_for(std::chrono::seconds(1));
					auto now = std::chrono::steady_clock::now();
					s_ScriptReloadTimeElapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
				}
				});

			Shell::RunPythonLoud(scriptPath);

			s_IsReloadingScripts = false;

			if (timerThread.joinable()) {
				timerThread.join();
			}

			if (onFinish) {
				onFinish();
			}
			});

			s_ScriptThread.detach();
	}


	bool ContentBrowserPanel::DisplayReloadScriptsButton()
	{
		if (s_IsReloadingScripts)
		{
			ImGui::Button(("Reloading... " + std::to_string(s_ScriptReloadTimeElapsed) + "s").c_str());
		}
		else
		{
			if (ImGui::Button("Reload Scripts"))
			{
#if _DEBUG
				std::string path = "./Assets/Plugin/all_in_one.py --configuration Debug";

#else
				std::string path = "./Assets/Plugin/all_in_one.py --configuration Release";
#endif
				RunScriptAsync(path, [&]()
					{
						SceneSerializer serializer(m_ActiveScene);
						std::filesystem::path savePath = std::filesystem::path(AssetManager::AssetsDirectory / "Scenes" / m_ActiveScene->GetName());
						savePath.replace_extension(".cine.temp");
						serializer.Serialize(savePath);

						Application::Get().SetUpdateUI(false);
						std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Right...
						m_ActiveScene->Clear();

						ScriptEngine::Get().UnloadLibrary();
						ScriptEngine::Get().LoadLibary("plugin.dll");
						ScriptEngine::Get().InitializeComponents(m_ActiveScene->GetRegistry());
						
						serializer.Deserialize(savePath);
						m_ActiveScene->SetUpdateScene(true);
						if (s_IsSceneRuntime)
						{
							m_ActiveScene->OnRuntimeStart();
							s_IsSceneRuntime = false;
						}
						Application::Get().SetUpdateUI(true);
					});
				return true;
			}
		}
		return false;
	}



	ContentBrowserPanel::ContentBrowserPanel(const std::filesystem::path& directory)
		: m_AssetsDirectory(directory), m_CurrentDirectory(m_AssetsDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (DisplayReloadScriptsButton())
		{
			m_ActiveScene->SetUpdateScene(false);
			if (m_ActiveScene->IsRuntime())
			{
				s_IsSceneRuntime = true;
				m_ActiveScene->OnRuntimeStop();
			}
		}

		float contentWidth = ImGui::GetContentRegionAvail().x;

		ImGui::SameLine(contentWidth - 120);
		if (ImGui::Button("Open Explorer"))
		{
			Shell::OpenExlorer(m_CurrentDirectory);
		}

		if (m_CurrentDirectory.filename() == "Scripts")
		{
			ImGui::NewLine();
			ImGui::SameLine(contentWidth - 120);
			if (ImGui::Button("Open Editor"))
			{
				Shell::OpenEditor(m_CurrentDirectory);
			}
		}

		if (m_CurrentDirectory != m_AssetsDirectory)
		{
			ImGui::NewLine();
			if (ImGui::Button("<"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		

		float padding = 8.0f;
		float thumbnailSize = 96;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount <= 0)
		{
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, m_AssetsDirectory);
			std::string filenameString = relativePath.filename().string();


			ImTextureID icon = directoryEntry.is_directory() ? (ImTextureID)m_DirectoryIcon->GetRendererID() : (ImTextureID)m_FileIcon->GetRendererID();
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
			ImGui::ImageButton(filenameString.c_str(), icon, { thumbnailSize , thumbnailSize }, { 0, 1 }, { 1, 0 });


			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(filenameString.c_str());



			ImGui::NextColumn();

		}

		ImGui::End();
	}

	void ContentBrowserPanel::SetContext(Ref<Scene> scene)
	{
		m_ActiveScene = scene;
	}

}
