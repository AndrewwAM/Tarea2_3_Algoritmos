#include <bits/stdc++.h>
#include "Aux.hpp"
#include "BruteForce.hpp"
#include "dynamicSolution.hpp"
#include "utilities.hpp"

bool w_output = true;
bool w_result = true;
int bfInputCap;
long double measuredTime;
long double averageMeasuredTime;
size_t usedMemory;

void writeOutput();
void benchmark(atomic<bool>& ejecutando);
void runTests(int testNum);
void writeResult();
size_t calculateMatrixMemory(const matrix& matrix);

void benchmark(atomic<bool>& ejecutando){
    chrono::high_resolution_clock::time_point start;
    chrono::duration<double, std::micro> duration;
    switch (choice)
    {
    case 0:
        if (input2.size() >= bfInputCap && !choice){
            input2 = input2.substr(0, bfInputCap-1);
        }
        start = chrono::high_resolution_clock::now();
        result = bruteForce(input1, 0, input2, 0);
        break;
    case 1:
        cache.clear();
        start = chrono::high_resolution_clock::now();
        cache = matrix(input1.size()+1, arr(input2.size()+1, 0));
        dynamic(input1, input2, cache);
        // printMatrix(cache);
        break;
    default:
        cerr << "Opción inválida" << endl;
        break;
    }
    auto end = chrono::high_resolution_clock::now();
    duration = end - start;
    measuredTime = duration.count();
    usedMemory = (choice) ? calculateMatrixMemory(cache) : 0;
    ejecutando = false;
}


size_t calculateMatrixMemory(const matrix& matrix) {
    size_t total_memory = 0;
    
    // 1. Memoria del vector externo
    total_memory += sizeof(matrix);  // Overhead del vector externo
    total_memory += sizeof(arr) * matrix.capacity();  // Espacio para los punteros a vectores internos
    
    // 2. Memoria de cada vector interno
    for (const auto& row : matrix) {
        total_memory += sizeof(arr);  // Overhead del vector interno
        total_memory += sizeof(int) * row.capacity();  // Espacio real reservado para los integers
    }
    
    return total_memory;
}


void runTests(int testNum){
    atomic<bool> ejecutando(true);
    int j = 0;
    int tests = testNum;
    averageMeasuredTime = 0;
    long double total = 0;
    input1 = inputQ.front();
    inputQ.pop();
    input2 = inputQ.front();
    inputQ.pop();

    while(j <= tests){
        ejecutando = true;

        cout << "Dataset: " << dataset;
        cout << "| s1: " << input1 << " (" << input1.size() << ") ";
        cout << "| s2: " << input2 << " (" << input2.size() << ")" << endl;

        std::thread timerThread(showTimer, std::ref(ejecutando));

        std::thread algorithmThread(benchmark, std::ref(ejecutando));

        algorithmThread.join();
        timerThread.join();
        cout << "\r";
        
        cout << "\nResult: " << result << endl;
        cout << endl;
        if (j != 0) total += measuredTime;

        string linea = "Caso: " + to_string(j) + " | " + "Tiempo de ejecucion: " + to_string(measuredTime) + " microsegundos / " + to_string(measuredTime/1000000) + " segundos | Memoria: " + to_string(usedMemory) + " bytes" + ((j == 0) ? " (cache)\t\n" : "\t\n");
        std::cout << linea;
        j++;
    }
    averageMeasuredTime = (tests == 0) ? total : (total/tests);
    if(choice) solutionReconstructor(input1, input2);
    if (w_output) writeOutput();
    if (w_result) writeResult();
}


void writeOutput(){
    cout << "\n>escribiendo..." << endl;
    string extra = (choice) ? "Dp" : "Bf";
    writeStatsDetail(outputPath + "outputDetail" + extra + ".txt", averageMeasuredTime, (choice) ? "DynamicSolution" : "BruteForce", dataset, to_string(input1.size()), to_string(input2.size()), to_string(usedMemory));
    writeStats(outputPath + "output" + extra + ".txt", averageMeasuredTime, (choice) ? "DynamicSolution" : "BruteForce", dataset, to_string(input1.size()), to_string(input2.size()), to_string(usedMemory));
    // std::cout << ">Output: " << outputPath << dataset << ".txt" << endl;
}


int main(){
    w_output = true;
    w_result = true;
    bool full = false; // Si se ejecuta a través de todos los datasets o es por selección manual (linea ~140)
    path = "./costs/";
    datasetPath = "./datasets/";
    outputPath = "./output/";
    resultsPath = "./results/";
    int tests = 1; // NO poner mas de 3 para bruteforce
    bfInputCap = 16; // 16 es bueno, 11 min maximo en el peor caso

    choice = 0; // 0 -> bruteForce | 1 -> dynamic 
    cout << "Seleccione el tipo de algoritmo: (0) Fuerza bruta / (1) Dinamica\n> ";
    cin >> choice;
    if(choice != 0 && choice != 1){
        cout << "Selección inválida" << endl;
        return -1;
    }

    vector<string> datasets = {"dataset_s1_empty", "dataset_s2_empty", "dataset_transposed", "dataset_fixed_s2", "dataset_repeated_chars"};
    loadCosts();
    if (!full){
        dataset = datasets[3]; // Selección manual
        
        // dataset = "dataset_test";
        w_output = false;
        w_result = false;
        
        loadInputData(dataset);
        cout << "#### Dataset: " << dataset << " ####" << endl;
        if (!choice) numDatasetCases = bfInputCap;
        for(int i = 0; i < numDatasetCases; i++){
            runTests(tests);
        }
        while(!inputQ.empty()){
            inputQ.pop();
        }
        return 0;
    }
    for (string datasetName : datasets){
        dataset = datasetName;
        loadInputData(dataset);
        cout << "#### Dataset: " << dataset << " ####" << endl;
        if (!choice) numDatasetCases = bfInputCap;
        for(int i = 0; i < numDatasetCases; i++){
            runTests(tests);
        }
        while(!inputQ.empty()){
            inputQ.pop();
        }
    }

    return 0;
}