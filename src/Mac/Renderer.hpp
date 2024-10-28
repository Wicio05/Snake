//
//  Renderer.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Config.hpp"

#include <simd/simd.h>

struct Color
{
    static constexpr simd::float4 BLACK =   (simd::float4) { 0.0f, 0.0f, 0.0f, 1.0f };
    static constexpr simd::float4 WHITE =   (simd::float4) { 1.0f, 1.0f, 1.0f, 1.0f };
    static constexpr simd::float4 RED =     (simd::float4) { 1.0f, 0.0f, 0.0f, 1.0f };
    static constexpr simd::float4 GREEN =   (simd::float4) { 0.0f, 1.0f, 0.0f, 1.0f };
    static constexpr simd::float4 BLUE =    (simd::float4) { 0.0f, 0.0f, 1.0f, 1.0f };
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
        static constexpr size_t NUM_INSTANCES = 1;

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
