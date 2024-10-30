//
//  Renderer.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Config.hpp"
#include "../Core.hpp"

#include <glm/glm.hpp>
#include <simd/simd.h>

//  TODO:
//      make begin and end scene to make draw method not independent 
//      make draw snake method 
//      make draw apple method

class Renderer
{
    public:
        Renderer(MTL::Device* device);
        ~Renderer();

        void buildShaders();
        void buildBuffers();

        void beginScene();
        void endScene(MTK::View *view);

        void draw();

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

        MTL::CommandBuffer* commandBuffer;
};
