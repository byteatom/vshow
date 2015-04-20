#ifndef ATTR_H
#define ATTR_H

#include <functional>
#include <list>

template <typename Type> struct Attr {

    typedef std::function<bool (Type &value, Type &newValue)> Check;
    typedef std::function<void (Type &newValue)> Changed;

    Type value = Type();

    Attr() {}
    Attr(const Type &other) : value{other} {}

    void addChecker(Check check) {
        checks.push_back(check);
    }

    void addObserver(Changed changed) {
        changeds.push_back(changed);
    }


    Type& operator = (const Type &newValue) {
        assign(newValue);
        return value;
    }

    operator Type() const{
        return value;
    }

    bool set(const Type &newValue) {
        for (auto check: checks) {
            if (!check(value, newValue)) return false;
        }
        assign(newValue);
        return true;
    }

private:
    void assign(const Type &newValue) {
        if (value != newValue) {
            value = newValue;
            for (auto changed: changeds) {
                changed(value);
            }
        }
    }

    std::list<Check> checks;
    std::list<Changed> changeds;
};

#endif // ATTR_H
