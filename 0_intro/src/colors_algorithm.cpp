#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <iterator>
#include <fstream>
using namespace std;

template<typename First, typename Second>
bool operator<(const pair<First, Second>& a, const pair<First, Second>& b) {
    return a.first < b.first;
}

int main(int, char**) {
    auto& in_colors = std::cin;
    
    map<string, int> counts;
    for_each(istream_iterator<string>{in_colors}, istream_iterator<string>{}, 
        [&counts](string s) { counts[std::move(s)]++; }
    );

    using cnt_clr = pair<int, string>;
    vector<cnt_clr> freq;
    std::transform(begin(counts), end(counts), std::back_inserter(freq),
        [](const pair<string, int>& p){ return std::make_pair(p.second, p.first); });

    auto last = freq.size() > 3? begin(freq) + 3: end(freq);
    partial_sort(begin(freq), last, end(freq), greater<cnt_clr>{});

    for_each (begin(freq), last, [](const cnt_clr& p) {
        cout << p.first << ' ' << p.second << '\n';
    });

}