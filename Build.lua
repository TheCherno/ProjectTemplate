-- premake5.lua
workspace "Stealify"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Frontend"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Backend"
	include "Backend/Build-Backend.lua"
group ""

include "Frontend/Build-Frontend.lua"