#define DEFAULT_CONSTRUCTOR(CLASS)											\
	public:																	\
		CLASS() CORE_DEFAULT;													

#define ABSTRACT_OBJ(CLASS)													\
	DEFAULT_CONSTRUCTOR(CLASS)												\
	public:																	\
		virtual ~CLASS() CORE_DEFAULT;										\
																			\
	private:																\
		virtual void Init(void) CORE_PURE;																										

#define OVERRIDE_OBJ(CLASS)													\
	public:																	\
		CLASS();															\
		virtual ~CLASS() override;											\
																			\
	protected:																\
		virtual void Init(void) override;									

#define OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(CLASS)						\
	public:																	\
		CLASS() CORE_DELETE;												\
		virtual ~CLASS() override;											\
																			\
	private:																\
		virtual void Init(void) override;									

#define OVERRIDE_GAME_OBJ(CLASS)											\
	OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(CLASS)							\
																			\
	public:																	\
		virtual void Update(void) override;

#define NO_COPY(CLASS)														\
	private:																\
		CLASS()	CORE_DEFAULT;												\
		CLASS(const CLASS&) CORE_DELETE;									\
		CLASS(const CLASS&&) CORE_DELETE;									\
		CLASS& operator= (const CLASS&) CORE_DELETE;						\
		CLASS& operator= (const CLASS&&) CORE_DELETE;		

#define DECLARE_SINGLETON(CLASS)											\
	NO_COPY(CLASS)															\
																			\
	public:																	\
		static CLASS& GetInstance(void);									\
																			\
	private:																\
		~CLASS();															\
																			\
	private:																\
		void Init(void);													\
		static void Release(void);											\
																			\
	private:																\
		static CLASS* instance;												\
		static std::once_flag onceFlag;										\
		static bool isDestroyed;

#define IMPLEMENT_SINGLETON(CLASS)											\
	CLASS* CLASS::instance;													\
	std::once_flag CLASS::onceFlag;											\
    bool CLASS::isDestroyed;												\
																			\
	CLASS& CLASS::GetInstance(void)											\
	{																		\
		call_once(CLASS::onceFlag, []()										\
		{																	\
			static CLASS inst;												\
			instance = &inst;												\
			instance->Init();												\
			isDestroyed = false;											\
		});																	\
																			\
		if(isDestroyed)														\
		{																	\
			new(instance) CLASS();											\
			atexit(Release);												\
			isDestroyed = false;											\
		}																	\
																			\
		return *instance;													\
	}																		\
																			\
	CLASS::~CLASS()															\
	{																		\
		isDestroyed = true;													\
	}

#define IMPLEMENT_TEMPLATE_SINGLETON(CLASS)									\
	template<typename T>													\
	CLASS<T>* CLASS<T>::instance;											\
	template<typename T>													\
	std::once_flag CLASS<T>::onceFlag;										\
	template<typename T>													\
	bool CLASS<T>::isDestroyed;												\
																			\
	template<typename T>													\
	CLASS<T>& CLASS<T>::GetInstance(void)									\
	{																		\
		call_once(CLASS<T>::onceFlag, []()									\
		{																	\
			static CLASS<T> inst;											\
			instance = &inst;												\
			instance->Init();												\
			isDestroyed = false;											\
		});																	\
																			\
		if(isDestroyed)														\
		{																	\
			new(instance) CLASS<T>();										\
			atexit(Release);												\
			isDestroyed = false;											\
		}																	\
																			\
		return *instance;													\
	}																		\
																			\
	template<typename T>													\
	CLASS<T>::~CLASS()														\
	{																		\
		isDestroyed = true;													\
	}

#define IMPLEMENT_BLOCK_TEMPLATE_SINGLETON(CLASS)							\
	template<typename T, size_t MAX_BLOCK_NUM>								\
	CLASS<T, MAX_BLOCK_NUM>* CLASS<T, MAX_BLOCK_NUM>::instance;				\
	template<typename T, size_t MAX_BLOCK_NUM>								\
	std::once_flag CLASS<T, MAX_BLOCK_NUM>::onceFlag;						\
	template<typename T, size_t MAX_BLOCK_NUM>								\
	bool CLASS<T, MAX_BLOCK_NUM>::isDestroyed;								\
																			\
	template<typename T, size_t MAX_BLOCK_NUM>								\
	CLASS<T, MAX_BLOCK_NUM>& CLASS<T, MAX_BLOCK_NUM>::GetInstance(void)		\
	{																		\
		call_once(CLASS<T, MAX_BLOCK_NUM>::onceFlag, []()					\
		{																	\
			static CLASS<T, MAX_BLOCK_NUM> inst;							\
			instance = &inst;												\
			instance->Init();												\
			isDestroyed = false;											\
		});																	\
																			\
		if(isDestroyed)														\
		{																	\
			new(instance) CLASS<T, MAX_BLOCK_NUM>();						\
			atexit(Release);												\
			isDestroyed = false;											\
		}																	\
																			\
		return *instance;													\
	}																		\
																			\
	template<typename T, size_t MAX_BLOCK_NUM>								\
	CLASS<T, MAX_BLOCK_NUM>::~CLASS()										\
	{																		\
		isDestroyed = true;													\
	}

#define GET_INSTANCE(CLASS) CLASS::GetInstance()	