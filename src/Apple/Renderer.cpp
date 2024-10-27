//
//  Renderer.cpp
//  metalTutorial
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "Renderer.hpp"

Renderer::Renderer(MTL::Device* device) : device(device->retain())
{
    commandQueue = device->newCommandQueue();
}

Renderer::~Renderer()
{
    commandQueue->release();
    device->release();
}

void Renderer::draw(MTK::View* view)
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* pRpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* encoder = cmd->renderCommandEncoder(pRpd);
    encoder->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
