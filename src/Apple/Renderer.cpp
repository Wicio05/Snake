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

Renderer::Renderer(MTL::Device *device) : device(device->retain())
{
    // create command queue
    commandQueue = device->newCommandQueue();

    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    // free memory
    shaderLibrary->release();

    argBuffer->release();
    colorsBuffer->release();
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
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        {-0.8f, 0.8f, 0.0f},
        {0.0f, -0.8f, 0.0f},
        {+0.8f, 0.8f, 0.0f}
    };

    simd::float3 colors[NumVertices] =
    {
        {1.0, 0.3f, 0.2f},
        {0.8f, 1.0, 0.0f},
        {0.8f, 0.0f, 1.0}
    };

    const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
    const size_t colorDataSize = NumVertices * sizeof(simd::float3);

    // allocate enough memory to store position array on the device
    vertexBuffer = device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);

    // allocate enough memory to store colors array on the device
    colorsBuffer = device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

    // upload values to the buffer on the device
    memcpy(vertexBuffer->contents(), positions, positionsDataSize);
    memcpy(colorsBuffer->contents(), colors, colorDataSize);

    // uploads a range from 0 to number of elements
    vertexBuffer->didModifyRange(NS::Range::Make(0, vertexBuffer->length()));
    colorsBuffer->didModifyRange(NS::Range::Make(0, colorsBuffer->length()));

    if(!shaderLibrary)
    {
        std::cerr << "Shader library was not initialized!\n";
        exit(EXIT_FAILURE);
    }

    MTL::Function* pVertexFn = shaderLibrary->newFunction(string("vertexMain"));

    // encode VertexData
    MTL::ArgumentEncoder* argEncoder = pVertexFn->newArgumentEncoder(0);

    // get vertexData buffer
    argBuffer = device->newBuffer(argEncoder->encodedLength(), MTL::ResourceStorageModeManaged);

    
    argEncoder->setArgumentBuffer(argBuffer, 0);

    argEncoder->setBuffer(vertexBuffer, 0, 0);
    argEncoder->setBuffer(colorsBuffer, 0, 1);

    argBuffer->didModifyRange(NS::Range::Make(0, argBuffer->length()));

    pVertexFn->release();
    argEncoder->release();
}

void Renderer::draw(MTK::View *view)
{
    // init pool
    NS::AutoreleasePool *pool = NS::AutoreleasePool::alloc()->init();

    // create command buffer
    MTL::CommandBuffer *cmd = commandQueue->commandBuffer();

    // create render pass descriptor
    MTL::RenderPassDescriptor *rpd = view->currentRenderPassDescriptor();

    // create encoder
    MTL::RenderCommandEncoder *encoder = cmd->renderCommandEncoder(rpd);

    // set render pipeline state
    encoder->setRenderPipelineState(pso);

    // upload verticies and colors
    encoder->setVertexBuffer(argBuffer, 0, 0);
    encoder->useResource(vertexBuffer, MTL::ResourceUsageRead);
    encoder->useResource(colorsBuffer, MTL::ResourceUsageRead);

    // draw triangle filled
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));

    // end encoding
    encoder->endEncoding();
    cmd->presentDrawable(view->currentDrawable());

    // commit command buffer
    cmd->commit();

    // relese pool
    pool->release();
}
