
function(make_unittest)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs FILES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Unparsed arguments to make_unittest: ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_FILES)
    set(ARG_FILES "${ARG_NAME}.cpp")
  endif()

  add_executable(${ARG_NAME} ${ARG_FILES})
  target_link_libraries(${ARG_NAME} migrate)
  add_test(NAME    "unittest:${ARG_NAME}"
           COMMAND "./${ARG_NAME}"
           WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()

make_unittest(NAME VersionTest)
