

#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <exception>
#include "Header.h"
using namespace std;


int main()
{
    vector <string> data = find_test_files("data");
    string buf;
    ofstream out_stream;
    out_stream.open("result.txt");
    string x;
    try {
        for (int i = 0; i < data.size(); i++)
        {
            x = data[i];
            cout << "Starting " << x << endl << flush;
            vector <Coordinates> coordinates;
            ifstream file("data/" + x);
            getline(file, buf);
            while (getline(file, buf))
            {
                if (buf == "") continue;
                coordinates.push_back(split(buf, x));
            }
            file.close();
            Transported_Graph test(coordinates);
            int counter = 0;
            while (!test.is_finish())
            {
                test.line_reduction();
                test.column_reduction();
                test.get_mass_of_line();
                test.matrix_reduction();
                counter++;
                cout << counter << " edge." << flush << endl;
            }
            cout << x << " exit " << endl;
            out_stream << x << ":" << test.return_cost() << endl;
        }
    }
    catch (exception& e)
    {
        cout << e.what();
        cin.get();
    }
    out_stream.close();
}

vector <string> find_test_files(string filepath) {
    vector <string> filenames;
    for (const auto& entry : filesystem::directory_iterator(filepath))
        filenames.push_back(entry.path().string().substr(5));
    return filenames;
}

Coordinates split(string & data, string file_debug) {
    auto pos = data.find(" ");
    int transp;
    if (data.find("  ") != string::npos) transp = 2;
    else transp = 1;
    return Coordinates(stod(data.substr(0, pos)), stod(data.substr(pos + transp)));
}

bool check_first(const vector <unsigned int>& base)
{
    for (int i = 0; i < base.size(); i++)
        if (base[base[i] - 1] == i + 1) return false;
    return true;
}

double get_mass_first(vector <unsigned int>& permutations, vector <Coordinates>& cords)
{
    double result = 0;
    for (int i = 0; i < permutations.size(); i++)
        result += dist(cords[i + 1], cords[permutations[i] - 1]);
    return result;
}

double brutforce_method(vector <Coordinates> coordinates, double result)
{
    vector <unsigned int> permutations(coordinates.size());
    for (int i = 0; i < coordinates.size(); i++)
        permutations[i] = i + 1;
    double buf = 0.0;
    do {
        if (check_first(permutations))
        {
            buf = get_mass_first(permutations, coordinates);
            if (buf > result)
                result = buf;
        }
    } while (next_permutation(permutations.begin(), permutations.end()));
    return buf;
}
