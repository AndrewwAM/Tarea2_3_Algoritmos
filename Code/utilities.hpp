#include <bits/stdc++.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <iomanip>

using namespace std;


/**
 * @brief Convierte segundos en un formato de tiempo legible.
 * 
 * @param seg El tiempo en segundos.
 */
void timeFormat(int seg){
    // Calcular las horas, minutos y segundos
    int horas = seg / 3600;
    int minutos = (seg % 3600) / 60;
    int segundos_restantes = seg % 60;

    // Mostrar el resultado en formato 00h:00m:00s
    std::cout << "\rTiempo transcurrido: "
              << std::setw(2) << std::setfill('0') << horas << "h:"
              << std::setw(2) << std::setfill('0') << minutos << "m:"
              << std::setw(2) << std::setfill('0') << segundos_restantes << "s";
}


/**
 * @brief Función para mostrar un temporizador en la consola.
 * 
 * @param ejecutando Variable atómica que controla la ejecución del temporizador.
 */
void showTimer(std::atomic<bool>& ejecutando) {
    float delay = 1;
    auto tiempo_inicio = chrono::high_resolution_clock::now();

    while (ejecutando) {
        // Tiempo actual
        auto tiempo_actual = chrono::high_resolution_clock::now();

        // Calcula la diferencia entre el tiempo actual y el tiempo de inicio
        auto tiempo_transcurrido = chrono::duration_cast<chrono::seconds>(tiempo_actual - tiempo_inicio).count();

        // Verifica si ha pasado la duración deseada
        if (tiempo_transcurrido >= delay) {
            break;
        }
    }
    int segundos = 1;
    while (ejecutando) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timeFormat(++segundos);
        std::cout.flush();
    }
}

