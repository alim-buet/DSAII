#include <iostream>
#include <set>
#include <ctime>

#include "utils.h"

int main()
{
    srand(time(0));

    vector<string> all_words = generateRandomWords();

    int N;
    cout << "Enter the table size: ";
    cin >> N;

    N = getNearestPrime(N);
    // cout << "nearest prime:  " << N << endl;

    cout << "Testing Uniqueness" << endl;

    for (double alpha = 0.4; alpha <= 0.9; alpha += 0.1)
    {
        int num_elements_to_consider = floor(alpha * N);

        if (num_elements_to_consider > all_words.size())
        {
            num_elements_to_consider = all_words.size();
        }

        cout << "\n--------------- Load Factor: " << alpha << " (Elements to consider: " << num_elements_to_consider << ") -------\n";

        set<int> unique_hashes_hash1;
        for (int i = 0; i < num_elements_to_consider; ++i)
        {
            unique_hashes_hash1.insert(hash1(all_words[i], N));
        }
        double percentage_unique_hash1 = unique_hashes_hash1.size() * 100.0 / num_elements_to_consider;
        cout << "For alpha = " << alpha << ", Hash1 produce " << percentage_unique_hash1 << "% unique hashes.\n";

        set<int> unique_hashes_hash2;
        for (int i = 0; i < num_elements_to_consider; ++i)
        {
            unique_hashes_hash2.insert(hash2(all_words[i], N));
        }
        double percentage_unique_hash2 = unique_hashes_hash2.size() * 100.0 / num_elements_to_consider;
        cout << "For alpha = " << alpha << ", Hash2 produce " << percentage_unique_hash2 << "% unique hashes.\n";
    }

    return 0;
}