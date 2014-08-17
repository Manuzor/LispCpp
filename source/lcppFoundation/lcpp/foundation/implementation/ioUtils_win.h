
void
lcpp::getCurrentWorkingDirectory(ezStringBuilder& out_path)
{
    static const unsigned long bufferSize = MAX_PATH;
    static wchar_t buffer[bufferSize] = { L'\0' };

    out_path.Clear();

    if(ezStringUtils::IsNullOrEmpty(buffer))
    {
        auto dirNameLength = GetCurrentDirectoryW(bufferSize, buffer);

        if (dirNameLength == 0)
        {
            // TODO throw proper exception.
            EZ_REPORT_FAILURE("Couldn't get current working dir.");
        }
        
        if (dirNameLength > bufferSize - 1)
        {
            // TODO throw proper exception.
            EZ_REPORT_FAILURE("buffer not big enough.");
        }
        
        buffer[dirNameLength] = '\0';
    }

    out_path.Append(buffer);
}
