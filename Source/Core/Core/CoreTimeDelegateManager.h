#pragma once

#define CORE_TIME_DELEGATE_MANAGER GET_INSTANCE(CoreTimeDelegateManager)

class CoreTimeDelegateManager
{
	DECLARE_SINGLETON(CoreTimeDelegateManager)

public:
	void Push(CoreTimeDelegate<> func);
	void Push(CoreTimeDelegate<void*> func);
	void Push(CoreTimeDelegate<int32_t> func);
	void Push(CoreTimeDelegate<int32_t, int32_t> func);

public:
	void Run(void);
	void Stop(void);

private:
	bool IsRunning = true;

private:
	CorePriorityQueue<CoreTimeDelegate<>> queueNoneArguments;
	CorePriorityQueue<CoreTimeDelegate<void*>> queueOnePointerArguments;
	CorePriorityQueue<CoreTimeDelegate<int32_t>> queueOneIntArguments;
	CorePriorityQueue<CoreTimeDelegate<int32_t, int32_t>> queueTwoIntArguments;
};