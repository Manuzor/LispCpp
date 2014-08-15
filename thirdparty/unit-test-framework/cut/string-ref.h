#pragma once
#include "cut/debug-break.h"

namespace cut
{
	struct StringRef
	{
		enum Enum
		{
			CString,
			StdString,
		};

	public:

		inline StringRef(const char* szString) :
			m_pCString(szString),
			m_type(CString)
		{
		}

		inline StringRef(const std::string& stdString) :
			m_pStdString(&stdString),
			m_type(StdString)
		{
		}

		inline StringRef(const StringRef& rhs) :
			m_pAll(rhs.m_pAll),
			m_type(rhs.m_type)
		{
		}

		inline StringRef& operator = (const StringRef& rhs)
		{
			m_pAll = rhs.m_pAll;
			m_type = rhs.m_type;
		}

		inline const char* cString() const
		{
			switch(m_type)
			{
			case CString:   return m_pCString;
			case StdString: return m_pStdString->c_str();
			}

			debugBreak();
			throw std::exception("Invalid type for StringRef!");
			return nullptr;
		}

		inline bool valid() const
		{
			return m_pAll != nullptr;
		}

		inline bool empty() const
		{
			if(valid())
			{
				switch(m_type)
				{
				case CString:   return strlen(m_pCString) == 0;
				case StdString: return m_pStdString->empty();
				}
			}
			
			return true;
		}

		inline operator bool() const
		{
			return valid();
		}

		inline operator const char*() const
		{
			return cString();
		}

	private:
		union
		{
			const char* m_pCString;
			const std::string* m_pStdString;

			void* m_pAll;
		};

		Enum m_type;
	};

	// Compares string contents
	inline bool operator == (const StringRef& lhs, const StringRef& rhs)
	{
		if (lhs.valid() && rhs.valid())
		{
			return strcmp(lhs, rhs) == 0;
		}
		return false;
	}
	
}
