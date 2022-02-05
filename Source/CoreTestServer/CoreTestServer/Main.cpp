#include "../../Core/Core/CoreInclude.h"

#include "person_generated.h"

void Run()
{
	CoreServer server(7777);
	server.Run();

	getchar();

	server.Stop();
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG


	//CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<int, int>(Attack, 0, 1));
	//CORE_THREAD_MANAGER.Push(Run);

	CORE_TIME_DELEGATE_MANAGER.Push(CoreTimeDelegate<>(Run));
	CORE_THREAD_MANAGER.Run();


	return 0;
}