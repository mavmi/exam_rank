#ifndef ATARGET_HPP
#define ATARGET_HPP

#include <string>
#include <iostream>

#include "ASpell.hpp"

class ASpell;

class ATarget{
public:
    ATarget();
    ATarget(const ATarget& another);
    ATarget(const std::string& type);
    ~ATarget();

    ATarget& operator=(const ATarget& another);

    const std::string& getType() const;

    virtual ATarget* clone() const = 0;
    void getHitBySpell(const ASpell& spell) const;

private:
    std::string type;

};

#endif
