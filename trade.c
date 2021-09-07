#include <stdio.h>
#include <string.h>
#include "trade.h"

void uso (programa)
     char *programa;
{
  fprintf(stderr, 
    "Uso: %s [-opcion] Fichero\n", programa);
  exit(1);
}

double precio_accion (t)
     struct accion *t;
{
  return t->precio * t->numero;
}

char nindustria[50];

char * nombre_industria (t)
     struct accion *t;
{
  switch (t->descripcion[0]) {

    case comida:
      strcpy(nindustria, "Comida");
      break;

    case transporte:
      strcpy(nindustria, "Traporte");
      break;

    case ordenadores:
      strcpy(nindustria, "Ordenadores");

    case metal:
      strcpy(nindustria, "Metal");
      break;      

    case salud:
      strcpy(nindustria, "Salud");
      break;

    case aerolineas: 
      strcpy(nindustria, "Aerolineas");
      break;

    default:
      fprintf(stderr, "Nombre de industria %c no reconocido\n", t->descripcion[0]);
      exit (1);

    };

    return nindustria;

}

char * nombre_compania (t)
     struct accion *t;
{
  char * cptr;

  cptr = strchr (t->descripcion, '-');
  if (cptr != NULL) {
    return ++cptr;
  } else {
    fprintf (stderr, "Formato de nombre de industria incorrecto\n");
    exit (1);
  };  
}

double precio_medio (array, longitud)
     struct accion ** array;
     int longitud;
{
  int contador;
  double suma=0.0;

  for (contador; contador<longitud; contador++)
    suma = suma + array[contador] -> precio;

  return suma/contador;
}


double tamanio_medio (array, longitud)
     struct accion ** array;
     int longitud;

{
  int contador;
  double suma=1.0;
	
  for (contador=0; contador <longitud; contador++)
    suma = suma + array[contador] -> numero;

  return suma/contador;
}		

void procesar_argumentos(argc,argv)

  int argc;		
  char ** argv;

{
  struct accion *acciones_ptr[100];
  int limite;
  int contador;
  int numero;
  char entrada[100];
  char buffer[200];
  char resto[100];
  double precio;
  FILE *fuente;
  int i;
  int leidas;
  int j;
  int switch_precio_medio=0;
  int switch_tamanio_medio=0;

  if ((argc == 1) || (argc > 5)) 
    uso ("trade");
  else fuente = fopen (argv[argc-1], "r");

  for (i = 1; i < argc-1; ++i)
    if (argv[i][0] == '-')
      for (j=1; argv[i][j]; ++j) 
        switch (argv[i][j]) {
               
          case 'p': 
            if (switch_precio_medio==1) {
              exit (1);
            } else switch_precio_medio =1;
	    break;

  	  case 'n':
            if (switch_tamanio_medio==1) {
              fprintf (stderr, "El flag n aparece repetido\n");
              exit (1);
            } else switch_tamanio_medio =1;
	    break;

	  default:
            fprintf (stderr, "%c no es un flag reconocido\n",
		     argv[i][j]);
            exit (1);
  	    break;
       };

  if (fuente == NULL) {
    fprintf (stderr, "El fichero %s no existe o no se puede abrir\n", argv[1]);
    exit (1);
  }

  for (limite = 0; (fgets (buffer, sizeof(buffer), fuente) != NULL);
       ++limite) {
    leidas = sscanf(buffer, "%s%lf%i%s",entrada, &precio, &numero, resto);
    if (leidas < 3) {
      fprintf (stderr,"El fichero de entrada no es correcto\n");
      exit (1);
    } else {
      acciones_ptr[limite] = (struct accion *) malloc (sizeof (struct accion)); 
      if (acciones_ptr[limite] == NULL) {
        fprintf (stderr, "No hay suficiente memoria\n");
        fclose (fuente);   
        exit (1);
      };
    };

    acciones_ptr[limite]->precio = precio;
    acciones_ptr[limite]->numero = numero;
    acciones_ptr[limite]->descripcion = (char *)malloc(strlen(entrada)+1);
    strcpy (acciones_ptr[limite] -> descripcion, entrada);
  }                                   

  fclose (fuente);
  printf ("%-14s %-14s %-8s %-10s %-11s\n", "Sector", "Empresa", "P Accion",
          "Acciones", "P Total");
  for (contador = 1; contador < limite; ++contador)
    printf ("%-14s %-14s %6.1f %10i %9.0f\n",
            nombre_industria(acciones_ptr[contador]),
            nombre_compania(acciones_ptr[contador]),
            acciones_ptr[contador]->precio,
            acciones_ptr[contador]->numero,
            precio_accion(acciones_ptr[contador]));

  if (switch_precio_medio == 1)
    printf ("El precio medio por accion en las industrias es %.2f.\n",
             precio_medio (acciones_ptr, limite));

  if (switch_tamanio_medio == 1)
    printf ("El numero medio de acciones en la industria es %f.\n",
             tamanio_medio (acciones_ptr, limite));

}




/**************************************************************
*************************************************************** 
 * Este es el programa principal.
 * No hay que crear datos de prueba para este, crear abstracciones, etc.
***************************************************************
 **************************************************************/

void main (argc, argv)
     int argc;
     char **argv;
{
  int res;

  if (argc == 1) {
    uso("trade");
    exit (1);
  }
  else procesar_argumentos(argc, argv);
}

