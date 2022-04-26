#include "SpellBook.hpp"

SpellBook::SpellBook(){

}

SpellBook::~SpellBook(){
    
}

void SpellBook::learnSpell(ASpell* spell){
    if (spell == NULL){
        return;
    }

    spells[spell->getName()] = spell->clone();
}

void SpellBook::forgetSpell(std::string const& spell){
    if (spells.count(spell)){
        spells.erase(spell);
    }
}

ASpell* SpellBook::createSpell(std::string const& spell){
    if (spells.count(spell)){
        return spells.at(spell)->clone();
    }
    return NULL;
}
