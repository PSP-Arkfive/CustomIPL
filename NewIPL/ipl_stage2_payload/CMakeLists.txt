add_library(ipl_stage2_payload INTERFACE)

target_sources(ipl_stage2_payload INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/crt0.S ${CMAKE_CURRENT_SOURCE_DIR}/main.c)
target_compile_options(ipl_stage2_payload INTERFACE -std=c99 -Wall -Os -G0 -fno-pic -fdata-sections -ffunction-sections)
target_link_libraries(ipl_stage2_payload INTERFACE microlz iplsdk)
target_link_options(ipl_stage2_payload INTERFACE -nostartfiles -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l "-Wl,--gc-sections")
add_dependencies(ipl_stage2_payload INTERFACE payloadex_nand payloadex_ms)

foreach(PSP_MODEL IN LISTS NEWIPL_PSP_MODELS)
   set(NAME ipl_stage2_payload_${PSP_MODEL})
   add_executable(${NAME})
   
   target_compile_definitions(${NAME} PRIVATE IPL_${PSP_MODEL} PSP_MODEL=${PSP_MODEL})
   target_link_libraries(${NAME} PRIVATE ipl_stage2_payload)
   target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

   add_custom_command(
      TARGET ${NAME}
      POST_BUILD
      COMMAND psp-strip -s ${NAME}
      COMMAND psp-objcopy -O binary ${NAME} ${NAME}.bin
   )
endforeach()