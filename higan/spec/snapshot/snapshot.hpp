struct Snapshot {
  Node::Peripheral node;
  Memory::Readable<uint8> rom;

  auto manifest() const -> string { return information.manifest; }
  auto name() const -> string { return information.name; }

  //cartridge.cpp
  auto allocate(Node::Port) -> Node::Peripheral;
  auto connect() -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

//private:
  struct Information {
    string manifest;
    string name;
  } information;
};

#include "slot.hpp"
extern Snapshot& snapshot;
