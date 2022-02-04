#pragma once

#define CORE_THREAD_MANAGER GET_INSTANCE(CoreThreadManager)

class CoreThreadManager
{
	DECLARE_SINGLETON(CoreThreadManager)

public:
	void Push(std::function<void()> func);
	void Run(void);

private:
	CoreVector<std::thread> threads;
};