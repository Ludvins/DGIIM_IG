// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
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

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no funciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis& cv )
{
   // DONE: práctica 3: recorrer las entradas y visualizar el nodo
   // ........

  glMatrixMode ( GL_MODELVIEW );   //operamos sobre la modelview
  glPushMatrix();                  // guarda la model view actual;

  // recorrer todas las entradas del array que hay en el modo.

  for ( unsigned i = 0; i < entradas.size(); i++)

    switch ( entradas[i].tipo )
      {
      case TipoEntNGE::objeto:
        entradas[i].objeto->visualizarGL(cv);
        break;

      case TipoEntNGE::transformacion:
        glMatrixMode ( GL_MODELVIEW );
        glMultMatrixf ( *(entradas[i].matriz ) );
        break;

      default:
        std::cout << "Tipo no contemplado" << std::endl;
        break;
      }

  glMatrixMode ( GL_MODELVIEW );
  glPopMatrix();


}


// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
  // DONE: práctica 3: inicializar un nodo vacío (sin entradas)
  // ........

ponerNombre("NodoGrafoEscena vacio...");
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f& nuevo_color )
{
  // DONE: práctica 3: asignarle un color plano al nodo, distinto del padre
  // ........

  if (nuevo_color != color) {
    color = nuevo_color;
    for (auto entrada : entradas) {
      if (entrada.tipo == TipoEntNGE::objeto)
        entrada.objeto->fijarColorNodo(color);
    }
  }
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE& entrada )
{
  // DONE: práctica 3: agregar la entrada al nodo, devolver índice de la entrada
   // ........

  entradas.push_back(entrada);
  return entradas.size() - 1;

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D* pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f& pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material* pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f* NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // DONE: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........

  assert( entradas[indice].tipo == TipoEntNGE::transformacion );

  return entradas[indice].matriz;

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int        ident_busc, // identificador a buscar
   const Matriz4f&  mmodelado,  // matriz de modelado
   Objeto3D**       objeto,     // (salida) puntero al puntero al objeto
   Tupla3f&         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........

}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
   // DONE: práctica 3: indicar cuantos parámetros hay
   // ........
  return parametros.size();

}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro* NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
   // DONE: práctica 3: devolver puntero al i-ésimo parámetro
   // ........

  assert (i < parametros.size());
  return &parametros[i];


}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
   // DONE: práctica 3: actualizar todos los parámetros al siguiente cuadro
   // ........

  for (auto& it : parametros)
    it.siguiente_cuadro();
}



// -----------------------------------------------------------------------------


