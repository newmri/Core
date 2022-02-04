#pragma once

#define CORE_TIME_DELEGATE_MANAGER GET_INSTANCE(CoreTimeDelegateManager)

class CoreTimeDelegateManager
{
	DECLARE_SINGLETON(CoreTimeDelegateManager)

public:
	void Push(CoreTimeDelegate<> func);
	void Push(CoreTimeDelegate<int> func);
	void Push(CoreTimeDelegate<int, int> func);

public:
	void Run(void);

private:
	CorePriorityQueue<CoreTimeDelegate<>> queueNoneArguments;
	CorePriorityQueue<CoreTimeDelegate<int>> queueOneIntArguments;
	CorePriorityQueue<CoreTimeDelegate<int, int>> queueTwoIntArguments;
};