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
    // Clean up the timer when application is closing
    if (timer) 
    {
        dispatch_source_cancel(timer);
        dispatch_release(timer);
        timer = nullptr;
    }
    
    view->release();
    window->release();
    device->release();
    delete viewDelegate;
}

NS::Menu * AppDelegate::createMenuBar()
{
    return nullptr;
}

void AppDelegate::update()
{
    frame = (frame + 1) % 60;
    std::cout << "Update func " << frame << '\n';
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    NS::Application* app = reinterpret_cast<NS::Application*>(notification->object());
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    CGRect frame = (CGRect){ {125.0, 125.0}, {800.0, 800.0} };

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
    
    // timer
    timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    if (timer) 
    {
        dispatch_source_set_timer(timer, dispatch_time(DISPATCH_TIME_NOW, 0), NSEC_PER_SEC / 60,  // 60 times per second
                                  (1ull * NSEC_PER_SEC) / 120); // leeway

        dispatch_source_set_event_handler(timer, 
        ^{
            this->update();
        });

        dispatch_resume(timer);
    }
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application* sender)
{
    return true;
}
