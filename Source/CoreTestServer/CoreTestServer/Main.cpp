#include "Include.h"

void Input(void)
{
	bool IsRunning = true;
	std::string command;
	command.reserve(CORE_SMALL_SIZE);

	while (IsRunning)
	{
		std::cout <<"Input Command: ";
		std::cin >> command;

		switch (HashCode(command.c_str()))
		{
		case HashCode("quit"):
			IsRunning = false;
			break;
		}
	}
}

void Run(void)
{
	LoginServer server(7777);
	server.Run();

	Input();

	server.Stop();
}

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	CORE_THREAD_MANAGER.Push(Run);
	CORE_THREAD_MANAGER.Run();

	return 0;
}