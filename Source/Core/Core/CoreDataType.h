#pragma once

typedef unsigned char CORE_BYTE;
typedef unsigned char* CORE_BYTE_PTR;

#define CORE_BYTE_MAX 0xff


typedef int CORE_INT;
typedef int* CORE_INT_PTR;
typedef unsigned int CORE_UINT;

typedef size_t* CORE_SIZE_PTR;

template <typename Child, typename T>
class Wrapper
{
    T n;
public:
    Wrapper(T n = T()) : n(n) {}
    T& value() { return n; }
    T value() const { return n; }
    Child operator+= (const Wrapper& other) { return Child(n += other.n); }
};