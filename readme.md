# Sistema de Gestión y Buscador de Canciones Spotifind

Este Proyecto es una aplicación que carga una base de datos de canciones con distintos "atributos" y permite buscarlas eficientemente con criterios ademas de poder ordenarlos 

## 1. Cómo compilar y ejecutar la tarea

El proyecto depende de los archivos genéricos de la estructura de datos ubicados en la carpeta 'tdas/'. Asegúrese de tener GCC instalado en su sistema.

**Para compilar**
Abre tu terminal en el directorio raíz del proyecto (donde se encuentra 'tarea2.c') y ejecuta el siguiente comando:
```bash
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2

```

**Para ejecutar**
Una vez finalizada la compilación, inicie la aplicación según su sistema operativo: 

```bash
# En Linux o Mac
./tarea2

# En Windows
tarea2.exe

```

## 2. Opciones y estado de las funcionalidades

Todas las funcionalidades requeridas operan correctamente y manejan la memoria de manera eficiente. No hay opciones defectuosas.

- **Cargar las canciones de la base de datos**: Funciona correctamente leyendo el CSV e indexando cada cancion directamente en los mapas correspondientes en tiempo de carga, ignorando saltos de linea o ingresos erroneos de campos faltantes.
- **Buscar por Genero**: Busca la cancion por su genero de manera muy eficiente con complejidad de O(1). Al usar un mapa que almacena una lista enlazada como valor, la busqueda retorna inmediatamente la coleccion entera de canciones para mostrarse.
- **Buscar por Artista**: Funciona bajo la misma logica eficiente de complejidad O(1) usando mapas. Note que el input del artista debe ser exacto, fijarse bien con las mayusculas si en el database aparece con mayusculas.
- **Buscar por Tempo**: Funciona en complejidad O(1) usando listas pre-clasificadas estaticas que se nutrieron durante la carga original, se ha limitado visualmente la consola para que la imprima maximo 30 canciones para no desbordar el bufer de pantalla. 
## 3. Ejemplo de interacción.

1. Se inicia la aplicación e ingresa al Menu Principal.
2. Se selecciona la Opción 1. La consola pide el data set, escribes la base de datos de song_dataset_.csv
3. Se seleccciona la Opcion 2, 3 o 4. 
4. Se ingresa a la Opción 2, se busca a 'Nicolás Garrido' y se le asigna la prioridad '1' (VIP).
5. 5 Se ingresa a la Opción 3. El sistema imprime primero a 'Nicolás Garrido' etiquetado como [VIP] y debajo a 'Ivan García' como [Estándar].
6. Se ingresa a la Opción 4. El sistema remueve a 'Nicolás Garrido' de la cola y le asigna la mesa.
7. Se selecciona la Opción 6 para cerrar el sistema.

## 4. Contribuciones
* Trabajo realizado de manera individual por Nicolás Ignacio Garrido Vergara
  