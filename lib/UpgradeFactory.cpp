#include "migrate/UpgradeFactory.h"

#include <fstream>
#include <sstream>

UpgradeFactory::UpgradeFactory() {}

std::unique_ptr<Upgrade>
UpgradeFactory::fromFile(const std::string &Path,
                         clang::ast_matchers::dynamic::Diagnostics &Diags) {
  assert(!Path.empty() && "File path is empty?");
  std::ifstream UpgradeFile(Path);
  std::stringstream Content;
  Content << UpgradeFile.rdbuf();
  return fromString(Content.str(), Diags);
}

std::unique_ptr<Upgrade>
UpgradeFactory::fromString(const std::string &Content,
                           clang::ast_matchers::dynamic::Diagnostics &Diags) {
  std::istringstream stream(Content);
  std::string line;

  std::stringstream MatcherCode, Replacement;
  bool parsingMatcher = true;

  while (std::getline(stream, line)) {
    if (line == "###") {
      parsingMatcher = false;
      continue;
    }
    if (parsingMatcher) {
      MatcherCode << line;
    } else {
      Replacement << line;
    }
  }

  std::unique_ptr<Upgrade> Result(new Upgrade(
      MatcherCode.str() + ".bind(\"all\")", Replacement.str(), Diags));
  return Result;
}
