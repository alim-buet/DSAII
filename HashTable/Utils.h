
#ifndef UTILS_H
#define UTILS_H

#include <bits/stdc++.h>

using namespace std; 

vector<string> generateRandomWords(int count = 10000, int min_len = 5, int max_len = 10)
{
    vector<string> strings;
    for (int i = 0; i < count; ++i)
    {
        int len = rand() % (max_len - min_len + 1) + min_len;
        string s;
        for (int j = 0; j < len; ++j)
        {
            char c = 'a' + rand() % 26;
            s += c;
        }
        strings.push_back(s);
    }
    return strings;
}

bool isPrime(long long n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (long long i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int getNearestPrime(int n)
{
    if (n <= 1)
        return 2;
    while (!isPrime(n))
    {
        n++;
    }
    return n;
}

int hash1(const string &key, int tableSize)
{
    // polynomial rolling hash function. source: https://cp-algorithms.com/string/string-hashing.html
    long long hash_val = 0;
    const int prime = 31;
    long long prime_power = 1;

    for (char c : key)
    {
        hash_val = (hash_val + (c - 'a' + 1) * prime_power) % tableSize;
        prime_power = (prime_power * prime) % tableSize;
    }
    return static_cast<int>(hash_val);
}

int hash2(const string &key, int tableSize)
{
    // djb2 hash function. source: https://www.cse.yorku.ca/~oz/hash.html
    unsigned long long hash_val = 5381;

    for (char c : key)
    {
        hash_val = ((hash_val << 5) + hash_val) + c;
        hash_val %= tableSize;
    }
    return static_cast<int>(hash_val);
}
// int hash1(const string &key, int tableSize)
// {
//     unsigned long long hash_val = 0;
//     const unsigned long long prime = 131; // a commonly used prime for hashing

//     for (char c : key)
//     {
//         hash_val = hash_val * prime + c;
//     }

//     return static_cast<int>(hash_val % tableSize);
// }
// int hash2(const string &key, int tableSize)
// {
//     unsigned long long hash_val = 0xcbf29ce484222325ULL; // FNV offset basis
//     const unsigned long long prime = 0x100000001b3ULL;   // FNV prime

//     for (char c : key)
//     {
//         hash_val ^= (unsigned char)c;
//         hash_val *= prime;
//     }

//     return static_cast<int>(hash_val % tableSize);
// }

#endif