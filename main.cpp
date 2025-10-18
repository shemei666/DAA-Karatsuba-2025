#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// Helper function to make two strings of equal length by padding with zeros
string makeEqualLength(string str, int len) {
    if (str.length() < len) {
        str = string(len - str.length(), '0') + str;
    }
    return str;
}

// Helper function to add two number strings
string addStrings(string first, string second) {
    if (first == "0") return second;
    if (second == "0") return first;
    
    string result = "";
    int carry = 0;
    
    // Make strings of equal length
    int length = max(first.length(), second.length());
    first = makeEqualLength(first, length);
    second = makeEqualLength(second, length);
    
    // Add from right to left
    for (int i = length - 1; i >= 0; i--) {
        int firstBit = first[i] - '0';
        int secondBit = second[i] - '0';
        int sum = firstBit + secondBit + carry;
        result = char(sum % 10 + '0') + result;
        carry = sum / 10;
    }
    
    if (carry) {
        result = "1" + result;
    }
    
    return result;
}

// Helper function to subtract two numbers (assuming first >= second)
string subtractStrings(string first, string second) {
    if (second == "0") return first;
    if (first == second) return "0";
    
    string result = "";
    int borrow = 0;
    
    // Make strings of equal length
    int length = max(first.length(), second.length());
    first = makeEqualLength(first, length);
    second = makeEqualLength(second, length);
    
    // Subtract from right to left
    for (int i = length - 1; i >= 0; i--) {
        int firstBit = first[i] - '0';
        int secondBit = second[i] - '0';
        
        int diff = firstBit - secondBit - borrow;
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result = char(diff + '0') + result;
    }
    
    // Remove leading zeros
    result.erase(0, min(result.find_first_not_of('0'), result.size() - 1));
    return result.empty() ? "0" : result;
}

// Traditional multiplication for small numbers (base case)
string traditionalMultiply(string x, string y) {
    if (x == "0" || y == "0") return "0";
    
    int lenX = x.length();
    int lenY = y.length();
    vector<int> result(lenX + lenY, 0);
    
    // Multiply each digit and sum at corresponding positions
    for (int i = lenX - 1; i >= 0; i--) {
        for (int j = lenY - 1; j >= 0; j--) {
            int product = (x[i] - '0') * (y[j] - '0');
            int sum = product + result[i + j + 1];
            
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }
    
    // Convert to string
    string strResult = "";
    for (int num : result) {
        if (!(strResult.empty() && num == 0)) {
            strResult += to_string(num);
        }
    }
    
    return strResult.empty() ? "0" : strResult;
}

// Optimized Karatsuba multiplication with larger base case
string karatsubaMultiply(string x, string y) {
    // Remove leading zeros
    x.erase(0, min(x.find_first_not_of('0'), x.size() - 1));
    y.erase(0, min(y.find_first_not_of('0'), y.size() - 1));
    
    if (x.empty() || x == "0") return "0";
    if (y.empty() || y == "0") return "0";
    if (x == "1") return y;
    if (y == "1") return x;
    
    // LARGER BASE CASE: Use traditional multiplication for small numbers
    // This threshold can be tuned based on performance testing
    const int BASE_CASE_THRESHOLD = 475; // 470 to 520 seem to work best
    
    int n = max(x.length(), y.length());
    
    if (n <= BASE_CASE_THRESHOLD) {
        return traditionalMultiply(x, y);
    }
    
    // Make both strings of equal length
    x = makeEqualLength(x, n);
    y = makeEqualLength(y, n);
    
    // Split the numbers into halves
    int half = n / 2;
    int second_half = n - half;
    
    string a = x.substr(0, second_half);
    string b = x.substr(second_half);
    string c = y.substr(0, second_half);
    string d = y.substr(second_half);
    
    // Remove leading zeros from splits
    a.erase(0, min(a.find_first_not_of('0'), a.size() - 1));
    b.erase(0, min(b.find_first_not_of('0'), b.size() - 1));
    c.erase(0, min(c.find_first_not_of('0'), c.size() - 1));
    d.erase(0, min(d.find_first_not_of('0'), d.size() - 1));
    
    if (a.empty()) a = "0";
    if (b.empty()) b = "0";
    if (c.empty()) c = "0";
    if (d.empty()) d = "0";
    
    // Recursively compute the three products
    string ac = karatsubaMultiply(a, c);
    string bd = karatsubaMultiply(b, d);
    
    // Compute (a+b)(c+d)
    string a_plus_b = addStrings(a, b);
    string c_plus_d = addStrings(c, d);
    string product_sum = karatsubaMultiply(a_plus_b, c_plus_d);
    
    // Compute ad + bc = (a+b)(c+d) - ac - bd
    string ac_plus_bd = addStrings(ac, bd);
    string ad_plus_bc = subtractStrings(product_sum, ac_plus_bd);
    
    // Combine the results: ac * 10^(2*half) + (ad+bc) * 10^half + bd
    string result = ac;
    
    // Add zeros for ac * 10^(2*half)
    result += string(2 * half, '0');
    
    // Add (ad+bc) * 10^half
    string temp = ad_plus_bc + string(half, '0');
    result = addStrings(result, temp);
    
    // Add bd
    result = addStrings(result, bd);
    
    // Remove leading zeros
    result.erase(0, min(result.find_first_not_of('0'), result.size() - 1));
    return result.empty() ? "0" : result;
}


   
    
   

int main() {
       
    std::string num1 = "";

    std::string num2 = "";

    

    cout << karatsubaMultiply(num1, num2);

    return 0;
}