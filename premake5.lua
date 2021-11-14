workspace "ArtCode"
	architecture "x64"

	configurations
	{
		"Debug"
	}

outdir = "%{cfg.buildcfg}-%{cfg.architecture}"

includedir = {}
includedir["glfw"] = "Ethereal/Vendor/GLFW/include"
includedir["glad"] = "Ethereal/Vendor/GLAD/include"
includedir["imgui"] = "Ethereal/Vendor/ImGUI"
includedir["glm"] = "Ethereal/Vendor/GLM"
includedir["stb"] = "Ethereal/Vendor/STB"
includedir["entt"] = "Ethereal/Vendor/ENTT/include"
includedir["yaml"] = "Ethereal/Vendor/YAML/include"
includedir["guizmo"] = "Ethereal/Vendor/ImGUIZMO"

include "Ethereal/Vendor/GLFW"
include "Ethereal/Vendor/GLAD"
include "Ethereal/Vendor/ImGUI"
include "Ethereal/Vendor/YAML"

project "Ethereal"
	location "Ethereal"
	kind "StaticLib"
	language "C++"

	targetdir ("build/" .. outdir .. "/%{prj.name}")
	objdir ("build-int/" .. outdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/**.h",
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/Vendor/GLM/glm/**.hpp",
		"%{prj.name}/Vendor/GLM/glm/**.inl",
		"%{prj.name}/Vendor/STB/**.h",
		"%{prj.name}/Vendor/STB/**.cpp",
		"%{prj.name}/Vendor/ImGUIZMO/ImGuizmo.h",
		"%{prj.name}/Vendor/ImGUIZMO/ImGuizmo.cpp"
	}

	removefiles
	{
		"%{prj.name}/Vendor/GLM/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/Source",
		"%{includedir.glfw}",
		"%{includedir.glad}",
		"%{includedir.glm}",
		"%{includedir.imgui}",
		"%{includedir.stb}",
		"%{includedir.entt}",
		"%{includedir.yaml}",
		"%{includedir.guizmo}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGUI",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
project "ArtCode"
	location "ArtCode"
	kind "ConsoleApp"
	language "C++"

	targetdir ("build/" .. outdir .. "/%{prj.name}")
	objdir ("build-int/" .. outdir .. "/%{prj.name}")

	files 
	{	
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}

	includedirs
	{
		"Ethereal",
		"Ethereal/Source",
		"%{prj.name}/Source",
		"%{includedir.glm}",
		"%{includedir.imgui}",
		"%{includedir.entt}"
	}

	links
	{
		"Ethereal"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		staticruntime "Off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

include "Editor"