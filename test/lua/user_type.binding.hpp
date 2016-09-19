// Auto generated code, do not edit. See gen_binding in project root.
#include "lua/user_type.hpp"

namespace Neptools
{
namespace Lua
{

// class foo
template<>
void TypeRegister::DoRegister<Foo>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld
        .Inherit<Foo, Neptools::Lua::DynamicObject>()

        .Add<
            void (Foo::*)(int), &Foo::DoIt
        >("do_it")
        .Add<
            decltype(&::Neptools::MakeSmart<Foo>), &::Neptools::MakeSmart<Foo>
        >("new")
        ;

}
static TypeRegister::StateRegister<Foo> reg_foo;

}
}

const char Foo::TYPE_NAME[] = "foo";

namespace Neptools
{
namespace Lua
{

// class bar.baz.asdfgh
template<>
void TypeRegister::DoRegister<Bar::Baz::Asdfgh>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld
        .Inherit<Bar::Baz::Asdfgh, Neptools::Lua::DynamicObject>()

        .Add<
            decltype(&::Neptools::MakeSmart<Bar::Baz::Asdfgh>), &::Neptools::MakeSmart<Bar::Baz::Asdfgh>
        >("new")
        ;

}
static TypeRegister::StateRegister<Bar::Baz::Asdfgh> reg_bar_baz_asdfgh;

}
}

const char Bar::Baz::Asdfgh::TYPE_NAME[] = "bar.baz.asdfgh";

namespace Neptools
{
namespace Lua
{

// class baz
template<>
void TypeRegister::DoRegister<Baz>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld
        .Inherit<Baz, Neptools::Lua::DynamicObject>()

        .Add<
            decltype(&::Neptools::MakeSmart<Baz>), &::Neptools::MakeSmart<Baz>
        >("new")
        .Add<
            void (Baz::*)(int), &Baz::SetGlobal
        >("set_global")
        .Add<
            int (Baz::*)(), &Baz::GetRandom
        >("get_random")
        ;

}
static TypeRegister::StateRegister<Baz> reg_baz;

}
}

const char Baz::TYPE_NAME[] = "baz";
