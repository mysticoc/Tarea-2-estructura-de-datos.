#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"
#include "tdas/extra.h"

typedef struct {
  char id_cancion[50];
  char nombre_artistas[250];
  char nombre_album[250];
  char nombre_cancion[250];
  float velocidad_tempo;
  char genero_musical[100];
} DatosCancion;

Map *mapa_por_genero;
Map *mapa_por_artista;

List *lista_canciones_lentas;
List *lista_canciones_moderadas;
List *lista_canciones_rapidas;

int comparar_cadenas_texto(void *clave1, void *clave2) {
  return strcmp((char *)clave1, (char *)clave2) == 0;
}

void inicializar_estructuras_datos() {
  mapa_por_genero = map_create(comparar_cadenas_texto);
  mapa_por_artista = map_create(comparar_cadenas_texto);

  lista_canciones_lentas = list_create();
  lista_canciones_moderadas = list_create();
  lista_canciones_rapidas = list_create();
}

void agregar_cancion_a_mapa_agrupado(Map *mapa_destino, char *clave_busqueda, DatosCancion *cancion_nueva) {
  MapPair *par_existente = map_search(mapa_destino, clave_busqueda);
  if (par_existente == NULL) {
    List *nueva_lista_agrupada = list_create();
    list_pushBack(nueva_lista_agrupada, cancion_nueva);
    map_insert(mapa_destino, clave_busqueda, nueva_lista_agrupada);
  } else {
    List *lista_existente = (List*)par_existente->value;
    list_pushBack(lista_existente, cancion_nueva);
  }
}

void imprimir_datos_cancion(DatosCancion *cancion_actual) {
  printf("- %s | Artista(s): %s | Album: %s | Tempo: %.1f BPM | Genero: %s\n",
  cancion_actual->nombre_cancion,
  cancion_actual->nombre_artistas,
  cancion_actual->nombre_album,
  cancion_actual->velocidad_tempo,
  cancion_actual->genero_musical);
}
void mostrar_canciones_paginadas(List *lista_a_mostrar) {
  if (lista_a_mostrar == NULL || list_size(lista_a_mostrar) == 0) {
    printf("No se encontraron canciones registradas enesta categoria.\n");
    return;
  }
  int contador_impresas = 0;
  int total_canciones = list_size(lista_a_mostrar);
  for (DatosCancion *cancion_iteradora = list_first(lista_a_mostrar);
       cancion_iteradora != NULL;
       cancion_iteradora = list_next(lista_a_mostrar)) {
      imprimir_datos_cancion(cancion_iteradora);
      contador_impresas++;

      if(contador_impresas >= 30) {
        printf("\n... (Mostrando 30 de %d canciones encontradas en total)\n", total_canciones);
        break;
      }
    }
  }

void cargar_base_de_datos() {
  char ruta_archivo_csv[256];
  printf("Ingrese el nombre/ruta del archivo CSV (ej. song_dataset_.csv): ");
  scanf("%255s", ruta_archivo_csv);

  int caracter_buffer;
  while ((caracter_buffer = getchar()) != '\n' && caracter_buffer != EOF);
  FILE *archivo_datos = fopen(ruta_archivo_csv, "r");
  if (archivo_datos == NULL) {
    printf("Error: No se pudo abrir el archivo en la ruta '%s'. Verifique el nombre.\n", ruta_archivo_csv);
    return;
  }
  char **campos_csv = leer_linea_csv(archivo_datos, ',');
  if (campos_csv == NULL) {
    printf("El archivo está vacío.\n");
    fclose(archivo_datos);
    return;
  }
  int indice_id = 1;
  int indice_artistas = 2;
  int indice_album = 3;
  int indice_cancion = 4;
  int indice_tempo = 18;
  int indice_genero = 20;

  int total_canciones_cargadas = 0;
  while ((campos_csv = leer_linea_csv(archivo_datos, ',')) !=NULL ) {
    int cantidad_columnas = 0;
    while (campos_csv[cantidad_columnas] != NULL) {
      cantidad_columnas++;
    }
    if (cantidad_columnas <= indice_genero || cantidad_columnas <= indice_tempo) {
      continue;
    }
    DatosCancion *cancion_nueva = (DatosCancion *)malloc(sizeof(DatosCancion));
    if (cancion_nueva == NULL) {
      printf("Error de memoria al intentar cargar una nueva cancion.\n");
      break;
    }
    strncpy(cancion_nueva->id_cancion, campos_csv[indice_id], 49);
    cancion_nueva->id_cancion[49] = '\0';
    strncpy(cancion_nueva->nombre_artistas, campos_csv[indice_artistas], 249);
    cancion_nueva->nombre_artistas[249] = '\0';
    strncpy(cancion_nueva->nombre_album, campos_csv[indice_album],  249);
    cancion_nueva->nombre_album[249] = '\0';
    strncpy(cancion_nueva->nombre_cancion, campos_csv[indice_cancion], 249);
    cancion_nueva->nombre_cancion[249] = '\0';
    cancion_nueva->velocidad_tempo = atof(campos_csv[indice_tempo]);
    strncpy(cancion_nueva->genero_musical, campos_csv[indice_genero], 99);
    cancion_nueva->genero_musical[99] = '\0';

    agregar_cancion_a_mapa_agrupado(mapa_por_genero, cancion_nueva->genero_musical, cancion_nueva);
    agregar_cancion_a_mapa_agrupado(mapa_por_artista, cancion_nueva->nombre_artistas, cancion_nueva);

    if (cancion_nueva->velocidad_tempo < 80.0) {
      list_pushBack(lista_canciones_lentas, cancion_nueva);
    } else if (cancion_nueva->velocidad_tempo >= 80.0 && cancion_nueva->velocidad_tempo <= 120.0) {
      list_pushBack(lista_canciones_moderadas, cancion_nueva);
    } else {
      list_pushBack(lista_canciones_rapidas, cancion_nueva);
    }
    total_canciones_cargadas++;
  }
  fclose(archivo_datos);
  printf("\nCarga exitosa. Se cargaron correctamente %d canciones a la memoria.\n", total_canciones_cargadas);
}

