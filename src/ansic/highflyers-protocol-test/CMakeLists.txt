include_directories(../highflyers-protocol)

add_executable(highflyers-protocol-c-test
	main.c
	TestFramework/TestFramework.c
)

target_link_libraries(highflyers-protocol-c-test highflyers-protocol-c)