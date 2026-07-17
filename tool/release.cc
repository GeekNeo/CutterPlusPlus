/* Cutter++ — Interactive Modern C++ for Binary Analysis */
/* Copyright (c) 2026 Jesse Liu <neoliu2011@gmail.com> */
/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See LICENSE file in the root directory for full license text. */

/*
This is a C++ script to release the built Cutter++ files, it'll create a
Cutter++ release package in the following layout, Cutter++-vx.x.x-os-arch:
---Cutter++.so/dll    ; Cutter plugin shared library
---Cutter++
------icpp            ; icpp execution engine
------include         ; Cutter++ headers
------QtCore          ; QtCore headers
------QtGui           ; QtGui headers
------QtWidgets       ; QtWidgets headers

Usage: icpp release.cc /path/to/build /path/to/prefix

The initial Cutter++ package can be downloaded for your local system at:
https://github.com/GeekNeo/CutterPlusPlus/releases
*/

// for Cutter++ package version
#include "../include/Cutter++.h"

// for standard c++ definitions
import std;
namespace fs = std::filesystem;

#if __APPLE__
#define LIBEXT ".so"
#define DLLEXT ".dylib"
#define EXEEXT ""
constexpr const std::string_view platform = "apple";
constexpr const std::string_view osname = "macos";
#elif __linux__
#define LIBEXT ".so"
#define DLLEXT ".so"
#define EXEEXT ""
constexpr const std::string_view platform = "linux";
constexpr const std::string_view osname = "linux";
#else
#define LIBEXT ".dll"
#define DLLEXT ".dll"
#define EXEEXT ".exe"
constexpr const std::string_view platform = "win";
constexpr const std::string_view osname = "windows";
#endif

#if __aarch64__ || __arm64__
#if __linux__
constexpr const std::string_view arch = "aarch64";
#else
constexpr const std::string_view arch = "arm64";
#endif
#else
constexpr const std::string_view arch = "x86_64";
#endif

static auto log(const std::string &text) { std::puts(text.data()); }

#define log_return(text, stmt)                                                 \
  {                                                                            \
    log(text.data());                                                          \
    stmt;                                                                      \
  }

static auto create_dir(const fs::path &path) {
  if (fs::exists(path))
    return;
  if (!fs::create_directory(path))
    log_return(std::format("Failed to create directory: {}.", path.string()),
               return);
  log(std::format("Created directory {}.", path.string()));
}

static auto pack_file(const fs::path &srcfile, const fs::path &dstdir,
                      bool strip, std::string_view dstname = "") {
  if (!fs::exists(srcfile)) {
    log(std::format("There's no {}, ignored packing it.", srcfile.string()));
    return;
  }

  auto dstfile =
      (dstdir / (dstname.size() ? fs::path(dstname) : srcfile.filename()))
          .string();
#if __APPLE__ || __linux__
  if (strip) {
    std::system(
        std::format("strip -x {} -o {}", srcfile.string(), dstfile).data());
#if __APPLE__
    std::system(std::format("codesign --force --sign - {}", dstfile).data());
    log(std::format("Packed, stripped and codesigned file {}.", dstfile));
#else
    log(std::format("Packed and stripped file {}.", dstfile));
#endif
    return;
  }
#endif

  std::error_code err;
  fs::copy_file(srcfile, dstfile, fs::copy_options::overwrite_existing, err);
  if (err)
    log_return(std::format("Failed to copy file: {} ==> {}, {}.",
                           srcfile.string(), dstfile, err.message()),
               return);
  log(std::format("Packed file {}.", dstfile));
}

static auto pack_dir(const fs::path &srcdir, const fs::path &dstroot,
                     std::string_view dstname = "") {
  auto dstdir = dstroot / (dstname.size() ? dstname : srcdir.filename());
  if (!dstname.size() && fs::exists(dstdir) &&
      srcdir.filename().string().starts_with("Qt"))
    log_return(std::format("Ignored packing {}, {} exists.", srcdir.string(),
                           dstdir.string()),
               return);
  std::error_code err;
  auto source = srcdir;
  if (fs::is_symlink(srcdir))
    source = fs::canonical(srcdir, err);
  fs::copy(source, dstdir,
           fs::copy_options::overwrite_existing | fs::copy_options::recursive |
               fs::copy_options::copy_symlinks,
           err);
  if (err)
    log(std::format("Failed to copy directory: {} ==> {}, {}.", source.string(),
                    dstdir.string(), err.message()));
  else
    log(std::format("Packed directory {} from {}.", dstdir.string(),
                    source.string()));
}

int main(int argc, char **argv) {
  if (argc != 3)
    log_return(
        std::format("Usage: {} /path/to/build /path/to/prefix.", argv[0]),
        return -1);

  // where the built outputs placed
  auto srcroot = fs::path(argv[1]);
  // create the destination directory if necessary
  auto dstroot = fs::path(argv[2]);
  create_dir(dstroot);
  // create Cutter++ package layout
  auto pkgdir =
      std::format("cutter++-v{}.{}.{}-{}-{}", cpp::version_major,
                  cpp::version_minor, cpp::version_patch, osname, arch);
  auto cutterpproot = dstroot / pkgdir;
  auto deproot = cutterpproot / "Cutter++";
  create_dir(cutterpproot);
  create_dir(deproot);

  // copy Cutter++.so/dll files
  pack_file(srcroot / "Cutter++" LIBEXT, cutterpproot, true);

  // copy dependent directories
  std::vector<std::string_view> depdirs = {
      "icpp", "include", "QtCore", "QtGui", "QtWidgets",
  };
  for (auto &name : depdirs) {
    auto srcdir = srcroot / "Cutter++" / name;
    auto dstdir = deproot / name;
    if (fs::exists(srcdir))
      pack_dir(srcdir, deproot);
    else
      log(std::format("There's no {}, ignored packing it.", srcdir.string()));

    // remove unused icpp files
    if (name == "icpp") {
      auto bindir = dstdir / "bin";
      std::vector<std::string_view> unused = {
          "icpp-gadget" DLLEXT, "icpp-server" EXEEXT, "imod" EXEEXT,
          "iopad" EXEEXT,       "clang" EXEEXT,       "clang++" EXEEXT};
      for (auto &file : unused)
        fs::remove(bindir / file);
    }
  }

  auto targz = pkgdir + ".tar.gz";
  log(std::format("Packing icpp release package {}...", targz));
#if __APPLE__
  std::system(
      std::format("find {} -name .DS_Store -delete", dstroot.string()).data());
#endif
  std::system(
      std::format("cd {} && tar czf {} {}", dstroot.string(), targz, pkgdir)
          .data());
  log(std::format("Created icpp package {}.", targz));
  return 0;
}
