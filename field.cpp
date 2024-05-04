#include <iostream>
#include <vector>
#include <array>
#include "field.h"

using namespace std;
using namespace FiniteField;

array<byte, 256> exponents;
array<byte, 256> logarithms;

ostream& operator<<(ostream& out, byte b){
    out << (int)b.num;
    return out;
}


byte::byte(unsigned char value) : num(value) {}

byte::byte() : num(0) {}

byte byte::operator+(byte other) const {
    return byte(num ^ other.num);
}

byte byte::operator-(byte other) const {
    return byte(num ^ other.num);
}

byte byte::operator^(byte other) const {
    return byte(num ^ other.num);
}

void byte::operator=(int value) {
    num = static_cast<unsigned char>(value); // Corrected cast
}

byte byte::operator*(byte other) const {
    if (num == 0 || other.num == 0) return byte(0);
    int index = logarithms[num].num + logarithms[other.num].num;
    if (index > 255) index -= 255;
    return exponents[index];
}

bool byte::operator!=(byte other) const {
    return num != other.num;
}

byte byte::operator~() const {
    int index = 255 - logarithms[num].num;
    return exponents[index];
}

byte byte::operator/(byte other) const {
    return *this * ~other;
}


byte byteMultiply(const byte& a, const byte& b) {
    unsigned int aa = a.num, bb = b.num, result = 0, temp;
    while (aa != 0) {
        if (aa & 1) result ^= bb;
        temp = (bb & 128);
        bb <<= 1;
        if (temp) bb ^= 283;
        aa >>= 1;
    }
    return byte(result);
}

bool operator<(const byte& lhs, const byte& rhs) {
    return lhs.num < rhs.num;
}

byte power(byte a, int exponent) {
    if (exponent == 0) return byte(1);
    byte halfPower = power(a, exponent / 2);
    if (exponent % 2 != 0) return halfPower * (halfPower * a);
    return halfPower * halfPower;
}

void generateMultiplicationTable() {
    byte generator = 3;
    exponents[0] = byte(1);
    for (int i = 1; i < 256; ++i) {
        exponents[i] = byteMultiply(generator, exponents[i - 1]);
    }
    for (int i = 0; i < 256; ++i) {
        logarithms[exponents[i].num] = byte(i);
    }
    logarithms[1] = byte(0);
}

unsigned char interpolate(vector<point>& share) {
/*
    Find the y-value for the given x, given n (x, y) points;
    nk points will define a polynomial of up to nth order.
*/

    byte secret = 0;
    int n = share.size();

    for(int i=0;i<n;i++) {
        byte currVal = 1;

        for(int j=0 ; j<n ; j++) {
            if(i == j) continue;
            currVal = currVal * (share[j].x / (share[j].x - share[i].x));
        }
        currVal = currVal * share[i].y;
        secret = secret + currVal;
    }
    return secret.num;
}