#pragma once

#define CORE_TIME_DELEGATE_MANAGER GET_INSTANCE(CoreTimeDelegateManager)

class CoreTimeDelegateManager
{
	DECLARE_SINGLETON(CoreTimeDelegateManager)

public:
	void Push(CoreTimeDelegate<> func);

public:
	void Run(void);
	void Stop(void);

private:
	bool IsRunning = true;

private:
	CorePriorityQueue<CoreTimeDelegate<>> queueNoneArguments;
};