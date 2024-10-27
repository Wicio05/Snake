//
//  AppDelegate.cpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "AppDelegate.hpp"

#include "Core.hpp"

AppDelegate::~AppDelegate()
{
    view->release();
    window->release();
    device->release();
    delete viewDelegate;
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    CGRect frame = (CGRect){ {100.0, 100.0}, {640.0, 480.0} };

    uint64_t windowStyleMask = NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled | NS::WindowStyleMaskMiniaturizable | NS::WindowStyleMaskResizable;

    window = NS::Window::alloc()->init(frame, windowStyleMask, NS::BackingStoreBuffered, false);

    device = MTL::CreateSystemDefaultDevice();

    view = MTK::View::alloc()->init(frame, device);
    view->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    view->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 0.6, 1.0));

    viewDelegate = new ViewDelegate(device);
    view->setDelegate(viewDelegate);

    window->setContentView(view);
    window->setTitle(string("Window"));
    window->makeKeyAndOrderFront(nullptr);

    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender)
{
    return true;
}
