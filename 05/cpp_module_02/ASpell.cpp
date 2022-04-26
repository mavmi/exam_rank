#include "ASpell.hpp"

ASpell::ASpell(){

}

ASpell::ASpell(const ASpell& another)
        : name(another.name), effects(another.effects) {
    
}

ASpell::ASpell(const std::string& name, const std::string& effects)
        : name(name), effects(effects) {

}

ASpell::~ASpell(){

}

ASpell& ASpell::operator=(const ASpell& another){
    name = another.name;
    effects = another.effects;
    
    return *this;
}

std::string ASpell::getName() const {
    return name;
}

std::string ASpell::getEffects() const {
    return effects;
}

void ASpell::launch(const ATarget& target) const {
    target.getHitBySpell(*this);
}

