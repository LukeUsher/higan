auto CPU::read(uint16 address) -> uint8 {
  if(Model::Spectrum48k()) {
    if (address < 0x4000) {
      return rom.bios.read(address);
    }

    contendMemory();
    return ram.read(address - 0x4000);
  }
  return 0xff;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if(Model::Spectrum48k()) {
    if (address >= 0x4000) {
      contendMemory();
      return ram.write(address - 0x4000, data);
    }
  }
}

auto CPU::in(uint16 address) -> uint8 {
  if ((address & 1) == 0) {
    contendIo();
    return ula.read(address);
  }

  return 0xFF;
}

auto CPU::out(uint16 address, uint8 data) -> void {
  if ((address & 1) == 0) {
    contendIo();
    ula.write(data);
  }
}

auto CPU::contendMemory() -> void {
  if (!ula.activeDisplay()) return;

  auto delay = (ula.contention_delay[ula.lineCycles() % 8]);
  step(delay);
}

auto CPU::contendIo() -> void {
  if (!ula.activeDisplay()) return;

  auto delay = (ula.contention_delay[ula.lineCycles() % 8]);
  step(delay);
}

