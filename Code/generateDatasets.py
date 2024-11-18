import random
import string
import os

# Ruta donde se guardarán los datasets
path = "./datasets/"

def random_string(length, alphabet=string.ascii_lowercase):
    """Genera un string aleatorio de un tamaño dado."""
    return ''.join(random.choices(alphabet, k=length))

def generate_dataset_s1_empty(max_length):
    """Dataset 1: S1 vacío y S2 de largo variable."""
    file_path = os.path.join(path, "dataset_s1_empty.txt")
    with open(file_path, "w") as f:
        f.write(f"{max_length + 1}\n")
        f.write("0 0\n\n\n")  # Caso ambos vacíos
        for size in range(1, max_length + 1):
            s1 = ""
            s2 = random_string(size)
            f.write(f"{len(s1)} {len(s2)}\n{s1}\n{s2}\n")

def generate_dataset_s2_empty(max_length):
    """Dataset 2: S2 vacío y S1 de largo variable."""
    file_path = os.path.join(path, "dataset_s2_empty.txt")
    with open(file_path, "w") as f:
        f.write(f"{max_length + 1}\n")
        f.write("0 0\n\n\n")  # Caso ambos vacíos
        for size in range(1, max_length + 1):
            s1 = random_string(size)
            s2 = ""
            f.write(f"{len(s1)} {len(s2)}\n{s1}\n{s2}\n")

def generate_dataset_repeated_chars(max_length):
    """Dataset 3: S1 y S2 iguales, compuestos por un único carácter repetido."""
    file_path = os.path.join(path, "dataset_repeated_chars.txt")
    with open(file_path, "w") as f:
        f.write(f"{max_length + 1}\n")
        f.write("0 0\n\n\n")  # Caso ambos vacíos
        for size in range(1, max_length + 1):
            char = random.choice(string.ascii_lowercase)  # Elegir un carácter al azar
            s1 = char * size
            s2 = char * size
            f.write(f"{len(s1)} {len(s2)}\n{s1}\n{s2}\n")

def generate_dataset_transposed(max_length):
    """Dataset 4: S1 es una transposición por pares de S2."""
    file_path = os.path.join(path, "dataset_transposed.txt")
    with open(file_path, "w") as f:
        f.write(f"{max_length + 1}\n")
        f.write("0 0\n\n\n")  # Caso ambos vacíos
        for size in range(1, max_length + 1):
            s2 = random_string(size)
            # Crear S1 como transposición de S2 por pares
            s1 = ''.join(
                s2[i + 1] + s2[i] if i + 1 < len(s2) else s2[i]
                for i in range(0, len(s2), 2)
            )
            f.write(f"{len(s1)} {len(s2)}\n{s1}\n{s2}\n")

def generate_dataset_fixed_s2(max_length):
    """Dataset 5: S2 fijo y S1 creciendo progresivamente."""
    file_path = os.path.join(path, "dataset_fixed_s2.txt")
    with open(file_path, "w") as f:
        f.write(f"{max_length + 1}\n")
        f.write("0 0\n\n\n")  # Caso ambos vacíos
        s1_full = random_string(max_length)  # Generar el string completo de S1
        s2 = random_string(max_length)      # S2 fijo
        for size in range(1, max_length + 1):
            s1 = s1_full[:size]  # Tomar los primeros 'size' caracteres de S1 completo
            f.write(f"{len(s1)} {len(s2)}\n{s1}\n{s2}\n")

def generate_all_datasets():
    """Genera todos los datasets especificados."""
    max_length = 22

    # Crear el directorio si no existe
    os.makedirs(path, exist_ok=True)

    # Generar cada dataset
    generate_dataset_s1_empty(max_length)
    generate_dataset_s2_empty(max_length)
    generate_dataset_repeated_chars(max_length)
    generate_dataset_transposed(max_length)
    generate_dataset_fixed_s2(max_length)

if __name__ == "__main__":
    generate_all_datasets()
