### Find source files with generate source groups ###
# Test that the target_path should be excluded
# if the path is excluded, IS_EXCLUDED will be set to TRUE
function(TEST_EXCLUDED TARGET_PATH EXCLUSION_PATTERN_LIST IS_EXCLUDED)
  set(IS_EXCLUDED FALSE PARENT_SCOPE)
  foreach(PAT ${EXCLUSION_PATTERN_LIST})
    if(TARGET_PATH MATCHES ${PAT})
      set(IS_EXCLUDED TRUE PARENT_SCOPE)
      break()
    endif()
  endforeach()
endfunction()

# set target extension list
set(TARGET_SOURCE_PATTERNS "*.c" "*.cc" "*.cpp" "*.h" "*.hpp")

if(MSVC)
  list(APPEND TARGET_SOURCE_PATTERNS "*.rc")
elseif(XCODE)
  list(APPEND TARGET_SOURCE_PATTERNS "*.mm")
endif()

# generate_source_list(
#   output
#   BASE_DIR dir
#   SOURCE_DIRS dir1 [dir2...]
#   EXCLUSION_PATTERNS [pattern1...]
#   )
function(generate_source_list output)
  cmake_parse_arguments(GSL "" "BASE_DIR" "SOURCE_DIRS;EXCLUSION_PATTERNS" ${ARGN})

  if(NOT DEFINED GSL_BASE_DIR)
    message(FATAL_ERROR "BASE_DIR must be specified.")
  endif()

  if(NOT DEFINED GSL_SOURCE_DIRS)
    message(FATAL_ERROR "SOURCE_DIRS must be specified.")
  endif()

  message("Print EXCLUSION_PATTERNS: ${GSL_EXCLUSION_PATTERNS}")

  # set the root directory of the source file tree in IDE
  get_filename_component(GSL_BASE_DIR "${GSL_BASE_DIR}" ABSOLUTE)

  foreach(SOURCE_DIR ${GSL_SOURCE_DIRS})
    get_filename_component(SOURCE_DIR "${SOURCE_DIR}" ABSOLUTE)

    set(PATTERNS "")
    foreach(PATTERN ${TARGET_SOURCE_PATTERNS})
      list(APPEND PATTERNS "${SOURCE_DIR}/${PATTERN}")
    endforeach()

    file(GLOB_RECURSE FILES ${PATTERNS})

    # Define SourceGroup reflecting filesystem hierarchy.
    foreach(FILE_PATH ${FILES})
      get_filename_component(FILEPATH "${FILE_PATH}" ABSOLUTE)
      TEST_EXCLUDED(${FILE_PATH} "${GSL_EXCLUSION_PATTERNS}" IS_EXCLUDED)
      if(IS_EXCLUDED)
        continue()
      endif()

      get_filename_component(PARENT_DIR "${FILE_PATH}" DIRECTORY)
      file(RELATIVE_PATH GROUP_NAME "${GSL_BASE_DIR}" "${PARENT_DIR}")
      string(REPLACE "/" "\\" GROUP_NAME "${GROUP_NAME}")
      source_group("${GROUP_NAME}" FILES "${FILE_PATH}")
      list(APPEND SOURCE_FILES "${FILE_PATH}")
    endforeach()
  endforeach()

  message("Files ${SOURCE_FILES}")
  set(${output} "${SOURCE_FILES}" PARENT_SCOPE)
endfunction()

generate_source_list(
    SOURCES
    BASE_DIR "${CMAKE_CURRENT_LIST_DIR}"
    SOURCE_DIRS "${CMAKE_CURRENT_LIST_DIR}"
#     EXCLUSION_PATTERNS ".+/Test/.+"
    )
###^^^ Find source files with generate source groups ^^^###

# Tell CMake to add a static library target
add_library(pkv STATIC)

# Add sources to the target
target_sources(pkv
    PRIVATE
        ${SOURCES}
    )

# Add compile definitions to the target
# When use this library, you should change these definitions here...
target_compile_definitions(pkv
    PRIVATE
        PKV_SUBKEY0_PARAM_A=16
        PKV_SUBKEY0_PARAM_B=2
        PKV_SUBKEY0_PARAM_C=128
        PKV_SUBKEY1_PARAM_A=18
        PKV_SUBKEY1_PARAM_B=0
        PKV_SUBKEY1_PARAM_C=84
        PKV_SUBKEY2_PARAM_A=9
        PKV_SUBKEY2_PARAM_B=1
        PKV_SUBKEY2_PARAM_C=91
        PKV_SUBKEY3_PARAM_A=21
        PKV_SUBKEY3_PARAM_B=3
        PKV_SUBKEY3_PARAM_C=220
        PKV_CHECKSUM_MAGIC_NUMBER_LEFT=124
        PKV_CHECKSUM_MAGIC_NUMBER_RIGHT=82
    )

# Link JUCE modules  to the target
target_link_libraries(pkv
    PRIVATE
        juce::juce_core
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
    )

# Tell CMake where our library's headers are located
target_include_directories(pkv
    PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
    )