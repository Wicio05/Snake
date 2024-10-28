//
//  Renderer.cpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "Renderer.hpp"

#include "Shader.hpp"
#include "Core.hpp"

#include <fstream>
#include <iostream>

#include <simd/simd.h>

struct InstanceData
{
    simd::float4x4 transform;
    simd::float4 color;
};

Renderer::Renderer(MTL::Device *device) : device(device->retain()), frame(0), angle(0)
{
    // create command queue
    commandQueue = device->newCommandQueue();

    buildShaders();
    buildBuffers();

    semaphore = dispatch_semaphore_create(MAX_FRAMES);
}

Renderer::~Renderer()
{
    // free memory
    shaderLibrary->release();

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        instanceDataBuffer[i]->release();
    }

    indexBuffer->release();
    vertexBuffer->release();

    pso->release();

    commandQueue->release();
    device->release();
}

void Renderer::buildShaders()
{
    // shader src
    std::vector<char> shaderSrc = Shader::readFile("assets/shaders/Shader.metal");

    NS::Error *error = nullptr;

    // create library from shader src
    shaderLibrary = device->newLibrary(string(shaderSrc.data()), nullptr, &error);

    // check for error
    if (!shaderLibrary)
    {
        std::cerr << error->localizedDescription()->utf8String();
        exit(EXIT_FAILURE);
    }

    // get vertex main function from shader src
    MTL::Function *vertFunc = shaderLibrary->newFunction(string("vertexMain"));

    // get fragment main function from shader src
    MTL::Function *fragFunc = shaderLibrary->newFunction(string("fragmentMain"));

    // init pipeline descriptor
    MTL::RenderPipelineDescriptor *rpd = MTL::RenderPipelineDescriptor::alloc()->init();

    // assign vertex and fragment functions
    rpd->setVertexFunction(vertFunc);
    rpd->setFragmentFunction(fragFunc);

    // set pixel format
    rpd->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    // create new pipeline state
    pso = device->newRenderPipelineState(rpd, &error);

    // check for errors
    if (!pso)
    {
        std::cerr <<  error->localizedDescription()->utf8String();
        exit(EXIT_FAILURE);
    }

    // free memory
    vertFunc->release();
    fragFunc->release();
    rpd->release();
}

void Renderer::buildBuffers()
{
    // constexpr size_t NUM_VERTICIES = 3;

    constexpr float V = 0.5f;

    simd::float3 verticies[] = 
    {
        { -V, -V, +V },
        { +V, -V, +V },
        { +V, +V, +V },
        { -V, +V, +V }
    };

    uint16_t indices[] = 
    {
        0, 1, 2,
        2, 3, 0,
    };

    constexpr size_t vertexDataSize = sizeof(verticies);
    constexpr size_t indexDataSize = sizeof(indices);

    vertexBuffer = device->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
    indexBuffer = device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    // upload values to the buffer on the device
    memcpy(vertexBuffer->contents(), verticies, vertexDataSize);
    memcpy(indexBuffer->contents(), indices, indexDataSize);

    // uploads a range from 0 to number of elements
    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    indexBuffer->didModifyRange(NS::Range::Make(0, indexBuffer->length()));

    constexpr size_t instanceDataSize = MAX_FRAMES * NUM_INSTANCES * sizeof(InstanceData);

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        instanceDataBuffer[i] = device->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged);
    }
}

void Renderer::draw(MTK::View *view)
{
    // init pool
    NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

    frame = (frame + 1) % MAX_FRAMES;

    // create command buffer
    MTL::CommandBuffer *cmd = commandQueue->commandBuffer();

    dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
    
    cmd->addCompletedHandler(^void(MTL::CommandBuffer* cmd)
    {
        dispatch_semaphore_signal(semaphore);
    });

    // angle += 0.01f;
    
    constexpr float scale = 1.0f;
    InstanceData* instanceData = reinterpret_cast<InstanceData*>(instanceDataBuffer[frame]->contents());

    for (size_t i = 0; i < NUM_INSTANCES; ++i)
    {
        float xoff = 0.0f;
        float yoff = 0.0f;

        simd::float4x4 transform = (simd::float4x4)
        { 
            (simd::float4) { scale * sinf(angle), scale * cosf(angle), 0.0f, 0.f },
            (simd::float4) { scale * cosf(angle), scale * -sinf(angle), 0.0f, 0.0f },
            (simd::float4) { 0.0f, 0.0f, scale, 0.0f },
            (simd::float4) { xoff, yoff, 0.0f, 1.0f } 
        };

        instanceData[i].transform = transform;
        instanceData[i].color = Color::GREEN;
    }

    instanceDataBuffer[frame]->didModifyRange(NS::Range::Make(0, instanceDataBuffer[frame]->length()));


    // create render pass descriptor
    MTL::RenderPassDescriptor *rpd = view->currentRenderPassDescriptor();

    // create encoder
    MTL::RenderCommandEncoder *encoder = cmd->renderCommandEncoder(rpd);

    // set render pipeline state
    encoder->setRenderPipelineState(pso);

    encoder->setVertexBuffer(vertexBuffer, /* offset */ 0, /* index */ 0);
    encoder->setVertexBuffer(instanceDataBuffer[frame], /* offset */ 0, /* index */ 1);

    //
    // void drawIndexedPrimitives( PrimitiveType primitiveType, NS::UInteger indexCount, IndexType indexType,
    //                             const class Buffer* pIndexBuffer, NS::UInteger indexBufferOffset, NS::UInteger instanceCount );
    encoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, 6, MTL::IndexType::IndexTypeUInt16,
                                indexBuffer, 0, NUM_INSTANCES );

    // end encoding
    encoder->endEncoding();
    cmd->presentDrawable(view->currentDrawable());

    // commit command buffer
    cmd->commit();

    // relese pool
    pool->release();
}
