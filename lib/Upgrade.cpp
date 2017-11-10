#include "migrate/Upgrade.h"

Upgrade::Upgrade(llvm::StringRef MatcherText, llvm::StringRef Replacement, clang::ast_matchers::dynamic::Diagnostics& Diags)
  : Replacement(Replacement)
{
  using namespace clang::ast_matchers::dynamic;

  Matcher = Parser::parseMatcherExpression(MatcherText, &Diags);
}

static void replaceAll(std::string& Text, const std::string& Needle, const std::string& Replacement) {
  assert(!Needle.empty());
  size_t StartPos = 0;
  while(true) {
    StartPos = Text.find(Needle, StartPos);
    if(StartPos == std::string::npos)
      break;
    Text.replace(StartPos, Needle.length(), Replacement);
    StartPos += Replacement.length();
  }
}

std::string Upgrade::getReplacement(const Upgrade::VariableMap &Variables) {
  std::string Result = Replacement;
  for(auto& V : Variables) {
    const Variable& Var = V.second;
    replaceAll(Result, "$" + Var.getName(), Var.getCode());
  }
  return Result;
}
