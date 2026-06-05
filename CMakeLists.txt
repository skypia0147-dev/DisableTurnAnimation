cmake_minimum_required(VERSION 3.21)

set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "" FORCE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(DisableTurnAnimation)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# CommonLibSSE-NG — build from source to match the current MSVC toolset.
# This avoids the binary compatibility issue with pre-built vcpkg packages
# built against an older CRT (__std_regex_transform_primary_char mismatch).
# ---------------------------------------------------------------------------
include(FetchContent)

set(SKSE_SUPPORT_XBYAK OFF CACHE BOOL "" FORCE)

# Disable CommonLibSSE-NG's own tests to avoid Catch2 dependency
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  CommonLibSSE
  GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE-NG.git
  GIT_TAG        main
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(CommonLibSSE)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

add_library(${PROJECT_NAME} MODULE ${SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLibSSE::CommonLibSSE)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "DisableTurnAnimation"
    PREFIX ""
    SUFFIX ".dll"
)

# Post-build copy (adjust path as needed)
set(TEST_PLUGINS_DIR "H:/Game/Lorerim/overwrite/skse/plugins")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TEST_PLUGINS_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${TEST_PLUGINS_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
    COMMENT "Copying DLL to test environment..."
)
