workspace "GooE"
	language "C++"
	cppdialect "c++17"
	startproject "Editoor"
	configurations {"Debug", "Release", "Dist"}
	platforms {"Win64"}

	filter "configurations:Debug"
-- For profiling add GOOE_PROFILE=1
		defines {"DEBUG", "GOOE_ENABLE_ASSERTS"}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines {"RELEASE", "GOOE_ENABLE_ASSERTS"}
		optimize "on"
		symbols "on"
		runtime "Release"
	
	filter "configurations:DIST"
		defines {"DIST"}
		optimize "on"
		symbols "off"
		runtime "Release"

	filter "platforms:Win64"
		defines {"WIN_64"}
		system "windows"
		architecture "x86_64"
		systemversion "latest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
vendorInclude = {}
vendorInclude["GLFW"] = "%{wks.location}/GooE/vendor/glfw/include"
vendorInclude["glad"] = "%{wks.location}/GooE/vendor/glad/include"
vendorInclude["ImGui"] = "%{wks.location}/GooE/vendor/imgui"
vendorInclude["glm"] = "%{wks.location}/GooE/vendor/glm"
vendorInclude["stb"] = "%{wks.location}/GooE/vendor/stb_image"
vendorInclude["entt"] = "%{wks.location}/GooE/vendor/entt/single_include"
vendorInclude["yamlcpp"] = "%{wks.location}/GooE/vendor/yaml-cpp/include"
vendorInclude["imguizmo"] = "%{wks.location}/GooE/vendor/ImGuizmo"

include "GooE"
include "Editoor"
include "Sandbox"
include "GooE/vendor/glad"

project "GLFW"
	location "GooE/vendor/glfw"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/include/GLFW/glfw3.h",
		"%{prj.location}/include/GLFW/glfw3native.h",
		"%{prj.location}/src/glfw_config.h",
		"%{prj.location}/src/context.c",
		"%{prj.location}/src/init.c",
		"%{prj.location}/src/input.c",
		"%{prj.location}/src/monitor.c",
		"%{prj.location}/src/vulkan.c",
		"%{prj.location}/src/window.c"
	}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		
		files
		{
			"%{prj.location}/src/x11_init.c",
			"%{prj.location}/src/x11_monitor.c",
			"%{prj.location}/src/x11_window.c",
			"%{prj.location}/src/xkb_unicode.c",
			"%{prj.location}/src/posix_time.c",
			"%{prj.location}/src/posix_thread.c",
			"%{prj.location}/src/glx_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c",
			"%{prj.location}/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		files
		{
			"%{prj.location}/src/win32_init.c",
			"%{prj.location}/src/win32_joystick.c",
			"%{prj.location}/src/win32_monitor.c",
			"%{prj.location}/src/win32_time.c",
			"%{prj.location}/src/win32_thread.c",
			"%{prj.location}/src/win32_window.c",
			"%{prj.location}/src/wgl_context.c",
			"%{prj.location}/src/egl_context.c",
			"%{prj.location}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

project "ImGui"
	location "GooE/vendor/imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	files
	{
		"%{prj.location}/imconfig.h",
		"%{prj.location}/imgui.h",
		"%{prj.location}/imgui.cpp",
		"%{prj.location}/imgui_draw.cpp",
		"%{prj.location}/imgui_internal.h",
		"%{prj.location}/imgui_tables.cpp",
		"%{prj.location}/imgui_widgets.cpp",
		"%{prj.location}/imstb_rectpack.h",
		"%{prj.location}/imstb_textedit.h",
		"%{prj.location}/imstb_truetype.h",
		"%{prj.location}/imgui_demo.cpp",
		"%{prj.location}/backends/imgui_impl_opengl3.h",
		"%{prj.location}/backends/imgui_impl_opengl3.cpp",
		"%{prj.location}/backends/imgui_impl_glfw.h",
		"%{prj.location}/backends/imgui_impl_glfw.cpp"
	}

	includedirs {
		"%{prj.location}",
		"%{vendorInclude.glad}",
		"%{vendorInclude.GLFW}",
	}

	filter "system:linux"
		pic "On"

project "yaml-cpp"
	location "GooE/vendor/yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/out/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/out/intermediates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.location}/src/**.h",
		"%{prj.location}/src/**.cpp",
		
		"%{prj.location}/include/**.h"
	}

	includedirs
	{
		"%{prj.location}/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"