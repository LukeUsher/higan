#include <spec/spec.hpp>

namespace higan::Spectrum {

ULA ula;
#include "color.cpp"
#include "serialization.cpp"

auto ULA::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("ULA");

  screen_ = node->append<Node::Screen>("Screen");
  screen_->colors(16, {&ULA::color, this});
  screen_->setSize(352, 296);
  screen_->setScale(1.0, 1.0);
  screen_->setAspect(1.0, 1.0);
}

auto ULA::unload() -> void {
  node = {};
  screen_ = {};
}

auto ULA::main() -> void {
  constexpr auto border_top_start = 16;
  constexpr auto screen_top_start = border_top_start + 48;
  constexpr auto border_bottom_start = screen_top_start + 192;
  constexpr auto border_left_start = 96;
  constexpr auto screen_left_start = border_left_start + 48;
  constexpr auto border_right_start = screen_left_start + 256;

  if (vcounter >= border_top_start && hcounter >= border_left_start) {
    const auto pixel = ((vcounter - border_top_start) * 352) + hcounter - border_left_start;

    if (vcounter < screen_top_start || vcounter >= border_bottom_start) {
      buffer[pixel] = io.borderColor;
    } else if (hcounter < screen_left_start || hcounter >= border_right_start) {
      buffer[pixel] = io.borderColor;
    } else {
      const uint16 y = vcounter - screen_top_start;
      const uint16 x = hcounter - screen_left_start;

      const uint8 x_tile = x / 8;

      uint16 pixel_addr;
      pixel_addr.bit( 0,  4) = x_tile;
      pixel_addr.bit( 5,  7) = y.bit(3, 5);
      pixel_addr.bit( 8, 10) = y.bit(0, 2);
      pixel_addr.bit(11, 12) = y.bit(6, 7);
      pixel_addr.bit(13, 15) = 0x02;
      const uint8 pixels = cpu.read(pixel_addr);

      uint16 attr_addr;
      attr_addr.bit( 0,  4) = x_tile;
      attr_addr.bit( 5,  7) = y.bit(3, 5);
      attr_addr.bit( 8,  9) = y.bit(6, 7);
      attr_addr.bit(10, 15) = 0x16;
      const uint8 attr = cpu.read(attr_addr);

      const auto ink   = attr.bit(0, 2);
      const auto paper = attr.bit(3, 5);
      const auto bright = attr.bit(6);
      const auto flash = attr.bit(7);

      const uint1 use_ink = (pixels.bit(7 - (x & 7))) ^ (flash & flashState);
      buffer[pixel] = (use_ink ? ink : paper) + (bright * 8);
    }
  }

  step(1);

  if (++hcounter == 448) {
    hcounter = 0;
    if (++vcounter == 312) {
      if (++flashFrameCounter == 31) {
        flashState ^= 1;
      }

      vcounter = 0;
      cpu.triggerIrq();
      frame();
    }
  }
}

auto ULA::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto ULA::frame() -> void {
  scheduler.exit(Event::Frame);
}

auto ULA::refresh() -> void {
  screen_->refresh(buffer, 352 * sizeof(uint32), 352, 296);
}

auto ULA::power() -> void {
  Thread::create(system.frequency() * 2, [&] { ULA::main(); });
  hcounter = 0;
  vcounter = 0;
  flashFrameCounter = 0;
  flashState = 0;
}

auto ULA::read(uint16 port) -> uint8 {
  uint8 value;

  value.bit(0,4) = 0x1F;

  switch(port.bit(8,15)) {
    case 0xFE: value.bit(0,4) = keyboard.read(0); break;
    case 0xFD: value.bit(0,4) = keyboard.read(1); break;
    case 0xFB: value.bit(0,4) = keyboard.read(2); break;
    case 0xF7: value.bit(0,4) = keyboard.read(3); break;
    case 0xEF: value.bit(0,4) = keyboard.read(4); break;
    case 0xDF: value.bit(0,4) = keyboard.read(5); break;
    case 0xBF: value.bit(0,4) = keyboard.read(6); break;
    case 0x7F: value.bit(0,4) = keyboard.read(7); break;
  }

  value.bit(5) = 1;
  value.bit(6) = io.mic | io.ear;
  value.bit(7) = 1;
  return value;
}

auto ULA::write(uint8 data) -> void {
  io.borderColor = data.bit(0, 2);
  io.mic = data.bit(3);
  io.ear = data.bit(4);
}

}
