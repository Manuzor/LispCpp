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

		CUT_FORCE_INLINE
		StringRef() :
			m_pCString(""),
			m_type(CString)
		{
		}
		
		CUT_FORCE_INLINE
		StringRef(const char* szString) :
			m_pCString(szString),
			m_type(CString)
		{
		}

		CUT_FORCE_INLINE
		StringRef(const std::string& stdString) :
			m_pStdString(&stdString),
			m_type(StdString)
		{
		}

		CUT_FORCE_INLINE
		StringRef(const StringRef& rhs) :
			m_pAll(rhs.m_pAll),
			m_type(rhs.m_type)
		{
		}

		CUT_FORCE_INLINE
		StringRef& operator = (const StringRef& rhs)
		{
			m_pAll = rhs.m_pAll;
			m_type = rhs.m_type;

			return *this;
		}

		CUT_FORCE_INLINE
		const char* cString() const
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

		CUT_FORCE_INLINE
		bool valid() const
		{
			return m_pAll != nullptr;
		}

		CUT_FORCE_INLINE
		bool empty() const
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

		CUT_FORCE_INLINE
		operator bool() const
		{
			return valid();
		}

		CUT_FORCE_INLINE
		operator const char*() const
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
	CUT_FORCE_INLINE
	bool operator == (const StringRef& lhs, const StringRef& rhs)
	{
		if (lhs.valid() && rhs.valid())
		{
			return strcmp(lhs, rhs) == 0;
		}
		return false;
	}
	
}
