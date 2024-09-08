workspace "Platformer"
   configurations { "Release" }
   location "build"

project "platformer"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "main.cpp", "app.cpp", "component.cpp", "tex_manager.cpp", "entityManager.cpp", "system.cpp", "map.cpp" }
 

   links { "SDL2", "SDL2_image" }

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
