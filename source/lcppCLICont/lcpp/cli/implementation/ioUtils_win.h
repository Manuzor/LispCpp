
lcpp::String
lcpp::getCurrentWorkingDirectory()
{
    static auto workingDirectory = String();

    if (workingDirectory.IsEmpty())
    {
        auto bufferSize = unsigned long(MAX_PATH);
        wchar_t buffer[MAX_PATH];

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
        workingDirectory = ezStringWChar(buffer).GetData();
    }

    return workingDirectory;
}
