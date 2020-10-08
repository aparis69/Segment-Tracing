solution "SegmentTracing"
	configurations { "Debug", "Release" }

	platforms { "x64" }
	
	includedirs { ".", "../Code/Include", "/usr/include/" }

	rootDir = path.getabsolute("../")
	
	configuration "Debug"
		targetdir "./Out/Debug"
		defines { "DEBUG" }
		flags { "Symbols" }

	configuration "Release"
		targetdir "./Out/Release"
		flags { "OptimizeSpeed" }

	configuration "linux"
		buildoptions { "-mtune=native -march=native" }
		buildoptions { "-std=c++14" }
		buildoptions { "-w" }
		buildoptions { "-flto -g"}
		linkoptions { "-fopenmp"}
		linkoptions { "-flto"}
		linkoptions { "-g"}

fileList = { rootDir .. "/Code/Source/*.cpp", rootDir .. "/Code/Include/*.h" }

project("SegmentTracing")
	language "C++"
	kind "ConsoleApp"
	targetdir "Out"
files ( fileList )
