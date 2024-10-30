//
//  ViewDelegate.cpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "ViewDelegate.hpp"

ViewDelegate::ViewDelegate(MTL::Device* device) : MTK::ViewDelegate(), renderer(device)
{
}

ViewDelegate::~ViewDelegate()
{

}

void ViewDelegate::drawInMTKView(MTK::View* view)
{
    renderer.beginScene();
    renderer.draw();
    renderer.endScene(view);
}
