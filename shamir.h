#ifndef SHAMIR
#define SHAMIR

namespace shamir{
/*
    This class serves as an implementation of Shamir's Secret Sharing scheme,
    which provides methods for managing shared secrets
*/

typedef std::vector<std::vector<FiniteField::point>> shares;

class scheme{
    private:
    /*
        n: number of shares
        k: threshold
    */
        int n,k;

    public:
        scheme(int members,int threshold);
        shares* createShares(const std::string& secret);
        std::string getSecret(shares* Kshares);
    };

  /*
    init() initialise the library functions and should be called before using the library. 
  */

  void init();
}

#endif