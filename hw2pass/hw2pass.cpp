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

using namespace llvm;

std::vector< std::vector<BasicBlock*> > get_paths(Loop *loop, llvm::BranchProbabilityAnalysis::Result &bpi) {
  std::vector<BasicBlock*> freq_path;
  std::vector<BasicBlock*> infreq_path;

  BasicBlock *header = loop->getHeader();
  BasicBlock *current = header;

  int i = 0;
  while (true) {
    // errs() << "BB" << i << ": ";
    // for (Instruction &I : *current) {
    //   unsigned int opcode = I.getOpcode();
    //   auto name = I.getOpcodeName();
    //   errs() << name << " ";
    // }
    // errs() << "\n";
    freq_path.push_back(current);

    Instruction *last = current->getTerminator();
    if (BranchInst *b = dyn_cast<BranchInst>(last)) {
      if (b->isConditional()) {
        BasicBlock *true_block = b->getSuccessor(0);
        BranchProbability true_f = bpi.getEdgeProbability(current, true_block);

        double freq = true_f.getNumerator() / (double)true_f.getDenominator();
        if (std::abs(freq - 0.8) < 1e-6 || freq - 0.8 > 1e-6) {
          current = true_block;
          // errs() << freq << "\n";
          infreq_path.push_back(b->getSuccessor(1));
        }
        else {
          current = b->getSuccessor(1);
          // errs() << 1 - freq << '\n';
          infreq_path.push_back(true_block);
        }
      }
      else {
        current = b->getSuccessor(0);
      }
    }
    if (current == header || !loop->contains(current)) break;
    i++;
  }

  // remove any infreq blocks found in freq_path
  std::unordered_set<BasicBlock*> freq_set(freq_path.begin(), freq_path.end());
  infreq_path.erase(std::remove_if(infreq_path.begin(), infreq_path.end(), [&](BasicBlock* bb) {
    return freq_set.find(bb) != freq_set.end();
  }), infreq_path.end());
  // run bfs on infreq blocks
  std::queue<BasicBlock*> bfs;
  std::set<BasicBlock*> visited(infreq_path.begin(), infreq_path.end());
  visited.insert(freq_set.begin(), freq_set.end());
  for (BasicBlock *bb : infreq_path) {
    bfs.push(bb);
  }
  while (!bfs.empty()) {
    BasicBlock *bb = bfs.front();
    bfs.pop();
    for (BasicBlock *s : successors(bb)) {
      if (visited.count(s)) continue;
      bfs.push(s);
      visited.insert(s);
      infreq_path.push_back(s);
    }
  }

  return std::vector< std::vector<BasicBlock*> >{freq_path, infreq_path};
}

bool is_invariant(LoadInst *load, std::vector<BasicBlock*> &path) {
  Value *load_ptr = load->getPointerOperand();

  for (BasicBlock *bb : path) {
    for (Instruction &i : *bb) {
      if (StoreInst *store = dyn_cast<StoreInst>(&i)) {
        Value *store_ptr = store->getPointerOperand();
        if (load_ptr == store_ptr) {
          // errs() << "Pointer " << load_ptr << " overwritten in BB " << bb << "\n";
          return false;
        }
      }
    }
  }
  return true;
}

std::vector<LoadInst*> get_almost_invariants(std::vector<BasicBlock*> &freq_path, std::vector<BasicBlock*> &infreq_path) {
  std::vector<LoadInst*> almost_invariant;
  for (BasicBlock *bb : freq_path) {
    for (Instruction &i : *bb) {
      if (LoadInst *load = dyn_cast<LoadInst>(&i)) {
        bool is_freq_invariant = is_invariant(load, freq_path);
        bool is_infreq_invariant = is_invariant(load, infreq_path);
        if (is_freq_invariant && !is_infreq_invariant) {
          // load->print(llvm::errs());
          // errs() << " is almost invariant\n";
          almost_invariant.push_back(load);
        }
      }
    }
  }
  return almost_invariant;
}

namespace {
  struct HW2CorrectnessPass : public PassInfoMixin<HW2CorrectnessPass> {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
      llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
      llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
      llvm::LoopAnalysis::Result &li = FAM.getResult<LoopAnalysis>(F);
      /* *******Implementation Starts Here******* */

