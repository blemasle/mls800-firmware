cmake_minimum_required(VERSION 2.8)

set(ARDUINO_DEFAULT_BOARD micro) #default board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) #default port, when not specified
set(VERBOSE 1)
set(Wire_RECURSE True)


add_definitions(-D_MCP23017_INTERRUPT_SUPPORT_ -D_MCP23017_INTERRUPT_DEBOUNCE_)

generate_arduino_firmware(mls800
	BOARD micro
	SKETCH ./
	HDRS MLS800_version.h PatchManager.h Menu.h MLS800Menu.h
	SRCS PatchManager.cpp
	)
