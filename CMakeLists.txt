# CMakeList.txt : CMake project for Raytracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Raytracer")


set(CMAKE_CXX_FLAGS "/utf-8")
set(CMAKE_C_FLAGS "/utf-8")


# Add source to this project's executable.
add_executable (Raytracer "Raytracer.cpp" "Raytracer.h" "vec3.h" "ray.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Raytracer PROPERTY CXX_STANDARD 20)
endif()

# Source - https://stackoverflow.com/a/47704050
# Posted by Florian, modified by community. See post 'Timeline' for change history
# Retrieved 2026-09-30, License - CC BY-SA 3.0



# TODO: Add tests and install targets if needed.
