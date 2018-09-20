cmake_minimum_required(VERSION 2.6)
project(AudioHash)
set(AudioCompare_VERSION_MAJOR 0)
set(AudioCompare_VERSION_MINOR 1)

add_definitions(-D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE -g -Wall -O3 -shared -fPIC)

add_library(audiocompare SHARED compareaudio.cpp hashcalculator.cpp audioreader.cpp Spectral.cpp compare.cpp capi.cpp)
target_link_libraries(audiocompare mpg123 samplerate)

add_executable(aucompare aucompare.cpp)
target_link_libraries(aucompare audiocompare)
set(PERMS OWNER_READ OWNER_WRITE OWNER_EXECUTE 
          GROUP_READ GROUP_EXECUTE 
          WORLD_READ WORLD_EXECUTE)

install(TARGETS aucompare DESTINATION bin)

install(TARGETS audiocompare DESTINATION lib)
