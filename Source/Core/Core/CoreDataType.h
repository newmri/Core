#pragma once

typedef unsigned char CORE_BYTE;
typedef unsigned char* CORE_BYTE_PTR;

#define CORE_BYTE_MAX 0xff

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