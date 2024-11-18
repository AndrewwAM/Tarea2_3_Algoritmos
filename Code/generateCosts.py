import numpy as np

# Frecuencias aproximadas de letras en inglés (en porcentaje)
LETTER_FREQUENCIES = {
    'a': 8.2, 'b': 1.5, 'c': 2.8, 'd': 4.3, 'e': 13, 'f': 2.2, 'g': 2.0, 
    'h': 6.1, 'i': 7.0, 'j': 0.15, 'k': 0.77, 'l': 4.0, 'm': 2.4, 'n': 6.7,
    'o': 7.5, 'p': 1.9, 'q': 0.095, 'r': 6.0, 's': 6.3, 't': 9.1, 'u': 2.8,
    'v': 0.98, 'w': 2.4, 'x': 0.15, 'y': 2.0, 'z': 0.074
}

def normalize_frequencies():
    """Normaliza las frecuencias a un rango de 0 a 10 (entero)"""
    max_freq = max(LETTER_FREQUENCIES.values())
    return {k: int(10 * v/max_freq) for k, v in LETTER_FREQUENCIES.items()}

def get_keyboard_distance(c1, c2):
    """Calcula una aproximación de la distancia en teclado QWERTY"""
    keyboard = [
        'qwertyuiop',
        'asdfghjkl',
        'zxcvbnm'
    ]
    
    # Encuentra posiciones
    pos1 = pos2 = None
    for row_idx, row in enumerate(keyboard):
        if c1 in row:
            pos1 = (row_idx, row.index(c1))
        if c2 in row:
            pos2 = (row_idx, row.index(c2))
        if pos1 and pos2:
            break
    
    if not (pos1 and pos2):
        return 3  # valor por defecto
    
    # Distancia Manhattan redondeada a entero
    return int(((pos1[0] - pos2[0])**2 + (pos1[1] - pos2[1])**2)**0.5)

def generate_insert_costs(normalized_freqs):
    """Genera costos de inserción basados en frecuencia de letras (1-3)"""
    costs = []
    for c in 'abcdefghijklmnopqrstuvwxyz':
        # Letras más comunes son más baratas de insertar
        # Rango: 1-3
        cost = 3 - min(2, normalized_freqs[c] // 4)
        costs.append(str(cost))
    return " ".join(costs)

def generate_delete_costs(normalized_freqs):
    """Genera costos de eliminación (2-4)"""
    costs = []
    for c in 'abcdefghijklmnopqrstuvwxyz':
        # Eliminar es más costoso que insertar
        # Rango: 2-4
        cost = 4 - min(2, normalized_freqs[c] // 4)
        costs.append(str(cost))
    return " ".join(costs)

def generate_replace_costs():
    """Genera matriz de costos de reemplazo (1-5)"""
    costs = []
    normalized_freqs = normalize_frequencies()
    
    for c1 in 'abcdefghijklmnopqrstuvwxyz':
        row = []
        for c2 in 'abcdefghijklmnopqrstuvwxyz':
            if c1 == c2:
                row.append("0")
            else:
                # Combina distancia de teclado con frecuencias
                keyboard_factor = get_keyboard_distance(c1, c2)
                freq_factor = abs(normalized_freqs[c1] - normalized_freqs[c2])
                cost = 1 + min(4, keyboard_factor + (freq_factor // 2))
                row.append(str(cost))
        costs.append(" ".join(row))
    return "\n".join(costs)

def generate_transpose_costs():
    """Genera matriz de costos de transposición (1-4)"""
    costs = []
    normalized_freqs = normalize_frequencies()
    
    for c1 in 'abcdefghijklmnopqrstuvwxyz':
        row = []
        for c2 in 'abcdefghijklmnopqrstuvwxyz':
            if c1 == c2:
                row.append("0")
            else:
                # Transposición más barata que reemplazo
                keyboard_factor = get_keyboard_distance(c1, c2)
                freq_factor = abs(normalized_freqs[c1] - normalized_freqs[c2])
                cost = 1 + min(3, keyboard_factor + (freq_factor // 3))
                row.append(str(cost))
        costs.append(" ".join(row))
    return "\n".join(costs)

def main():
    path = "./costs/"
    normalized_freqs = normalize_frequencies()
    operations = {
        'insert': generate_insert_costs,
        'delete': generate_delete_costs,
        'replace': generate_replace_costs,
        'transpose': generate_transpose_costs
    }
    
    for op_name, generator in operations.items():
        with open(path + f"cost_{op_name}.txt", "w") as f:
            if op_name in ['insert', 'delete']:
                content = generator(normalized_freqs)
            else:
                content = generator()
            f.write(content)
            f.write("\n")

if __name__ == "__main__":
    main()