project "Frontend"
   kind "WindowedApp"
   language "C#"
   files { "*.cs", "*.xaml", "*.xaml.cs" }
   links { "System", "WindowsBase", "PresentationCore", "PresentationFramework", "System.Xaml", "System.Data", "Backend" }

   targetdir ("../Binaries/" .. OutputDir .. "/Stealify")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/Stealify")

   filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       optimize "On"
       symbols "Off"