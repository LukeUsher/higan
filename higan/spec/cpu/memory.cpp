auto CPU::read(uint16 address) -> uint8 {
  if (address < 0x4000) {
    if (expansionPort.connected() && expansionPort.romcs()) {
      return expansionPort.read(address);
    }

    if (system.romBank) {
      return rom.sub.read(address);
    }

    return rom.bios.read(address);
  }

  if (Model::Spectrum48k()) {
    return ram.read(address - 0x4000);
  }

  if (address < 0x8000) {
    return readBanked(5, address - 0x4000);
  }

  if (address < 0xC000) {
    return readBanked(2, address - 0x8000);
  }

  return readBanked(system.ramBank, address - 0xC000);
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if (expansionPort.connected() && expansionPort.mapped(address, false)) {
    expansionPort.write(address, data);
  }

  if (address < 0x4000) {
    return;
  }

  if (Model::Spectrum48k()) {
    return ram.write(address - 0x4000, data);
  }

  if (address < 0x8000) {
    return writeBanked(5, address - 0x4000, data);
  }

  if (address < 0xC000) {
    return writeBanked(2, address - 0x8000, data);
  }

  return writeBanked(system.ramBank, address - 0xC000, data);
}

auto CPU::readBanked(uint3 bank, uint16 address) -> uint8 {
  return ram.read((0x4000 * (uint32)bank) + address);
}

auto CPU::writeBanked(uint3 bank, uint16 address, uint8 data) -> void {
  return ram.write((0x4000 * (uint32)bank) + address, data);
}

auto CPU::in(uint16 address) -> uint8 {
  if (expansionPort.connected() && expansionPort.mapped(address, true)) {
    if ((address & 1) == 0) {
      // Some expansion port devices override keyboard (bits 0-4) of the ULA
      uint8 value = ula.in(address);
      uint8 expValue = expansionPort.in(address);
      value.bit(0, 4) &= expValue.bit(0,4);
      return value;
    }

    return expansionPort.in(address);
  }

  if ((address & 1) == 0) {
    return ula.in(address);
  }

  if (Model::Spectrum128()) {
    if (address == 0xfffd) {
      return psg.read();
    }
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


  if (Model::Spectrum128()) {
    if ((address & ~0x7ffd) == 0 && !system.pagingDisabled) {
      system.ramBank = data.bit(0, 2);
      system.screenBank = data.bit(3);
      system.romBank = data.bit(4);
      system.pagingDisabled = data.bit(5);
    }

    if (address == 0xfffd) {
      psg.select(data);
    }

    if (address == 0xbffd) {
      psg.write(data);
    }
  }
}
