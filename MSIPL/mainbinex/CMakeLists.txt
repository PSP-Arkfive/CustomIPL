add_executable(ms_mainbinex)

target_compile_definitions(ms_mainbinex PRIVATE MSIPL)
target_link_libraries(ms_mainbinex PRIVATE mainbinex_int iplsdk)
target_include_directories(ms_mainbinex PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_options(ms_mainbinex PRIVATE -nostdlib -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l)

add_custom_command(
   TARGET ms_mainbinex
   POST_BUILD
   COMMAND psp-strip -s ms_mainbinex
   COMMAND psp-objcopy -O binary ms_mainbinex ms_mainbinex.bin
   COMMAND python3 $<TARGET_PROPERTY:ipltool_make_tm_ipl,EXEC> --input_payload ms_mainbinex.bin --payload_addr 0x040D6000 --ipl_addr 0x040EC000 --output payload.bin
   COMMAND ${CMAKE_COMMAND} -E copy payload.bin ${CMAKE_CURRENT_LIST_DIR}/
)