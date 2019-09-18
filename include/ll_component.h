#ifndef COREZERO_LL_COMPONENT_H
#define COREZERO_LL_COMPONENT_H



namespace ll_system
{
	class system_component
	{
	public:
		virtual void Initialize() noexcept = 0;
	};
}

#endif