function(add_payloadex TYPE HEADER_OUTPUT_DIR)
   set(NAME payloadex_${TYPE})
   
   add_executable(${NAME})
   
   target_sources(${NAME} PRIVATE main.c)
   target_compile_options(${NAME} PRIVATE -std=c99 -Wall -Os -G0 -fno-pic)
   target_link_libraries(${NAME} PRIVATE bootloadex)

   if(DEBUG)
      target_link_libraries(${NAME} PRIVATE colordebugger)
   endif()

   add_dependencies(${NAME} build_testmini)

   add_custom_command(
      TARGET ${NAME} POST_BUILD
      COMMAND psp-strip -s ${NAME}
      COMMAND psp-objcopy -O binary ${NAME} ${NAME}.bin
      COMMAND ${TESTMINI} ${NAME}.bin ${NAME}.lzo
      COMMAND bin2c ${NAME}.lzo "${HEADER_OUTPUT_DIR}/payload.h" payloadex
   )

endfunction()

add_subdirectory(Ms)
add_subdirectory(Nand)