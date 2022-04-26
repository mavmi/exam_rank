#ifndef SPELLBOOK_HPP
#define SPELLBOOK_HPP

#include <map>
#include <string>

#include "ASpell.hpp"

class SpellBook{
public:
    SpellBook();
    ~SpellBook();

    void learnSpell(ASpell* spell);
    void forgetSpell(std::string const& spell);
    ASpell* createSpell(std::string const& spell);

private:
    SpellBook(const SpellBook& another);
    SpellBook& operator=(const SpellBook& another);

    std::map<std::string, ASpell*> spells;

};

#endif
