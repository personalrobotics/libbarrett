set(programs
	gravitycal
	zerocal
	autotension
)

execute_process(COMMAND xeno-config --skin=alchemy --cflags
                   RESULT_VARIABLE XENOMAI_NOTFOUND
                   OUTPUT_VARIABLE RT_DEFS
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND xeno-config --skin=alchemy --ldflags
	           RESULT_VARIABLE XENOMAI_NOTFOUND
	           OUTPUT_VARIABLE RT_LIBS
	           OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic ${RT_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic ${RT_DEFS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${RT_LIBS}")

foreach (prog ${programs})
	add_executable(${prog} get_key.cpp manage_backups.cpp ${prog}.cpp)
	target_link_libraries(${prog}
		barrett
		${CURSES_LIBRARIES}
		${Boost_LIBRARIES}
		${GSL_LIBRARIES}
		${XENOMAI_LIBRARY_XENOMAI} ${XENOMAI_LIBRARY_ALCHEMY}
		"-Wl,--wrap=malloc"
		"-Wl,--wrap=free"
	)
	set_target_properties(${prog} PROPERTIES
		PREFIX "bt-wam-"
	)

	install(TARGETS ${prog} RUNTIME DESTINATION bin)
endforeach()


# Don't install wamdiscover. It's intended for the development system, not the
# WAM-PC.
#install(PROGRAMS wamdiscover DESTINATION bin)
