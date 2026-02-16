add_executable(ms_ipl_stage2)

target_sources(ms_ipl_stage2 PRIVATE crt0.S main.c)
target_compile_options(ms_ipl_stage2 PRIVATE -std=c99 -Wall -Os -G0 -fno-pic -fdata-sections -ffunction-sections)
target_link_libraries(ms_ipl_stage2 PRIVATE iplsdk)
target_link_options(ms_ipl_stage2 PRIVATE -nostartfiles -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l "-Wl,--gc-sections")

add_dependencies(ms_ipl_stage2 build_testmini)

add_custom_command(
   TARGET ms_ipl_stage2 POST_BUILD
   COMMAND psp-strip -s ms_ipl_stage2
   COMMAND psp-objcopy -O binary ms_ipl_stage2 ms_ipl_stage2.bin
   COMMAND ${TESTMINI} ms_ipl_stage2.bin ms_ipl_stage2.lzo
   COMMAND ${CMAKE_COMMAND} -E copy ms_ipl_stage2.bin ${CMAKE_CURRENT_LIST_DIR}/msipl.bin
)