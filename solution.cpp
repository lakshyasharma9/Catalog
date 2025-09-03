#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>
#include <map>

using namespace std;
using json = nlohmann::json;

// Function to decode value from given base
long long decodeValue(int base, const string& value) {
    long long result = 0;
    long long multiplier = 1;
    
    for (int i = value.size() - 1; i >= 0; i--) {
        char digit = value[i];
        int num;
        if (isdigit(digit)) {
            num = digit - '0';
        } else {
            num = tolower(digit) - 'a' + 10;
        }
        result += num * multiplier;
        multiplier *= base;
    }
    return result;
}

// Function to perform Lagrange interpolation and find the constant term
long long lagrangeInterpolation(const vector<pair<int, long long>>& points) {
    int k = points.size();
    long long constantTerm = 0;
    
    for (int i = 0; i < k; i++) {
        long long numerator = points[i].second;
        long long denominator = 1;
        
        // Calculate Lagrange basis polynomial at x=0
        for (int j = 0; j < k; j++) {
            if (i != j) {
                numerator *= (0 - points[j].first);
                denominator *= (points[i].first - points[j].first);
            }
        }
        
        constantTerm += numerator / denominator;
    }
    
    return constantTerm;
}

void processTestCase(const string& filename) {
    cout << "\n=== Processing " << filename << " ===" << endl;
    
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open " << filename << endl;
        return;
    }
    
    json jsonData;
    inputFile >> jsonData;
    inputFile.close();

    // Extract n and k
    int n = jsonData["keys"]["n"];
    int k = jsonData["keys"]["k"];
    
    cout << "n = " << n << ", k = " << k << endl;

    // Extract points
    vector<pair<int, long long>> points;
    int count = 0;
    
    for (auto& el : jsonData.items()) {
        if (el.key() == "keys") continue;
        if (count >= k) break;
        
        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string encodedY = el.value()["value"];
        
        long long y = decodeValue(base, encodedY);
        points.push_back({x, y});
        
        cout << "Point " << count + 1 << ": (" << x << ", " << y << ")" << endl;
        count++;
    }
    
    // Calculate constant term
    long long constantTerm = lagrangeInterpolation(points);
    
    cout << "The constant term 'c' is: " << constantTerm << endl;
}

int main() {
    cout << "=== POLYNOMIAL INTERPOLATION SOLUTION ===" << endl;
    
    // Process both test cases
    processTestCase("input.json");
    processTestCase("input2.json");
    
    return 0;
}