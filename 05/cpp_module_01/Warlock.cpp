#include "Warlock.hpp"

Warlock::Warlock(const std::string& name, const std::string& title)
        : name(name), title(title) {
    std::cout
        << this->name 
        << ": This looks like another boring day."
        << std::endl;
}

Warlock::~Warlock(){
    std::cout
        << this->name
        << ": My job here is done!"
        << std::endl;
}

const std::string& Warlock::getName() const {
    return name;
}
const std::string& Warlock::getTitle() const {
    return title;
}

void Warlock::setTitle(const std::string& title) {
    this->title = title;
}

void Warlock::introduce() const {
    std::cout
        << name
        << ": I am "
        << name
        << ", "
        << title
        << "!"
        << std::endl; 
}

void Warlock::learnSpell(ASpell* spell){
    if (spell == NULL){
        return;
    }

    spells[spell->getName()] = spell;
}

void Warlock::forgetSpell(const std::string& spell){
    if (spells.count(spell)){
        spells.erase(spell);
    }
}

void Warlock::launchSpell(const std::string& spell, ATarget& target){
    if (spells.count(spell)){
        target.getHitBySpell(*spells.at(spell));
    }
}
