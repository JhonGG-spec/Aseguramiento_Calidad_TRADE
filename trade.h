#ifndef TRADEH
#define TRADEH

struct accion{
  double precio; 
  int numero;
  char *descripcion;
};

enum industria {
  comida = 'C',
  transporte = 'T',
  ordenadores = 'O',
  metal = 'M',
  salud = 'S',
  aerolineas = 'A'
};


#endif		