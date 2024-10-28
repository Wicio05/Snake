-- premake5.lua
workspace "Snake"
    architecture "arm64"
    configurations { "Debug", "Release" }

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Metal"] = "vendor/MetalSDK"
IncludeDir["GLM"] = "vendor/glm"

project "Snake"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    warnings "default"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    files {
        "src/**.h", "src/**.cpp", "src/**.hpp"
    }

    includedirs {
        "%{IncludeDir.Metal}",
        "%{IncludeDir.GLM}",
    }

    links { 
        "Metal.framework",
        "MetalKit.framework",
        "Foundation.framework"
    }

    -- Use `buildoptions` to add the path to work around "Header Search Paths" in Xcode
    filter { "action:xcode*" }
        buildoptions { '-I"%{IncludeDir.Metal}" -I"%{IncludeDir.GLM}"' }

    -- Reset the filter for other settings
    filter {}