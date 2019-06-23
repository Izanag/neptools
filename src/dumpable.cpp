#include "dumpable.hpp"

#include "sink.hpp"

#include <libshit/platform.hpp>

#include <boost/filesystem/operations.hpp>
#include <fstream>

#if LIBSHIT_OS_IS_WINDOWS
#  include <vector>
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#include <windows.h>

namespace
{
  struct DeleteOnExitHelper
  {
    ~DeleteOnExitHelper()
    {
      for (auto& p : pths)
        DeleteFileW(p.c_str());
    }

    std::vector<boost::filesystem::path> pths;
  };
  void DeleteOnExit(boost::filesystem::path pth)
  {
    static DeleteOnExitHelper hlp;
    hlp.pths.push_back(std::move(pth));
  }
}
#endif

namespace Neptools
{

  Libshit::NotNullSharedPtr<TxtSerializable>
  Dumpable::GetDefaultTxtSerializable(
    const Libshit::NotNullSharedPtr<Dumpable>& thiz)
  { LIBSHIT_THROW(Libshit::DecodeError, "Not txt-serializable file"); }

  void Dumpable::Dump(const boost::filesystem::path& path) const
  {
#if LIBSHIT_OS_IS_VITA
    // no unique_path on vita
    Dump(*Sink::ToFile(path, GetSize()));
#else
    auto path2 = path;
    {
      auto sink = Sink::ToFile(path2+=boost::filesystem::unique_path(), GetSize());
      Dump(*sink);
    }

#if LIBSHIT_OS_IS_WINDOWS
    if (LIBSHIT_OS_IS_WINDOWS && boost::filesystem::is_regular_file(path))
    {
      auto path3 = path;
      boost::filesystem::rename(path, path3+=boost::filesystem::unique_path());
      if (!DeleteFileW(path3.c_str()) && GetLastError() == ERROR_ACCESS_DENIED)
        DeleteOnExit(std::move(path3));
    }
#endif
    boost::filesystem::rename(path2, path);
#endif
  }

  void Dumpable::Inspect(const boost::filesystem::path& path) const
  {
    return Inspect(OpenOut(path));
  }

  std::ostream& Dumpable::Indent(std::ostream& os, unsigned indent)
  {
    std::ostreambuf_iterator<char> it{os};
    for (size_t i = 0; i < 2*indent; ++i) *it = ' ';
    return os;
  }


  std::ostream& operator<<(std::ostream& os, const Dumpable& dmp)
  {
    dmp.Inspect(os);
    return os;
  }

  std::string Dumpable::Inspect() const
  {
    std::stringstream ss;
    Inspect(ss);
    return ss.str();
  }

}

#include "dumpable.binding.hpp"
