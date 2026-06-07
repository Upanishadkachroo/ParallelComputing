#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include<vector>

class Statistics{
    private:
    std::vector<int> data;

    public:
    Statistics(const std::vector<int> &input);

    void display();

    void calculate();

};

#endif