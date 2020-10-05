#include <spec/spec.hpp>

namespace higan::Spectrum {

CPU cpu;
#include "memory.cpp"
#include "debugger.cpp"
#include "serialization.cpp"

auto CPU::load(Node::Object parent) -> void {
  if(Model::Spectrum48k()) ram.allocate(48_KiB);

  node = parent->append<Node::Component>("CPU");

  debugger.load(node);
}

auto CPU::unload() -> void {
  ram.reset();
  node = {};
  debugger = {};
}

auto CPU::main() -> void {
  if(irqPending) {
    debugger.interrupt("IRQ");
    irq(1, 0x0038, 0xff);
    irqPending = false;
  }

  debugger.instruction();
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize();
}

auto CPU::power() -> void {
  Z80::bus = this;
  Z80::power();
  Thread::create(system.frequency(), {&CPU::main, this});

  r.pc = 0x0000;  //reset vector address

  irqPending = false;
}

auto CPU::triggerIrq() -> void {
  irqPending = true;
}

}
