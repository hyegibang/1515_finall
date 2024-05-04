#ifndef FF
#define FF


namespace FiniteField {
    class byte {
    public:
        unsigned char num;
        byte();
        byte(unsigned char n);
        byte operator+(byte other) const;
        byte operator-(byte other) const;
        byte operator*(byte other) const;
        byte operator/(byte other) const;
        byte operator^(byte other) const;
        byte operator~() const;
        void operator=(int value);
        bool operator!=(byte other) const;
    };

    typedef struct point {
        byte x;
        byte y;
    } point;
}

FiniteField::byte byteMultiply(const FiniteField::byte& a, const FiniteField::byte& b);
std::ostream& operator<<(std::ostream& out, FiniteField::byte b);
bool operator<(const FiniteField::byte& lhs,const FiniteField::byte& rhs);
FiniteField::byte power(FiniteField::byte a,int b);
unsigned char interpolate(std::vector<FiniteField::point>& share);

/*
 
*/
void generateMultiplicationTable();


/*
  lagrange interpolation algorithm.
*/
unsigned char interpolate(std::vector<FiniteField::point>& share);
#endif