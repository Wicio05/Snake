-- premake5.lua
workspace "Snake"
    architecture "arm64"
    configurations { "Debug", "Release" }

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Metal"] = "vendor/MetalSDK"

project "Snake"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    warnings "default"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    files {
        "src/**.h", "src/**.cpp" 
    }

    includedirs {
        "%{IncludeDir.Metal}"
    }

    links { 
        "Metal.framework",
        "MetalKit.framework",
        "Foundation.framework"
    }

    -- Define settings specifically for macOS
    filter "system:macosx"
        defines "NS_PRIVATE_IMPLEMENTATION"
        defines "MTL_PRIVATE_IMPLEMENTATION"
        defines "MTK_PRIVATE_IMPLEMENTATION"
        defines "CA_PRIVATE_IMPLEMENTATION"

    -- Use `buildoptions` to add the path to work around "Header Search Paths" in Xcode
    filter { "action:xcode*" }
        buildoptions { '-I"%{IncludeDir.Metal}"' }

    -- Reset the filter for other settings
    filter {}