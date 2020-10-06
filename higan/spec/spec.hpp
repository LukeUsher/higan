#pragma once

#include <higan/higan.hpp>

#include <component/processor/z80/z80.hpp>

namespace higan::Spectrum {
  #include <higan/inline.hpp>

  struct Model {
    inline static auto Spectrum48k() -> bool;
  };

  struct Region {
    inline static auto PAL() -> bool;
  };

  #include <spec/system/system.hpp>
  #include <spec/snapshot/snapshot.hpp>
  #include <spec/keyboard/keyboard.hpp>
  #include <spec/expansion/expansion.hpp>
  #include <spec/cpu/cpu.hpp>
  #include <spec/ula/ula.hpp>
}

#include <spec/interface/interface.hpp>
