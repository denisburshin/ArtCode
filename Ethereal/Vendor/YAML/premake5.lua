project "yaml-cpp"
    kind "StaticLib"
    language "C++"

    targetdir ("build/" .. outdir .. "/%{prj.name}")
	objdir ("build-int/" .. outdir .. "/%{prj.name}")

    files
	{
		"src/**.h",
		"src/**.cpp",

		"include/**.h"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "Off"

    filter "configurations:Debug"
	runtime "Debug"
	symbols "on"
