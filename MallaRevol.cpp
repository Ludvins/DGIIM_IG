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
                        const bool     cerrar_malla,
                        const bool     crear_texturas
                        )
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

   crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);
  // calcular la tabla de normales
  calcular_normales();

}


void MallaRevol::crearMallaRevol( const std::vector<Tupla3f>&  perfil_original,
                                  const unsigned nperfiles,
                                  const bool     crear_tapas,
                                  const bool     cerrar_malla,
                                  const bool     crear_texturas
                                  )
{

  cout << "Crear MallaRevol con:" << endl;
  if (crear_tapas)cout << "\t Crear Tapas " << endl;
  if (cerrar_malla) cout << "\t Cerrar Malla " << endl;
  if (crear_texturas) cout << "\t Crear texturas " << endl;
  nvp = perfil_original.size();

  float angulo_giro = cerrar_malla ? 360.0/(1.0*nperfiles) : 360.0/(1.0*nperfiles-1);


  auto norm = [] (Tupla3f v)
              {
                return sqrt(v.lengthSq());
              };

  vector<float> distancias_perfil;
  float distancia_total = 0.0;

  if (crear_texturas) // Si se van a crear texturas, inicializo un vector auxiliar de las distancias entre los puntos del perfil.
    {
      std::cout << "Creando texturas. " << endl;
      distancias_perfil.push_back(0.0);
      for (int i = 1; i < nvp; i++)
        {
          float d = norm(perfil_original[i] - perfil_original[i-1]);
          distancia_total += d;
          distancias_perfil.push_back(distancia_total) ;
        }
    }


  cout << "Inicializar vertices" << endl;
  // Inicializacion de los vertices
  for (int i = 0; i < nperfiles; i++)
    {
      for ( unsigned j = 0; j < nvp; j++)
        {
          Matriz4f matriz_giro = MAT_Rotacion((float)i*angulo_giro, 0 , 1 , 0);
          Tupla3f vec = matriz_giro*perfil_original[j];
          vertices.push_back(vec);

          if (crear_texturas) //Si se van a crear texturas.
              texturas.push_back({1 - 1.0*i/(nperfiles-1),1- distancias_perfil[j]/distancia_total});
        }
    }

  if(crear_texturas){
    cout << vertices.size() << " " << texturas.size() << endl;
    for(int i = 0; i < vertices.size(); i++)
      cout << vertices[i] << " " << texturas[i] << endl;
  }

  cout << "Vertices inicializados " << endl;

  // Inicializacion de las caras, el vertice[i][j] corresponde al del perfil i-ésimo, en la posicion j-ésima de este desde abajo.
   for ( int i = 0; i < nperfiles - 1; i++)
     for ( unsigned j = 0; j < nvp-1; j++)
       {
         caras.push_back({ (i+1)*nvp+j+1 ,  i*nvp + j,(i+1)*nvp+j}); // Cara: [i][j]-[i+1][j+1]-[i+1][j]
         caras.push_back({i*nvp + j+1,  i*nvp + j, (i+1)*nvp + j+1}); // Cara: [i][j]-[i+1][j+1]-[i][j+1]
       }

   // Si se ha querido cerrar la malla, es necesario crear las ultimas caras.
   if (cerrar_malla){
     for (unsigned j = 0; j < nvp-1; j++)
       {
         caras.push_back({  j+1, (nperfiles - 1)*nvp + j,j}); // Cara: [0][j]-[0][j+1]-[-1][j]
         caras.push_back ({  (nperfiles - 1)*nvp + j+1, (nperfiles - 1)*nvp + j,j+1}); // Cara: [0][j+1]-[-1][j+1]-[-1][j]
       }
   }

   // Crear tapas
   if (crear_tapas){

     vertices.push_back({0, perfil_original[0][1], 0});
     vertices.push_back({0, perfil_original[nvp-1][1], 0});

     // Se crean las caras de las tapas.
     for (int i = 0 ; i < nperfiles - 1; i++ )
       {
         caras.push_back( { vertices.size()-1, (i+1)*nvp - 1,(i+2)*nvp -1} ); // Cara: [i][max]-Central arriba-[i+1][max]
         caras.push_back( {i*nvp, vertices.size()-2, (i+1)*nvp} ); // Cara: [i][0]-Central abajo-[i+1][0]
       }

     // Si se ha cerrado la malla hay caras adicionales.
     if (cerrar_malla)
       {
       caras.push_back ( { nvp*(nperfiles - 1), vertices.size()-2 , 0 } );
       caras.push_back ( {vertices.size()-1,  nvp*(nperfiles - 1) + nvp - 1, nvp - 1});
       }
   }

   num_ver = vertices.size();
   num_caras = caras.size();

   setColorVertices();

}

Cilindro::Cilindro ( const int num_verts_per, //Numero de vertices del perfil original
                     const unsigned nperfiles, //Numero de perfiles
                     const bool crear_tapas,
                     const bool cerrar_malla,
                     const bool crear_texturas
                     )
{

  cout << "Creando cilindro con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( unsigned i = 0; i < num_verts_per; i++){
    perfil.push_back( {1.0, 0.0 + (i+1)/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);

}

CustomCilindro::CustomCilindro ( const int num_verts_per, //Numero de vertices del perfil original
                                 const unsigned nperfiles, //Numero de perfiles
                                 const bool crear_tapas,
                                 const bool cerrar_malla,
                                 const float tapa_superior,
                                 const bool crear_texturas

                     )
{

  cout << "Creando cilindro con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( unsigned i = 0; i < num_verts_per; i++){
    float s = (num_verts_per - 1 - i)/(num_verts_per - 1);
    perfil.push_back( {s + (1-s)*tapa_superior , 0.0 + (i+1)/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);

}


Cono::Cono( const int num_verts_per,
            const unsigned nperfiles,
            const bool     crear_tapas,
            const bool     cerrar_malla,
            const bool     crear_texturas
            )
{

  cout << "Creando cono con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( float i = 0.0; i <= num_verts_per; i+=1.0){
    perfil.push_back( {1.0 - i/num_verts_per, 0.0 + i/num_verts_per, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);

}


Esfera::Esfera( const int num_verts_per,
                const unsigned nperfiles,
                const bool     crear_tapas,
                const bool     cerrar_malla,
                const bool     crear_texturas
                )
{

  cout << "Creando cono con " << num_verts_per << " " << nperfiles << endl;
  vector<Tupla3f> perfil;

  for ( float i = 0.0; i <= num_verts_per; i+=1.0){
    float y = -1*1.0 + 2*i/num_verts_per;
    perfil.push_back( {0.0 + sqrt(1 - (y*y)), y, 0.0});
  }

  crearMallaRevol(perfil, nperfiles, crear_tapas, cerrar_malla, crear_texturas);

}


// *****************************************************************************
