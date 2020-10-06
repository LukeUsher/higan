SnapshotLoader snapshotLoader{"Snapshot"};

SnapshotLoader::SnapshotLoader(string name) : name(name) {
}

auto SnapshotLoader::load(Node::Object parent) -> void {
  port = parent->append<Node::Port>(name);
  port->setFamily(interface->name());
  port->setType("Snapshot");
  port->setAllocate([&](auto name) { return snapshot.allocate(port); });
  port->setConnect([&] { return snapshot.connect(); });
  port->setDisconnect([&] { return snapshot.disconnect(); });
}

auto SnapshotLoader::unload() -> void {
  snapshot.disconnect();
  port = {};
}
