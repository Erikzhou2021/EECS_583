//===-- Frequent Path Loop Invariant Code Motion Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
//
// EECS583 W24 - This pass can be used as a template for your FPLICM homework
//               assignment.
//               The passes get registered as "fplicm-correctness" and
//               "fplicm-performance".
//
//
////===-------------------------------------------------------------------===//
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
/* *******Implementation Starts Here******* */
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <queue>
/* *******Implementation Ends Here******* */
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

// #include "llvm/Transforms/Scalar/MachineFunctionPassManager.h"
using namespace llvm;
namespace {
struct RegMappingPass : public MachineFunctionPass {
  static char ID;
  RegMappingPass() : MachineFunctionPass(ID) {}
  bool runOnMachineFunction(MachineFunction &MF) {
    errs() << "FUNCTION RAN\n";
    const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
    // provides methods for querying the status of registers, such as whether a register is virtual or physical and the physical register assigned to a virtual register
    MachineRegisterInfo &MRI = MF.getRegInfo();
    for (auto &MBB : MF) { // for each basic block in the MF
      errs() << "Analyzing Basic Block: " << MBB.getName() << "\n";
      for (auto &MI : MBB) { // for each instruction in the basic block
        errs() << "  Instruction: " << MI << "\n";
        for (unsigned i = 0; i < MI.getNumOperands(); ++i) { // for each operand in the instruction
          MachineOperand &MO = MI.getOperand(i);
          // getReg() returns a Register object
          if (MO.isReg() && MO.getReg().isVirtual()) { // if operand is a virtual register
            unsigned VirtReg = MO.getReg(); // get virtual register
            // if (MRI.isVirtualRegister(VirtReg)) {
            // unsigned PhysReg = MRI.getRegAllocHint(VirtReg).first; // get corresponding physical register
            errs() << "    Virtual Register %" << VirtReg
                  << " has no mapping.\n";
            // }
          }
          if(MO.isReg() && MO.getReg().isPhysical()){
            unsigned PhysReg = MO.getReg();
            errs() << " Physical Register %" << PhysReg << "\n";
          }
        }
      }
    }
    // return PreservedAnalyses::all();
    return false;
  }
};
}

char RegMappingPass::ID = 0;
static RegisterPass<RegMappingPass> X("reg-mapping", "Register Mapping Analysis Pass", false, true);


// namespace {
//   struct HW2CorrectnessPass : public PassInfoMixin<HW2CorrectnessPass> {
//     PreservedAnalyses run(MachineFunction &MF, MachineFunctionAnalysisManager &AM) {
//       const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
//       // provides methods for querying the status of registers, such as whether a register is virtual or physical and the physical register assigned to a virtual register
//       MachineRegisterInfo &MRI = MF.getRegInfo();
//       for (auto &MBB : MF) { // for each basic block in the MF
//         errs() << "Analyzing Basic Block: " << MBB.getName() << "\n";
//         for (auto &MI : MBB) { // for each instruction in the basic block
//           errs() << "  Instruction: " << MI << "\n";
//           for (unsigned i = 0; i < MI.getNumOperands(); ++i) { // for each operand in the instruction
//             MachineOperand &MO = MI.getOperand(i);
//             // getReg() returns a Register object
//             if (MO.isReg() && MO.getReg().isVirtual()) { // if operand is a virtual register
//               unsigned VirtReg = MO.getReg(); // get virtual register
//               // if (MRI.isVirtualRegister(VirtReg)) {
//               // unsigned PhysReg = MRI.getRegAllocHint(VirtReg).first; // get corresponding physical register
//               errs() << "    Virtual Register %" << VirtReg
//                     << " has no mapping.\n";
//               // }
//             }
//             else if(MO.isReg() && MO.getReg().isPhysical()){
//               unsigned PhysReg = MO.getReg();
//               errs() << " Physical Register %" << PhysReg << "\n";
//             }
//           }
//         }
//       }
//       return PreservedAnalyses::all();
//       // return false;
//     }
//   };
// }


// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
//   return {
//     LLVM_PLUGIN_API_VERSION, "HW2Pass", "v0.1",
//     [](PassBuilder &PB) {
//       PB.registerPipelineParsingCallback(
//         [](StringRef Name, MachineFunctionPassManager &MFPM,
//         ArrayRef<PassBuilder::PipelineElement>) {
//           if(Name == "print-machine-code"){
//             // FPM.addPass(HW2CorrectnessPass());
//             MFPM.addPass(RegMappingPass());
//             return true;
//           }
//           return false;
//         }
//       );
//     }
//   };
// }


// extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
//     return {LLVM_PLUGIN_API_VERSION, "HW2Pass", LLVM_VERSION_STRING,
//     [](PassBuilder &PB) {
//         // Register the pass for the MachineFunctionPass pipeline
//         PB.registerPipelineParsingCallback(
//             [](StringRef Name, MachineFunctionPassManager &MFPM,
//                 ArrayRef<PassBuilder::PipelineElement>) {
//                 if (Name == "print-machine-code") {
//                     MFPM.addPass(RegMappingPass());
//                     return true;
//                 }
//                 return false;
//             });
//     }};
// }