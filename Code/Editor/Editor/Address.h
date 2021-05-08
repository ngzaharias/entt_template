#pragma once

namespace editor
{
	class Address
	{
		using Positions = Array<size_t>;

	public:
		void Push(int32 index);
		void Push(const str::StringView& string);
		void Pop();

		const char* GetFull() const { return m_Full.c_str(); }
		const char* GetLast() const { return m_Last.c_str(); }

	private:
		str::String m_Full;
		str::String m_Last;
		Positions m_Positions;
	};
}