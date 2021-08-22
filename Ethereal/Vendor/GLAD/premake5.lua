project "GLAD"
	kind "StaticLib"
	language "C"

	targetdir ("build/" .. outdir .. "/%{prj.name}")
	objdir ("buil-int/" .. outdir .. "/%{prj.name}")

	files
	{
		"include/glad/gl.h",
		"include/KHR/khrplatform.h",
		"src/gl.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "Off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"