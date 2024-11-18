import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path
import math

# Crear directorio para imágenes si no existe
Path("imagenes").mkdir(exist_ok=True)

# Configuración global de matplotlib para mejor legibilidad
plt.rcParams.update({
    'font.size': 12,
    'axes.labelsize': 14,
    'axes.titlesize': 16,
    'legend.fontsize': 12,
    'lines.markersize': 10,
    'lines.linewidth': 2
})

# Función para leer y procesar los archivos
def read_algorithm_data(filename):
    df = pd.read_csv(filename, sep=' ', header=None,
                     names=['Algorithm', 'Dataset', 'S1_Length', 'S2_Length', 
                           'Time_Microseconds', 'Time_Seconds', 'Memory_Bytes'])
    return df

# Leer los datos
df_bf = read_algorithm_data('output/outputBf.txt')
df_dp = read_algorithm_data('output/outputDp.txt')

# Función para calcular complejidades teóricas
def bf_complexity(n, m):
    return 4 ** min(n, m)

def dp_complexity(n, m):
    return n * m

# Función para generar gráfico por dataset con mejoras visuales
def plot_dataset_comparison(dataset_name, df_bf, df_dp, normalize_complexity=True):
    plt.figure(figsize=(8, 8)) # Tamaño más compacto
    
    # Filtrar datos para el dataset específico
    bf_data = df_bf[df_bf['Dataset'] == dataset_name]
    dp_data = df_dp[df_dp['Dataset'] == dataset_name]
    
    # Determinar variable para eje x
    if dataset_name == 'dataset_fixed_s2':
        bf_x = bf_data['S1_Length']
        dp_x = dp_data['S1_Length']
        x_label = 'Tamaño de s1 (con |s2|=22)'
        max_x = max(max(bf_x), max(dp_x))
    else:
        bf_x = np.maximum(bf_data['S1_Length'], bf_data['S2_Length'])
        dp_x = np.maximum(dp_data['S1_Length'], dp_data['S2_Length'])
        x_label = 'Tamaño máximo de entrada (max(|s1|, |s2|))'
        max_x = max(max(bf_x), max(dp_x))
    
    # Graficar tiempos reales con marcadores distintivos
    plt.semilogy(bf_x, bf_data['Time_Microseconds'], 'D-', 
                 label='Fuerza Bruta', color='#FF4136', markersize=8)
    plt.semilogy(dp_x, dp_data['Time_Microseconds'], 's-', 
                 label='Programación Dinámica', color='#0074D9', markersize=8)
    
    # Calcular y graficar complejidades teóricas
    x_range = np.linspace(min(min(bf_x), min(dp_x)), max_x, 100)
    
    if dataset_name == 'dataset_fixed_s2':
        bf_theoretical = [bf_complexity(x, 22) for x in x_range]
        dp_theoretical = [dp_complexity(x, 22) for x in x_range]
    else:
        bf_theoretical = [bf_complexity(x, x) for x in x_range]
        dp_theoretical = [dp_complexity(x, x) for x in x_range]
    
    # Normalizar las líneas teóricas para que coincidan con los datos reales
    if normalize_complexity:
        # Prevenir división por cero y valores muy pequeños
        bf_theoretical_values = [bf_complexity(x, 22 if dataset_name == 'dataset_fixed_s2' else x) 
                               for x in bf_x]
        dp_theoretical_values = [dp_complexity(x, 22 if dataset_name == 'dataset_fixed_s2' else x) 
                               for x in dp_x]
        
        # Usar valores máximos para la normalización
        bf_scale = max(bf_data['Time_Microseconds']) / max(bf_theoretical_values)
        dp_scale = max(dp_data['Time_Microseconds']) / max(dp_theoretical_values)
    
    plt.semilogy(x_range, bf_theoretical, '--', color='#FF851B', 
                 label='O(4^min(n,m))', alpha=0.7, linewidth=2)
    plt.semilogy(x_range, dp_theoretical, '--', color='#7FDBFF', 
                 label='O(n*m)', alpha=0.7, linewidth=2)
    
    plt.title(f'Comparación de Rendimiento - {dataset_name}', pad=20)
    plt.xlabel(x_label, labelpad=10)
    plt.ylabel('Tiempo de ejecución (microsegundos)', labelpad=10)
    plt.grid(True, which="both", ls="-", alpha=0.2)
    # Cambio en la posición de la leyenda
    plt.legend(loc='best')
    
    plt.tight_layout()
    plt.savefig(f'imagenes/comparacion_{dataset_name}.png', dpi=300)
    plt.close()

