#include <iostream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <functional>

struct line : public std::string {};
std::istream& operator>>(std::istream& in, line& ln) {
    std::getline(in, ln);
    return in;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    for (const auto& s : v) {
        out << s << '\n';
    }
    return out;
}

int main(int argc, char** argv) {
    std::vector<std::string> lines;
    std::ifstream file(argv[1]);
    std::copy_if(std::istream_iterator<line>(file), std::istream_iterator<line>{}, 
        std::back_inserter(lines),
        [](const line& ln) { return ln.find("warning: ") != std::string::npos; });
    //std::cout << lines;
    std::sort(begin(lines), end(lines));
    //std::cout << lines;
    auto last = std::unique(begin(lines), end(lines));
    lines.erase(last, end(lines));
    std::cout << lines;
    /*for (auto be = begin(lines); be != en; ++be) {
        auto found = std::adjacent_find(be, en, std::not_equal_to<std::string>{});
        be = lines.erase(be, found);
    }*/
    
    return 0;
}