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
	private:																\
		virtual void Init(void) override;									

#define OVERRIDE_OBJ_DELETE_DEFAULT_CONSTRUCTOR(CLASS)						\
	public:																	\
		CLASS() CORE_DELETE;												\
		virtual ~CLASS() override;											\
																			\
	private:																\
		virtual void Init(void) override;									

#define NO_COPY(CLASS)														\
	private:																\
		CLASS()	CORE_DEFAULT;												\
		CLASS(const CLASS&) CORE_DELETE;									\
		CLASS(const CLASS&&)CORE_DELETE;									\
		CLASS& operator= (const CLASS&) CORE_DELETE;						\
		CLASS& operator= (const CLASS&&) CORE_DELETE;		

#define DECLARE_SINGLETON(CLASS)											\
	NO_COPY(CLASS)															\
																			\
	public:																	\
		static CLASS& GetInstance(void);									\
																			\
		friend std::unique_ptr<CLASS> std::make_unique<CLASS>();			\
		friend std::unique_ptr<CLASS>::deleter_type;						\
																			\
	private:																\
		void Init(void);													\
																			\
	private:																\
		static std::unique_ptr<CLASS> instance;								\
		static std::once_flag onceFlag;					

#define IMPLEMENT_SINGLETON(CLASS)											\
	std::unique_ptr<CLASS> CLASS::instance;									\
	std::once_flag CLASS::onceFlag;											\
																			\
	CLASS& CLASS::GetInstance(void)											\
	{																		\
		call_once(CLASS::onceFlag, []()										\
		{																	\
			instance = std::make_unique<CLASS>();							\
			(*(instance.get())).Init();										\
		});																	\
																			\
		return *(instance.get());											\
	}												

#define GET_INSTANCE(CLASS) CLASS::GetInstance()	