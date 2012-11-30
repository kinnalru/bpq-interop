
set(TARGET boostpython)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt4 REQUIRED QtCore QtGui)
include(${QT_USE_FILE})

include_directories(/usr/include/python2.7/)

find_package( Boost REQUIRED COMPONENTS
    program_options
    python
)

set(HEADERS
)

set(MOC
)

set(SOURCES
    main.cpp
)

set(IFACES
)

set(RESOURCES
)

qt4_wrap_cpp(SOURCES ${MOC})
qt4_wrap_ui(SOURCES ${IFACES})
qt4_add_resources(SOURCES ${RESOURCES})



add_executable(${TARGET} ${HEADERS} ${SOURCES})
target_link_libraries(${TARGET} ${QT_LIBRARIES} ${Boost_LIBRARIES})
