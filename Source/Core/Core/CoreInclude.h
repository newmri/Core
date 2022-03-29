#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif // _WIN32_WINNT

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#endif // _DEBUG

#include <iostream>
#include <fstream>
#include <memory>
#include <shared_mutex>
#include <chrono>
#include <string>
#include <string_view>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <direct.h>
#include <type_traits>
#include <map> // 추후 제작한걸로 대체
#include <functional>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <windows.h>  
#include <sqlext.h>
#include <locale>
#include <regex>
#include <atlbase.h>
#include <atlconv.h>

#include "flatbuffers/flatbuffers.h"

#include "CoreMacro.h"
#include "CoreDataType.h"
#include "CoreObject.h"
#include "CoreUIDObject.h"
#include "CoreGameObject.h"
#include "CoreLogInclude.h"
#include "CoreMemoryPool.h"
#include "CoreMemoryPoolObj.h"
#include "CoreSharedPtr.h"
#include "CoreMemoryPoolManager.h"
#include "CoreTimeManager.h"
#include "CoreContainer.h"
#include "CoreArray.h"
#include "CoreVector.h"
#include "CoreQueue.h"
#include "CoreList.h"
#include "CorePriorityQueue.h"
#include "CoreAlgorithm.h"
#include "CoreRandomManager.h"
#include "CoreLoader.h"
#include "CoreCSVLoader.h"
#include "CoreCSVMapLoader.h"
#include "CoreLoadManager.h"
#include "CoreDataTypeManager.h"
#include "CoreStringManager.h"
#include "CoreDelegate.h"
#include "CoreTimeDelegate.h"
#include "CoreTimeDelegateManager.h"
#include "CoreThreadManager.h"
#include "CorePacket.h"
#include "CoreSession.h"
#include "CoreClientSession.h"
#include "CoreServerSession.h"
#include "CoreClient.h"
#include "CoreServer.h"
#include "CoreDB.h"
#include "WorldDB.h"
#include "CoreCharacter.h"
#include "CoreAccount.h"
#include "CoreAccountManager.h"
#include "CoreUtil.h"
#include "CoreTestManager.h"