#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
typedef vector<vector<int>> matrix;
typedef vector<int> arr;

arr cost_insert;
arr cost_delete;
matrix cost_replace;
matrix cost_transpose;

matrix cache;
string path, outputPath, datasetPath, resultsPath;
string input1, input2;
string dataset;
int choice = 1;
queue<string> inputQ;
vector<string> categories = {"insert", "delete", "replace", "transpose"};
int result;
int numDatasetCases;

void printRow(arr row){
    for(int index : row){
        cout << "." << index << " ";
    }
    cout << endl;
}


void printMatrix(matrix mtx){
    for(arr row : mtx){
        printRow(row);   
    }
}


void readInsert(string path){
    cost_insert.clear();
    ifstream file(path);
    int i = 26;
    while(i--){
        if(file.eof()) break;
        int value;
        file >> value;
        cost_insert.push_back(value);
    }
    file.close();
    return;
}


void readDelete(string path){
    cost_delete.clear();
    ifstream file(path);
    int i = 26;
    while(i--){
        if(file.eof()) break;
        int value;
        file >> value;
        cost_delete.push_back(value);
    }
    file.close();
    return;
}


void readReplace(string path){
    cost_replace.clear();
    ifstream file(path);
    arr row;
    int i = 26;
    while(i--){
        row.clear();
        if(file.eof()) break;
        int j = 26;
        while(j--){
            int value;
            file >> value;
            row.push_back(value);
        }
        cost_replace.push_back(row);
    }
    file.close();
    return;
}


void readTranspose(string path){
    cost_transpose.clear();
    ifstream file(path);
    arr row;
    int i = 26;
    while(i--){
        row.clear();
        if(file.eof()) break;
        int j = 26;
        while(j--){
            int value;
            file >> value;
            row.push_back(value);
        }
        cost_transpose.push_back(row);
    }
    file.close();
    return;
}


void loadCosts(){
    string fileName;
    int i = 0;
    for(string category : categories){
        fileName = path + "cost_" + category + ".txt";
        switch (i)
        {
        case 0:
            readInsert(fileName);
            break;
        case 1:
            readDelete(fileName);
            break;
        case 2:
            readReplace(fileName);
            break;
        case 3:
            readTranspose(fileName);
            break;
        default:
            cerr << "Oh no" << endl;
            break;
        }
        i++;
    }
}


/**
 * @brief Escribe estadísticas de tiempo en un archivo.
 * 
 * @param path La ruta del archivo de salida.
 * @param time El tiempo de ejecución.
 * @param algorithm El nombre del algoritmo.
 * @param datatype El tipo de datos.
 * @param numCases El número de casos.
 */
void writeStatsDetail(string path, long double time, string algorithm, string datatype, string s1size, string s2size, string memoryUse){
    ofstream file(path, ios::app);
    if (!file.is_open()){
        cerr << "No se pudo abrir el archivo: " << path << endl;
        return;
    }
    string line = "Algorithm: " + algorithm + " | Dataset: " + datatype + " | S1 size: " + s1size + " S2 size: " + s2size + " | Time: " + to_string(time) + " microsegundos | " + to_string(time/1000000) + " segundos | Memoria: " + memoryUse + " bytes";
    file << line << endl;
    file.close();
}


void writeStats(string path, long double time, string algorithm, string datatype, string s1size, string s2size, string memoryUse){
    ofstream file(path, ios::app);
    if (!file.is_open()){
        cerr << "No se pudo abrir el archivo: " << path << endl;
        return;
    }
    string line = algorithm + " " + datatype + " " + s1size + " " + s2size + " " + to_string(time) + " " + to_string(time/1000000) + " " + memoryUse;
    file << line << endl;
    file.close();
}


void writeResult(){
    string path = resultsPath;
    path += (choice) ? "resultsDp.txt" : "resultsBf.txt";
    string line;
    ofstream file(path, ios::app);
    if (!file.is_open()){
        cerr << "No se pudo abrir el archivo: " << path << endl;
        return;
    }
    line = "From: " + dataset;
    file << line << endl;
    line = "s1: " + input1;
    file << line << endl;
    line = "s2: " + input2;
    file << line << endl;
    line = "Result: ";
    line += to_string(result);
    file << line << endl;
    file.close();
}


void loadInputData(string dataset){
    ifstream file(datasetPath + dataset + ".txt");
    if (!file.is_open()){
        cerr << "No se pudo abrir el archivo: " << path << dataset << ".txt" << endl;
        return;
    }
    string in1, in2;
    int numcases, n, m;
    file >> numcases;
    numDatasetCases = numcases;
    for (int i = 0; i < numcases; i ++){
        file >> n >> m;
        
        if (n == 0 && m == 0){
            in1 = "";
            in2 = "";
        }
        else if (n == 0){
            in1 = "";
            file >> in2;
        } else if (m == 0){
            in2 = "";
            file >> in1;
        } else {
            file >> in1;
            file >> in2;
        }

        inputQ.push(in1);
        inputQ.push(in2);
    }
    return;
}


int getIndex( char a ){
    int index;
    int offset = 97;
    index = (a - offset);
    return index;
}


// Calcularemos el costo de sustituir el carácter.
// Parámetros:
// − a : carácter original
// − b : carácter con el que se sustituye
// Return:  costo de sustituir 'a' por 'b'
int costo_sub( char a , char b ) {
    int costo;
    int i = getIndex(a);
    int j = getIndex(b);
    costo = cost_replace[i][j];
    return costo;
}


// Calcularemos el costo de insertar el carácter 'b'.
// Parámetros:
// − b : carácter a insertar
// Return:  costo de insertar 'b'
int costo_ins( char b ) {
    int costo;
    int j = getIndex(b);
    costo = cost_insert[j];
    return costo;
}


// Calcularemos el costo de eliminar el carácter 'a'.
// Parámetros:
// − a : carácter a eliminar
// Return:  costo de eliminar 'a'
int costo_del ( char a ) {
    int costo;
    int i = getIndex(a);
    costo = cost_delete[i];
    return costo;
}


// Calcularemos el costo de transponer los caracteres 'a' y 'b'.
// Parámetros:
// − a : primer carácter a transponer
// − b : segundo carácter a transponer
// Return:  costo de transponer 'a' y 'b'
int costo_trans ( char a , char b ) {
    int costo;
    int i = getIndex(a);
    int j = getIndex(b);
    costo = cost_transpose[i][j];
    return costo;
}

