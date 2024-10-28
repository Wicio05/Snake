//
//  Renderer.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Config.hpp"

#include <glm/glm.hpp>
#include <simd/simd.h>

struct Color
{
    static constexpr glm::vec4 BLACK =          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    static constexpr glm::vec4 WHITE =          glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    static constexpr glm::vec4 RED =            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    static constexpr glm::vec4 GREEN =          glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    static constexpr glm::vec4 BLUE =           glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    static constexpr glm::vec4 LIGHT_GREEN =    glm::vec4(167.0f / 255.0f, 216.0f / 255.0f, 73.0f / 255.0f, 1.0f);
    static constexpr glm::vec4 DARK_GREEN =     glm::vec4(117.0f / 255.0f, 166.0f / 255.0f, 47.0f / 255.0f, 1.0f);
};

class Renderer
{
    public:
        Renderer(MTL::Device* device);
        ~Renderer();

        void buildShaders();
        void buildBuffers();

        void draw(MTK::View* view);

    private:
        static constexpr int MAX_FRAMES = 3;
        static constexpr size_t GRID_SIZE = 25;

    private:
        MTL::Device* device;
        MTL::CommandQueue* commandQueue;

        MTL::Library* shaderLibrary;

        MTL::RenderPipelineState* pso;
        
        MTL::Buffer* vertexBuffer;
        MTL::Buffer* indexBuffer;
        MTL::Buffer* instanceDataBuffer[MAX_FRAMES];

        float angle;
        int frame;

        dispatch_semaphore_t semaphore;
};
