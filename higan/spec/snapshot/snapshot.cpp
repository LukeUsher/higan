#include <spec/spec.hpp>

namespace higan::Spectrum {

Snapshot& snapshot = snapshotLoader.snapshot;
#include "slot.cpp"

auto Snapshot::allocate(Node::Port parent) -> Node::Peripheral {
  return node = parent->append<Node::Peripheral>("ZX Spectrum (Snapshots)");
}

auto Snapshot::connect() -> void {
  node->setManifest([&] { return information.manifest; });

  information = {};

  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  auto document = BML::unserialize(information.manifest);
  information.name = document["game/label"].text();

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(node, "program.rom", File::Read, File::Required)) {
      rom.load(fp);
    }
  }
}

auto Snapshot::disconnect() -> void {
  if(!node) return;
  rom.reset();
  node = {};
}

auto Snapshot::power() -> void {
  if (!node) return;

  // Setup CPU state
  cpu.r.ir.byte.hi = rom.read(0x00);
  cpu.r.hl_.word = rom.read(0x01) | rom.read(0x02) << 8;
  cpu.r.de_.word = rom.read(0x03) | rom.read(0x04) << 8;
  cpu.r.bc_.word = rom.read(0x05) | rom.read(0x06) << 8;
  cpu.r.af_.word = rom.read(0x07) | rom.read(0x08) << 8;
  cpu.r.hl.word = rom.read(0x09) | rom.read(0x0A) << 8;
  cpu.r.de.word = rom.read(0x0B) | rom.read(0x0C) << 8;
  cpu.r.bc.word = rom.read(0x0D) | rom.read(0x0E) << 8;
  cpu.r.iy.word = rom.read(0x0F) | rom.read(0x10) << 8;
  cpu.r.ix.word = rom.read(0x11) | rom.read(0x12) << 8;
  cpu.r.iff2 = rom.read(0x13);
  cpu.r.ir.byte.lo = rom.read(0x14);
  cpu.r.af.word = rom.read(0x15) | rom.read(0x16) << 8;
  cpu.r.sp = rom.read(0x17) | rom.read(0x18) << 8;
  cpu.r.im = rom.read(0x19);
  ula.io.borderColor = rom.read(0x1A);

  // Load the memory contents
  for(uint n : range(48_KiB)) {
    cpu.ram.write(n, rom.read(n + 0x1B));
  };

  // simulate retn, this will prepare the snapshot for execution
  uint16 data = cpu.read(cpu.r.sp++);
  cpu.r.pc = data | cpu.read(cpu.r.sp++) << 8;
  cpu.r.iff1 = cpu.r.iff2;
}
}
