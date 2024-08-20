#pragma 
namespace glash
{
	class SomeClass
	{
		int m_Val = 0;
	public:
		constexpr int GetVal() const
		{
			return m_Val;
		}

		void print() const;
	};

	constexpr int return_two()
	{
		return 2;
	}
}