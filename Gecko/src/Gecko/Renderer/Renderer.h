#pragma once
#include "PerspectiveCameraController.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"

#include "Gecko/Core/Core.h"
#include <glm/glm.hpp>
namespace Gecko
{
    class Renderer
    {
    private:
    public:
        Renderer();
        ~Renderer();
        static void Clear(glm::vec4 color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        static void Init();

        static void BeginScene(PerspectiveCamera &camera); //TODO: remove
        static void BeginScene(Camera &camera, const glm::mat4 &transform);
        static void EndScene();

        static void Submit(const Ref<Shader> &shader, const glm::mat4 transform = glm::mat4(1.0f));

        //TODO: optimize renderer
        static void DrawModel(Ref<Model> &model, const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
        static void DrawMesh(Ref<Mesh>& mesh,const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation );
        
        static void DrawQuad(const glm::vec3 &position = glm::vec3(0.0f,0.0f,0.0f), const glm::vec3 &size = glm::vec3(1.0f,1.0f,1.0f), const glm::vec3 &rotation = glm::vec3(0.0f,0.0f,0.0f)){}
        static void DrawCube(Ref<Texture>& texture, const glm::vec3 &position = glm::vec3(0.0f,0.0f,0.0f), const glm::vec3 &scale = glm::vec3(1.0f,1.0f,1.0f), const glm::vec3 &rotation = glm::vec3(0.0f,0.0f,0.0f));
    };

} // namespace Gecko