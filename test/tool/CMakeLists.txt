set(ListOfTool
ctpp
mktable
lddexport
)

## build -------------------------------------------------------
foreach(tool ${ListOfTool})
    add_executable(${tool} ${tool}.cpp)
    output_build_path(${tool})
endforeach()

target_compile_features(lddexport PUBLIC cxx_std_17)

## install -----------------------------------------------------
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
install(TARGETS ${ListOfTool}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

