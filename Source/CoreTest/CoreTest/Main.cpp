#include "../../Core/Core/CoreInclude.h"

#include "login_protocol_generated.h"

int main(void)
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	boost::asio::io_context ioContext;
	boost::asio::ip::tcp::resolver resolver(ioContext);

	CoreServerSession session(ioContext);
	session.Connect(resolver.resolve("127.0.0.1", "7777"));
	std::thread t([&ioContext]() { ioContext.run(); });

	while (true)
	{
		if (session.IsConnected())
		{
			flatbuffers::FlatBufferBuilder builder;

			auto name = builder.CreateString("aa");
			auto data = Login::CreateCS_LOGIN_REQ(builder, name);
			auto root = Login::CreateRoot(builder, Login::Packet_CS_LOGIN_REQ, data.Union());

			builder.Finish(root);

			session.Write(CorePacket(builder.GetBufferPointer(), builder.GetSize()));
		}
	}

	t.join();

	return 0;
}