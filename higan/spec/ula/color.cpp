auto ULA::color(uint32 color) -> uint64 {
  switch(color) {
    case  0: return 0x0000'0000'0000ull; //transparent
    case  1: return 0x0000'0000'0000ull; //black
    case  2: return 0x0000'0000'd7d7ull; //blue
    case  3: return 0xd7d7'0000'0000ull; //red
    case  4: return 0xd7d7'0000'd7d7ull; //magenta
    case  5: return 0x0000'd7d8'0000ull; //green
    case  6: return 0x0000'd7d7'd7d7ull; //cyan
    case  7: return 0xd7d7'd7d7'0000ull; //yellow
    case  8: return 0xd7d7'd7d7'd7d7ull; //white
    case  9: return 0x0000'0000'0000ull; //black
    case 10: return 0x0000'0000'ffffull; //blue
    case 11: return 0xffff'0000'0000ull; //red
    case 12: return 0xffff'0000'ffffull; //magenta
    case 13: return 0x0000'ffff'0000ull; //green
    case 14: return 0x0000'ffff'ffffull; //cyan
    case 15: return 0xffff'ffff'0000ull; //yellow
    case 16: return 0xffff'ffff'ffffull; //white
  }

  unreachable;
}