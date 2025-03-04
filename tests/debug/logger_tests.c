#include "../../src/debug/logger.h"
#include <assert.h>
#include <locale.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

/* --------- */
/* utilities */
/* --------- */

jmp_buf test_state;

void get_log_output(char** buffer)
{
	fflush(stdout);
	*buffer = malloc(1024);
	fread(buffer, 1, 1023, stdout);
	(*buffer)[1023] = '\0';
}

bool run_log_test(void(*log_function)(const char*,...), const char* message, ...)
{
	puts("in run_log_test");
	int state = setjmp(test_state);
	if (state == 0)
	{
		va_list args;
		va_start(args, message);

		log_function(message, args);
		
		va_end(args);
		return true;
	}

	puts("state was bad");
	return false;
}

/* ----- */
/* tests */
/* ----- */

void logdebug_shouldthrow_withnullmessage()
{
	bool result = run_log_test(log_debug, NULL);
	assert(result == false && "Validate log_debug does not throw when provided a null message.");
}

void logdebug_shouldsucceed_withvalidmessage()
{
	char* buffer;
	const char* message = "test message";
	bool result = run_log_test(log_debug, message);

	get_log_output(&buffer);

	assert(result == true && "Validate log_debug does not throw when provided a valid message.");
	assert(buffer == message && "Validate log_debug output matches provided valid message.");
}

void logdebug_shouldsucceed_withvariadricmessage()
{
	char* buffer;
	const char* message = "test message is %s";
	char expected[1024];
	sprintf(expected, message, "testable");

	bool result = run_log_test(log_debug, message, "testable");
	get_log_output(&buffer);

	assert(result == true && "Validate log_debug does not throw when provided a variadric args message.");
	assert(buffer == expected && "Validate log_debug output matches provided variadric args message.");
}

/* ------ */
/* runner */
/* ------ */

int main()
{
    setlocale(LC_ALL, "");
	wprintf(L"%lc %lc %lc\tlogger tests running\n", (wchar_t)187, (wchar_t)187, (wchar_t)187);
	//logdebug_shouldthrow_withnullmessage();
	logdebug_shouldsucceed_withvalidmessage();
	logdebug_shouldsucceed_withvariadricmessage();
	wprintf(L"%lc %lc %lc\tlog debug tests passed\n", (wchar_t)164, (wchar_t)164, (wchar_t)164);
}