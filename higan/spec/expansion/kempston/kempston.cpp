Kempston::Kempston(Node::Port parent) {
  node = parent->append<Node::Peripheral>("Kempston");
}

auto Kempston::in(uint16 address) -> uint8
{
  return 0;
}
