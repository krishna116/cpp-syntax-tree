function(output_build_path Target)
    add_custom_command(TARGET ${Target}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:${Target}>"
    )
endfunction()