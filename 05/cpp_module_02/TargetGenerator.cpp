#include "TargetGenerator.hpp"


TargetGenerator::TargetGenerator(){

}

TargetGenerator::~TargetGenerator(){

}

void TargetGenerator::learnTargetType(ATarget* target){
    if (target == NULL){
        return;
    }

    targets[target->getType()] = target;
}

void TargetGenerator::forgetTargetType(std::string const & target){
    if (targets.count(target)){
        targets.erase(target);
    }
}

ATarget* TargetGenerator::createTarget(std::string const & target){
    if (targets.count(target)){
        return targets.at(target)->clone();
    }
    return NULL;
}