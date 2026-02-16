
set(STAGE2_NAME ms_ipl_stage2)

add_executable(ms_ipl_stage1)

target_sources(ms_ipl_stage1 PRIVATE crt0.S main.c msipl_compressed.h)
target_compile_options(ms_ipl_stage1 PRIVATE -std=c99 -Wall -Os -G0 -fno-pic -fdata-sections -ffunction-sections)
target_include_directories(ms_ipl_stage1 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(ms_ipl_stage1 PRIVATE microlz)
target_link_options(ms_ipl_stage1 PRIVATE -nostartfiles -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l "-Wl,--gc-sections")

add_custom_command(
   OUTPUT msipl_compressed.h
   DEPENDS ${STAGE2_NAME}
   COMMAND bin2c "$<TARGET_FILE:${STAGE2_NAME}>.lzo" msipl_compressed.h msipl_compressed
)

add_custom_command(
   TARGET ms_ipl_stage1
   POST_BUILD
   COMMAND psp-strip -s ms_ipl_stage1
   COMMAND psp-objcopy -O binary ms_ipl_stage1 ms_ipl_stage1.bin
   COMMAND python3 $<TARGET_PROPERTY:ipltool_make_ipl,EXEC> ms_ipl_stage1.bin ipl.bin reset_block 0x4000000
   COMMAND ${CMAKE_COMMAND} -E copy ipl.bin ${CMAKE_CURRENT_LIST_DIR}/
)