auto CPU::read(uint16 address) -> uint8 {
  if(Model::Spectrum48k()) {
    if (address < 0x4000) {
      return rom.bios.read(address);
    }

    return ram.read(address - 0x4000);
  }
  return 0xff;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if(Model::Spectrum48k()) {
    if (address >= 0x4000) {
      return ram.write(address - 0x4000, data);
    }
  }
}

auto CPU::in(uint16 address) -> uint8 {
  if ((address & 1) == 0) {
    return ula.read(address);
  }

  return 0xFF;
}

auto CPU::out(uint16 address, uint8 data) -> void {
  if ((address & 1) == 0) {
    ula.write(data);
  }
}
