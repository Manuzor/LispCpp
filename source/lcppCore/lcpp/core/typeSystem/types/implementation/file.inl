
inline
lcpp::LispFile::LispFile(const char* szFileName) :
    m_fileName(szFileName),
    m_file(),
    m_lastFileMode(ezFileMode::None)
{
}

inline
lcpp::LispFile::LispFile(const ezString& fileName) :
    m_fileName(fileName),
    m_file(),
    m_lastFileMode(ezFileMode::None)
{
}
inline
lcpp::LispFile::LispFile(const LispFile& toCopy) :
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
lcpp::LispFile::~LispFile()
{
    m_file.Close();
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispFile::copy() const
{
    return copy(*this);
}

inline
const lcpp::Type&
lcpp::LispFile::type() const
{
    return LispFile::typeInfo();
}

inline
ezString
lcpp::LispFile::toString() const
{
    ezStringBuilder out;
    out.AppendFormat("<file:\"%s\">", m_fileName.GetData());
    return out;
}

inline
lcpp::Ptr<lcpp::LispBool>
lcpp::LispFile::open(Ptr<LispObject> pMode)
{
    if(isNil(pMode)) { m_lastFileMode = ezFileMode::Read; }
    else
    {
        auto pModeString = pMode.cast<LispString>();
        if     (pModeString->value().IsEqual("r")){ m_lastFileMode = ezFileMode::Read; }
        else if(pModeString->value().IsEqual("w")){ m_lastFileMode = ezFileMode::Write; }
        else if(pModeString->value().IsEqual("a")){ m_lastFileMode = ezFileMode::Append; }
    }

    return LispBool::create(doOpen());
}

inline
bool
lcpp::LispFile::doOpen()
{
    bool forWriting = m_lastFileMode == ezFileMode::Write || m_lastFileMode == ezFileMode::Append;

    ezString absoluteFileName;
    ezFileSystem::ResolvePath(m_fileName.GetData(), forWriting, &absoluteFileName, nullptr);
    return m_file.Open(absoluteFileName.GetData(), m_lastFileMode).Succeeded();
}

inline
lcpp::Ptr<lcpp::LispBool>
lcpp::LispFile::isOpen() const
{
    return LispBool::create(m_file.IsOpen());
}

inline
void
lcpp::LispFile::close()
{
    m_file.Close();
}

inline
ezString
lcpp::LispFile::readString()
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
lcpp::LispFile::operator ==(const LispObject& rhs) const
{
    if(rhs.is<LispFile>())
    {
        return *this == static_cast<const LispFile&>(rhs);
    }
    return false;
}

inline
const ezString&
lcpp::LispFile::name() const
{
    return m_fileName;
}

inline
const ezOSFile&
lcpp::LispFile::file() const
{
    return m_file;
}

inline
bool
lcpp::operator ==(const LispFile& lhs, const LispFile& rhs)
{
    // Check if the handles are the same.
    return lhs.file().GetFileData().m_pFileHandle
        == rhs.file().GetFileData().m_pFileHandle;
}
