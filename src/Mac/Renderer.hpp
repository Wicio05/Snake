//
//  Renderer.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Config.hpp"

class Renderer
{
    public:
        Renderer(MTL::Device* device);
        ~Renderer();

        void buildShaders();
        void buildBuffers();

        void draw(MTK::View* view);

    private:
        MTL::Device* device;
        MTL::CommandQueue* commandQueue;

        MTL::Library* shaderLibrary;

        MTL::RenderPipelineState* pso;
        MTL::Buffer* argBuffer;
        MTL::Buffer* vertexBuffer;
        MTL::Buffer* colorsBuffer;

        MTL::Buffer* frameData[3];
        float angle;
        int frame;

        dispatch_semaphore_t semaphore;
        static constexpr int maxFrames = 3;
};
