include(${CMAKE_SOURCE_DIR}/cmake/helper.cmake)

set(TestTargets 
test01_lexer
test02_parser
test03_layouter
test04_boxy
test05_renderer
test06_property_parser
)

foreach(tgt ${TestTargets})
    add_executable(${tgt} ${tgt}.cpp)
    target_link_libraries(${tgt} PRIVATE CppSyntaxTreeLib)
    output_build_path(${tgt})
    add_test(NAME "unit-${tgt}" COMMAND ${tgt})
endforeach()

# add_subdirectory(tool)
