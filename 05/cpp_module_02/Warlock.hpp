#ifndef WARLOC_HPP
#define WARLOC_HPP

#include <map>
#include <string>
#include <iostream>

#include "ASpell.hpp"
#include "ATarget.hpp"
#include "SpellBook.hpp"

class Warlock {
public:
    Warlock(const std::string& name, const std::string& title);
    ~Warlock();

    const std::string& getName() const;
    const std::string& getTitle() const;

    void setTitle(const std::string& title);
    void introduce() const;

    void learnSpell(ASpell* spell);
    void forgetSpell(std::string const& spell);
    void launchSpell(const std::string& spell, ATarget& target);

private:
    std::string name;
    std::string title;
    std::map<std::string, ASpell*> spells;
    SpellBook spell_book;

    Warlock();
    Warlock(const Warlock& another);

    Warlock& operator=(const Warlock& another);
};

#endif
