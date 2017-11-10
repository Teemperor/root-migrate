#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
  std::string Name;
  std::string Code;
public:
  Variable() = default;
  Variable(const std::string& Name, const std::string& Code);

  const std::string &getName() const {
      return Name;
  }

  const std::string& getCode() const {
      return Code;
  }
};

#endif // VARIABLE_H
