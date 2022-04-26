#ifndef ASPELL_HPP
#define ASPELL_HPP

#include <string>

#include "ATarget.hpp"

class ATarget;

class ASpell {
public:
    ASpell();
    ASpell(const ASpell& another);
    ASpell(const std::string& name, const std::string& effects);
    ~ASpell();

    ASpell& operator=(const ASpell& another);

    std::string getName() const;
    std::string getEffects() const;

    virtual ASpell* clone() const = 0;
    void launch(const ATarget& target) const;

protected:
    std::string name;
    std::string effects;

};

#endif
