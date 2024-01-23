cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("TestWorkConsole")

if (LINUX)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

include(GNUInstallDirs)
find_package(Armadillo REQUIRED)

add_executable(
    ${PROJECT_NAME}
    "src/AssemblyInfo.cpp" 
    "src/CmdCall.cpp" "src/CmdCall.h"
    "src/Learning.cpp" "src/learning.h"
    "src/Resource.h"
    "src/stdafx.cpp" "src/stdafx.h"
    "src/TestWorkConsole.cpp"
)

include_directories(${ARMADILLO_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} 
    PUBLIC ${ARMADILLO_LIBRARIES}
)

