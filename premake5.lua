workspace "Workspace"
    architecture "x64"
    configurations { "Debug", "Release" }
    libdirs {"external/lib"}
    includedirs {"external/include"}

    OutputDir = "output/bin/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}"
    ObjDir = "output/obj/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}"


project "library"
    kind "SharedLib"
    language "C++"
    cppdialect "c++20"
    buildoptions {"-fPIC"}
    targetdir ( OutputDir .. "/")
    objdir (ObjDir .. "/%{prj.name}")
    
    files { "src/library/include/**.h", "src/library/src/**.cpp" }

    includedirs { "src/library/include" }

    --links { "spdlog" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        debugger "GDB"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "app"
    kind "ConsoleApp"
    language "C++"
    targetdir ( OutputDir .. "/")
    objdir (ObjDir .. "/%{prj.name}")
    
    files { "src/app/include/**.h", "src/app/src/**.cpp" }

    includedirs { "src/library/include", "src/app/include" }

    --links { "qfengine", "spdlog", "raylib", "rlImGui" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        debugger "GDB"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"