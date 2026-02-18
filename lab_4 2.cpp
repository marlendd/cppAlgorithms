#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>
#include <set>

#define COUNT 500
#define N 1
#define kInf 2147483647
#define timeit(x) begin = std::chrono::system_clock::now();x;end = std::chrono::system_clock::now();
#define cast2NS(x) chrono::duration_cast<std::chrono::nanoseconds>(x)
using namespace std;


template<typename ...T>
void writefile(ofstream& file,
               const char* filename,
               bool append,
               const string& sep,
               const string& end,
               T&& ...args) {
    if (!file)
        file.open(filename, ios::out | ios::app);
    int dummy[sizeof...(T)] = { (file << args << sep ,0)... };
    file << end;
    if (!append)
        file.close();
}

long** get_matrix(const int n, int fill=0){
    long** matrix = new long*[n];
    for (int i = 0; i < n; i++){
        matrix[i] = new long[n];
        for (int j = 0; j < n; j++) {
            matrix[i][j] = fill;
        }
    }
    return matrix;
}
void fill_matrix(long** matrix, const int n, int fill=0){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            matrix[i][j] = fill;
        }
    }
}

long** get_graph(const int n){
    long** graph = get_matrix(n);
    int d;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < n; i ++){
        for (int j = i+1; j < n; j++){
            d = rand() % 16;
            graph[i][j] = d;
            graph[j][i] = d;
        }
    }
    return graph;
}

void floyd(long **graph, int n, long** distances) {
    int weight, col, v, row;
    for (row = 0; row < n; ++row) {
        for (col = 0; col < n; ++col) {
            distances[row][col] = graph[row][col] == 0 ? kInf : graph[row][col];
            if (row == col)
                distances[row][col] = 0;
        }
    }
    for (v = 0; v != n; ++v) {
        for (row = 0; row != n; ++row) {
            for (col = 0; col != n; ++col) {
                weight = distances[row][v] + distances[v][col];
                if (distances[row][v] != kInf and distances[v][col] != kInf and distances[row][col] > weight)
                    distances[row][col] = weight;
            }
        }
    }
}
template <typename T>
void bellman(long **graph, int n, long **distances, T edges) {
    int cnt = 0, v, u, w;
    for(v = 0; v < n; v++){
        distances[v][v] = 0;
        for (u = v + 1; u < n; u++) {
            w = graph[u][v];
            if (w) {
                edges[cnt][0] = u;
                edges[cnt][1] = v;
                edges[cnt][2] = w;
                cnt++;
            }
        }
    }

    for (int i = 0; i < cnt; i++)
        cout << edges[i][0] << '\t' << edges[i][1] << '\t' << edges[i][2] << '\t' << endl;
    cout << cnt << endl;

    for(int start = 0; start< n; start++ ){
        for (int i = 0; i < n-1; i++){
            for (int j = 0; j < cnt; j++){
                u = edges[j][0];
                v = edges[j][1];
                w = edges[j][2];
                if((distances[start][v]+w < distances[start][u])){
                    distances[start][u] = distances[start][v] + w;
                    distances[u][start] = distances[start][u];
                }
                if ((distances[start][u]+w < distances[start][v])){
                    distances[start][v] = distances[start][u] + w;
                    distances[v][start] = distances[start][v];
                }
            }
        }
    }
}

void dijkstra(long **graph, int n, long** distances) {
    for (int start = 0; start < n; start++){
        for (int end = start; end < n; end++){
            distances[start][start] = 0;
            std::set<std::pair<int, int>> q;
            q.insert({distances[start][start], start});
            while (!q.empty()) {
                auto from = q.begin()->second;
                q.erase(q.begin());

                for (int to = 0, size = n; to != size; ++to) {
                    bool edge_exists = graph[from][to] != 0;
                    bool new_edge_is_shorter = distances[start][to] > distances[start][from] + graph[from][to];

                    if (edge_exists and new_edge_is_shorter) {
                        q.erase({distances[start][to], to});
                        distances[start][to] = distances[start][from] + graph[from][to];
                        q.insert({distances[start][to], to});
                    }
                }
            }
        }
    }
}

void print_matrix(long** matrix, int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << '\t';
        }
        cout << '\n';
    }
    cout << endl;
}

typedef chrono::nanoseconds nsec;

int main() {
    ofstream file;
    long** graph;
    long** min_graph;
    auto edges = new long[rand()%1][3];
    delete[] edges;
    chrono::nanoseconds avgTime[3];
    auto begin = std::chrono::system_clock::now(),
            end = std::chrono::system_clock::now();
    file.open("graphs.csv", ios::out | ios::app);
    for (int i = 5; i <= COUNT; i+=5) {
        avgTime[0] = nsec(0);
        avgTime[1] = nsec(0);
        avgTime[2] = nsec(0);
        graph = get_graph(i);
        min_graph = get_matrix(i);
        edges = new long[i*(i-1)][3];
        for (int j = 0; j < N; j++) {
            //print_matrix(graph, i);
            fill_matrix(min_graph, i );
            timeit(floyd(graph, i, min_graph));
            //print_matrix(min_graph, i);
            avgTime[0] += cast2NS(end - begin);
            fill_matrix(min_graph, i, kInf );
            timeit(dijkstra(graph, i, min_graph));
            //print_matrix(min_graph, i);
            avgTime[1] += cast2NS(end - begin);
            fill_matrix(min_graph, i, kInf );
            timeit(bellman(graph, i, min_graph, edges));
            //print_matrix(min_graph, i);
            avgTime[2] += cast2NS(end - begin);
        }
        for(int j = 0; j < i; j++){
            delete[] graph[j];
            delete[] min_graph[j];
        }
        delete[] graph;
        delete[] min_graph;
        delete[] edges;
        cout << "---------"<<endl;
        writefile(file, "optimization.csv", i >= COUNT, ";", "\n",
                  1-i, avgTime[0].count() / N, avgTime[1].count() / N, avgTime[2].count() / N);
        file << i << ';' << avgTime[0].count() / N
             << ';' << avgTime[1].count() / N
             << ';' << avgTime[2].count() / N << endl;
    }
    file.close();
    return 0;
}
