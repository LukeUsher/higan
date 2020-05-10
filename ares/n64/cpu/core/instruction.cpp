auto CPU::raiseException(uint code, uint interrupt, uint coprocessor) -> void {
  scc.status.exceptionLevel = 1;

  scc.cause.exceptionCode = code;
  scc.cause.interruptPending |= interrupt;
  scc.cause.coprocessorError = coprocessor;
  scc.cause.branchDelay = (bool)IP;

  scc.epc.u64 = PC;
  if(IP) IP = nothing, scc.epc.u64 -= 4;

  PC = scc.status.vectorLocation ? 0xbfc0'0200 : 0x8000'0000;
  PC += 0x180;
}

auto CPU::instruction() -> void {
  if(auto interrupts = scc.cause.interruptPending & scc.status.interruptMask) {
    if(scc.status.interruptEnable && !scc.status.exceptionLevel) {
      raiseException(0, interrupts);
    }
  }

  pipeline.address = PC;
  pipeline.instruction = readWord(pipeline.address)(0);
  if(IP) {
    PC = IP();
    IP = nothing;
  } else {
    PC += 4;
  }
  debugger.instruction();
//instructionDEBUG();
  instructionEXECUTE();
  GPR[0].u64 = 0;

  if(--scc.random.value < scc.wired.value) {
    scc.random.value = 31;
  }
}

auto CPU::instructionDEBUG() -> void {
  static uint counter = 0;
  if(++counter <  20000000) return;
  if(++counter >= 20002500) return;
//static bool dis = false;
//if(pipeline.address==0x8000'01ac) dis=true;//{ GPR[Core::Register::T0] = GPR[Core::Register::A3]; }
//if(pipeline.address==0x8000'01b8) { GPR[Core::Register::T0] = GPR[Core::Register::S0]; }
//if(pipeline.address==0x8000'02ac) { GPR[Core::Register::T1].u64 |= 0x3000'0000; }
//if(!dis) return;
  print(
    disassembler.hint(hex(pipeline.address, 8L)), "  ",
  //disassembler.hint(hex(pipeline.instruction, 8L)), "  ",
    disassembler.disassemble(pipeline.address, pipeline.instruction), "\n"
  );
}

auto CPU::instructionEXECUTE() -> void {
  switch(OP >> 26) {
  case 0x00: return instructionSPECIAL();
  case 0x01: return instructionREGIMM();
  case 0x02: return instructionJ();
  case 0x03: return instructionJAL();
  case 0x04: return instructionB(RS.u64 == RT.u64);  //BEQ
  case 0x05: return instructionB(RS.u64 != RT.u64);  //BNE
  case 0x06: return instructionB(RS.i64 <= 0);       //BLEZ
  case 0x07: return instructionB(RS.i64 >  0);       //BGTZ
  case 0x08: return instructionADDI();
  case 0x09: return instructionADDIU();
  case 0x0a: return instructionSLTI();
  case 0x0b: return instructionSLTIU();
  case 0x0c: return instructionANDI();
  case 0x0d: return instructionORI();
  case 0x0e: return instructionXORI();
  case 0x0f: return instructionLUI();
  case 0x10: return instructionCOP0();
  case 0x11: return instructionCOP1();
  case 0x12: return exception.coprocessor2();         //COP2
  case 0x13: return exception.coprocessor3();         //COP3
  case 0x14: return instructionBL(RS.u64 == RT.u64);  //BEQL
  case 0x15: return instructionBL(RS.u64 != RT.u64);  //BNEL
  case 0x16: return instructionBL(RS.i64 <= 0);       //BLEZL
  case 0x17: return instructionBL(RS.i64 >  0);       //BGTZL
  case 0x18: return instructionDADDI();
  case 0x19: return instructionDADDIU();
  case 0x1a: return instructionLDL();
  case 0x1b: return instructionLDR();
  case 0x1c: break;
  case 0x1d: break;
  case 0x1e: break;
  case 0x1f: break;
  case 0x20: return instructionLB();
  case 0x21: return instructionLH();
  case 0x22: return instructionLWL();
  case 0x23: return instructionLW();
  case 0x24: return instructionLBU();
  case 0x25: return instructionLHU();
  case 0x26: return instructionLWR();
  case 0x27: return instructionLWU();
  case 0x28: return instructionSB();
  case 0x29: return instructionSH();
  case 0x2a: return instructionSWL();
  case 0x2b: return instructionSW();
  case 0x2c: return instructionSDL();
  case 0x2d: return instructionSDR();
  case 0x2e: return instructionSWR();
  case 0x2f: return instructionCACHE();
  case 0x30: return instructionLL();
  case 0x31: return instructionLWC1();
  case 0x32: return exception.coprocessor2();  //LWC2
  case 0x33: return exception.coprocessor3();  //LWC3
  case 0x34: return instructionLLD();
  case 0x35: return instructionLDC1();
  case 0x36: return exception.coprocessor2();  //LDC2
  case 0x37: return instructionLD();
  case 0x38: return instructionSC();
  case 0x39: return instructionSWC1();
  case 0x3a: return exception.coprocessor2();  //SWC2
  case 0x3b: return exception.coprocessor3();  //SWC3
  case 0x3c: return instructionSCD();
  case 0x3d: return instructionSDC1();
  case 0x3e: return exception.coprocessor2();  //SDC2
  case 0x3f: return instructionSD();
  }
  exception.reservedInstruction();
}

