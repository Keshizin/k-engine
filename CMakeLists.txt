cmake_minimum_required(VERSION 3.18)

# -----------------------------------------------------------------------------
#  P R O J E C T   N A M E
# -----------------------------------------------------------------------------
project(k-engine)

if(MSVC)
	set (CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHs -Wall /wd4710 /wd4711 /wd4820")
endif(MSVC)

if(MINGW)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif(MINGW)

message ("cxx Flags: " ${CMAKE_CXX_FLAGS})

# -----------------------------------------------------------------------------
#  G A M E   N A M E
# -----------------------------------------------------------------------------
set(APP_EXE game)

# -----------------------------------------------------------------------------
#  S E T T I N G   S U B   D I R E C T O R Y
# -----------------------------------------------------------------------------
add_subdirectory(core)
add_subdirectory(game)