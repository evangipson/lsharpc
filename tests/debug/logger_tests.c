#include "../../src/core/logger/logger.h"
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

const char* listen_to_output()
{
	const char* buffer;
	if(fscanf(stdout, "%s", buffer) != 1)
	{
		puts("fscanf not successful.");
		return NULL;
	}

	return buffer;
}

bool run_log_test(void(*log_function)(const char*,...), const char* message, ...)
{
	va_list args;
	va_start(args, message);

	log_function(message, args);

	va_end(args);
	return true;
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
	const char* message = "test message";
	const char* log_output = listen_to_output();
	bool result = run_log_test(log_debug, message);

	assert(result == true && "Validate log_debug does not throw when provided a valid message.");
	assert(log_output == message && "Validate log_debug output matches provided valid message.");
}

void logdebug_shouldsucceed_withvariadricmessage()
{
	const char* message = "test message is %s";
	char expected[1024];
	sprintf(expected, message, "testable");
	const char* log_output = listen_to_output();

	bool result = run_log_test(log_debug, message, "testable");

	assert(result == true && "Validate log_debug does not throw when provided a variadric args message.");
	assert(log_output == expected && "Validate log_debug output matches provided variadric args message.");
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