auto CPU::instructionSPECIAL() -> void {
  switch(OP & 0x3f) {
  case 0x00: return instructionSLL();
  case 0x01: break;
  case 0x02: return instructionSRL();
  case 0x03: return instructionSRA();
  case 0x04: return instructionSLLV();
  case 0x05: break;
  case 0x06: return instructionSRLV();
  case 0x07: return instructionSRAV();
  case 0x08: return instructionJR();
  case 0x09: return instructionJALR();
  case 0x0a: break;
  case 0x0b: break;
  case 0x0c: return instructionSYSCALL();
  case 0x0d: return instructionBREAK();
  case 0x0e: break;
  case 0x0f: return;  //SYNC
  case 0x10: return instructionMFHI();
  case 0x11: return instructionMTHI();
  case 0x12: return instructionMFLO();
  case 0x13: return instructionMTLO();
  case 0x14: return instructionDSLLV();
  case 0x15: break;
  case 0x16: return instructionDSRLV();
  case 0x17: return instructionDSRAV();
  case 0x18: return instructionMULT();
  case 0x19: return instructionMULTU();
  case 0x1a: return instructionDIV();
  case 0x1b: return instructionDIVU();
  case 0x1c: return instructionDMULT();
  case 0x1d: return instructionDMULTU();
  case 0x1e: return instructionDDIV();
  case 0x1f: return instructionDDIVU();
  case 0x20: return instructionADD();
  case 0x21: return instructionADDU();
  case 0x22: return instructionSUB();
  case 0x23: return instructionSUBU();
  case 0x24: return instructionAND();
  case 0x25: return instructionOR();
  case 0x26: return instructionXOR();
  case 0x27: return instructionNOR();
  case 0x28: break;
  case 0x29: break;
  case 0x2a: return instructionSLT();
  case 0x2b: return instructionSLTU();
  case 0x2c: return instructionDADD();
  case 0x2d: return instructionDADDU();
  case 0x2e: return instructionDSUB();
  case 0x2f: return instructionDSUBU();
  case 0x30: return instructionTGE();
  case 0x31: return instructionTGEU();
  case 0x32: return instructionTLT();
  case 0x33: return instructionTLTU();
  case 0x34: return instructionTEQ();
  case 0x35: break;
  case 0x36: return instructionTNE();
  case 0x37: break;
  case 0x38: return instructionDSLL();
  case 0x39: break;
  case 0x3a: return instructionDSRL();
  case 0x3b: return instructionDSRA();
  case 0x3c: return instructionDSLL32();
  case 0x3d: break;
  case 0x3e: return instructionDSRL32();
  case 0x3f: return instructionDSRA32();
  }
  exception.reservedInstruction();
}

