auto CPU::read(uint16 address) -> uint8 {
  if (address < 0x4000) {
    if (expansionPort.connected() && expansionPort.romcs()) {
      return expansionPort.read(address);
    }

    return rom.bios.read(address);
  }

  return ram.read(address - 0x4000);
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if (expansionPort.connected() && expansionPort.mapped(address, false)) {
    expansionPort.write(address, data);
  }

  if (address < 0x4000) {
    return;
  }

  return ram.write(address - 0x4000, data);
}

auto CPU::in(uint16 address) -> uint8 {
  if (expansionPort.connected() && expansionPort.mapped(address, true)) {
    if ((address & 1) == 0) {
      return ula.in(address) | expansionPort.in(address);
    }

    return expansionPort.in(address);
  }

  if ((address & 1) == 0) {
    return ula.in(address);
  }

  return ula.floatingBus();
}

auto CPU::out(uint16 address, uint8 data) -> void {
  if (expansionPort.connected() && expansionPort.mapped(address, true)) {
    expansionPort.out(address, data);
  }

  if ((address & 1) == 0) {
    ula.out(data);
  }
}
