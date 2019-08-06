#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <numeric>

class UF {
public:
    UF(const std::string& file_name) {
        std::ifstream file(file_name);
        file >> count;
        id = std::vector<int>(count);
        sz = std::vector<int>(count, 1);
        std::iota(id.begin(), id.end(), 0);
        int p, q;
        while (file) {
            file >> p >> q;
            weigthUnionMerge(p, q);
        }
    }

    bool isConnected(int p, int q) { return find(p) == find(q); }

private:
    std::vector<int> id;
    std::vector<int> sz;
    int              count;

    int find(int p) {
        int pRoot = p;
        while (id[pRoot] != pRoot) pRoot = id[pRoot];
        while (id[p] != p) {
            int t = p;
            p     = id[p];
            id[t] = pRoot;
        }

        return pRoot;
    }

    void unionMerge(int p, int q) {
        int pRoot = find(p), qRoot = find(q);
        if (pRoot == qRoot) return;
        id[pRoot] = qRoot;
        count--;
    }

    void weigthUnionMerge(int p, int q) {
        int pRoot = find(p), qRoot = find(q);
        if (pRoot == qRoot) return;
        if (sz[pRoot] < sz[qRoot]) {
            id[pRoot] = qRoot;
            sz[qRoot] += sz[pRoot];
        } else {
            id[qRoot] = pRoot;
            sz[pRoot] += sz[qRoot];
        }
    }
};