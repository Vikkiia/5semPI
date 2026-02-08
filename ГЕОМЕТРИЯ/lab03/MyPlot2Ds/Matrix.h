#pragma once
#include <vector>

class CMatrix
{
private:
    std::vector<std::vector<double>> data;
    int rows, cols;

public:
    CMatrix() : rows(0), cols(0) {}
    CMatrix(int r, int c) : rows(r), cols(c) {
        data.resize(r, std::vector<double>(c, 0.0));
    }

    void RedimMatrix(int r, int c) {
        rows = r;
        cols = c;
        data.resize(r, std::vector<double>(c, 0.0));
    }

    double& operator()(int i, int j) { return data[i][j]; }
    const double& operator()(int i, int j) const { return data[i][j]; }

    int GetRowCount() const { return rows; }
    int GetColCount() const { return cols; }
};