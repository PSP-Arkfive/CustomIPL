add_subdirectory(mainbinex)

add_custom_command(
   OUTPUT ipl_block_01g.bin
   DEPENDS mainbinex
   COMMAND python3 $<TARGET_PROPERTY:ipltool_combine,EXEC> ${RESOURCES_DIR}/ipl_header_01g.bin "$<TARGET_FILE:mainbinex>.bin" ipl_block_01g.bin
   COMMAND ${CMAKE_COMMAND} -E copy ipl_block_01g.bin ${CMAKE_CURRENT_LIST_DIR}/combine/
)

add_custom_target(
    classicipl_01G
    DEPENDS ipl_block_01g.bin
)

add_custom_command(
   OUTPUT ipl_block_02g.bin
   DEPENDS mainbinex
   COMMAND python3 $<TARGET_PROPERTY:ipltool_combine,EXEC> -l 0x24000 ${RESOURCES_DIR}/ipl_header_02g.bin "$<TARGET_FILE:mainbinex>.bin" ipl_block_02g.bin
   COMMAND ${CMAKE_COMMAND} -E copy ipl_block_02g.bin ${CMAKE_CURRENT_LIST_DIR}/combine/

)

add_custom_target(
    classicipl_02G
    DEPENDS ipl_block_02g.bin
)

add_custom_command(
   OUTPUT kbooti_ipl_block_01g.bin
   DEPENDS mainbinex
   COMMAND python3 $<TARGET_PROPERTY:ipltool_combine,EXEC> -l 0x5000 -k ${RESOURCES_DIR}/kbooti_header_01g.bin "$<TARGET_FILE:mainbinex>.bin" kbooti_ipl_block_01g.bin
   COMMAND ${CMAKE_COMMAND} -E copy kbooti_ipl_block_01g.bin ${CMAKE_CURRENT_LIST_DIR}/combine/
)

add_custom_target(
    classicipl_kbooti_01G
    DEPENDS kbooti_ipl_block_01g.bin
)