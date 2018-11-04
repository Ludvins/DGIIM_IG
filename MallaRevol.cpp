// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"
#include <cmath>

using namespace std ;

// *****************************************************************************


MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla  )
{
   ponerNombre( std::string("Malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // ...........................
   // ...........................

   nper = nperfiles;
   vector<float> vector_vertices;

   ply::read_vertices(nombre_arch.c_str(), vector_vertices);

   std::cout << "Leidos vertices del perfil." << endl;
   vector<Tupla3f> perfil;

   for (int i = 0; i < vector_vertices.size(); i+=3)
     perfil.push_back({vector_vertices[i], vector_vertices[i+1], vector_vertices[i+2]});

   crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla);

  // calcular la tabla de normales
   calcular_normales();


}
void MallaRevol::crearMallaRevol( const std::vector<Tupla3f>& perfil_original,
                                  const unsigned nperfiles,
                                  const bool     crear_tapas,
                                  const bool     cerrar_malla ) 
{

  cout << "Crear MallaRevol con:" << endl;
  cout << "\t Crear Tapas " << crear_tapas << endl;
  cout << "\t Cerrar Malla " << cerrar_malla << endl;
  cout << "HOla :D"<<endl;

  nvp = perfil_original.size();


  int nper = cerrar_malla ? nperfiles : nperfiles - 1; //Si se quiere cerrar la malla se crea un perfil menos (el ultimo).

  float angulo_giro = 360.0/(nper);


  // Inicializacion de los vertices
  for (int i = 0; i < nperfiles; i++)
    for ( unsigned j = 0; j < nvp; j++)
      {
        Matriz4f matriz_giro = MAT_Rotacion((float)i*angulo_giro, 0 , 1 , 0);
        vertices.push_back(matriz_giro*perfil_original[j]);
      }

  // Inicializacion de las caras
   for ( int i = 0; i < nperfiles - 1; i++)
     for ( unsigned j = 0; j < nvp-1; j++)
       {
         caras.push_back({ (i+1)*nvp+j+1 ,  i*nvp + j,(i+1)*nvp+j});
         caras.push_back({i*nvp + j+1,  i*nvp + j, (i+1)*nvp + j+1});
       }

   // Si se ha querido cerrar la malla, es necesario crear las ultimas caras.
   if (cerrar_malla){
     for (unsigned j = 0; j < nvp-1; j++)
       {
         caras.push_back({  j+1, (nper - 1)*nvp + j,j});
         caras.push_back ({  (nper - 1)*nvp + j+1, (nper - 1)*nvp + j,j+1});
       }
   }
   // Crear tapas
   if (crear_tapas){

     std::cout << "Crear tapas." << endl;

     vertices.push_back({0, perfil_original[0][1], 0});
     vertices.push_back({0, perfil_original[nvp-1][1], 0});


     std::cout << "Creados vertices de tapas: " << vertices[vertices.size()-2] << " y " << vertices[vertices.size()-1]<< endl;

     // Se crean las caras de las tapas.
     for (int i = 0 ; i < nperfiles - 1; i++ )
       {
         caras.push_back( {i*nvp, vertices.size()-2, (i+1)*nvp} );
         caras.push_back( { vertices.size()-1, (i+1)*nvp - 1,(i+2)*nvp -1} );
       }

     // Si se ha cerrado la malla hay caras adicionales.
     if (cerrar_malla)
       {
       caras.push_back ( { nvp*(nper - 1), vertices.size()-2 , 0 } );
       caras.push_back ( {vertices.size()-1,  nvp*(nper - 1) + nvp - 1, nvp - 1});
       }
   }

   num_ver = vertices.size();
   num_caras = caras.size();

   setColorVertices();

}

Cilindro::Cilindro ( const int num_verts_per, //Numero de vertices del perfil original
                     const unsigned nperfiles, //Numero de perfiles
                     const bool crear_tapas,
                     const bool cerrar_malla
                     )
{

  cout << "Creando cilindro con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( unsigned i = 0; i < num_verts_per; i++){
    perfil.push_back( {1.0, 0.0 + (i+1)/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla);

}

CustomCilindro::CustomCilindro ( const int num_verts_per, //Numero de vertices del perfil original
                     const unsigned nperfiles, //Numero de perfiles
                     const bool crear_tapas,
                     const bool cerrar_malla,
                     const float tapa_superior
                     )
{

  cout << "Creando cilindro con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( unsigned i = 0; i < num_verts_per; i++){
    float s = (num_verts_per - 1 - i)/(num_verts_per - 1);
    perfil.push_back( {s + (1-s)*tapa_superior , 0.0 + (i+1)/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla);

}


Cono::Cono( const int num_verts_per,
            const unsigned nperfiles,
            const bool     crear_tapas,
            const bool     cerrar_malla
            )
{

  cout << "Creando cono con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( float i = 0.0; i <= num_verts_per; i+=1.0){
    perfil.push_back( {1.0 - i/num_verts_per, 0.0 + i/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla);

}


Esfera::Esfera( const int num_verts_per,
                const unsigned nperfiles,
                const bool     crear_tapas,
                const bool     cerrar_malla  )
{

  cout << "Creando cono con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( float i = 0.0; i <= num_verts_per; i+=1.0){
    float y = -1*1.0 + 2*i/num_verts_per;
    perfil.push_back( {0.0 + sqrt(1 - (y*y)), y, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla);

}


// *****************************************************************************
