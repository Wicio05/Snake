//
//  Renderer.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "config.hpp"

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

        MTL::RenderPipelineState* pso;
        MTL::Buffer* vertexBuffer;
        MTL::Buffer* colorsBuffer;
};
