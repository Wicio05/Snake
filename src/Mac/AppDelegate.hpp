//
//  AppDelegate.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

// #include <dispatch/dispatch.h>

#include "ViewDelegate.hpp"

#include <iostream>

class AppDelegate : public NS::ApplicationDelegate
{
    public:
        ~AppDelegate();

        NS::Menu* createMenuBar();
    
        void update();

        virtual void applicationWillFinishLaunching(NS::Notification* notification) override;
        virtual void applicationDidFinishLaunching(NS::Notification* notification) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender) override;


    private:
        dispatch_source_t timer;
        int frame = 0;

        NS::Window* window;
        MTK::View* view;
        MTL::Device* device;
        ViewDelegate* viewDelegate = nullptr;
};