# Función para generar gráfico general con mejoras visuales
def plot_general_comparison(df_bf, df_dp):
    plt.figure(figsize=(15, 10))
    
    # Definir marcadores distintos para cada dataset
    markers = ['o', 's', 'D', '^', 'v', '<', '>', 'p', 'h']
    
    datasets = df_bf['Dataset'].unique()
    
    for i, dataset in enumerate(datasets):
        # Datos de Fuerza Bruta
        bf_data = df_bf[df_bf['Dataset'] == dataset]
        if dataset == 'dataset_fixed_s2':
            bf_x = bf_data['S1_Length']
        else:
            bf_x = np.maximum(bf_data['S1_Length'], bf_data['S2_Length'])
            
        plt.semilogy(bf_x, bf_data['Time_Microseconds'], 
                    marker=markers[i], linestyle='-',
                    label=f'BF - {dataset}', 
                    color=plt.cm.Reds(0.5 + i * 0.5/len(datasets)),
                    markersize=10)
        
        # Datos de Programación Dinámica
        dp_data = df_dp[df_dp['Dataset'] == dataset]
        if dataset == 'dataset_fixed_s2':
            dp_x = dp_data['S1_Length']
        else:
            dp_x = np.maximum(dp_data['S1_Length'], dp_data['S2_Length'])
            
        plt.semilogy(dp_x, dp_data['Time_Microseconds'], 
                    marker=markers[i], linestyle='--',
                    label=f'DP - {dataset}', 
                    color=plt.cm.Blues(0.5 + i * 0.5/len(datasets)),
                    markersize=10)
    
    # Añadir líneas teóricas
    max_x = max(
        max(np.maximum(df_bf['S1_Length'], df_bf['S2_Length'])),
        max(np.maximum(df_dp['S1_Length'], df_dp['S2_Length']))
    )
    x_range = np.linspace(1, max_x, 100)
    
    # Calcular valores teóricos
    bf_theoretical = [bf_complexity(x, x) for x in x_range]
    dp_theoretical = [dp_complexity(x, x) for x in x_range]
    
    # Usar el percentil 75 para la normalización de la línea de fuerza bruta
    bf_percentile = np.percentile(df_bf['Time_Microseconds'], 75)
    dp_max_real = max(df_dp['Time_Microseconds'])
    
    # Calcular los valores teóricos en el punto del percentil 75
    x_percentile = np.percentile(np.maximum(df_bf['S1_Length'], df_bf['S2_Length']), 75)
    bf_theoretical_at_percentile = bf_complexity(x_percentile, x_percentile)
    dp_theoretical_at_max = dp_complexity(max_x, max_x)
    
    # Calcular las escalas
    bf_scale = bf_percentile / bf_theoretical_at_percentile
    dp_scale = dp_max_real / dp_theoretical_at_max
    
    plt.semilogy(x_range, [x * bf_scale for x in bf_theoretical], '--',
                 color='#FF851B', label='O(4^n)', alpha=0.7, linewidth=2)
    plt.semilogy(x_range, [x * dp_scale for x in dp_theoretical], '--',
                 color='#7FDBFF', label='O(n*m)', alpha=0.7, linewidth=2)
    
    plt.title('Comparación General de Rendimiento', pad=20)
    plt.xlabel('Tamaño de entrada', labelpad=10)
    plt.ylabel('Tiempo de ejecución (microsegundos)', labelpad=10)
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    
    plt.tight_layout(pad=2.0)
    plt.savefig('imagenes/comparacion_general.png', dpi=300, bbox_inches='tight')
    plt.close()

# Función para gráfico de memoria con mejoras visuales
def plot_memory_comparison(df_bf, df_dp):
    plt.figure(figsize=(15, 10))
    
    markers = ['o', 's', 'D', '^', 'v', '<', '>', 'p', 'h']
    datasets = df_bf['Dataset'].unique()
    max_x = 0
    
    for i, dataset in enumerate(datasets):
        # Datos de Fuerza Bruta
        bf_data = df_bf[df_bf['Dataset'] == dataset]
        if dataset == 'dataset_fixed_s2':
            bf_x = bf_data['S1_Length']
        else:
            bf_x = np.maximum(bf_data['S1_Length'], bf_data['S2_Length'])
            
        plt.plot(bf_x, bf_data['Memory_Bytes'] / 1024, 
                marker=markers[i], linestyle='-',
                label=f'BF - {dataset}', 
                color=plt.cm.Reds(0.5 + i * 0.5/len(datasets)),
                markersize=10)
        max_x = max(max_x, max(bf_x))
        
        # Datos de Programación Dinámica
        dp_data = df_dp[df_dp['Dataset'] == dataset]
        if dataset == 'dataset_fixed_s2':
            dp_x = dp_data['S1_Length']
        else:
            dp_x = np.maximum(dp_data['S1_Length'], dp_data['S2_Length'])
            
        plt.plot(dp_x, dp_data['Memory_Bytes'] / 1024, 
                marker=markers[i], linestyle='--',
                label=f'DP - {dataset}', 
                color=plt.cm.Blues(0.5 + i * 0.5/len(datasets)),
                markersize=10)
        max_x = max(max_x, max(dp_x))
    
    # Líneas teóricas para memoria
    x_range = np.linspace(1, max_x, 100)
    
    # Memoria teórica para BF (O(1))
    bf_theoretical_memory = [1000 for x in x_range]  # Valor constante arbitrario
    
    # Memoria teórica para DP (O(n*m))
    dp_theoretical_memory = [4 * x * x / 1024 for x in x_range]  # Convertir a KB
    
    # Normalizar las líneas teóricas
    bf_scale = np.mean(df_bf['Memory_Bytes'] / 1024) / np.mean(bf_theoretical_memory)
    dp_scale = np.mean(df_dp['Memory_Bytes'] / 1024) / np.mean(dp_theoretical_memory)
    
    plt.plot(x_range, [m * bf_scale for m in bf_theoretical_memory], '--', 
             color='#FF851B', label='Memoria teórica BF - O(1)', alpha=0.7, linewidth=2)
    plt.plot(x_range, [m * dp_scale for m in dp_theoretical_memory], '--', 
             color='#7FDBFF', label='Memoria teórica DP - O(n*m)', alpha=0.7, linewidth=2)
    
    plt.title('Comparación de Uso de Memoria', pad=20)
    plt.xlabel('Tamaño de entrada', labelpad=10)
    plt.ylabel('Uso de memoria (KB)', labelpad=10)
    plt.grid(True, which="both", ls="-", alpha=0.2)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    
    plt.tight_layout(pad=2.0)
    plt.savefig('imagenes/comparacion_memoria.png', dpi=300, bbox_inches='tight')
    plt.close()

# Generar todos los gráficos
datasets = df_bf['Dataset'].unique()
for dataset in datasets:
    plot_dataset_comparison(dataset, df_bf, df_dp)

plot_general_comparison(df_bf, df_dp)
plot_memory_comparison(df_bf, df_dp)