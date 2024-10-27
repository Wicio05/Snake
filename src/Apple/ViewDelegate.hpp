//
//  ViewDelegate.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Renderer.hpp"

class ViewDelegate : public MTK::ViewDelegate
{
    public:
        ViewDelegate(MTL::Device* device);
        virtual ~ViewDelegate() override;
        virtual void drawInMTKView(MTK::View* view) override;

    private:
        Renderer renderer;
};
