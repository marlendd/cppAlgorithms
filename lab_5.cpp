#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <chrono>
#include <random>

#define VERT 30000
#define N 10

using namespace std;
using namespace std::chrono;

// Функция для записи данных в файл
template<typename ...T>
void writefile(ofstream& file,
               const char* filename,
               bool append,
               const string& sep,
               const string& end,
               T&& ...args) {
    if (!file.is_open())
        file.open(filename, append ? ios::out | ios::app : ios::out);
    int dummy[sizeof...(T)] = { (file << args << sep, 0)... };
    file << end;
    if (!append)
        file.close();
}

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Алгоритм Борувки
vector<Edge> boruvka(int vertices, const vector<Edge> edges) {
    vector<int> parent(vertices);
    vector<int> rank(vertices, 0);
    vector<Edge> result;

    for (int i = 0; i < vertices; ++i)
        parent[i] = i;

    auto find = [&](int u) {
        while (parent[u] != u) {
            parent[u] = parent[parent[u]];
            u = parent[u];
        }
        return u;
    };

    auto unite = [&](int u, int v) {
        int u_root = find(u);
        int v_root = find(v);
        if (u_root != v_root) {
            if (rank[u_root] < rank[v_root])
                parent[u_root] = v_root;
            else {
                parent[v_root] = u_root;
                if (rank[u_root] == rank[v_root])
                    rank[u_root]++;
            }
        }
    };

    while (result.size() < vertices - 1) {
        vector<Edge> cheapest(vertices, {-1, -1, INT_MAX});
        for (const auto& edge : edges) {
            int set_u = find(edge.u);
            int set_v = find(edge.v);
            if (set_u != set_v) {
                if (cheapest[set_u].weight > edge.weight)
                    cheapest[set_u] = edge;
                if (cheapest[set_v].weight > edge.weight)
                    cheapest[set_v] = edge;
            }
        }
        for (int i = 0; i < vertices; ++i) {
            if (cheapest[i].weight != INT_MAX) {
                int set_u = find(cheapest[i].u);
                int set_v = find(cheapest[i].v);
                if (set_u != set_v) {
                    result.push_back(cheapest[i]);
                    unite(set_u, set_v);
                }
            }
        }
    }
    return result;
}

// Алгоритм Прима
vector<Edge> prim(int vertices, const vector<Edge> edges) {
    vector<vector<pair<int, int>>> adj(vertices);
    for (const auto& edge : edges) {
        adj[edge.u].emplace_back(edge.v, edge.weight);
        adj[edge.v].emplace_back(edge.u, edge.weight);
    }

    vector<bool> visited(vertices, false);
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> min_heap;
    vector<Edge> result;

    visited[0] = true;
    for (const auto& neighbor : adj[0])
        min_heap.push({neighbor.second, {0, neighbor.first}});

    while (!min_heap.empty() && result.size() < vertices - 1) {
        auto edge = min_heap.top();
        min_heap.pop();
        int u = edge.second.first;
        int v = edge.second.second;
        int w = edge.first;

        if (!visited[v]) {
            visited[v] = true;
            result.push_back({u, v, w});
            for (const auto& neighbor : adj[v]) {
                if (!visited[neighbor.first])
                    min_heap.push({neighbor.second, {v, neighbor.first}});
            }
        }
    }
    return result;
}

// Генерация случайного дерева
vector<Edge> generate_random_tree(int vertices) {
    vector<Edge> edges;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> weight_dist(1, 100);

    for (int i = 1; i < vertices; ++i) {
        int u = i;
        int v = uniform_int_distribution<>(0, i - 1)(gen);
        int w = weight_dist(gen);
        edges.push_back({u, v, w});
    }
    return edges;
}

int main() {
    ofstream file;
    //double mn;
    chrono::nanoseconds avgTime[2] = {chrono::nanoseconds(0), chrono::nanoseconds(0)};
    auto begin = chrono::system_clock::now(), end = chrono::system_clock::now();

    // Открываем файл для записи
    file.open("graphs.csv", ios::out);
    writefile(file, "graphs.csv", false, ";", "\n", "Vertices", "Boruvka", "Prim");

    for (double i = 1000; i <= VERT; i += 1000) {
        avgTime[0] = chrono::nanoseconds(0);
        avgTime[1] = chrono::nanoseconds(0);

        for (int j = 0; j < N; j++) {
            auto edges = generate_random_tree(i);
            // Метод перебора
            begin = chrono::system_clock::now();
            boruvka(i, edges);
            end = chrono::system_clock::now();
            avgTime[0] += chrono::duration_cast<chrono::nanoseconds>(end - begin);

            // Метод дихотомии
            begin = chrono::system_clock::now();
            prim(i, edges);
            end = chrono::system_clock::now();
            avgTime[1] += chrono::duration_cast<chrono::nanoseconds>(end - begin);

        }

        // Запись результатов в файл
        cout << "test" << endl;
        writefile(file, "graphs.csv", true, ";", "\n",
                  i, avgTime[0].count() / N, avgTime[1].count() / N);
    }

    file.close();
    cout << "Программа завершена" << endl;
    return 0;
}
