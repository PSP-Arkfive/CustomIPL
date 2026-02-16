add_library(ipl_stage1_payload INTERFACE)

target_sources(ipl_stage1_payload INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
target_compile_options(ipl_stage1_payload INTERFACE -std=c99 -Wall -Os -G0 -fno-pic -fdata-sections -ffunction-sections)
target_link_libraries(ipl_stage1_payload INTERFACE iplsdk)
target_link_options(ipl_stage1_payload INTERFACE -nostartfiles -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l "-Wl,--gc-sections")

foreach(PSP_MODEL IN LISTS NEWIPL_PSP_MODELS)
   set(NAME ipl_stage1_payload_${PSP_MODEL})
   set(STAGE2_NAME ipl_stage2_payload_${PSP_MODEL})
   
   add_executable(${NAME})

   target_sources(${NAME} PRIVATE ${STAGE2_NAME}/ipl_stage2_payload.h)
   target_compile_definitions(${NAME} PRIVATE IPL_${PSP_MODEL} PSP_PSP_MODEL=${PSP_MODEL})
   target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/${STAGE2_NAME})
   target_link_libraries(${NAME} PRIVATE ipl_stage1_payload)

   add_custom_command(
      OUTPUT ${STAGE2_NAME}/ipl_stage2_payload.h
      DEPENDS ${STAGE2_NAME}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${STAGE2_NAME}
      COMMAND bin2c "$<TARGET_FILE:${STAGE2_NAME}>.bin" ${STAGE2_NAME}/ipl_stage2_payload.h payload
   )

   add_custom_command(
      TARGET ${NAME}
      POST_BUILD
      COMMAND psp-strip -s ${NAME}
      COMMAND psp-objcopy -O binary ${NAME} ${NAME}.bin
   )
endforeach()