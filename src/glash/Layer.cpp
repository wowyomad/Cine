#include "Layer.hpp"
#include "glash/Layer.hpp"

#include "ImGui/ImGuiLayer.hpp"
#include "glash/Applicatoin.hpp"


namespace glash
{
	Layer::Layer(const std::string& name)
		: m_Name(name)
	{
		
	}

	void Layer::OnAttach()
	{
		ImGuiContext* p_context = glash::Application::Get().GetImGuiContext();
		ImGuiContext* this_context = ImGui::GetCurrentContext();

		if (p_context) {
			ImGui::SetCurrentContext(p_context);
		}
	}

}