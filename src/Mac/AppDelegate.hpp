//
//  AppDelegate.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "ViewDelegate.hpp"

class AppDelegate : public NS::ApplicationDelegate
{
    public:
        ~AppDelegate();

        NS::Menu* createMenuBar();

        virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
        virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
        virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

    private:
        NS::Window* window;
        MTK::View* view;
        MTL::Device* device;
        ViewDelegate* viewDelegate = nullptr;
};
