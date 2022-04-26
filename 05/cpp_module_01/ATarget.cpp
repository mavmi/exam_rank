#include "ATarget.hpp"

ATarget::ATarget(){

}

ATarget::ATarget(const ATarget& another)
        : type(another.type) {

}

ATarget::ATarget(const std::string& type)
        : type(type) {

}

ATarget::~ATarget(){

}

ATarget& ATarget::operator=(const ATarget& another){
    type = another.type;

    return *this;
}

const std::string& ATarget::getType() const {
    return type;
}

void ATarget::getHitBySpell(const ASpell& spell) const {
    std::cout
        << type
        << " has been "
        << spell.getEffects()
        << std::endl;
}
