-- premake5.lua
workspace "New Project"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "../../output/bin/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}"
ObjDir = "../../output/obj/%{cfg.architecture}/%{cfg.system}/%{cfg.buildcfg}"
CoresDir = "../../cores/"
AppsDir = "../../apps/"

group "Cores"
	include "cores/Core/Build-Core.lua"

group "Apps"
   include "apps/App/Build-App.lua"

group "Dependencies"