#ifndef UPGRADE_H
#define UPGRADE_H

#include "Variable.h"

#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/Dynamic/Parser.h"

#include <memory>

class Upgrade
{
  std::string Replacement;

public:
  llvm::Optional<clang::ast_matchers::dynamic::DynTypedMatcher> Matcher;
  Upgrade(llvm::StringRef MatcherText, llvm::StringRef Replacement, clang::ast_matchers::dynamic::Diagnostics &Diags);

  typedef std::map<std::string, Variable> VariableMap;

  std::string getReplacement(const VariableMap& Variables);
};

#endif // UPGRADE_H
