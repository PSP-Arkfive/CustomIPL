cmake_minimum_required(VERSION 3.28)

project(customipl VERSION 1.0 LANGUAGES C ASM)
set(PSPCFWSDK_PATH "" CACHE PATH "Path to local psp-cfw-sdk (for local development)")
set(DEBUG "" CACHE STRING "Debug level (1,2,3)")
message(STATUS "DEBUG LEVEL = ${DEBUG}")

set(RESOURCES_DIR ${CMAKE_CURRENT_LIST_DIR}/Resources)
set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist)
set(NEWIPL_PSP_MODELS 01G 02G 03G 04G 05G 07G 09G 11G)

include(FetchContent)
if(PSPCFWSDK_PATH)
message(STATUS "Using local pspcfw repo: ${PSPCFWSDK_PATH}")
FetchContent_Declare(
   pspcfwsdk
   SOURCE_DIR ${PSPCFWSDK_PATH}
   EXCLUDE_FROM_ALL
)
else()
FetchContent_Declare(
   pspcfwsdk
   GIT_REPOSITORY https://github.com/pspdev/psp-cfw-sdk.git
   GIT_TAG main
   EXCLUDE_FROM_ALL
)
endif()
FetchContent_MakeAvailable(pspcfwsdk)

set(CFWSDK ${pspcfwsdk_SOURCE_DIR})
set(BUILDTOOLS ${CFWSDK}/build-tools)

remove_definitions("-D_PSP_FW_VERSION=600")
add_definitions("-D_PSP_FW_VERSION=660")

set(TESTMINI ${PROJECT_SOURCE_DIR}/minilzo/testmini)

add_custom_command(
    OUTPUT ${TESTMINI}
    COMMAND make gcc -C ${PROJECT_SOURCE_DIR}/minilzo
)

add_custom_target(build_testmini
    DEPENDS ${TESTMINI}
)

add_subdirectory(ClassicIPL)
add_subdirectory(Installer)
add_subdirectory(MSIPL)
add_subdirectory(NewIPL)
add_subdirectory(Payloadex)

message(STATUS "${PROJECT_BINARY_DIR}")

add_custom_command(
   OUTPUT CIPL.ARK
   DEPENDS
      mainbinex
      ms_mainbinex
      ms_ipl_stage1
      classicipl_01G
      classicipl_02G
      classicipl_kbooti_01G
      newipl_01G
      newipl_02G
      newipl_03G
      newipl_04G
      newipl_05G
      newipl_07G
      newipl_09G
      newipl_11G
      msipl_01G
      msipl_02G
      msipl_03G
      msipl_04G
      msipl_05G
      msipl_07G
      msipl_09G
      msipl_11G
   
   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
   COMMAND python3 $<TARGET_PROPERTY:tool_pack,EXEC> -p ${CMAKE_CURRENT_BINARY_DIR}/CIPL.ARK ${CMAKE_CURRENT_LIST_DIR}/package.txt -s
)

add_custom_target(dist ALL
   DEPENDS
      CIPL.ARK
      cipl_flasher

   WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
   COMMENT "Creating dist"
   COMMAND ${CMAKE_COMMAND} -E make_directory "${DIST_DIR}/CustomIPL"
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/CIPL.ARK" "${DIST_DIR}/CustomIPL/"
   COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE_DIR:cipl_flasher>/EBOOT.PBP" "${DIST_DIR}/CustomIPL/"
)