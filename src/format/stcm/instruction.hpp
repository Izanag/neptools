#ifndef UUID_032F4E0D_CAAE_429A_9928_2ACC24EC51E1
#define UUID_032F4E0D_CAAE_429A_9928_2ACC24EC51E1
#pragma once

#include "../item.hpp"
#include "../../check.hpp"
#include "../../source.hpp"

#include <boost/endian/arithmetic.hpp>
#include <variant>

namespace Neptools
{
class RawItem;

namespace Stcm
{

class InstructionItem final : public ItemWithChildren
{
    NEPTOOLS_DYNAMIC_OBJECT;
public:
    struct Header
    {
        boost::endian::little_uint32_t is_call;
        boost::endian::little_uint32_t opcode;
        boost::endian::little_uint32_t param_count;
        boost::endian::little_uint32_t size;

        enum Opcode : uint32_t
        {
            USER_OPCODES = 0x1000,
            SYSTEM_OPCODES_BEGIN = 0xffffff00,
            SYSTEM_OPCODES_END   = 0xffffff14,
        };

        void Validate(FilePosition file_size) const;
    };
    NEPTOOLS_STATIC_ASSERT(sizeof(Header) == 0x10);

    struct Parameter
    {
        struct Type0
        {
            static constexpr uint32_t MEM_OFFSET = 0;
            static constexpr uint32_t UNK = 1;
            static constexpr uint32_t INDIRECT = 2;
            static constexpr uint32_t SPECIAL = 3;
        };

        struct Type0Special
        {
            static constexpr uint32_t READ_STACK_MIN = 0xffffff00;
            static constexpr uint32_t READ_STACK_MAX = 0xffffff0f;
            static constexpr uint32_t READ_4AC_MIN   = 0xffffff20;
            static constexpr uint32_t READ_4AC_MAX   = 0xffffff27;
            static constexpr uint32_t INSTR_PTR0     = 0xffffff40;
            static constexpr uint32_t INSTR_PTR1     = 0xffffff41;
            static constexpr uint32_t COLL_LINK      = 0xffffff42;
        };

        struct Type48
        {
            static constexpr uint32_t MEM_OFFSET = 0;
            static constexpr uint32_t IMMEDIATE = 1;
            static constexpr uint32_t INDIRECT = 2;
            static constexpr uint32_t SPECIAL = 3;
        };

        struct Type48Special
        {
            static constexpr uint32_t READ_STACK_MIN = 0xffffff00;
            static constexpr uint32_t READ_STACK_MAX = 0xffffff0f;
            static constexpr uint32_t READ_4AC_MIN   = 0xffffff20;
            static constexpr uint32_t READ_4AC_MAX   = 0xffffff27;
        };

        boost::endian::little_uint32_t param_0;
        boost::endian::little_uint32_t param_4;
        boost::endian::little_uint32_t param_8;

        static constexpr inline uint32_t TypeTag(uint32_t x) { return x >> 30; }
        static constexpr inline uint32_t Value(uint32_t x) { return x & 0x3fffffff; }
        static constexpr inline uint32_t Tag(uint32_t tag, uint32_t val)
        { return (tag << 30) | val; }
        void Validate(FilePosition file_size) const;
    };
    NEPTOOLS_STATIC_ASSERT(sizeof(Parameter) == 0xc);

    InstructionItem(Key k, Context* ctx) : ItemWithChildren{k, ctx} {}
    InstructionItem(Key k, Context* ctx, Source src);
    static InstructionItem& CreateAndInsert(ItemPointer ptr);

    FilePosition GetSize() const noexcept override;
    void Fixup() override;
    void Dispose() noexcept override;

    bool IsCall() const noexcept
    { return !std::holds_alternative<uint32_t>(opcode_target); }
    uint32_t GetOpcode() const { return std::get<0>(opcode_target); }
    void SetOpcode(uint32_t oc) noexcept { opcode_target = oc; }
    NotNull<LabelPtr> GetTarget() const { return std::get<1>(opcode_target); }
    void SetTarget(NotNull<LabelPtr> label) noexcept { opcode_target = label; }

    class Param48
    {
    public:
        enum class Type
        {
            MEM_OFFSET,
            IMMEDIATE,
            INDIRECT,
            READ_STACK,
            READ_4AC,
        };
        using Variant = std::variant<
            NotNull<LabelPtr>, uint32_t, uint32_t, uint32_t, uint32_t>;

