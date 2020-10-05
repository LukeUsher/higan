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

  auto read(uint16 port) -> uint8;
  auto write(uint8 data) -> void;

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
};

extern ULA ula;
