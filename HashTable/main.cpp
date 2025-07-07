#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <unordered_set>
#include <algorithm>
#include "Utils.h"
#include "RedBlackTree.h"
#include "HashTable.h"

using namespace std;

int main()
{
    vector<string> all_words = generateRandomWords();
    int N;
    cout << "Enter the table size: ";
    cin >> N;
    N = getNearestPrime(N);

    cout << fixed << setprecision(2);

    for (double alpha = 0.4; alpha <= 0.9; alpha += 0.1)
    {
        int elements_to_insert = floor(alpha * N);
        vector<string> actually_inserted_words;

        // first we will build all kinds of hash tables for same alpha and same dataset

        // ------------------- CHAINING HASH TABLES -------------------
        int value = 1;
        int collisions_chain1 = 0;
        ChainingHashTable chainingTable1(N, 1);
        int inserted = 0, i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            if (chainingTable1.search(word) == -1)
            {
                // search return -1 if not found, so that word is unique, insert it
                collisions_chain1 += chainingTable1.insert(word, value);
                value++;
                inserted++;
                actually_inserted_words.push_back(word);
            }
            i++;
        }

        int collisions_chain2 = 0;
        ChainingHashTable chainingTable2(N, 2);
        value = 1, inserted = 0, i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            if (chainingTable2.search(word) == -1)
            {
                collisions_chain2 += chainingTable2.insert(word, value);
                value++;
                inserted++;
            }
            i++;
        }

        // ------------------- LINEAR PROBING HASH TABLES -------------------
        int collisions_linear1 = 0;
        LinearProbingHashTable linearTable1(N, 1, 3); // 1- which hash funciton, 3- step size
        value = 1;
        inserted = 0;
        i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            pair<bool, int> result = linearTable1.searchWithProbes(word);
            if (!result.first)
            {
                collisions_linear1 += linearTable1.insert(word, value);
                value++;
                inserted++;
            }
            i++;
        }

        int collisions_linear2 = 0;
        LinearProbingHashTable linearTable2(N, 2, 3);
        value = 1;
        inserted = 0;
        i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            pair<bool, int> result = linearTable2.searchWithProbes(word);
            if (!result.first)
            {
                collisions_linear2 += linearTable2.insert(word, value);
                value++;
                inserted++;
            }
            i++;
        }

        // ------------------- DOUBLE HASHING HASH TABLES -------------------
        int collisions_double1 = 0;
        DoubleHashingHashTable doubleTable1(N, 1); // 1-primary hash
        value = 1;
        inserted = 0;
        i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            pair<bool, int> result = doubleTable1.searchWithProbes(word);
            if (!result.first)
            {
                collisions_double1 += doubleTable1.insert(word, value);
                value++;
                inserted++;
            }
            i++;
        }

        int collisions_double2 = 0;
        DoubleHashingHashTable doubleTable2(N, 2);
        value = 1;
        inserted = 0;
        i = 0;
        while (inserted < elements_to_insert && i < all_words.size())
        {
            string &word = all_words[i];
            pair<bool, int> result = doubleTable2.searchWithProbes(word);
            if (!result.first)
            {
                collisions_double2 += doubleTable2.insert(word, value);
                value++;
                inserted++;
            }
            i++;
        }

        // ------------------- SEARCH BEFORE DELETION -------------------
        int search_count = max(1, (int)(0.1 * actually_inserted_words.size()));

        //---------------------------------helper func.
        auto timeSearch = [](auto &table, const vector<string> &keys)
        {
            auto start = chrono::high_resolution_clock::now();
            for (const string &key : keys)
                table.search(key);
            auto end = chrono::high_resolution_clock::now();
            return chrono::duration<double, nano>(end - start).count() / keys.size();
        };
        auto probeSearch = [](auto &table, const vector<string> &keys)
        {
            int total = 0;
            for (const string &key : keys)
                total += table.searchWithProbes(key).second;
            return (double)total / keys.size();
        };
        //------------------------------------
        //
        //-------------------------------------BEFORE DELETION MEASUREMENTS-------------------------
        double avg_time_chain1 = timeSearch(chainingTable1, actually_inserted_words);
        double avg_time_chain2 = timeSearch(chainingTable2, actually_inserted_words);

        double avg_time_linear1 = timeSearch(linearTable1, actually_inserted_words);
        double avg_probe_linear1 = probeSearch(linearTable1, actually_inserted_words);

        double avg_time_linear2 = timeSearch(linearTable2, actually_inserted_words);
        double avg_probe_linear2 = probeSearch(linearTable2, actually_inserted_words);

        double avg_time_double1 = timeSearch(doubleTable1, actually_inserted_words);
        double avg_probe_double1 = probeSearch(doubleTable1, actually_inserted_words);

        double avg_time_double2 = timeSearch(doubleTable2, actually_inserted_words);
        double avg_probe_double2 = probeSearch(doubleTable2, actually_inserted_words);
        // -------------------------------------END OF BEFORE DELETION MEASUREMENTS-------------------------

        // ------------------- DELETION + MIXED SEARCH -------------------
        int delete_count = max(1, (int)(0.1 * actually_inserted_words.size()));
        shuffle(actually_inserted_words.begin(), actually_inserted_words.end(), default_random_engine(42));            // GOTTA TAKE RANDOM 10%
        vector<string> deleted_words(actually_inserted_words.begin(), actually_inserted_words.begin() + delete_count); // delted words will contain the words to be deleted

        unordered_set<string> deleted_set;
        for (const string &word : deleted_words)
        {
            chainingTable1.remove(word);
            chainingTable2.remove(word);
            linearTable1.remove(word);
            linearTable2.remove(word);
            doubleTable1.remove(word);
            doubleTable2.remove(word);
            deleted_set.insert(word);
        }

        vector<string> mixed_search_list;
        for (int k = 0; k < delete_count / 2; ++k)
            mixed_search_list.push_back(deleted_words[k]); // first half of deleted words will be searched

        int added = 0;
        for (const string &word : actually_inserted_words)
        {
            // actually inserted words theke not deleted rest of the words
            if (deleted_set.find(word) == deleted_set.end())
            {
                mixed_search_list.push_back(word);
                if (++added == delete_count / 2)
                    break;
            }
        }

        double avg_time_mix_chain1 = timeSearch(chainingTable1, mixed_search_list);
        double avg_time_mix_chain2 = timeSearch(chainingTable2, mixed_search_list);

        double avg_time_mix_linear1 = timeSearch(linearTable1, mixed_search_list);
        double avg_probe_mix_linear1 = probeSearch(linearTable1, mixed_search_list);

        double avg_time_mix_linear2 = timeSearch(linearTable2, mixed_search_list);
        double avg_probe_mix_linear2 = probeSearch(linearTable2, mixed_search_list);

        double avg_time_mix_double1 = timeSearch(doubleTable1, mixed_search_list);
        double avg_probe_mix_double1 = probeSearch(doubleTable1, mixed_search_list);

        double avg_time_mix_double2 = timeSearch(doubleTable2, mixed_search_list);
        double avg_probe_mix_double2 = probeSearch(doubleTable2, mixed_search_list);

        // ------------------- REPORT -------------------
        cout << "\n--- Load Factor alpha = " << alpha << " ---\n";
        cout << "-------------------------------------------------------------------------------\n";

        // ===================== CHAINING =====================
        cout << "===================== CHAINING =====================\n";
        cout << "Chaining (Hash1):\n";
        cout << "  Insert Collisions: " << collisions_chain1 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_chain1 << " ns\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_chain1 << " ns\n";
        cout << "\n";
        cout << "Chaining (Hash2):\n";
        cout << "  Insert Collisions: " << collisions_chain2 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_chain2 << " ns\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_chain2 << " ns\n";
        cout << "\n";

        // ===================== LINEAR PROBING =====================
        cout << "===================== LINEAR PROBING =====================\n";
        cout << "Linear (Hash1):\n";
        cout << "  Insert Collisions: " << collisions_linear1 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_linear1 << " ns\n";
        cout << "  Avg Probes (Before Deletion): " << avg_probe_linear1 << "\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_linear1 << " ns\n";
        cout << "  Avg Probes (After Deletion): " << avg_probe_mix_linear1 << "\n";
        cout << "\n";
        cout << "Linear (Hash2):\n";
        cout << "  Insert Collisions: " << collisions_linear2 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_linear2 << " ns\n";
        cout << "  Avg Probes (Before Deletion): " << avg_probe_linear2 << "\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_linear2 << " ns\n";
        cout << "  Avg Probes (After Deletion): " << avg_probe_mix_linear2 << "\n";
        cout << "\n";

        // ===================== DOUBLE HASHING =====================
        cout << "===================== DOUBLE HASHING =====================\n";
        cout << "Double Hashing (Hash1):\n";
        cout << "  Insert Collisions: " << collisions_double1 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_double1 << " ns\n";
        cout << "  Avg Probes (Before Deletion): " << avg_probe_double1 << "\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_double1 << " ns\n";
        cout << "  Avg Probes (After Deletion): " << avg_probe_mix_double1 << "\n";
        cout << "\n";
        cout << "Double Hashing (Hash2):\n";
        cout << "  Insert Collisions: " << collisions_double2 << "\n";
        cout << "  Avg Search Time (Before Deletion): " << avg_time_double2 << " ns\n";
        cout << "  Avg Probes (Before Deletion): " << avg_probe_double2 << "\n";
        cout << "  Avg Search Time (After Deletion): " << avg_time_mix_double2 << " ns\n";
        cout << "  Avg Probes (After Deletion): " << avg_probe_mix_double2 << "\n";
        cout << "\n";
        cout << "-------------------------------------------------------------------------------\n";
    }

    return 0;
}