        //Param48() : val{std::in_place_index<Type::IMMEDIATE>, 0} {}
        Param48(Context& ctx, uint32_t val) : val{GetVariant(ctx, val)} {}
        template <size_t type, typename T>
        Param48(std::in_place_index_t<type> x, T val) : val{x, std::move(val)} {}

        uint32_t Dump() const noexcept;

        Type GetType() const noexcept { return static_cast<Type>(val.index()); }

        template <typename Visitor>
        auto Visit(Visitor&& v) const
        { return std::visit(std::forward<Visitor>(v), val); }

        template <Type type>
        auto Get() const { return std::get<static_cast<size_t>(type)>(val); }
        template <Type type>
        void Set(std::variant_alternative_t<static_cast<size_t>(type), Variant> nval)
        { val.emplace(std::in_place_index<type>(std::move(nval))); }

        template <Type type>
        static Param48 New(std::variant_alternative_t<static_cast<size_t>(type), Variant> nval)
        { return {std::in_place_index<type>, std::move(nval)}; }


        // NEPTOOLS_GEN_INT(Immediate, IMMEDIATE,  Param48, Parameter::TypeTag(val) == 0)
        // NEPTOOLS_GEN_INT(Indirect,  INDIRECT,   Param48, Parameter::TypeTag(val) == 0)
        // NEPTOOLS_GEN_INT(ReadStack, READ_STACK, Param48, val < 16)
        // NEPTOOLS_GEN_INT(Read4ac,   READ_4AC,   Param48, val < 16)

    private:
        Variant val;
        static Variant GetVariant(Context& ctx, uint32_t val);
    };

    class Param
    {
    public:
        struct MemOffset
        {
            NotNull<LabelPtr> target;
            Param48 param_4;
            Param48 param_8;
        };
        struct Indirect
        {
            uint32_t param_0;
            Param48 param_8;
        };
        enum class Type
        {
            MEM_OFFSET,
            INDIRECT,
            READ_STACK,
            READ_4AC,
            INSTR_PTR0,
            INSTR_PTR1,
            COLL_LINK,
        };
        using Variant = std::variant<
            MemOffset, Indirect, uint32_t, uint32_t, NotNull<LabelPtr>,
            NotNull<LabelPtr>, NotNull<LabelPtr>>;

        Param(Context& ctx, const Parameter& p) : val{GetVariant(ctx, p)} {}
        template <size_t type, typename T>
        Param(std::in_place_index_t<type> x, T val) : val{x, std::move(val)} {}

        void Dump(Sink& sink) const;
        void Dump(Sink&& sink) const { Dump(sink); }

        Type GetType() const noexcept { return static_cast<Type>(val.index()); }

        template <typename Visitor>
        auto Visit(Visitor&& v) const
        { return std::visit(std::forward<Visitor>(v), val); }

        template <Type type>
        auto Get() const { return std::get<static_cast<size_t>(type)>(val); }
        template <Type type>
        void Set(std::variant_alternative_t<static_cast<size_t>(type), Variant> nval)
        { val.emplace(std::in_place_index<type>(std::move(nval))); }

        template <Type type>
        static Param New(std::variant_alternative_t<static_cast<size_t>(type), Variant> nval)
        { return {std::in_place_index<type>, std::move(nval)}; }


        // NEPTOOLS_GEN_LABEL(MemOffset, MEM_OFFSET, Param)
        // NEPTOOLS_GEN_INT  (Indirect,  INDIRECT,   Param, Parameter::TypeTag(val) == 0)
        // NEPTOOLS_GEN_INT  (ReadStack, READ_STACK, Param, val < 16)
        // NEPTOOLS_GEN_INT  (Read4ac,   READ_4AC,   Param, val < 16)
        // NEPTOOLS_GEN_LABEL(InstrPtr0, INSTR_PTR0, Param)
        // NEPTOOLS_GEN_LABEL(InstrPtr1, INSTR_PTR1, Param)
        // NEPTOOLS_GEN_LABEL(CollLink,  COLL_LINK,  Param)

    private:
        Variant val;
        static Variant GetVariant(Context& ctx, const Parameter& in);
    };
    std::vector<Param> params;

private:
    std::variant<uint32_t, NotNull<LabelPtr>> opcode_target;

    void Dump_(Sink& sink) const override;
    void Inspect_(std::ostream& os) const override;
    void Parse_(Source& src);
};

std::ostream& operator<<(std::ostream& os, const InstructionItem::Param48& p);
std::ostream& operator<<(std::ostream& os, const InstructionItem::Param& p);

}
}
#endif
