

#ifndef Header_h
#define Header_h
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cmath>
#include <exception>
using namespace std;

class Coordinates
{
private:
    double x, y;
public:
    Coordinates(double a, double b) : x(a), y(b)
    {}
    friend double dist(Coordinates a, Coordinates b);
};

double dist(Coordinates a, Coordinates b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}


class Transported_Graph {
private:
    vector < vector < double >> base;
    vector < vector < double >> base_buf;
    vector <double> min_line;
    vector <double> min_column;
    int ribe_counter = 0;
    vector <bool> black_line;
    vector <bool> black_column;
    double all_cost = 0;
public:
    
    bool is_finish()
    {
        if (ribe_counter == base.size())
            return true;
        else
            return false;
    }
    
    double return_cost()
    {
        return all_cost;
    }

    Transported_Graph(vector <Coordinates> & cords)
    {
        base.resize(cords.size(), vector<double>(cords.size(), -1));
        for (int i = 0; i < cords.size(); i++)
            for (int k = 0; k < cords.size(); k++)
                if (i != k)
                    base[k][i] = dist(cords[k], cords[i]);
        min_line.resize(cords.size(), -1);
        min_column.resize(cords.size(), -1);
        black_line.resize(cords.size(), false);
        black_column.resize(cords.size(), false);
        base_buf = base;
    }

    void line_reduction()
    {
        double counter;
        for (int i = 0; i < base.size(); i++)
        {
            if (black_line[i])
                continue;
            counter = -1;
            for (int k = 0; k < base.size(); k++)
                if ((i != k) and (base[i][k] >= 0) and ((counter == -1) or (base[i][k] < counter)))
                    counter = base[i][k];
            min_line[i] = counter;
            for (int k = 0; k < base.size(); k++)
                if ((i != k) and (base[i][k] > 0))
                    base[i][k] -= counter;
        }
    }

    void column_reduction()
    {
        double counter;
        for (int i = 0; i < base.size(); i++)
        {
            if (black_column[i])
                continue;
            counter = -1;
            for (int k = 0; k < base.size(); k++)
                if ((i != k) and (base[k][i] >= 0)  and ((counter == -1) or (base[k][i] < counter)))
                    counter = base[k][i];
            min_column[i] = counter;
            for (int k = 0; k < base.size(); k++)
                if ((i != k) and (base[k][i] > 0))
                    base[k][i] -= counter;
        }
    }

    double get_min(int i, int k)
    {
        double result = 0;
        double local_min = -1;
        for (int p = 0; p < base.size(); p++)
            if (((local_min == -1) or (local_min > base[i][p])) and (base[i][p] >= 0) and (p != k))
                local_min = base[i][p];
        result += local_min;
        local_min = -1;
        for (int p = 0; p < base.size(); p++)
            if (((local_min == -1) or (local_min > base[p][k])) and (base[p][k] >= 0) and (p != i))
                local_min = base[i][p];
        result += local_min;
        return result;
    }
    
    void get_mass_of_line()
    {
        double counter = 0;
        double cost_ribe = 0;
        pair <int, int> buf;
        for (int i = 0; i < base.size(); i++)
            for (int k = 0; k < base.size(); k++)
                if (base[i][k] == 0)
                    if (get_min(i, k) > counter)
                    {
                        buf = make_pair(i, k);
                        cost_ribe = base_buf[i][k];
                    }
        ribe_counter++;
        base[buf.first][buf.second] = -1;
        base[buf.second][buf.first] = -1;
        all_cost += cost_ribe;
    }

    void matrix_reduction()
    {
        int counter;
        for (int i = 0; i < base.size(); i++)
        {
            counter = 0;
            for (int k = 0; k < base.size(); k++)
            {
                if (base[i][k] == -1) counter++;
                if (counter >= 2)
                {
                    black_line[i] = true;
                    for (int k = 0; k < base.size(); k++)
                        base[i][k] = -2;
                    break;
                }
            }
        }

        for (int i = 0; i < base.size(); i++)
        {
            counter = 0;
            for (int k = 0; k < base.size(); k++)
            {
                if (base[k][i] == -1) counter++;
                if (counter >= 2)
                {
                    black_column[i] = true;
                    for (int k = 0; k < base.size(); k++)
                        base[k][i] = -2;
                    break;
                }
            }
        }
            
    }

};

vector <string> find_test_files(string path);
Coordinates split(string & data, string file_debug = "");
bool check_first(const vector <unsigned int>& base);
double get_mass_first(vector <unsigned int>& permutations, vector <Coordinates>& cords);
double brutforce_method(vector <Coordinates> cords, double result);
#endif /* Header_h */
