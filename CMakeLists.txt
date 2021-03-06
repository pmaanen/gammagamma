#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gammagamma)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Setup Boost include directories and compile definitions
#
find_package(Boost 1.36 COMPONENTS program_options REQUIRED)
#----------------------------------------------------------------------------
# Setup Root include directories and compile definitions
#
find_package(ROOT REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include  
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
set(DISABLED_WARNINGS "-Wno-c99-extensions -Wno-c++11-extensions")
	
#Get rid of warning messages introduced by boost
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DISABLED_WARNINGS}")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DISABLED_WARNINGS}")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${DISABLED_WARNINGS}")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${DISABLED_WARNINGS}")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DISABLED_WARNINGS}")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  ${DISABLED_WARNINGS}")
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  ${DISABLED_WARNINGS}")
set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}
#${DISABLED_WARNINGS}")

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(gammagamma main.cc ${sources} ${headers})
target_link_libraries(gammagamma ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} ${Boost_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build gammagamma. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(polarimeter_SCRIPTS
	vis_T0.mac
	test_gun.mac
	Na22.mac
	singleNa22.mac
	Co60.mac
	singleCo60.mac
) 

foreach(_script ${polarimeter_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/scripts/${_script}
    ${PROJECT_BINARY_DIR}/scripts/${_script}
    COPYONLY
    )
endforeach()

set(geometry_FILES
)
foreach(_script ${geometry_FILES})
	configure_file(
	${PROJECT_SOURCE_DIR}/xml/${_script}
	${PROJECT_BINARY_DIR}/xml/${_script}
	COPYONLY
	)
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gammagamma DESTINATION bin)
