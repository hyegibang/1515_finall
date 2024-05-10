#include <iostream>
#include <vector>
#include <random>
#include "field.h"
#include "shamir.h"

using namespace std;
using namespace FiniteField;
using namespace shamir;

int main(int argc,char** argv) {
    int share_input, threshold_input;
    
    string secret_input;
    
    // prompt the user for input
    cout << "Enter a number of shares: ";
    cin >> share_input; 

    // must be a number  
    if (cin.fail()) {
        cout << "Invalid shares inputted. Please enter a number." << endl;
        return 0;
    }

    // User input for threshold
    cout << "Enter a threshold: ";
    cin >> threshold_input;

    // must be a number
    if (cin.fail()) {
        cout << "Invalid threshold inputted. Please enter a number." << endl;
        return 0;
    }

    // Get secret from user
    cout << "Enter a secret: ";
    cin.ignore();
    getline(cin, secret_input);

    cout << "Number of shares entered: " << share_input << ", threshold value : " << threshold_input << ", the secret:  " << secret_input << endl;
    cout << "\nPress any key to create shares" << endl;

    getchar();


    // to initialise the library functions.Must be used to use the library.
    init(); 
    // creating a scheme with 8 shares and 5 threshold.
    scheme currScheme(share_input,threshold_input); 
    shares* currScheme_shares = currScheme.createShares(secret_input);

    // printing shares of all the participants...
    for(int i=0; i < share_input; i++) {
        cout << "Share " << (i+1) << ": " ; 
        for(auto val:(*currScheme_shares)[i]) {
            cout << val.y << ". ";
        }
        cout << endl;
    }
    

    //reconstructing the secret by giving shares of all members
    string secret = currScheme.getSecret(currScheme_shares);
    cout << "construted secret using all "<< share_input <<" shares -> " <<secret << endl;


    cout << "\nPress any key to reconstruct the secret using threshold shares" << endl;
    getchar();

    // reconstructing the secret by only shares of threshold members
    shares* thres_share = new shares(threshold_input);
    for(int i = 0;i < threshold_input ;i++) {
    for(auto val : ( *currScheme_shares)[i]) {
        (*thres_share)[i].push_back(val);
    }
    }
    string thres_secret = currScheme.getSecret(thres_share);
    cout << "construted secret using " << threshold_input <<" threshold shares -> " << thres_secret << endl;

    // Verify reconstructing does not work if less than specified threshold
    cout << "\nPress any key to reconstruct the secret only using less than threshold, "<< threshold_input - 1 <<  " shares" << endl;
    getchar();

    // Ensure there are more than 1 thresholds being used
    if (threshold_input < 2) {
        cout << "Threshold value should be greater than 1" << endl;
        return 0;
    }

    shares* less_share = new shares(threshold_input - 1);
    for(int i=0 ;i<threshold_input-1 ; i++) {
        for(auto val:(*currScheme_shares)[i]) {
            (*less_share)[i].push_back(val);
        }
    }
    string less_secret = currScheme.getSecret(less_share);
    return 0;
    }