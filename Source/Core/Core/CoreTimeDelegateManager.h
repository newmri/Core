#pragma once

#define CORE_TIME_DELEGATE_MANAGER GET_INSTANCE(CoreTimeDelegateManager)

class CoreTimeDelegateManager
{
	DECLARE_SINGLETON(CoreTimeDelegateManager)

public:
	void Push(CoreTimeDelegate<int, int> func);

public:
	void Run(void);

private:
	CorePriorityQueue<CoreTimeDelegate<int, int>> queueTwoIntArguments;
};