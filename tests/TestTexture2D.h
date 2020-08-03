#pragma once

#include "Test.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace test
{

    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        VertexArray vertexArray_;
        IndexBuffer indexBuffer_;
        VertexBuffer vertexBuffer_;
        Shader shader_;
        Texture texture_;

        glm::mat4 proj_;
        glm::mat4 view_;

        glm::vec3 translationA_;
        glm::vec3 translationB_;
    };

}
