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
    spell_book.learnSpell(spell);
}

void Warlock::forgetSpell(std::string const& spell){
    spell_book.forgetSpell(spell);
}

void Warlock::launchSpell(const std::string& spell, ATarget& target){
    ASpell* spell_ptr = spell_book.createSpell(spell);
    if (spell_ptr == NULL){
        return;
    }

    target.getHitBySpell(*spell_ptr);
}
