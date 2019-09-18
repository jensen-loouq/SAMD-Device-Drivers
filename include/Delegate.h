#ifndef __MBD_DELEGATE_H__
#define __MBD_DELEGATE_H__

namespace mbd
{
	template <typename T>
	class delegate;		
}



namespace mbd
{
	template <typename RETTy_, typename... ARGS>
	class delegate<RETTy_(ARGS...)>
	{
	public:
		using delegateType = RETTy_(*)(ARGS...);

	//
	//	Constructors
	//
	public:
		/// Construct from function reference
		constexpr delegate(delegateType delegateFunction) : m_delegateFunction(delegateFunction) {}

		/// Construct from other delegate; copy function reference
		constexpr delegate(const delegate& other) : m_delegateFunction(other.m_delegateFunction) {}

		/// Construct from other delegate; copy function reference
		constexpr delegate(delegate&& other) : m_delegateFunction(other.m_delegateFunction) {}

		/// Copy Assignment
		delegate& operator=(const delegate& other) { m_delegateFunction = other.m_delegateFunction; }

		/// Function Operator
		operator delegateType() { return m_delegateFunction; }

	private:
		delegateType m_delegateFunction;
	};	
}


#define DELEGATE_DECLARATION(DELEGATE_NAME, FN_SIGNATURE) using DELEGATE_NAME = mbd::delegate<FN_SIGNATURE>
#endif // !__MBD_EVENT_H__
