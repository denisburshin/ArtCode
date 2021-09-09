project "Editor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/build/" .. outdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build-int/" .. outdir .. "/%{prj.name}")

	files 
	{	
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs
	{
		"Source",
		"%{wks.location}/Ethereal",
		"%{wks.location}/Ethereal/Source",
		"%{wks.location}/%{includedir.glm}",
		"%{wks.location}/%{includedir.imgui}"
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