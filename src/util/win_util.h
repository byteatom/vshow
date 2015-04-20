#ifndef WIN_H
#define WIN_H

#include <cstdio>
#include <iostream>

#include "Windows.h"

enum {
    WM_PULSE = WM_USER + 1,
};

/*
class outbuf : public std::streambuf {
public:
    outbuf() {
        setp(0, 0);
    }

    virtual int_type overflow(int_type c = traits_type::eof()) {
        return fputc(c, stdout) == EOF ? traits_type::eof() : c;
    }
};
*/
void showConsole();

#endif // WIN_H

