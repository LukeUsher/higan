auto ZXSpectrumTape::export(string location) -> vector<uint8_t> {
  vector<uint8_t> data;
  append(data, {location, "program.tape"});
  return data;
}

auto ZXSpectrumTape::heuristics(vector<uint8_t>& data, string location) -> string {
  string s;
  s += "game\n";
  s +={"  name: ",  Media::name(location), "\n"};
  s +={"  label: ", Media::name(location), "\n"};
  return s;
}

auto ZXSpectrumTape::import(string location) -> string {
  auto input = Media::read(location);

  location = {pathname, Location::prefix(location), "/"};
  if(!directory::create(location)) return "output directory not writable";

  file::write({location, "program.tape"}, input);
  return {};
}