auto CPU::instructionREGIMM() -> void {
  switch(OP >> 16 & 0x1f) {
  case 0x00: return instructionB (RS.i64 <  0);  //BLTZ
  case 0x01: return instructionB (RS.i64 >= 0);  //BGEZ
  case 0x02: return instructionBL(RS.i64 <  0);  //BLTZL
  case 0x03: return instructionBL(RS.i64 >= 0);  //BGEZL
  case 0x04: break;
  case 0x05: break;
  case 0x06: break;
  case 0x07: break;
  case 0x08: return instructionTGEI();
  case 0x09: return instructionTGEIU();
  case 0x0a: return instructionTLTI();
  case 0x0b: return instructionTLTIU();
  case 0x0c: return instructionTEQI();
  case 0x0d: break;
  case 0x0e: return instructionTNEI();
  case 0x0f: break;
  case 0x10: return instructionBAL (RS.i64 <  0);  //BLTZAL
  case 0x11: return instructionBAL (RS.i64 >= 0);  //BGEZAL
  case 0x12: return instructionBALL(RS.i64 <  0);  //BLTZALL
  case 0x13: return instructionBALL(RS.i64 >= 0);  //BGEZALL
  case 0x14: break;
  case 0x15: break;
  case 0x16: break;
  case 0x17: break;
  case 0x18: break;
  case 0x19: break;
  case 0x1a: break;
  case 0x1b: break;
  case 0x1c: break;
  case 0x1d: break;
  case 0x1e: break;
  case 0x1f: break;
  }
  exception.reservedInstruction();
}

auto CPU::instructionCOP0() -> void {
//if(!scc.status.enable.coprocessor0) return exception.coprocessor0();
  switch(OP >> 21 & 0x1f) {
  case 0x00: return instructionMFC0();
  case 0x01: return instructionDMFC0();
  case 0x02: return instructionCFC0();
  case 0x04: return instructionMTC0();
  case 0x05: return instructionDMTC0();
  case 0x06: return instructionCTC0();
  case 0x08: return instructionBC0();
  }
  if(!(OP >> 25 & 1)) return exception.reservedInstruction();
  switch(OP & 0x3f) {
  case 0x01: return instructionTLBR();
  case 0x02: return instructionTLBWI();
  case 0x06: return instructionTLBWR();
  case 0x08: return instructionTLBP();
  case 0x18: return instructionERET();
  }
  exception.reservedInstruction();
}

auto CPU::instructionCOP1() -> void {
//if(!scc.status.enable.coprocessor1) return exception.coprocessor1();
  switch(OP >> 21 & 0x1f) {
  case 0x00: return instructionMFC1();
  case 0x01: return instructionDMFC1();
  case 0x02: return instructionCFC1();
  case 0x04: return instructionMTC1();
  case 0x05: return instructionDMTC1();
  case 0x06: return instructionCTC1();
  case 0x08: return instructionBC1();
  }
  if(!(OP >> 25 & 1)) return exception.reservedInstruction();
//if(!scc.status.floatingPointMode) {
//  OP &= ~0b0000'0000'0010'0001'0000'1000'0100'0000;
//}
  switch(OP & 0x3f) {
  case 0x00: return instructionFADD();
  case 0x01: return instructionFSUB();
  case 0x02: return instructionFMUL();
  case 0x03: return instructionFDIV();
  case 0x04: return instructionFSQRT();
  case 0x05: return instructionFABS();
  case 0x06: return instructionFMOV();
  case 0x07: return instructionFNEG();
  case 0x08: return instructionFROUNDL();
  case 0x09: return instructionFTRUNCL();
  case 0x0a: return instructionFCEILL();
  case 0x0b: return instructionFFLOORL();
  case 0x0c: return instructionFROUNDW();
  case 0x0d: return instructionFTRUNCW();
  case 0x0e: return instructionFCEILW();
  case 0x0f: return instructionFFLOORW();
  case 0x20: return instructionFCVTS();
  case 0x21: return instructionFCVTD();
  case 0x24: return instructionFCVTW();
  case 0x25: return instructionFCVTL();
  case 0x30: case 0x38: return instructionFCF();
  case 0x31: case 0x39: return instructionFCUN();
  case 0x32: case 0x3a: return instructionFCEQ();
  case 0x33: case 0x3b: return instructionFCUEQ();
  case 0x34: case 0x3c: return instructionFCOLT();
  case 0x35: case 0x3d: return instructionFCULT();
  case 0x36: case 0x3e: return instructionFCOLE();
  case 0x37: case 0x3f: return instructionFCULE();
  }
  exception.reservedInstruction();
}