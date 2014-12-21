add_library(highflyers-protocol-c
	frame_builder.c
	frame_parser_helper.c
	frames.c
	parser.c
)

add_custom_command(TARGET highflyers-protocol-c POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${highflyers-protocol_SOURCE_DIR}/build/highflyers-protocol-c/)

set(headers_hf_pt_c
	frame_builder.h
	frame_parser_helper.h
	frames.h
	parser.h
	types.h
)

foreach( file_i ${headers_hf_pt_c})
add_custom_command(TARGET highflyers-protocol-c POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/${file_i} 
		${highflyers-protocol_SOURCE_DIR}/build/highflyers-protocol-c/)
endforeach( file_i )