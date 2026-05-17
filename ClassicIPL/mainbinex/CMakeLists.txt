add_library(mainbinex_int INTERFACE)

target_sources(mainbinex_int INTERFACE 
   ${CMAKE_CURRENT_SOURCE_DIR}/main.c
   ${CMAKE_CURRENT_SOURCE_DIR}/cache.c
)
target_compile_options(mainbinex_int INTERFACE -std=c99 -Wall -Os -G0 -fno-pic)
target_link_libraries(mainbinex_int INTERFACE ansic microlz)
add_dependencies(mainbinex_int INTERFACE payloadex_nand payloadex_ms)

add_executable(mainbinex)

target_link_libraries(mainbinex PRIVATE mainbinex_int)
target_include_directories(mainbinex PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_options(mainbinex PRIVATE -nostdlib -T${CMAKE_CURRENT_SOURCE_DIR}/linkfile.l)

add_custom_command(
   TARGET mainbinex
   POST_BUILD
   COMMAND psp-strip -s mainbinex
   COMMAND psp-objcopy -O binary mainbinex mainbinex.bin
)