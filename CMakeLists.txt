# deepgram/CMakeLists.txt

project(deepgram)

set(deepgram_SOURCES
      deepgram.c
      deepgram_source.c)

add_library(deepgram MODULE
      ${deepgram_SOURCES})
target_link_libraries(deepgram
      libobs)

install_obs_plugin_with_data(deepgram data)
