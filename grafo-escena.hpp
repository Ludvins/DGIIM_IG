// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "matrices-tr.hpp"
#include "materiales.hpp"
#include "Objeto3D.hpp"
#include "MallaInd.hpp"
#include "MallaRevol.hpp"
#include "MallaPLY.hpp"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {
     Objeto3D* objeto ;  // ptr. a un objeto (no propietario)
     Matriz4f* matriz ;  // ptr. a matriz 4x4 transf. (propietario)
     Material* material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D* pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f& pMatriz ) ;// (crea copia en el heap)
   EntradaNGE( Material* pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
   // DONE: práctica 3: definir variables y métodos privados del nodo
   // .......
  std::vector <EntradaNGE> entradas; // vector de entradas.
  Tupla3f color;

   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis& cv ) ;
   void fijarColorNodo( const Tupla3f& nuevo_color ) ;

   // calcular y obtener la caja englobante
   //virtual CajaEngf cajaEnglobante() ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE& entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D* pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f& pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material* pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f* leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f& mmodelado,
                    Objeto3D** objeto, Tupla3f& centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;

} ;

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos que tienen además un vector de
// parámetros

class NodoGrafoEscenaParam : public NodoGrafoEscena
{
   protected:
      // DONE: práctica 3: declarar vector de parámetros del nodo.
      // .......

  std::vector <Parametro> parametros; // vector de parámetros

   public:
      // devuelve el número de parámetros
      int numParametros();

      // devuelve un puntero al i-ésimo parámetro (i < numParametros())
      Parametro* leerPtrParametro( unsigned i ) ;

      // actualiza el objeto para ir al siguiente cuadro,
      // se usa cuando están activadas las animaciones, una vez antes de cada frame
      void siguienteCuadro();
} ;


class Peones : public NodoGrafoEscenaParam{
protected:

  class PeonMadera : public NodoGrafoEscena{
  public:
    PeonMadera();
  };
  class PeonBlanco : public NodoGrafoEscena{
  public:
    PeonBlanco();
  };
  class PeonNegro : public NodoGrafoEscena{
  public:
    PeonNegro();
  };

public:
  Peones();
};

class CocaCola : public NodoGrafoEscenaParam{
protected:
  class Cuerpo : public NodoGrafoEscena{
  public:
    Cuerpo();
  };
  class TapaAbajo : public NodoGrafoEscena{
  public:
    TapaAbajo();
  };
  class TapaArriba : public NodoGrafoEscena {
  public:
    TapaArriba();
  };

public: CocaCola();
};

class EscenaP4 : public NodoGrafoEscenaParam {
public:
  EscenaP4();
};

class Muneco : public NodoGrafoEscenaParam {

 public:
  Muneco();
};

class Brazo : public  NodoGrafoEscena {
  int indice_hombro;
  int indice_codo;

public:
  Brazo();
  Matriz4f* getArticulacionHombro(){ return leerPtrMatriz(indice_hombro); }
  Matriz4f* getArticulacionCodo() { return leerPtrMatriz(indice_codo); }
};

class Cola : public NodoGrafoEscena {

  int indice1, indice2, indice3, indice4, indice5, indice6, indice7, indice8;

public:
  Cola();

  Matriz4f* getArticulacion1(){ return leerPtrMatriz(indice1); }
  Matriz4f* getArticulacion2(){ return leerPtrMatriz(indice2); }
  Matriz4f* getArticulacion3(){ return leerPtrMatriz(indice3); }
  Matriz4f* getArticulacion4(){ return leerPtrMatriz(indice4); }
  Matriz4f* getArticulacion5(){ return leerPtrMatriz(indice5); }
  Matriz4f* getArticulacion6(){ return leerPtrMatriz(indice6); }
  Matriz4f* getArticulacion7(){ return leerPtrMatriz(indice7); }
  Matriz4f* getArticulacion8(){ return leerPtrMatriz(indice8); }

};

class Ojo : public NodoGrafoEscena {

public:
  Ojo();
};

class SegmentoCola : public NodoGrafoEscena {

public:
  SegmentoCola();
};

class ejercicio1 : public NodoGrafoEscenaParam {

public:
  ejercicio1();

};

class ejercicio2 : public NodoGrafoEscenaParam {

public:
  ejercicio2();

};

#endif // GRAFO_ESCENA_HPP

// *********************************************************************
