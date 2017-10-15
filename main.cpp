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

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

class LoopPrinter : public MatchFinder::MatchCallback {
public :
  SourceManager *SourceMgr;
  Rewriter *R = nullptr;

  virtual void run(const MatchFinder::MatchResult &Result) {
    SourceMgr = &Result.Context->getSourceManager();
    if(!R) {
      R = new Rewriter(*SourceMgr, Result.Context->getLangOpts());
    }
    if (const ForStmt *FS = Result.Nodes.getNodeAs<clang::ForStmt>("forLoop")) {
        R->ReplaceText(FS->getSourceRange(), "foo");
    }
  }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

class WriteOutCallback : public SourceFileCallbacks {
public:
  WriteOutCallback(LoopPrinter &P) : Printer(P) {
  }

  LoopPrinter &Printer;

  virtual bool handleBeginSource(CompilerInstance &CI) {
    return true;
  }

  virtual void handleEndSource() {
    const RewriteBuffer *RewriteBuf =
          Printer.R->getRewriteBufferFor(Printer.SourceMgr->getMainFileID());
    if (RewriteBuf)
      llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());
  }
};

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  using namespace clang::ast_matchers::dynamic;

  Diagnostics diags;

  llvm::Optional<DynTypedMatcher> matcher = Parser::parseMatcherExpression(
                 "forStmt(hasLoopInit(declStmt("
                 "hasSingleDecl(varDecl(hasInitializer(integerLiteral(equals(0))"
                 ")))))).bind(\"forLoop\")", &diags);

  LoopPrinter Printer;
  MatchFinder Finder;
  Finder.addDynamicMatcher(*matcher, &Printer);

  WriteOutCallback CB(Printer);

  return Tool.run(newFrontendActionFactory(&Finder, &CB).get());
}
