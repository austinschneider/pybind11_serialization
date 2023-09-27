#ifndef ABC_H
#define ABC_H

/* Define an abstract base class that we can subclass later */
class ABC {
public:
    ABC() = default; // default constructor uses compiler generated constructor
    virtual void method(); // A method to be overriden
    virtual void call(); // Calls this->method()
    virtual ~ABC(); // destructor is marked virtual
};

#endif // ABC_H
