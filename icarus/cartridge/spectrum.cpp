struct Spectrum : Cartridge {
  auto name() -> string override { return "Spectrum (Snapshots)"; }
  auto extensions() -> vector<string> override { return {"sna"}; }
  auto export(string location) -> vector<uint8_t> override;
  auto heuristics(vector<uint8_t>& data, string location) -> string override;
};

auto Spectrum::export(string location) -> vector<uint8_t> {
  vector<uint8_t> data;
  append(data, {location, "program.rom"});
  return data;
}

auto Spectrum::heuristics(vector<uint8_t>& data, string location) -> string {
  string s;
  s += "game\n";
  s +={"  name:  ", Media::name(location), "\n"};
  s +={"  label: ", Media::name(location), "\n"};
  s += "  board: snapshot\n"
       "    memory\n";
  s += "      type: ROM\n";
  s +={"      size: 0x", hex(data.size()),     "\n"};
  s += "      content: Program\n";
  return s;
}
