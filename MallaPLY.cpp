// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("Malla leída del archivo '") + nombre_arch + "'" );
   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................

   vector<float> vector_vertices;
   vector<int> vector_caras;

   ply::read(nombre_arch.c_str(), vector_vertices, vector_caras);

   for (int i = 0; i < vector_vertices.size(); i+=3)
     vertices.push_back({vector_vertices[i], vector_vertices[i+1], vector_vertices[i+2]});

   for (int i = 0; i < vector_caras.size(); i+=3)
     caras.push_back({vector_caras[i], vector_caras[i+1], vector_caras[i+2]});

   tam_ver = vertices.size();
   tam_tri = caras.size();

   setColorVertices();


   cout << "Vectores cargados" << endl;
   // calcular la tabla de normales
   calcular_normales();

}

// *****************************************************************************
