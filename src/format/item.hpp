#ifndef UUID_294A7F35_D9EC_4A07_961F_A74307C4FA29
#define UUID_294A7F35_D9EC_4A07_961F_A74307C4FA29
#pragma once

#include "item_base.hpp"
#include "../check.hpp"
#include "../dumpable.hpp"
#include "../shared_ptr.hpp"
#include "../container/parent_list.hpp"

#include <iosfwd>
#include <vector>
#include <map>
#include <boost/intrusive/set.hpp>

namespace Neptools
{

class ItemWithChildren;
struct ItemListTraits;

NEPTOOLS_GEN_EXCEPTION_TYPE(InvalidItemState, std::logic_error);

class Item : public RefCounted, public Dumpable, public ParentListBaseHook<>
{
    NEPTOOLS_LUA_CLASS;
protected:
    struct Key {};
public:
    // do not change Context* to Weak/Shared ptr
    // otherwise Context's constructor will try to construct a WeakPtr before
    // RefCounted's constructor is finished, making an off-by-one error and
    // freeing the context twice
    NEPTOOLS_NOLUA explicit Item(
        Key, Context* ctx, FilePosition position = 0) noexcept
        : position{position}, context{ctx} {}
    Item(const Item&) = delete;
    void operator=(const Item&) = delete;
    virtual ~Item();

    RefCountedPtr<Context> GetContextMaybe() noexcept
    { return context.lock(); }
    NotNull<RefCountedPtr<Context>> GetContext() noexcept
    { return NotNull<RefCountedPtr<Context>>{context}; }
    NEPTOOLS_NOLUA Context& GetUnsafeContext() noexcept
    { return *context.unsafe_get(); }
    ItemWithChildren* GetParent() noexcept;

    NEPTOOLS_NOLUA RefCountedPtr<const Context> GetContextMaybe() const noexcept
    { return context.lock(); }
    NEPTOOLS_NOLUA NotNull<RefCountedPtr<const Context>>
    GetContext() const noexcept
    { return NotNull<RefCountedPtr<const Context>>{context}; }
    NEPTOOLS_NOLUA const Context& GetUnsafeContext() const noexcept
    { return *context.unsafe_get(); }
    NEPTOOLS_NOLUA const ItemWithChildren* GetParent() const noexcept;
    NEPTOOLS_NOLUA auto Iterator() const noexcept;
    NEPTOOLS_NOLUA auto Iterator() noexcept;

    FilePosition GetPosition() const noexcept { return position; }

    template <typename Checker = Check::Assert>
    void Replace(const NotNull<RefCountedPtr<Item>>& nitem) noexcept
    {
        NEPTOOLS_CHECK(InvalidItemState, GetParent(), "no parent");
        if constexpr (!Checker::IS_NOP)
        {
            auto nsize = nitem->GetSize();
            for (auto& l : labels)
                NEPTOOLS_CHECK(InvalidItemState, l.ptr.offset <= nsize,
                               "would invalidate labels");
        }
        NEPTOOLS_CHECK(InvalidItemState, nitem->labels.empty(),
                       "new item has labels");
        Replace_(nitem);
    }

    // properties needed: none (might help if ordered)
    // update Slice if no longer ordered
    using LabelsContainer = boost::intrusive::multiset<
        Label,
        boost::intrusive::base_hook<LabelOffsetHook>,
        boost::intrusive::constant_time_size<false>,
        boost::intrusive::key_of_value<LabelOffsetKeyOfValue>>;
    NEPTOOLS_NOLUA
    const LabelsContainer& GetLabels() const { return labels; }

    void Dispose() noexcept override;

protected:
    void UpdatePosition(FilePosition npos);

    void Inspect_(std::ostream& os) const override = 0;

    using SlicePair = std::pair<NotNull<RefCountedPtr<Item>>, FilePosition>;
    using SliceSeq = std::vector<SlicePair>;
    void Slice(SliceSeq seq);

    FilePosition position;

private:
    WeakRefCountedPtr<Context> context;

    LabelsContainer labels;

    void Replace_(const NotNull<RefCountedPtr<Item>>& nitem) noexcept;

    friend class Context;
    friend struct ItemListTraits;
    friend class ItemWithChildren;
};
NEPTOOLS_STATIC_ASSERT(
    std::is_same<SmartPtr<Item>, RefCountedPtr<Item>>::value);

std::ostream& operator<<(std::ostream& os, const Item& item);
inline FilePosition ToFilePos(ItemPointer ptr) noexcept
{ return ptr.item->GetPosition() + ptr.offset; }

using ItemList = ParentList<Item, ItemListTraits>;
struct ItemListTraits
{
    static void add(ItemList&, Item& item) noexcept
    { item.AddRef(); }
    static void remove(ItemList&, Item& item) noexcept
    { item.RemoveRef(); }
};

inline auto Item::Iterator() const noexcept
{ return ItemList::s_iterator_to(*this); }
inline auto Item::Iterator() noexcept
{ return ItemList::s_iterator_to(*this); }

class ItemWithChildren : public Item, private ItemList
{
    NEPTOOLS_LUA_CLASS;
public:
    using Item::Item;

    NEPTOOLS_NOLUA ItemList& GetChildren() noexcept { return *this; }
    NEPTOOLS_NOLUA const ItemList& GetChildren() const noexcept { return *this; }

    FilePosition GetSize() const override;
    void Fixup() override { Fixup_(0); }

    NEPTOOLS_NOLUA void MoveNextToChild(size_t size) noexcept;

    void Dispose() noexcept override;

protected:
    void Dump_(Sink& sink) const override;
    void Inspect_(std::ostream& sink) const override;
    void Fixup_(FilePosition offset);

    friend struct ::Neptools::ItemListTraits;
    friend class Item;
};

inline ItemWithChildren* Item::GetParent() noexcept
{ return static_cast<ItemWithChildren*>(ItemList::opt_get_parent(*this)); }
inline const ItemWithChildren* Item::GetParent() const noexcept
{ return static_cast<const ItemWithChildren*>(ItemList::opt_get_parent(*this)); }

}
#endif
