//
//  main.cpp
//  Snake
//
//  Created by Witek Cybulski on 26/10/2024.
//

// metal headers
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "Mac/AppDelegate.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();

    AppDelegate del;

    NS::Application* sharedApplication = NS::Application::sharedApplication();
    sharedApplication->setDelegate(&del);
    sharedApplication->run();

    autoreleasePool->release();

    return 0;
}
