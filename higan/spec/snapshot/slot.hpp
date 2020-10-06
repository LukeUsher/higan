struct SnapshotLoader {
  Node::Port port;
  Snapshot snapshot;

  //slot.cpp
  SnapshotLoader(string name);
  auto load(Node::Object) -> void;
  auto unload() -> void;

  const string name;
};

extern SnapshotLoader snapshotLoader;
