###############################################################################
## Name:        CMakeLists.txt
## Purpose:     To build NNBSS application
## Author:      Orkhan Aliyev
## Created:     14-11-2021
## Copyright:   (c) 2021 Orkhan Aliyev
###############################################################################

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(NNBSS)

# Main Directory
if (NOT DEFINED NNBSS_DIR)
	set (NNBSS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Set Code Directory
set(NNBSS_CODE_DIR ${NNBSS_DIR}/Code)
# Set SDKs Directory
set(SDK_DIR ${NNBSS_DIR}/Code/SDKs)

# set version variables
set(wxWidgets_USE_VERSION 3.1.5)
set(OpenCV_USE_VERSION 4.5.5)
set(BOOST_USE_VERSION 1.78.0)
set(SDL2_USE_VERSION 2.0.20)

# Find wxWidgets in given path
set(wxWidgets_DIR ${SDK_DIR}/wxWidgets/${wxWidgets_USE_VERSION})
find_package(wxWidgets ${wxWidgets_USE_VERSION} COMPONENTS base core aui propgrid xrc html xml adv gl net qa REQUIRED PATHS "${wxWidgets_DIR}")
# Find OpenCV in given path
set(OpenCV_DIR ${SDK_DIR}/OpenCV/${OpenCV_USE_VERSION}/build)
find_package(OpenCV ${OpenCV_USE_VERSION} EXACT REQUIRED PATHS "${OpenCV_DIR}")

# Config & Find Boost
if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()
set(Boost_DEBUG ON)
set(BOOST_ROOT ${SDK_DIR}/boost/1.78.0) 
set(BOOST_DIR ${SDK_DIR}/boost/1.78.0) #set(BOOST_DIR ${SDK_DIR}//boost/1.78.0) 
set(BOOST_INCLUDEDIR ${SDK_DIR}/boost/1.78.0)
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost ${BOOST_USE_VERSION} REQUIRED COMPONENTS chrono context filesystem iostreams locale system)# header only libraries must not be added here!

# Find SDL2
set(SDL2_DIR ${SDK_DIR}/SDL2/${SDL2_USE_VERSION})
include(${SDL2_DIR}/FindSDL2.cmake)
#--------------------------------------------------------------------

# Find FFmpeg
set(FFMPEG_DIR ${SDK_DIR}/ffmpeg)

find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h HINTS ${FFMPEG_DIR}/include)
find_library(AVCODEC_LIBRARY avcodec HINTS ${FFMPEG_DIR}/lib)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h HINTS ${FFMPEG_DIR}/include)
find_library(AVDEVICE_LIBRARY avdevice HINTS ${FFMPEG_DIR}/lib)

find_path(AVFILTER_INCLUDE_DIR libavfilter/avfilter.h HINTS ${FFMPEG_DIR}/include)
find_library(AVFILTER_LIBRARY avfilter HINTS ${FFMPEG_DIR}/lib)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h HINTS ${FFMPEG_DIR}/include)
find_library(AVFORMAT_LIBRARY avformat HINTS ${FFMPEG_DIR}/lib)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h HINTS ${FFMPEG_DIR}/include)
find_library(AVUTIL_LIBRARY avutil HINTS ${FFMPEG_DIR}/lib)

find_path(POSTPROC_INCLUDE_DIR libpostproc/postprocess.h HINTS ${FFMPEG_DIR}/include)
find_library(POSTPROC_LIBRARY postproc HINTS ${FFMPEG_DIR}/lib)

find_path(SWRESAMPLE_INCLUDE_DIR libswresample/swresample.h HINTS ${FFMPEG_DIR}/include)
find_library(SWRESAMPLE_LIBRARY swresample HINTS ${FFMPEG_DIR}/lib)

find_path(SWSCALE_INCLUDE_DIR libswscale/swscale.h HINTS ${FFMPEG_DIR}/include)
find_library(SWSCALE_LIBRARY swscale HINTS ${FFMPEG_DIR}/lib)

if(SWSCALE_LIBRARY AND SWSCALE_INCLUDE_DIR AND SWRESAMPLE_LIBRARY AND SWRESAMPLE_INCLUDE_DIR
AND POSTPROC_LIBRARY AND POSTPROC_INCLUDE_DIR AND AVUTIL_LIBRARY AND AVUTIL_INCLUDE_DIR
AND AVFORMAT_LIBRARY AND AVFORMAT_INCLUDE_DIR AND AVFILTER_LIBRARY AND AVFILTER_INCLUDE_DIR
AND AVDEVICE_LIBRARY AND AVDEVICE_INCLUDE_DIR AND AVCODEC_LIBRARY AND AVCODEC_INCLUDE_DIR)
message(STATUS "FFMPEG found correctly")
else()
message(ERROR "FFMPEG Could not be found!")
endif()
#--------------------------------------------------------------------

