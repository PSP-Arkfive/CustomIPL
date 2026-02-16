add_subdirectory(stage1)
add_subdirectory(stage2)

foreach(PSP_MODEL IN LISTS NEWIPL_PSP_MODELS)

   set(STAGE1_NAME ms_ipl_stage1_payload_${PSP_MODEL})
   set(STAGE2_NAME ms_ipl_stage2_payload_${PSP_MODEL})
   string(TOLOWER ${PSP_MODEL} PSP_MODEL_LOWER)

   if(PSP_MODEL STREQUAL "01G" OR PSP_MODEL STREQUAL "02G")
      set(XOR_VAL "")
   elseif(PSP_MODEL STREQUAL "05G")
      set(XOR_VAL --xor 2)
   else()
      set(XOR_VAL --xor 1)
   endif()

   add_executable(${STAGE2_NAME})

   target_compile_definitions(${STAGE2_NAME} PRIVATE IPL_${PSP_MODEL} PSP_MODEL=${PSP_MODEL} MSIPL=1)
   target_link_libraries(${STAGE2_NAME} PRIVATE ipl_stage2_payload)
   target_include_directories(${STAGE2_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

   add_custom_command(
      OUTPUT STAGE2_${PSP_MODEL}/ipl_stage2_payload.h
      DEPENDS ${STAGE2_NAME}
      POST_BUILD
      COMMAND psp-strip -s ${STAGE2_NAME}
      COMMAND psp-objcopy -O binary ${STAGE2_NAME} ${STAGE2_NAME}.bin
      COMMAND ${CMAKE_COMMAND} -E make_directory STAGE2_${PSP_MODEL}
      COMMAND bin2c ${STAGE2_NAME}.bin STAGE2_${PSP_MODEL}/ipl_stage2_payload.h payload
   )

   add_executable(${STAGE1_NAME})

   target_sources(${STAGE1_NAME} PRIVATE STAGE2_${PSP_MODEL}/ipl_stage2_payload.h)
   target_compile_definitions(${STAGE1_NAME} PRIVATE IPL_${PSP_MODEL} PSP_MODEL=${PSP_MODEL} MSIPL=1)
   target_include_directories(${STAGE1_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/STAGE2_${PSP_MODEL})
   target_link_libraries(${STAGE1_NAME} PRIVATE ipl_stage1_payload)

   add_custom_command(
      OUTPUT msipl_${PSP_MODEL}.bin
      DEPENDS ${STAGE1_NAME}
      COMMAND psp-strip -s ${STAGE1_NAME}
      COMMAND psp-objcopy -O binary ${STAGE1_NAME} ${STAGE1_NAME}.bin
      COMMAND dd if=${STAGE1_NAME}.bin of=msipl_${PSP_MODEL}.bin
      COMMAND python3 $<TARGET_PROPERTY:ipltool_unpack_ipl,EXEC> ${RESOURCES_DIR}/nandipl_${PSP_MODEL}.bin ipl_${PSP_MODEL}.dec ${XOR_VAL}
      COMMAND dd if=ipl_${PSP_MODEL}.dec of=msipl_${PSP_MODEL}.bin bs=1 seek=49152
      COMMAND ${CMAKE_COMMAND} -E copy msipl_${PSP_MODEL}.bin ${CMAKE_CURRENT_LIST_DIR}/msipl_${PSP_MODEL_LOWER}.bin
   )

   add_custom_target(msipl_${PSP_MODEL}
      DEPENDS msipl_${PSP_MODEL}.bin
   )

endforeach()
