#include "stdafx.h"
#include "Reporter.h"

#include <MathBase/ClassHandler.h>
#include <MathBase/Cluster.h>
#include <MathBase/IDivisionFunction.h>
#include <MathBase/math/Vector3D.h>

#include <algorithm>

#include <iomanip>      
#include <sstream>
#include <clocale>
#include <locale>

namespace
{

	void _Calculate(std::wstring& o_state_string, MathBase::TClasses& i_clusters)
	{
		std::wstringstream stream;
		size_t number = 0;
	}

} // namespace

namespace Algorithms
{

	namespace Reporters
	{
		Reporter::Reporter(size_t i_precision)
			: m_state(L"")
			, m_precision(i_precision)
		{
		}

		Reporter::~Reporter()
		{

		}

		std::wstring Reporter::GetState(const std::wstring& i_new_line_pattern /*= L"\n"*/, const std::wstring& i_offset_pattern /*= L"  "*/, const std::wstring i_delimiter /*= L":"*/) const
		{
			std::wstring state = L"";
			for (size_t i = 0; i < m_state.size(); ++i)
			{
				if (m_state[i] == Delimiter)
					state += i_delimiter;
				else if (m_state[i] == Offset)
					state += i_offset_pattern;
				else if (m_state[i] == NewLine)
					state += i_new_line_pattern;
				else
					state += m_state[i];
			}
			return std::move(state);
		}

		bool Reporter::IsEmpty() const
		{
			return m_state.empty();
		}

		void Reporter::Flush()
		{
			m_state.clear();
		}

		size_t Reporter::Precision() const
		{
			return m_precision;
		}

		void Reporter::Precision(size_t i_new_precision)
		{
			m_precision = i_new_precision;
		}

	} // Reporters

	std::string convert_to_string(const std::wstring& ws)
	{
		if (ws.empty())
			return std::string{};
		std::setlocale(LC_ALL, "");
		const std::locale locale("");
		typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
		const converter_type& converter = std::use_facet<converter_type>(locale);
		std::vector<char> to(ws.length() * converter.max_length());
		std::mbstate_t state;
		const wchar_t* from_next;
		char* to_next;
		const converter_type::result result = converter.out(state, ws.data(), ws.data() + ws.length(), from_next, &to[0], &to[0] + to.size(), to_next);
		if (result == converter_type::ok || result == converter_type::noconv) {
			const std::string s(&to[0], to_next);
			return s;
		}
		return std::string{};
	}

} // Algorithms