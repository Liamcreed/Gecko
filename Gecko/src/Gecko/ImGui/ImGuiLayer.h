#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Gecko/Core/Layer.h"

namespace Gecko
{

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
	void ApplyDefaultStyle();

} // namespace Gecko