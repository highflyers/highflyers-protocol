#ifndef HIGHFLYERS_TEST_FRAMEWORK_H
#define HIGHFLYERS_TEST_FRAMEWORK_H

extern int passed;
extern int failed;

#define ASSERT_TRUE(expression) \
	if (expression) { \
		printf("%c[1;32mPASSED: %s\n", 27, __FUNCTION__);\
		passed++; \
	} else { \
		printf("%c[1;31mFAILED: %s\n", 27, __FUNCTION__); \
		failed++; \
	}

#define ASSERT_EQ(expected, actual, format) \
	if (expected == actual) { \
		printf("%c[1;32mPASSED: %s\n", 27, __FUNCTION__);\
		passed++; \
	} else { \
		printf("%c[1;31mFAILED: %s\n", 27, __FUNCTION__); \
		printf("Expected: "); printf(format, expected); \
		printf("\nActual: "); printf(format, actual); \
		failed++; \
	}
	
#define TEST_SUMMARY \
	printf ("\n%c[0;0mFailed: %d Passed: %d\n", 27, failed, passed);	\
	if (!failed) \
		printf ("%c[1;32mNice, no failing tests!\n", 27); \
	printf ("%c[0;0m", 27);


#endif