void buscar_por_genero_musical() {
  char genero_ingresado[100];
  printf("Ingrese el género musical a buscar (ej. anime, acoustic, salsa): ");
  fgets(genero_ingresado, 100, stdin);
  genero_ingresado[strcspn(genero_ingresado, "\n")] = 0;

  MapPair *par_resultado = map_search(mapa_por_genero, genero_ingresado);

  if (par_resultado != NULL) {
    printf("\n=== Resultados para el género: '%s' ===\n", genero_ingresado);
    List *lista_resultados = (List *)par_resultado->value;
    mostrar_canciones_paginadas(lista_resultados);
  } else {
    printf("No se encontraron canciones asociadas al género '%s'.\n", genero_ingresado);
  }
}

void buscar_por_artista_musical() {
  char artista_ingresado[250];
  printf("Ingresa el nombre del artista a buscar: ");
  fgets(artista_ingresado, 250, stdin);
  artista_ingresado[strcspn(artista_ingresado, "\n")] = 0;

  MapPair *par_resultado = map_search(mapa_por_artista, artista_ingresado);

  if (par_resultado != NULL) {
    printf("\n=== Canciones disponibles de : '%s' ===\n", artista_ingresado);
    List *lista_resultados = (List *)par_resultado->value;
    mostrar_canciones_paginadas(lista_resultados);
  } else {
    printf("No se encontraron coincidencias exactas para el artista '%s'.\n", artista_ingresado);
  }
}

void buscar_por_rango_de_tempo() {
  int opcion_tempo;
  printf("\nSeleccione la categoria de velocidad deseada:\n");
  printf(" 1. Lentas (Menos de 80 BPM)\n");
  printf(" 2. Moderadas (Entre 80 y 120 BPM)\n");
  printf(" 3. Rápidas (Más de 120 BPM)\n");

  if (scanf("%d", &opcion_tempo) != 1) {
    printf("Entrada inválida. Debe ingresar un número.\n");
  }
  int caracter_buffer;
  while ((caracter_buffer = getchar()) != '\n' && caracter_buffer != EOF);
  switch (opcion_tempo) {
    case 1:
      printf("\n=== Colección de Canciones Lentas ===\n");
      mostrar_canciones_paginadas(lista_canciones_lentas);
      break;
    case 2:
      printf("\n=== Colección de Canciones Moderadas ===\n");
      mostrar_canciones_paginadas(lista_canciones_moderadas);
      break;
    case 3:
      printf("\n=== Colección de Canciones Rápidas ===\n");
      mostrar_canciones_paginadas(lista_canciones_rapidas);
      break;
    default:
      printf("Opción de tempo fuera de rango. Seleccione 1, 2 o 3\n");
  }
}

int main() {
  inicializar_estructuras_datos();
  int opcion_menu_principal;

  do {
    printf("\n===========================\n");
    printf("        SPOTIFIND        ");
    printf("\n 1. Cargar Base de Datos(CSV)\n");
    printf(" 2. Buscar Canciones por Género\n");
    printf(" 3. Buscar Canciones por Artista\n");
    printf(" 4. Buscar Canciones por Velocidad (Tempo)\n");
    printf(" 5. Cerrar Aplicación\n");
    printf("\n===========================\n");
    printf("Seleccione una opción: ");
    if (scanf("%d", &opcion_menu_principal) != 1) {
      opcion_menu_principal = 0;
    }
    int caracter_buffer;
    while ((caracter_buffer = getchar()) != '\n' && caracter_buffer != EOF);

    switch (opcion_menu_principal) {
      case 1:
        cargar_base_de_datos();
        break;
      case 2:
        buscar_por_genero_musical();
        break;
      case 3:
        buscar_por_artista_musical();
        break;
      case 4:
        buscar_por_rango_de_tempo();
        break;
      case 5:
        printf("Saliendo del programa con éxito.\n");
        break;
      default:
        printf("Opción no válida. Por favor ingrese un número del 1 al 5.\n");
    }
  } while(opcion_menu_principal != 5);
  return 0;
}