Muneco::Muneco()
{
  Cilindro* c = new Cilindro(2, 20, true, true);
  Esfera* esfera = new Esfera(20, 20, false, true);

  int i_rotacion_principal = agregar(MAT_Ident());

  Cola* drat = new Cola;
  agregar(drat);



  agregar(MAT_Escalado(1.0,2.0,1.0));
  agregar(c);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));
  agregar(esfera);
  int i_rotacion_1 = agregar(MAT_Ident());
  agregar(c);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  // Tronco
  agregar(esfera);
  int i_rotacion_2 = agregar(MAT_Ident());
  agregar(c);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  // Cabeza
  agregar(esfera);
  int i_rotacion_3 = agregar(MAT_Ident());
  agregar(c);

  Brazo* br1 = new Brazo;
  Brazo* br2 = new Brazo;

  agregar(br1);

  agregar (MAT_Rotacion(180, 0, 1, 0));

  agregar(br2);

  Ojo* ojo = new Ojo();

  agregar(MAT_Traslacion(-0.3, 0.7, 1));
  agregar(MAT_Escalado(1, 0.5, 1));
  agregar(ojo);
  agregar(MAT_Traslacion(0.6, 0, 0));
  agregar(ojo);
  agregar(MAT_Escalado(1, 2, 1));
  agregar(MAT_Traslacion(-0.3, -0.7, -1));

  agregar(MAT_Traslacion(0.0, 0.5, 1.0));
  agregar(MAT_Rotacion(-90, 1, 0, 0));

  Cola* drat2 = new Cola;
  agregar(drat2);

  // Rotacion del muneco

  Parametro rotacion_principal (
                                "Rotacion",
                                leerPtrMatriz(i_rotacion_principal),
                                [=] (float v) {return MAT_Rotacion(v, 0, 1, 0);},
                                false,
                                0.0,
                                10.0,
                                0.1
                                );
  Parametro rotacion_1 (
                        "Rotacion",
                        leerPtrMatriz(i_rotacion_1),
                        [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                        true,
                        0.0,
                        10.0,
                        0.1
                        );

  Parametro rotacion_2 (
                        "Rotacion",
                        leerPtrMatriz(i_rotacion_2),
                        [=] (float v) {return MAT_Rotacion(v,0,0,1);},
                        true,
                        0.0,
                        10.0,
                        0.1
                        );
  Parametro rotacion_3 (
                        "Rotacion",
                        leerPtrMatriz(i_rotacion_3),
                        [=] (float v) {return MAT_Rotacion(v,0,0,1);},
                        true,
                        0.0,
                        10.0,
                        0.1
                        );
  Parametro rotacion_hombro_1 (
                               "Rotacion",
                               br1->getArticulacionHombro(),
                               [=] (float v) {return MAT_Rotacion(v, 0, 1 ,0);},
                               true,
                               0.0,
                               40.0,
                               0.1
                               );

  Parametro rotacion_hombro_2 (
                               "Rotacion",
                               br2->getArticulacionHombro(),
                               [=] (float v) {return MAT_Rotacion(v, 0, 1 ,0);},
                               true,
                               0.0,
                               65.0,
                               0.1
                               );
  Parametro rotacion_codo_1 (
                               "Rotacion",
                               br1->getArticulacionCodo(),
                               [=] (float v) {return MAT_Rotacion(v, 0, 1,0);},
                               true,
                               0.0,
                               40.0,
                               0.1
                               );

  Parametro rotacion_codo_2 (
                               "Rotacion",
                               br2->getArticulacionCodo(),
                               [=] (float v) {return MAT_Rotacion(v, 0, 1 ,0);},
                               true,
                               0.0,
                               40.0,
                               0.1
                               );

  Parametro rotacion_cola_1 (
                                "Rotacion",
                                drat->getArticulacion1(),
                                [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                                true,
                                0.0,
                                10.0,
                                0.1
                                );

  Parametro rotacion_cola_2 (
                             "Rotacion",
                             drat->getArticulacion2(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_cola_3 (
                             "Rotacion",
                             drat->getArticulacion3(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_cola_4 (
                             "Rotacion",
                             drat->getArticulacion4(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );


  Parametro rotacion_cola_5 (
                             "Rotacion",
                             drat->getArticulacion5(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_cola_6 (
                             "Rotacion",
                             drat->getArticulacion6(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_cola_7 (
                             "Rotacion",
                             drat->getArticulacion7(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );


  Parametro rotacion_cola_8 (
                             "Rotacion",
                             drat->getArticulacion8(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_gorro_1 (
                                "Rotacion",
                                drat2->getArticulacion1(),
                                [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                                true,
                                0.0,
                                10.0,
                                0.1
                                );

  Parametro rotacion_gorro_2 (
                             "Rotacion",
                             drat2->getArticulacion2(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_gorro_3 (
                             "Rotacion",
                             drat2->getArticulacion3(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_gorro_4 (
                             "Rotacion",
                             drat2->getArticulacion4(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );


  Parametro rotacion_gorro_5 (
                             "Rotacion",
                             drat2->getArticulacion5(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_gorro_6 (
                             "Rotacion",
                             drat2->getArticulacion6(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );

  Parametro rotacion_gorro_7 (
                             "Rotacion",
                             drat2->getArticulacion7(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );


  Parametro rotacion_gorro_8 (
                             "Rotacion",
                             drat2->getArticulacion8(),
                             [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                             true,
                             0.0,
                             10.0,
                             0.1
                             );


  parametros.push_back(rotacion_cola_1);
  parametros.push_back(rotacion_cola_2);
  parametros.push_back(rotacion_cola_3);
  parametros.push_back(rotacion_cola_4);
  parametros.push_back(rotacion_cola_5);
  parametros.push_back(rotacion_cola_6);
  parametros.push_back(rotacion_cola_7);
  parametros.push_back(rotacion_cola_8);

  parametros.push_back(rotacion_gorro_1);
  parametros.push_back(rotacion_gorro_2);
  parametros.push_back(rotacion_gorro_3);
  parametros.push_back(rotacion_gorro_4);
  parametros.push_back(rotacion_gorro_5);
  parametros.push_back(rotacion_gorro_6);
  parametros.push_back(rotacion_gorro_7);
  parametros.push_back(rotacion_gorro_8);


  parametros.push_back(rotacion_principal);
  parametros.push_back(rotacion_1);
  parametros.push_back(rotacion_2);
  parametros.push_back(rotacion_3);
  parametros.push_back(rotacion_hombro_1);
  parametros.push_back(rotacion_hombro_2);
  parametros.push_back(rotacion_codo_1);
  parametros.push_back(rotacion_codo_2);
 }

SegmentoCola::SegmentoCola(){
  float escalado = 0.8;
  Esfera* esfera = new Esfera(20, 20, false, true);

  CustomCilindro* c = new CustomCilindro(2, 20, true, true, escalado);

  agregar(esfera);
  agregar(c);


}

Cola::Cola(){

  agregar(MAT_Traslacion(0.0 , 1.0, 0.5));
  agregar(MAT_Rotacion(90, 1, 0 ,0));
  float escalado = 0.8;
  Esfera* esfera = new Esfera(20, 20, false, true);
  SegmentoCola* sg = new SegmentoCola();
  CustomCilindro* c = new CustomCilindro(2, 20, true, true, escalado);
  agregar(c);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice1 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice2 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice3 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice4 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice5 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice6 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice7 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);
  agregar(MAT_Traslacion(0.0, 1.0, 0.0));

  indice8 = agregar(MAT_Ident());
  agregar(MAT_Escalado(escalado, escalado, escalado));
  agregar(sg);

}

Brazo::Brazo()
{

  Cilindro* c = new Cilindro(2, 20, true, true);
  Esfera* esfera = new Esfera(20, 20, false, true);

  agregar(MAT_Traslacion(-1.0, -0.5, 0.0)); //Colocacion del hombro

  indice_hombro = agregar(MAT_Ident());

  agregar(MAT_Rotacion(75, 0, 0, 1)); // Rotacion del brazo

  agregar(MAT_Escalado(0.25, 0.25, 0.25)); // Escalado esfera del hombro
  agregar(esfera);

  agregar(MAT_Escalado(1.0, 10.0, 1.0)); // Longitud del brazo
  agregar(c);

  agregar(MAT_Traslacion(0.0, 1.0, 0.0)); // Codo

  agregar(MAT_Escalado(1.0, 0.1, 1.0));
  agregar(esfera);

  indice_codo = agregar(MAT_Ident());

  agregar(MAT_Rotacion(-65,0,0,1));
  agregar(MAT_Escalado(1.0, 6.0, 1.0));
  agregar(c);

}

Ojo::Ojo(){
  Esfera* e1 = new Esfera(20, 20, true, true);    // Esfera de fuera
  Esfera* e2 = new Esfera(20, 20, true, true);    // Pupila
  e1->fijarColorNodo({1,1,1});
  e2->fijarColorNodo({0,0,0});

  agregar(MAT_Escalado(0.2, 0.2, 0.15));

  agregar(e1);
  agregar(MAT_Traslacion(-0.2, -0.2, 0.3));
  agregar(MAT_Escalado(0.7, 0.7, 1));
  agregar(e2);

}
ejercicio1::ejercicio1()
{

  MallaPLY* b = new MallaPLY("../plys/ant.ply");
  agregar(b);

  Tupla3f a = b->size();

  agregar(MAT_Traslacion(-1*a[0], -1*a[1], -1*a[2]));
  agregar(MAT_Escalado((float)2*a[0], (float)2*a[1], (float)2*a[2]));

  Cubo* c = new Cubo();
  c->fijarColorNodo({0.0, 0.0, 0.0});
  agregar(c);

}

ejercicio2::ejercicio2(){

  Cubo* c = new Cubo ();

  int a1 = agregar(MAT_Ident());
  int b1 = agregar(MAT_Ident());
  agregar (c);

  agregar(MAT_Traslacion(1.0, 1.0, 1.0));

  int a2 = agregar(MAT_Ident());

  agregar (c);

  agregar(MAT_Traslacion(1.0, 1.0, 1.0));

  int a3 = agregar(MAT_Ident());
  agregar(c);



  Parametro rotacion_1 (
                        "Rotacion",
                        leerPtrMatriz(a1),
                        [=] (float v) {return MAT_Rotacion(v, 1, 0, 1);},
                        false,
                        0.0,
                        10.0,
                        0.1
                        );
  Parametro rotacionb_1 (
                        "Rotacion",
                        leerPtrMatriz(b1),
                        [=] (float v) {return MAT_Rotacion(v, 1, 1, 1);},
                        false,
                        0.0,
                        -20.0,
                        0.5
                        );
  Parametro rotacion_2 (
                        "Rotacion",
                        leerPtrMatriz(a2),
                        [=] (float v) {return MAT_Rotacion(v, 1, 1, 1);},
                        false,
                        0.0,
                        -30.0,
                        0.5
                        );


  Parametro rotacion_3 (
                        "Rotacion",
                        leerPtrMatriz(a3),
                        [=] (float v) {return MAT_Rotacion(v, 1, 1, 1);},
                        false,
                        0.0,
                        60.0,
                        0.5
                        );


  parametros.push_back(rotacion_1);
  parametros.push_back(rotacion_2);
  parametros.push_back(rotacionb_1);
  parametros.push_back(rotacion_3);
}
