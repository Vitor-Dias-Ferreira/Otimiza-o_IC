#include <iostream>
#include <algorithm> // Necessário para a função std::reverse
#include <cmath>
#include <sstream>
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
bool lerTSPLIB(vector<vector<int>>& matriz, int& n, const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        cerr << "Erro: Arquivo TSPLIB nao encontrado!" << endl;
        return false;
    }

    string linha;
    vector<pair<double, double>> coordenadas;
    bool lendoCoordenadas = false;

    // Lê o arquivo linha por linha
    while (getline(arquivo, linha)) {
        // Ignora linhas vazias
        if (linha.empty()) continue;

        // Pega a dimensão (quantidade de nós)
        if (linha.find("DIMENSION") != string::npos) {
            size_t pos = linha.find(":");
            if (pos != string::npos) {
                n = stoi(linha.substr(pos + 1));
            }
        } 
        // Identifica onde começam as coordenadas
        else if (linha.find("NODE_COORD_SECTION") != string::npos) {
            lendoCoordenadas = true;
            continue;
        } 
        // Identifica o fim do arquivo
        else if (linha.find("EOF") != string::npos) {
            break;
        }

        // Lê os nós (formato: ID X Y)
        if (lendoCoordenadas) {
            int id;
            double x, y;
            stringstream ss(linha);
            ss >> id >> x >> y;
            coordenadas.push_back({x, y});
        }
    }
    arquivo.close();

    // Se não leu nada, retorna erro
    if (coordenadas.empty() || coordenadas.size() != n) {
        cerr << "Erro: Coordenadas nao carregadas corretamente." << endl;
        return false;
    }

    // Inicializa e calcula a matriz de distâncias (Euclidiana com arredondamento padrão TSPLIB)
    matriz.assign(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double dx = coordenadas[i].first - coordenadas[j].first;
                double dy = coordenadas[i].second - coordenadas[j].second;
                // A TSPLIB recomenda arredondar para o inteiro mais próximo
                matriz[i][j] = round(sqrt(dx * dx + dy * dy)); 
            }
        }
    }
    return true;
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

    vector<vector<int>> matriz;
    int n = 0;
    int opcao = 0;

    // --- MENU DE ESCOLHA ---
    cout << "======================================" << endl;
    cout << "  PROBLEMA DO CAIXEIRO VIAJANTE (PCV) " << endl;
    cout << "======================================" << endl;
    cout << "Escolha como deseja ler os dados:" << endl;
    cout << "1 - Digitar matriz manualmente no terminal" << endl;
    cout << "2 - Ler matriz de distancias (.txt)" << endl;
    cout << "3 - Ler coordenadas TSPLIB (.tsp)" << endl;
    cout << "Opcao: ";
    cin >> opcao;

    if (opcao == 1) {
        cout << "Digite o numero de cidades (n): ";
        cin >> n;
        matriz.assign(n, vector<int>(n, 0));
        lerMatriz(matriz, n);
    } 
    else if (opcao == 2 || opcao == 3) {
        string nomeArquivo;
        cout << "Digite o nome do arquivo (ex: 8x8.txt ou berlin52.tsp): ";
        cin >> nomeArquivo;

        // Monta o caminho completo apontando para a pasta instances
        string caminhoCompleto = "../instances/" + nomeArquivo;
        if (opcao == 2) {
            if (!lerMatrizArquivo(matriz, n, caminhoCompleto)) return 1;
        } else {
            if (!lerTSPLIB(matriz, n, caminhoCompleto)) return 1;
        }
    } 
    else {
        cout << "Opcao invalida! Encer=ando..." << endl;
        return 1;
    }

    cout << "\nMatriz carregada com sucesso! N = " << n << endl;
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
        //cout << " -> Rota: ";
        // for (size_t i = 0; i < r.caminho.size(); i++) {
        //     cout << r.caminho[i] << (i < r.caminho.size() - 1 ? " -> " : "");
        // }
        // cout << endl;
        
    }

    cout << "\nMenor custo encontrado: Cidade " << melhorOrigemGeral 
         << " (Custo: " << melhorCustoGeral << ")" << endl;

    return 0;
}