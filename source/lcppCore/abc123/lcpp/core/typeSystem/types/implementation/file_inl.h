
inline
lcpp::SchemeFile::SchemeFile(const char* szFileName) :
    m_fileName(szFileName),
    m_file(),
    m_lastFileMode(ezFileMode::None)
{
}

inline
lcpp::SchemeFile::SchemeFile(const ezString& fileName) :
    m_fileName(fileName),
    m_file(),
    m_lastFileMode(ezFileMode::None)
{
}
inline
lcpp::SchemeFile::SchemeFile(const SchemeFile& toCopy) :
    m_fileName(toCopy.m_fileName),
    m_file(),
    m_lastFileMode(toCopy.m_lastFileMode)
{
    if(toCopy.isOpen())
    {
        doOpen();
    }
}

inline
lcpp::SchemeFile::~SchemeFile()
{
    m_file.Close();
}

inline
ezString
lcpp::SchemeFile::toString() const
{
    ezStringBuilder out;
    out.AppendFormat("<file:\"%s\">", m_fileName.GetData());
    return out;
}

inline
lcpp::Ptr<lcpp::SchemeBool>
lcpp::SchemeFile::open(Ptr<SchemeObject> pMode)
{
    if(isNil(pMode)) { m_lastFileMode = ezFileMode::Read; }
    else
    {
        auto pModeString = pMode.cast<SchemeString>();
        if     (pModeString->value().IsEqual("r")){ m_lastFileMode = ezFileMode::Read; }
        else if(pModeString->value().IsEqual("w")){ m_lastFileMode = ezFileMode::Write; }
        else if(pModeString->value().IsEqual("a")){ m_lastFileMode = ezFileMode::Append; }
    }

    return doOpen() ? SCHEME_TRUE_PTR : SCHEME_FALSE_PTR;
}

inline
bool
lcpp::SchemeFile::doOpen()
{
    bool forWriting = m_lastFileMode == ezFileMode::Write || m_lastFileMode == ezFileMode::Append;

    ezString absoluteFileName;
    ezFileSystem::ResolvePath(m_fileName.GetData(), forWriting, &absoluteFileName, nullptr);
    return m_file.Open(absoluteFileName.GetData(), m_lastFileMode).IsSuccess();
}

inline
lcpp::Ptr<lcpp::SchemeBool>
lcpp::SchemeFile::isOpen() const
{
    return m_file.IsOpen() ? SCHEME_TRUE_PTR : SCHEME_FALSE_PTR;
}

inline
void
lcpp::SchemeFile::close()
{
    m_file.Close();
}

inline
ezString
lcpp::SchemeFile::readString()
{
    auto size = m_file.GetFileSize();
    auto bufferSize = ezUInt32(size) + 1;

    auto buffer = new char[bufferSize];
    LCPP_SCOPE_EXIT{ delete[] buffer; };

    // null terminator
    buffer[bufferSize - 1] = '\0';

    ezStringBuilder content;
    content.Reserve(bufferSize);

    m_file.Read(buffer, bufferSize - 1);
    m_file.SetFilePosition(0, ezFilePos::FromStart);

    content.Append(buffer);
    return content;
}

inline
bool
lcpp::SchemeFile::operator ==(const SchemeObject& rhs) const
{
    if(rhs.is<SchemeFile>())
    {
        return *this == static_cast<const SchemeFile&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::SchemeFile::name() const
{
    return m_fileName;
}

inline
const ezOSFile&
lcpp::SchemeFile::file() const
{
    return m_file;
}

inline
bool
lcpp::operator ==(const SchemeFile& lhs, const SchemeFile& rhs)
{
    // Check if the handles are the same.
    return lhs.file().GetFileData().m_pFileHandle
        == rhs.file().GetFileData().m_pFileHandle;
}
