// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/Dynamic/Parser.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "migrate/Upgrade.h"
#include "migrate/UpgradeFactory.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  SourceManager *SourceMgr = nullptr;
  Rewriter *R = nullptr;
  Upgrade& Up;
  LoopPrinter(Upgrade& U) : Up(U) {}

  virtual void run(const MatchFinder::MatchResult &Result) {
    SourceMgr = &Result.Context->getSourceManager();
    if(!R) {
      R = new Rewriter(*SourceMgr, Result.Context->getLangOpts());
    }
    std::map<std::string, Variable> Variables;

    for(auto& node : Result.Nodes.getMap()) {
     std::string NodeName = node.first;
     std::string Code;
     if (const Stmt *FS = Result.Nodes.getNodeAs<clang::Stmt>(NodeName)) {
       const char* start = SourceMgr->getCharacterData(FS->getLocStart());
       const char* end = SourceMgr->getCharacterData(FS->getLocEnd());
       Code = std::string(start, (end - start + 1)/sizeof(char));
     }
     llvm::errs() << "Node: " << NodeName << ":" << Code << "\n";
     Variables[NodeName] = Variable(NodeName, Code);
    }
    if (const Stmt *FS = Result.Nodes.getNodeAs<clang::Stmt>("all")) {
        R->ReplaceText(FS->getSourceRange(), Up.getReplacement(Variables));
    }
  }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("root-migrate options");


// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

static cl::opt<std::string> UpgradeFile("u", cl::desc("Specify an upgrade file"), cl::value_desc("path to upgrade file"), cl::Required);

class WriteOutCallback : public SourceFileCallbacks {
  unsigned Depth = 0;
public:
  WriteOutCallback(LoopPrinter &P) : Printer(P) {
  }

  LoopPrinter &Printer;

  virtual bool handleBeginSource(CompilerInstance &CI) {
    ++Depth;
    return true;
  }

  virtual void handleEndSource() {
    --Depth;
    if (Depth == 0 && Printer.SourceMgr) {
      const RewriteBuffer *RewriteBuf =
            Printer.R->getRewriteBufferFor(Printer.SourceMgr->getMainFileID());
      if (RewriteBuf) {
        Printer.R->overwriteChangedFiles();
      }
    }
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  using namespace clang::ast_matchers::dynamic;

  Diagnostics Diags;

  UpgradeFactory Factory;
  std::unique_ptr<Upgrade> upgrade = Factory.fromFile(UpgradeFile, Diags);

  assert(Diags.errors().empty() && "Errors while parsing matcher?");
  /* TODO: Uncomment and test.
  if (!Diags.errors().empty()) {
    Diags.printToStreamFull(llvm::errs());
    return 1;
  }*/

  LoopPrinter Printer(*upgrade);
  MatchFinder Finder;
  Finder.addDynamicMatcher(*upgrade->Matcher, &Printer);

  WriteOutCallback CB(Printer);

  return Tool.run(newFrontendActionFactory(&Finder, &CB).get());
}
