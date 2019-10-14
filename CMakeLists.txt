cmake_minimum_required (VERSION 2.6)
project (tm1640-rpi)

add_library (libtm1640 SHARED
	src/libtm1640/tm1640.c)
set_target_properties (libtm1640
	PROPERTIES
	PUBLIC_HEADER src/libtm1640/tm1640.h)
target_link_libraries (libtm1640 wiringPi)

add_executable (tm1640 src/main.c)
target_link_libraries (tm1640 libtm1640)
target_include_directories (tm1640 PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/libtm1640>
	$<INSTALL_INTERFACE:include>)

include (GNUInstallDirs)
install (TARGETS tm1640 libtm1640
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
