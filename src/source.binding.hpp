// Auto generated code, do not edit. See gen_binding in project root.
#include "lua/user_type.hpp"

namespace Neptools
{
namespace Lua
{

// class neptools.source
template<>
void TypeRegister::DoRegister<Neptools::Source>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld
        .ValueDtor<Neptools::Source>()

        .Add<
            decltype(&::Neptools::Lua::ValueObjectCtorWrapper<Neptools::Source, const Neptools::Source &,FilePosition,FilePosition>), &::Neptools::Lua::ValueObjectCtorWrapper<Neptools::Source, const Neptools::Source &,FilePosition,FilePosition>
        >("new")
        .Add<
            Neptools::Source (*)(boost::filesystem::path), &Neptools::Source::FromFile
        >("from_file")
        .Add<
            void (Neptools::Source::*)(FilePosition,FilePosition), &Neptools::Source::Slice<Check::Throw>
        >("slice")
        .Add<
            FilePosition (Neptools::Source::*)() const, &Neptools::Source::GetOffset
        >("get_offset")
        .Add<
            FilePosition (Neptools::Source::*)() const, &Neptools::Source::GetOrigSize
        >("get_orig_size")
        .Add<
            const boost::filesystem::path & (Neptools::Source::*)() const, &Neptools::Source::GetFileName
        >("get_file_name")
        .Add<
            FilePosition (Neptools::Source::*)() const, &Neptools::Source::GetSize
        >("get_size")
        .Add<
            void (Neptools::Source::*)(FilePosition), &Neptools::Source::Seek<Check::Throw>
        >("seek")
        .Add<
            FilePosition (Neptools::Source::*)() const, &Neptools::Source::Tell
        >("tell")
        .Add<
            FilePosition (Neptools::Source::*)() const, &Neptools::Source::GetRemainingSize
        >("get_remaining_size")
        .Add<
            bool (Neptools::Source::*)() const, &Neptools::Source::Eof
        >("eof")
        .Add<
            void (Neptools::Source::*)(FilePosition) const, &Neptools::Source::CheckSize
        >("check_size")
        .Add<
            void (Neptools::Source::*)(FilePosition) const, &Neptools::Source::CheckRemainingSize
        >("check_remaining_size")
        .Add<
            uint8_t (Neptools::Source::*)(), &Neptools::Source::ReadLittleUint8<Check::Throw>
        >("read_little_uint8")
        .Add<
            uint8_t (Neptools::Source::*)(FilePosition) const, &Neptools::Source::PreadLittleUint8<Check::Throw>
        >("pread_little_uint8")
        .Add<
            uint16_t (Neptools::Source::*)(), &Neptools::Source::ReadLittleUint16<Check::Throw>
        >("read_little_uint16")
        .Add<
            uint16_t (Neptools::Source::*)(FilePosition) const, &Neptools::Source::PreadLittleUint16<Check::Throw>
        >("pread_little_uint16")
        .Add<
            uint32_t (Neptools::Source::*)(), &Neptools::Source::ReadLittleUint32<Check::Throw>
        >("read_little_uint32")
        .Add<
            uint32_t (Neptools::Source::*)(FilePosition) const, &Neptools::Source::PreadLittleUint32<Check::Throw>
        >("pread_little_uint32")
        .Add<
            uint64_t (Neptools::Source::*)(), &Neptools::Source::ReadLittleUint64<Check::Throw>
        >("read_little_uint64")
        .Add<
            uint64_t (Neptools::Source::*)(FilePosition) const, &Neptools::Source::PreadLittleUint64<Check::Throw>
        >("pread_little_uint64")
        .Add<
            std::string (Neptools::Source::*)(), &Neptools::Source::ReadCString
        >("read_cstring")
        .Add<
            std::string (Neptools::Source::*)(FilePosition) const, &Neptools::Source::PreadCString
        >("pread_cstring")
        .Add<
            Lua::RetNum (*)(Lua::StateRef,Neptools::Source &,FileMemSize), &LuaRead
        >("read")
        .Add<
            Lua::RetNum (*)(Lua::StateRef,Neptools::Source &,FilePosition,FileMemSize), &LuaPread
        >("pread")
        ;

}
static TypeRegister::StateRegister<Neptools::Source> reg_neptools_source;

// class neptools.dumpable_source
template<>
void TypeRegister::DoRegister<Neptools::DumpableSource>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld
        .Inherit<Neptools::DumpableSource, Neptools::Dumpable, Neptools::Source>()

        .Add<
            decltype(&::Neptools::MakeSmart<Neptools::DumpableSource, const Neptools::Source &>), &::Neptools::MakeSmart<Neptools::DumpableSource, const Neptools::Source &>
        >("new")
        ;

}
static TypeRegister::StateRegister<Neptools::DumpableSource> reg_neptools_dumpable_source;

}
}