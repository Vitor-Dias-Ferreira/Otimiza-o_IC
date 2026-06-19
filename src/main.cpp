#include <iostream>
#include <vector>
#include <climits>   
#include <iomanip>  
#include <fstream> 
#include <string>  
#include <algorithm> 
#include <chrono> 

using namespace std;

// Estrutura para organizar os dados de cada teste
struct Resultado {
    int origem;
    vector<int> caminho;
    int custo;
    double tempo; 
};

// --- FUNÇÕES DE LEITURA ---

void lerMatriz(vector<vector<int>>& matriz, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "Custo de [" << i << "][" << j << "]: ";
            cin >> matriz[i][j];
        }
    }
}

bool lerMatrizArquivo(vector<vector<int>>& matriz, int& n, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro: Arquivo nao encontrado!" << endl;
        return false;
    }
    arquivo >> n;
    matriz.assign(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return true;
}

// --- ALGORITMO GULOSO ---

int tspGuloso(const vector<vector<int>>& matriz, int n, int origem, vector<int>& caminho) {
    vector<bool> visitado(n, false);
    int custoTotal = 0;
    int cidadeAtual = origem;

    visitado[cidadeAtual] = true;
    caminho.push_back(cidadeAtual);

    for (int i = 0; i < n - 1; i++) {
        int menorCusto = INT_MAX;
        int proximaCidade = -1;

        for (int j = 0; j < n; j++) {
            if (!visitado[j] && matriz[cidadeAtual][j] < menorCusto) {
                menorCusto = matriz[cidadeAtual][j];
                proximaCidade = j;
            }
        }

        if (proximaCidade != -1) {
            caminho.push_back(proximaCidade);
            visitado[proximaCidade] = true;
            custoTotal += menorCusto;
            cidadeAtual = proximaCidade;
        }
    }
    custoTotal += matriz[cidadeAtual][origem];
    caminho.push_back(origem);
    return custoTotal;
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    int n = 0;
    vector<vector<int>> matriz;
    int opcao; 

    cout << "1 - Manual / 2 - Arquivo: ";
    cin >> opcao;

    if (opcao == 2) {
        string nome;
        cout << "Nome do arquivo: ";
        cin >> nome;
        if (!lerMatrizArquivo(matriz, n, nome)) return 1;
    } else {
        cout << "N de cidades: ";
        cin >> n;
        matriz.assign(n, vector<int>(n, 0));
        lerMatriz(matriz, n);
    }

    vector<Resultado> resultados;
    int melhorCustoGeral = INT_MAX;
    int melhorOrigemGeral = -1;

    for (int i = 0; i < n; i++) {
        Resultado res;
        res.origem = i;
        
        auto t1 = chrono::high_resolution_clock::now();
        res.custo = tspGuloso(matriz, n, i, res.caminho);
        auto t2 = chrono::high_resolution_clock::now();

        chrono::duration<double, std::milli> duracao = t2 - t1;
        res.tempo = duracao.count(); 
        
        resultados.push_back(res);

        if (res.custo < melhorCustoGeral) {
            melhorCustoGeral = res.custo;
            melhorOrigemGeral = i;
        }
    }

    // --- RELATÓRIO FINAL ---
    cout << "\n" << setfill('=') << setw(60) << "" << endl;
    cout << "  ORIGEM   |   CUSTO   |   TEMPO (ms)" << endl;
    cout << setfill('=') << setw(60) << "" << setfill(' ') << endl;

    for (const auto& r : resultados) {
        cout << "  " << setw(6) << r.origem 
             << "   |   " << setw(5) << r.custo 
             << "   |   " << fixed << setprecision(2) << setw(8) << r.tempo << " ms" << endl;
        
        //Desativado para matrizes grandes:
        cout << " -> Rota: ";
         for (size_t i = 0; i < r.caminho.size(); i++) {
             cout << r.caminho[i] << (i < r.caminho.size() - 1 ? " -> " : "");
         }
         cout << endl;
        
    }

    cout << "\nMenor custo encontrado: Cidade " << melhorOrigemGeral 
         << " (Custo: " << melhorCustoGeral << ")" << endl;

    return 0;
}