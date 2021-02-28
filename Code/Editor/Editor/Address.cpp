#include "EditorPCH.h"
#include "Editor/Address.h"

void editor::Address::Push(int32 index)
{
	Push(std::to_string(index));
}

void editor::Address::Push(const str::StringView& string)
{
	m_Positions.push_back(m_Full.length());
	m_Last = string;
	m_Full += "/" + m_Last;
}

void editor::Address::Pop()
{
	const size_t last = m_Positions.back();
	m_Positions.pop_back();
	const size_t prev = !m_Positions.empty() ? m_Positions.back() + 1 : 0;
	m_Full.erase(last);
	m_Last = m_Full.substr(prev, last - prev);
}