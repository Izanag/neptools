#include "gbnl.hpp"
#include "data.hpp"
#include "../context.hpp"
#include "../gbnl_lua.hpp"
#include "../raw_item.hpp"

#include <libshit/container/vector.lua.hpp>

namespace Neptools::Stcm
{

  void GbnlItem::Dispose() noexcept
  {
    if (auto ctx = GetContextMaybe())
      if (auto file = ctx.DynamicPointerCast<File>())
        file->UnsetGbnl(*this);
    Item::Dispose();
  }

  GbnlItem& GbnlItem::CreateAndInsert(ItemPointer ptr)
  {
    auto x = RawItem::GetSource(ptr, -1);
    return x.ritem.SplitCreate<GbnlItem>(ptr.offset, x.src);
  }

  static DataFactory factory{[](DataItem& it)
  {
    auto child = dynamic_cast<RawItem*>(&it.GetChildren().front());
    if (child && child->GetSize() > sizeof(Gbnl::Header))
    {
      char buf[4];
      child->GetSource().Pread(child->GetSize() - sizeof(Gbnl::Header), buf, 4);
      if (memcmp(buf, "GBNL", 4) == 0)
      {
        GbnlItem::CreateAndInsert({child, 0});
        return true;
      }
    }
    return false;
  }};

}

LIBSHIT_STD_VECTOR_FWD(gbnl_struct, Neptools::Gbnl::StructPtr);
#include "gbnl.binding.hpp"
