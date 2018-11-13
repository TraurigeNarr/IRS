#pragma once

#include <MathBase/Types.h>

#include <sstream>

namespace Algorithms
{
	namespace Reporters
	{
		const wchar_t Delimiter = L':';
		const wchar_t Offset = 1;
		const wchar_t NewLine = L'\n';

		class Reporter
		{
		private:
			std::wstring m_state;
			size_t m_precision;
		public:
			Reporter(size_t i_precision = 3);
			~Reporter();

			template <typename T>
			bool Report(const T& i_data);

			template <typename... T>
			void ReportMultiple(const T&... data)
			{
				bool x[] = { Report<T>(data)... };
			}

			std::wstring GetState(const std::wstring& i_new_line_pattern = L"\n", const std::wstring& i_offset_pattern = L"  ", const std::wstring i_delimiter = L":") const;
			template <typename TStream>
			TStream& Write(TStream& io_stream, const std::wstring& i_new_line_pattern = L"\n", const std::wstring& i_offset_pattern = L"  ", const std::wstring i_delimiter = L":") const;
			bool IsEmpty() const;

			void Flush();
			size_t Precision() const;
			void Precision(size_t i_new_precision);
		};

		template <typename T>
		bool Reporter::Report(const T& i_data)
		{
			std::wstringstream stream;
			stream.precision(m_precision);
			stream << i_data;
			m_state += stream.str();
			return true;
		}

		template <typename TStream>
		TStream& Reporter::Write(TStream& io_stream, const std::wstring& i_new_line_pattern /*= L"\n"*/, const std::wstring& i_offset_pattern /*= L"  "*/, const std::wstring i_delimiter /*= L":"*/) const
		{
			io_stream << GetState(i_new_line_pattern, i_offset_pattern, i_delimiter).c_str();

			return io_stream;
		}

		//////////////////////////////////////////////////////////////////////////

		struct IterationData
		{

		};

		class ClusterReporter
		{
		public:
			ClusterReporter();
			~ClusterReporter();

			void Report(MathBase::TPointCloud& i_points, MathBase::TClusters& i_clusters);
		};


	}

	std::string convert_to_string(const std::wstring& ws);
}