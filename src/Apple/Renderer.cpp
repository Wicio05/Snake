//
//  Renderer.cpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "Renderer.hpp"

#include <fstream>

#include <simd/simd.h>

Renderer::Renderer(MTL::Device* device) : device(device->retain())
{
    // create command queue
    commandQueue = device->newCommandQueue();

    buildShaders();
    buildBuffers();
}

Renderer::~Renderer()
{
    // free memory
    colorsBuffer->release();
    vertexBuffer->release();
    pso->release();

    commandQueue->release();
    device->release();
}

void Renderer::buildShaders()
{
    using NS::StringEncoding::UTF8StringEncoding;

    // shader src
    const char* shaderSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };

        v2f vertex vertexMain( uint vertexId [[vertex_id]],
                               device const float3* positions [[buffer(0)]],
                               device const float3* colors [[buffer(1)]] )
        {
            v2f o;
            o.position = float4( positions[ vertexId ], 1.0 );
            o.color = half3 ( colors[ vertexId ] );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";

    NS::Error* error = nullptr;
    
    // create library from shader src
    MTL::Library* library = device->newLibrary(NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &error);
    
    // check for error
    if (!library)
    {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert( false );
    }

    // get vertex main function from shader src
    MTL::Function* vertFunc = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    
    // get fragment main function from shader src
    MTL::Function* fragFunc = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    // init pipeline descriptor
    MTL::RenderPipelineDescriptor* rpd = MTL::RenderPipelineDescriptor::alloc()->init();
    
    // assign vertex and fragment functions
    rpd->setVertexFunction(vertFunc);
    rpd->setFragmentFunction(fragFunc);
    
    // set pixel format?
    rpd->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    // upload new pipeline state?
    pso = device->newRenderPipelineState(rpd, &error);
    
    // check for errors
    if (!pso)
    {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert( false );
    }

    // free memory
    vertFunc->release();
    fragFunc->release();
    rpd->release();
    library->release();
}

void Renderer::buildBuffers()
{
    const size_t NumVertices = 3;

    simd::float3 positions[NumVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };

    simd::float3 colors[NumVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
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

    // uploads a range from 0 to number of elements? (idk)
    vertexBuffer->didModifyRange(NS::Range::Make( 0, vertexBuffer->length()));
    colorsBuffer->didModifyRange(NS::Range::Make( 0, colorsBuffer->length()));
}

void Renderer::draw(MTK::View* view)
{
    // init pool
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    // create command buffer
    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();

    // create render pass descriptor
    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();

    // create encoder
    MTL::RenderCommandEncoder* encoder = cmd->renderCommandEncoder(rpd);

    // set render pipeline state
    encoder->setRenderPipelineState(pso);
    
    // upload verticies and colors
    encoder->setVertexBuffer(vertexBuffer, 0, 0);
    encoder->setVertexBuffer(colorsBuffer, 0, 1);
    
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
