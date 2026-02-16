add_subdirectory(ipl_stage2_payload)
add_subdirectory(ipl_stage1_payload)

foreach(PSP_MODEL IN LISTS NEWIPL_PSP_MODELS)

   if(PSP_MODEL STREQUAL "01G" OR PSP_MODEL STREQUAL "02G")
      set(XOR_VAL "")
   elseif(PSP_MODEL STREQUAL "05G")
      set(XOR_VAL --xor 2)
   else()
      set(XOR_VAL --xor 1)
   endif()

   add_custom_command(
      OUTPUT ${CMAKE_CURRENT_LIST_DIR}/cipl_${PSP_MODEL}.bin
      DEPENDS ipl_stage1_payload_${PSP_MODEL}
      COMMAND python3 $<TARGET_PROPERTY:ipltool_unpack_ipl,EXEC> ${RESOURCES_DIR}/nandipl_${PSP_MODEL}.bin ipl_${PSP_MODEL}.dec ${XOR_VAL}
      COMMAND dd if=$<TARGET_FILE_DIR:ipl_stage1_payload_${PSP_MODEL}>/ipl_stage1_payload_${PSP_MODEL}.bin of=ipl_${PSP_MODEL}.bin
      COMMAND dd if=ipl_${PSP_MODEL}.dec of=ipl_${PSP_MODEL}.bin bs=1 seek=49152
      COMMAND python3 $<TARGET_PROPERTY:ipltool_make_ipl,EXEC> ipl_${PSP_MODEL}.bin cipl_${PSP_MODEL}.bin reset_block 0x40e0000
      COMMAND ${CMAKE_COMMAND} -E copy cipl_${PSP_MODEL}.bin ${CMAKE_CURRENT_LIST_DIR}/
   )

   add_custom_target(newipl_${PSP_MODEL}
      DEPENDS ${CMAKE_CURRENT_LIST_DIR}/cipl_${PSP_MODEL}.bin
   )
endforeach()
