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

include "Ethereal/Vendor/GLFW"
include "Ethereal/Vendor/GLAD"
include "Ethereal/Vendor/ImGUI"

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
		"%{includedir.imgui}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGUI",
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
		"%{includedir.glm}"
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
		