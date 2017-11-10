#ifndef UPGRADEFILEPARSER_H
#define UPGRADEFILEPARSER_H

#include "Upgrade.h"

#include "clang/ASTMatchers/Dynamic/Parser.h"

class UpgradeFactory {
public:
  UpgradeFactory();

  std::unique_ptr<Upgrade>
  fromFile(const std::string &Path,
           clang::ast_matchers::dynamic::Diagnostics &Diags);
  std::unique_ptr<Upgrade>
  fromString(const std::string &Content,
             clang::ast_matchers::dynamic::Diagnostics &Diags);
};

#endif // UPGRADEFILEPARSER_H
