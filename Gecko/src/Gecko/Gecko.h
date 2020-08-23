#pragma once
#include "gkpch.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "Gecko/ImGui/ImGuiFilebrowser.h"

#include "Gecko/Core/KeyCodes.h"
#include "Gecko/Core/Window.h"
#include "Gecko/Core/Input.h"
#include "Gecko/Core/Application.h"
#include "Gecko/Core/KeyCodes.h"
#include "Gecko/Core/DeltaTime.h"

#include "Gecko/Renderer/Shader.h"
#include "Gecko/Renderer/Texture.h"
#include "Gecko/Renderer/PerspectiveCamera.h"
#include "Gecko/Renderer/PerspectiveCameraController.h"
#include "Gecko/Renderer/Model.h"
#include "Gecko/Renderer/FrameBuffer.h"
#include "Gecko/Renderer/Skybox.h"
#include "Gecko/Renderer/Renderer.h"

#include "Gecko/Scene/Scene.h"
#include "Gecko/Scene/Entity.h"
#include "Gecko/Scene/EntityBehaviour.h"
#include "Gecko/Scene/Components.h"

#ifdef GK_INCLUDE_ENTRYPOINT
#include "Gecko/Core/Entrypoint.h"
#endif
using namespace Gecko;