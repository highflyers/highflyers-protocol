cmake_minimum_required(VERSION 2.8.9)

project(highflyers-protocol-c)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_executable(	highflyers-protocol-c
		frame_builder.c
		frame_parser_helper.c
		frames.c
		frame.c
		main.c
		parser.c
		TestFramework/TestFramework.c
	      )