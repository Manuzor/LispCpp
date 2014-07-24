#pragma once
#include "cut/logging/log-manager.h"
#include "cut/logging/mode.h"

namespace cut
{
	class DefaultLogManager : public ILogManager
	{
	public:
		DefaultLogManager();
		virtual ~DefaultLogManager();

		virtual void logMessage(LogMode mode, StringRef formattedMessage) CUT_OVERRIDE;

		/// \remark This method closes the output file,
		///         if the new fileName is different than the existing one.
		virtual void setLogFileName(StringRef fileName) CUT_OVERRIDE;
		virtual const StringRef getLogFileName() const CUT_OVERRIDE;

	protected:
		virtual void blockBegin() CUT_OVERRIDE;
		virtual void blockEnd() CUT_OVERRIDE;

	private:
		void writeToStdOut(LogMode mode, const std::ostringstream& formattedString);
		void writeToFile(LogMode mode, const std::ostringstream& formattedMessage);

		virtual std::size_t getBlockIndentation() const override;
		virtual void setBlockIndentation(std::size_t value) override;

		std::ofstream m_file;
		std::string m_fileName;
		std::size_t m_blockLevel;
		std::size_t m_blockIndentation;
	};
}