      // determine frequent path
      for (Loop *L : li) {
        std::vector< std::vector<BasicBlock*> > paths = get_paths(L, bpi);
        // errs() << '\n';
        // errs() << paths[0].size() << ", " << paths[1].size() << "\n";

        // get pre-header
        BasicBlock* pre_header;
        for (BasicBlock *pred : predecessors(paths[0][0])) {
          pre_header = pred;
        }
        // get last instruction of pre-header
        Instruction *last = pre_header->getTerminator();
        // errs() << last->getOpcodeName() << '\n';
        // find almost invariant loads
        std::vector<LoadInst*> almost_invariant = get_almost_invariants(paths[0], paths[1]);
        for (LoadInst *load : almost_invariant) {
          // alloc new variable before hoist
          AllocaInst *alloc = new AllocaInst(load->getType(), 0, nullptr, load->getAlign(), "", last);
          // get the %_load_old of the original load
            // should be load
          // get the pointer of the original load
          Value *load_ptr = load->getPointerOperand();
          // clone and hoist load to line before last
          LoadInst *hoist = cast<LoadInst>(load->clone());
          hoist->insertBefore(last);
          // errs() << "Hoisted ";
          // load->print(errs());
          // errs() << '\n';
          // get %_clone of cloned load
            // should be hoist
          // store %_clone into new variable
          StoreInst *store = new StoreInst(hoist, alloc, false, load->getAlign(), last);
          // get the %_load_new variable of original load AFTER hoisting (should change)
          // replace all occurrences of %_load_old with %_load_new
            // happens automatically?
          // replace all occurrences pointer with new variable
            // setOperand(0, newPointer)
          Value *alloc_value = alloc;
          std::vector<Instruction*> users;
          for (auto u : load_ptr->users()) {
            if (auto inst = dyn_cast<Instruction>(u)) {
              users.push_back(inst);
            }
          }

          for (auto inst : users) {
            // inst->print(errs());
            // errs() << '\n';
            for (unsigned i = 0; i < inst->getNumOperands(); i++) {
              if (inst->getOperand(i) == load_ptr && inst != hoist && inst->getParent() != pre_header) {
                // errs() << "Updating instruction after hoist: ";
                // inst->print(llvm::errs());
                // errs() << '\n';
                inst->setOperand(i, alloc_value);
                // errs() << "Updated instruction after hoist: ";
                // inst->print(llvm::errs());
                // errs() << '\n';
              }
            }
          }

          // for (auto u : load_ptr->users()) {
          //   if (auto inst = dyn_cast<Instruction>(u)) {
          //     inst->print(llvm::errs());
          //     errs() << '\n';
          //     for (unsigned i = 0; i < inst->getNumOperands(); i++) {
          //       if (inst->getOperand(i) == load_ptr && inst != hoist) {
          //         errs() << "Updating instruction after hoist: ";
          //         inst->print(llvm::errs());
          //         errs() << '\n';
          //         // inst->setOperand(i, alloc_value);
          //         errs() << "Updated instruction after hoist: ";
          //         inst->print(llvm::errs());
          //         errs() << '\n';
          //       }
          //     }
          //   }
          // }
          // load->setOperand(0, alloc);
          // errs() << "Updated instruction after hoist: ";
          // load->print(llvm::errs());
          // errs() << '\n';
          // load->eraseFromParent();
        }
      }



      /* *******Implementation Ends Here******* */
      // Your pass is modifying the source code. Figure out which analyses
      // are preserved and only return those, not all.
      // errs() << '\n';
      // F.print(errs());
      return PreservedAnalyses::none();
    }
  };
  struct HW2PerformancePass : public PassInfoMixin<HW2PerformancePass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
      llvm::BlockFrequencyAnalysis::Result &bfi = FAM.getResult<BlockFrequencyAnalysis>(F);
      llvm::BranchProbabilityAnalysis::Result &bpi = FAM.getResult<BranchProbabilityAnalysis>(F);
      llvm::LoopAnalysis::Result &li = FAM.getResult<LoopAnalysis>(F);
      /* *******Implementation Starts Here******* */
      // This is a bonus. You do not need to attempt this to receive full credit.
      /* *******Implementation Ends Here******* */

      // Your pass is modifying the source code. Figure out which analyses
      // are preserved and only return those, not all.
      return PreservedAnalyses::all();
    }
  };
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "HW2Pass", "v0.1",
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
        ArrayRef<PassBuilder::PipelineElement>) {
          if(Name == "fplicm-correctness"){
            FPM.addPass(HW2CorrectnessPass());
            return true;
          }
          if(Name == "fplicm-performance"){
            FPM.addPass(HW2PerformancePass());
            return true;
          }
          return false;
        }
      );
    }
  };
}