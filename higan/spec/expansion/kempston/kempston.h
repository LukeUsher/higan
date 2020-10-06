struct Kempston : Expansion {
  Kempston(Node::Port parent);

  auto romcs() -> bool { return false; }
  auto mapped(uint16 address, bool io) -> bool { return io && address == 0x1f; }

  auto read(uint16 address) -> uint8 { return 0xff; }
  auto write(uint16 address, uint8 data) -> void {}

  auto in(uint16 address) -> uint8;
  auto out(uint16 address, uint8 data) -> void {}
};