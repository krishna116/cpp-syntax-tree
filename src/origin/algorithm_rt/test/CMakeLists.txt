set(TestTargets 
    test1
)

foreach(tgt ${TestTargets})
    add_executable(${tgt} ${tgt}.cpp)
    target_link_libraries(${tgt} PRIVATE rt)
endforeach()

