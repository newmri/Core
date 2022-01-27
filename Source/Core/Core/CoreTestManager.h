#pragma once

#define CORE_TEST_MANAGER GET_INSTANCE(CoreTestManager)

static const size_t TEST_START_THREAD_NUMBER = 1;
static const size_t TEST_END_THREAD_NUMBER = 64;
static const size_t TEST_INCREASE_THREAD_NUMBER_TIMES = 2;

static const size_t TEST_NUMBER = 30000;
static const size_t TEST_RESERVE_DEVIDE_NUMBER = 10;

struct TestInfo
{
	size_t startThreadNumber = TEST_START_THREAD_NUMBER;
	size_t endThreadNumber = TEST_END_THREAD_NUMBER;
	size_t increaseThreadNumberTimes = TEST_INCREASE_THREAD_NUMBER_TIMES;
	size_t testNumber = TEST_NUMBER;
	size_t reserveDevideNumber = TEST_RESERVE_DEVIDE_NUMBER;
};

class CoreTestManager
{
	DECLARE_SINGLETON(CoreTestManager)

public:
	void Work(void (*func)(void), TestInfo testInfo = TestInfo());
};