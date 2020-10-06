struct ULA : Thread {
  Node::Component node;
  Node::Screen screen_;

  auto load(Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;
  auto frame() -> void;
  auto refresh() -> void;
  auto power() -> void;

  auto fetch(uint16 address) -> uint8;
  auto read(uint16 port) -> uint8;
  auto write(uint8 data) -> void;

  auto activeDisplay() -> uint1 { return vcounter >= screen_top_start && vcounter < border_bottom_start && hcounter >= screen_left_start && hcounter < border_right_start; }
  auto lineCycles() -> uint32 { return (hcounter - screen_left_start) / 2; }
  auto floatingBus() -> uint8 { return activeDisplay() ? busValue : (uint8)0xFF; }

  auto serialize(serializer&) -> void;

  auto color(uint32 color) -> uint64;

  struct IO {
    uint3 borderColor;
    uint1 mic;
    uint1 ear;
  } io;

  uint32 buffer[352 * 296];
  uint16 hcounter;
  uint16 vcounter;
  uint5 flashFrameCounter;
  uint1 flashState;
  uint8 busValue;

  const uint border_top_start = 16;
  const uint screen_top_start = border_top_start + 48;
  const uint border_bottom_start = screen_top_start + 192;
  const uint border_bottom_end = border_bottom_start + 56;
  const uint border_left_start = 96;
  const uint screen_left_start = border_left_start + 48;
  const uint border_right_start = screen_left_start + 256;
  const uint border_right_end = border_right_start + 48;
  const uint contention_delay[8] = { 5, 4, 3, 2, 1, 0, 0, 6 };
};

extern ULA ula;
