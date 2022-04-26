#ifndef TARGETGENERATOR_HPP
#define TARGETGENERATOR_HPP

#include <map>
#include <string>

#include "ATarget.hpp"

class TargetGenerator{
public:
    TargetGenerator();
    ~TargetGenerator();

    void learnTargetType(ATarget* target);
    void forgetTargetType(std::string const & target);
    ATarget* createTarget(std::string const & target);

private:
    TargetGenerator(const TargetGenerator& another);
    TargetGenerator& operator=(const TargetGenerator& another);

    std::map<std::string, ATarget*> targets;
};

#endif
