
namespace lcpp
{
    inline
    ezStreamWriterBase& operator << (ezStreamWriterBase& Stream, const ezStringBuilder& builder)
    {
        Stream.WriteBytes(reinterpret_cast<const ezUInt8*>(builder.GetData()), builder.GetElementCount());
        return Stream;
    }
}
