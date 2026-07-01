// A sample C++ script to find all the call instructions within the current
// function

#include <Cutter++.h>
#include <cutter/core/Cutter.h>

int main() {
  auto addr = Core()->getOffset();
  // get the current function
  auto func = Core()->functionIn(addr);
  if (!func)
    return -1;
  auto fnstart = rz_analysis_function_min_addr(func);
  auto fnend = rz_analysis_function_max_addr(func);
  cpp::print("Current function 0x%x - 0x%x", fnstart, fnend);

  RzAnalysisOp op{0};
  uint8_t buf[16]{0};
  auto core = Core()->lock();
  for (addr = fnstart; addr < fnend;) {
    // read the current machine opcode buffer
    rz_io_nread_at(core->io, addr, &buf[0], sizeof(buf));
    // analyze the current instruction
    auto bytes = rz_analysis_op(core->analysis, &op, addr, &buf[0], sizeof(buf),
                                RZ_ANALYSIS_OP_MASK_BASIC);
    if (bytes <= 0)
      break;
    // print the call instruction
    if (op.type == RZ_ANALYSIS_OP_TYPE_CALL)
      Core()->message(QString("%1 %2")
                          .arg(addr, 8, 16, QChar('0'))
                          .arg(Core()->disassembleSingleInstruction(addr)));
    addr += bytes;
  }
  return 0;
}
