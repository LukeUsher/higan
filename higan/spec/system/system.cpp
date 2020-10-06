#include <spec/spec.hpp>

namespace higan::Spectrum {

Scheduler scheduler;
ROM rom;
System system;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Event::Frame) ula.refresh();
}

auto System::load(Node::Object& root) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "ZX Spectrum 48k" ) information.model = Model::Spectrum48k;

  node = Node::System::create(interface->name());
  root = node;

  regionNode = node->append<Node::String>("Region", "PAL");
  regionNode->setAllowedValues({
    "PAL",
  });

  scheduler.reset();
  cpu.load(node);
  snapshotLoader.load(node);
  keyboard.load(node);
  expansionPort.load(node);
  ula.load(node);
}

auto System::save() -> void {
  if(!node) return;
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cpu.unload();
  ula.unload();
  keyboard.unload();
  snapshotLoader.unload();
  expansionPort.unload();
  node = {};
  rom.bios.reset();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  auto setRegion = [&](string region) {
    if(region == "PAL") {
      information.region = Region::PAL;
    }
  };
  auto regions = regionNode->latch().split("→").strip();
  setRegion(regions.first());

  rom.bios.allocate(16_KiB);
  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
    rom.bios.load(fp);
  }

  cpu.power();
  keyboard.power();
  ula.power();
  snapshot.power();
  scheduler.power(cpu);

  information.serializeSize[0] = serializeInit(0);
  information.serializeSize[1] = serializeInit(1);
}

}
