add_executable(cipl_flasher)

target_sources(cipl_flasher PRIVATE main.c flasher.c)
target_compile_options(cipl_flasher PRIVATE -O2 -G0 -Wall -fshort-wchar)
target_link_libraries(cipl_flasher PRIVATE
   mini2d
   png
   z
   pspgu
   pspgum
   psppower
   pspkubridge
   pspsystemctrl_user
   arkctrl
   pspiplupdate
   pspkbootiupdate
   kpspident
   pspdebug
   pspdisplay
   pspge
   pspctrl
   pspnet
   pspnet_apctl
)

create_pbp_file(
   TARGET cipl_flasher
   TITLE "ARK cIPL Flasher"
   ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res/icon0.png
   BACKGROUND_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res/pic1.png
   BUILD_PRX
)