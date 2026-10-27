cmake_minimum_required(VERSION 3.19)

project(qonvert LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)

qt_add_executable(qonvert WIN32 MACOSX_BUNDLE
	main.cpp
	qonvert.cpp qonvert.h
	progress.cpp progress.h
	qonvertwidget.ui progresswidget.ui
	qonvert.qrc
)

if(WIN32)
	target_sources(qonvert PRIVATE qonvert.rc)
endif()

target_link_libraries(qonvert PRIVATE Qt6::Widgets)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(qonvert PRIVATE -Wall -Wextra)
endif()

install(TARGETS qonvert
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
