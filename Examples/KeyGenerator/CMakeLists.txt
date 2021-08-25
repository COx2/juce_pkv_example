juce_add_console_app(KeyGenerator
    PRODUCT_NAME "KeyGenerator") 
    
juce_generate_juce_header(KeyGenerator)

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
    BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/Source"
    SOURCE_DIRS "${CMAKE_CURRENT_LIST_DIR}/Source"
#     EXCLUSION_PATTERNS ".+/Test/.+"
    )

target_sources(KeyGenerator
    PRIVATE
        ${SOURCES}
    )

###^^^ Find source files with generate source groups ^^^###

target_compile_definitions(KeyGenerator
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(KeyGenerator
    PRIVATE
        pkv
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
