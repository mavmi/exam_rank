#ifndef WARLOC_HPP
#define WARLOC_HPP

#include <string>
#include <iostream>

class Warlock {
public:
    Warlock(const std::string& name, const std::string& title);
    ~Warlock();

    const std::string& getName() const;
    const std::string& getTitle() const;

    void setTitle(const std::string& title);

    void introduce() const;

private:
    std::string name;
    std::string title;

    Warlock();
    Warlock(const Warlock& another);

    Warlock& operator=(const Warlock& another);
};

#endif
