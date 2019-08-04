#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

using namespace std;

struct Config {
    unsigned                   cities;
    unsigned                   roads;
    unsigned                   curr;
    unsigned                   target;
    vector<unsigned>           teams;
    vector<array<unsigned, 3>> graph;

    friend ostream& operator<<(ostream& out, Config& c) {
        out << setw(10) << left << "N(cities):" << c.cities << endl;
        out << setw(10) << left << "M(roads):" << c.roads << endl;
        out << setw(10) << left << "C1(from):" << c.curr << endl;
        out << setw(10) << left << "C2(to):" << c.target << endl;

        return out;
    }
};

int main(int argc, char const* argv[]) {
    Config c;
    cin >> c.cities >> c.roads >> c.curr >> c.target;
    for (unsigned i = 0, teams = 0; i < c.cities; i++) {
        cin >> teams;
        c.teams.push_back(teams);
    }

    for (unsigned i = 0; i < c.roads; i++) {
        unsigned c1, c2, length;
        cin >> c1 >> c2 >> length;
    }

    cout << c;
    system("PAUSE");
    return 0;
}
