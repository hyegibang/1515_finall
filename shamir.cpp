#include <iostream>
#include <vector>
#include <random>
#include "field.h"
#include "shamir.h"

using namespace std;
using namespace FiniteField;
using namespace shamir;

typedef vector<vector<point>> Shares;

scheme::scheme(int totalShares, int threshold) : n(totalShares), k(threshold) {}

// Generates shares for a secret using the Shamir's Secret Sharing algorithm
Shares* scheme::createShares(const string& secret) {
    random_device device;
    default_random_engine generator(device());
    uniform_int_distribution<int> distribution(0, 255);

    Shares* allShares = new Shares(n);
    for (char data : secret) {
        vector<byte> coefficients(k);
        coefficients[0] = byte(static_cast<int>(data));
        for (int i = 1; i < k; ++i) {
            coefficients[i] = byte(distribution(generator));
        }
        for (int i = 0; i < n; ++i) {
            byte x(i + 1);
            byte y(0);
            for (int j = 0; j < k; ++j) {
                y = y + coefficients[j] * power(x, j);
            }
            point temp{x, y};
            (*allShares)[i].push_back(temp);
        }
    }
    return allShares;
}

// Recovers the secret from a set of shares
string scheme::getSecret(Shares* providedShares) {
    if (providedShares->size() < k) {
        cout << "Minimum of " << k << " shares are required out of " << n << " shares" << endl;
        return "";
    }

    string secret;
    int secretSize = (*providedShares)[0].size();
    for (int di = 0; di < secretSize; ++di) {
        vector<point> currentShares(k);
        for (int i = 0; i < k; ++i) {
            currentShares[i] = (*providedShares)[i][di];
        }
        secret += interpolate(currentShares);
    }
    return secret;
}

void shamir::init() {
    generateMultiplicationTable();
}
