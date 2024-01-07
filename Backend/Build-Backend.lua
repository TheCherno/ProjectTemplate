project "Backend"
   kind "SharedLib" -- Corrected from "Dynamic"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "./**.h", "./**.cpp" }

    links {
        "jsoncpp"
    }

    defines {
        "BACKEND_EXPORTS"
    }

    libdirs{
        "../Vendor/dlls"
    }

   includedirs
   {
      "./",
      "../Vendor/dlls"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/Stealify")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/Stealify")

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"

   filter { "system:windows", "configurations:Dist" }
       optimize "Full"

   filter { "system:windows", "configurations:Release or Dist", "kind:SharedLib" }
       linkoptions { "/OPT:REF" } -- link-time code optimization