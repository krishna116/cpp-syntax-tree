include(${CMAKE_SOURCE_DIR}/cmake/helper.cmake)

add_library(CppSyntaxTreeLib
    BaseType.cpp
    SyntaxTree.cpp
    Lexer.cpp
    Parser.cpp
    PropertyParser.cpp
    Layouter.cpp
    CairoContext.cpp
    Boxy.cpp
    Renderer.cpp
)

if(MSVC)
    find_library(CairoLib cairo REQUIRED)
    target_link_libraries(CppSyntaxTreeLib PRIVATE ${CairoLib})
    find_path(CairoIncludeDir cairo/cairo.h REQUIRED)
    target_include_directories(CppSyntaxTreeLib PRIVATE ${CairoIncludeDir} ${CairoIncludeDir}/cairo)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CairoLib REQUIRED cairo)
    target_link_libraries(CppSyntaxTreeLib PRIVATE ${CairoLib_LIBRARIES})
endif()

target_include_directories(CppSyntaxTreeLib
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
PRIVATE
    ${CairoLib_INCLUDE_DIRS}
)

##-------------------------------------------------------------------
add_executable(${PROJECT_NAME} app.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE CppSyntaxTreeLib)
output_build_path(${PROJECT_NAME})

##-------------------------------------------------------------------
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