#CrashRpt (Only x64 arch for now)
if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
set(CRASHRPT_USE_VERSION 1403)
set(CRASHRPT_DIR ${SDK_DIR}/CrashRpt/${CRASHRPT_USE_VERSION})
set(CRASHRPT_INCLUDE_DIR ${CRASHRPT_DIR}/include)
set(CRASHRPT_LIBRARY CrashRpt${CRASHRPT_USE_VERSION} ${CRASHRPT_DIR}/build/lib/x64)
endif()
#--------------------------------------------------------------------

# add_subdirectory helps us to add source files (.cpp,.h or .hpp) into the project
add_subdirectory(${NNBSS_DIR}/Code)
get_property(SOURCES GLOBAL PROPERTY NNBSS_SOURCE_PROPERTY)# get list from other file

# RC 
option(NNBSS_USE_RC "Don't uncheck if you don't know what to do!" ON)
if(NNBSS_USE_RC)
    if (WIN32)	  
      list(APPEND SOURCES ${wxWidgets_DIR}/include/wx/msw/wx.rc)
    endif()
endif (NNBSS_USE_RC)

# Project filters/folders
foreach(FILE ${SOURCES}) 
  get_filename_component(PARENT_DIR ${FILE} PATH)

  string(REPLACE / // GROUP ${PARENT_DIR})

#  if (${FILE} MATCHES .*//.cpp OR ${FILE} MATCHES .*//.h OR ${FILE} MATCHES .*//.hpp)
    set(GROUP //${GROUP})
#  else()
#    set(GROUP Others//${GROUP})
#  endif()

  source_group(${GROUP} FILES ${FILE})
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Set Startup Project
set_property (DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Add all files into project
add_executable(${PROJECT_NAME} ${SOURCES})

include(${wxWidgets_USE_FILE})

# C++ 17 required
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

if (WIN32)
  
  if(NNBSS_USE_RC)
     set(WX_CPU_XXX_VALUE "WX_CPU_AMD64" CACHE STRING "According to your CPU architecture, set a value. Otherwise you'll get an error at the end of compile time.")
     target_compile_definitions(${PROJECT_NAME} PRIVATE ${WX_CPU_XXX_VALUE} wxUSE_RC_MANIFEST wxUSE_DPI_AWARE_MANIFEST=2)
	 message(STATUS "USE RC Enabled")
  else()
     message(STATUS "USE RC Disabled")
  endif(NNBSS_USE_RC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)# To prevent winsock redefinition error
  
  set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES)

  if (MSVC)
     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  endif(MSVC)
  
	# I have not tested this application on other platforms.
	# Probably, I'll face many problems that wait to be solved.
elseif(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE YES)
endif()

option(NNBSS_USE_PCH "Enable Precompiled Headers" ON)
if(NNBSS_USE_PCH)
  target_precompile_headers(${PROJECT_NAME} PUBLIC ${NNBSS_CODE_DIR}/NNBSSBridge/MainBridge.h)
  message(STATUS "Use Precompiled headers is Enabled!")
else()
  message(STATUS "Use Precompiled headers is Disabled")
endif (NNBSS_USE_PCH)

target_include_directories(${PROJECT_NAME} PRIVATE ${NNBSS_CODE_DIR} ${Boost_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} 
${SWSCALE_INCLUDE_DIR} ${SWRESAMPLE_INCLUDE_DIR} ${POSTPROC_INCLUDE_DIR} ${AVUTIL_INCLUDE_DIR} ${AVFORMAT_INCLUDE_DIR} ${AVFILTER_INCLUDE_DIR} 
${AVDEVICE_INCLUDE_DIR} ${AVCODEC_INCLUDE_DIR} ${CRASHRPT_INCLUDE_DIR})
# Add wxWidgets, OpenCV, Boost, SDL2 and ffmpeg libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${SDL2_LIBRARIES} 
${SWSCALE_LIBRARY} ${SWRESAMPLE_LIBRARY} ${POSTPROC_LIBRARY} ${AVUTIL_LIBRARY} ${AVFORMAT_LIBRARY} ${AVFILTER_LIBRARY} 
${AVDEVICE_LIBRARY} ${AVCODEC_LIBRARY}
${CRASHRPT_LIBRARY})
