cmake_minimum_required (VERSION 3.15)

project ("Scoreboard"
	VERSION 1.0
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "D:/Games/Grand Theft Auto San Andreas Samp RP")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "D:/Games/Grand Theft Auto San Andreas Samp RP")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(IMGUI_EXAMPLES OFF)
set(IMGUI_DEMO OFF)
set(IMGUI_IMPL_SDL OFF)
set(IMGUI_IMPL_GLFW OFF)
set(IMGUI_IMPL_GLUT OFF)
set(IMGUI_IMPL_OPENGL OFF)
set(IMGUI_IMPL_OPENGL2 OFF)
set(IMGUI_IMPL_DX9 ON)
set(IMGUI_IMPL_DX10 OFF)
set(IMGUI_IMPL_DX11 OFF)
set(IMGUI_IMPL_DX12 OFF)

include(FetchContent)

FetchContent_Declare(
	sampapi
	GIT_REPOSITORY https://github.com/BlastHackNet/SAMP-API.git
	GIT_TAG f89b35edb7eaf5db374e32a63f2366c4bb714418
)
FetchContent_Declare(
	ktsignal
	GIT_REPOSITORY https://github.com/kin4stat/ktsignal.git
	GIT_TAG f08a19d9a00ad5284871bf44178d835da7b26ebf
)
FetchContent_Declare(
	kthook
	GIT_REPOSITORY https://github.com/kin4stat/kthook.git
	GIT_TAG 21ca70481273d1efcc98ca49063178e46277009b
)
FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/Qix-/imgui.git
	GIT_TAG 6c3b86dd78bac7ac7ce045b10825153f27603229
)
FetchContent_Declare(
	rakhook
	GIT_REPOSITORY https://github.com/imring/RakHook.git
	GIT_TAG 625fd17897395997feb56ae293c7c07dbe49d26b
)
FetchContent_MakeAvailable(sampapi kthook ktsignal rakhook imgui)

add_library(${PROJECT_NAME} SHARED)
target_link_libraries(${PROJECT_NAME} PUBLIC sampapi kthook rakhook imgui imgui-win32 imgui-dx9 d3d9)
target_sources(${PROJECT_NAME} PRIVATE
	src/main.cpp
	src/Plugin.cpp
	src/PluginRender.cpp
	src/PluginGUI.cpp
	src/PluginRPC.cpp
	src/PlayerList.cpp
)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	OUTPUT_NAME ${PROJECT_NAME}
	SUFFIX ".asi"
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")