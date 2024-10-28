
import random

def generar_numeros_aleatorios(cantidad, archivo):
    with open(archivo, 'w') as f:
        for _ in range(cantidad):
            numero = round(random.uniform(0, 5), 2)  # Genera un número aleatorio entre 0 y 5 con dos decimales
            f.write(f"{numero}\n")  # Escribe el número en el archivo con un salto de línea

# Configuración
cantidad_numeros = 100000  # Cambia esto por la cantidad de números que desees generar
nombre_archivo = 'data.txt'

# Generar números aleatorios y guardar en el archivo
generar_numeros_aleatorios(cantidad_numeros, nombre_archivo)

print(f"{cantidad_numeros} números aleatorios generados en '{nombre_archivo}'.")
