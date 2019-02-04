
== Sistema operativo utilizado para compilar ==

Arch Linux 64-bit con núcleo 4.18.12-arch1-1-ARCH

== Implementación de OpenGL y librerías auxiliares ==

- mesa 18.2.3-1
- glew 2.1.0-1
- glfw-x11 3.2.1-1
- libjpeg-turbo 2.0.0-1

== Teclas ==

Teclas que se pueden utilizar:
q/Q - Permite salir del programa.

p/P - Permite ir a la siguiente práctica, despueés de la última se va a la primera.

`+`  - Permite acercar la cámara

`-` - Permite alejar la cámara

v/V - P1-P4 Permite cambiar entre modo inmediato y modo diferido.
      P5 - Permite cambiar entre las distintas cámaras.

m/M - Permite cambiar el modo de visualizacion en un ciclo.

o/O - Permite cambiar entre objetos dentro de la misma práctica.

g/G - P3 - Permite seleccionar los diferentes parámetros.
    - P4 - Permite cambiar entre el angulo de la longitud y el de la latitud.

a/A - Permite activar o desactivar las animaciones

`>` - P3 - Permite incrementar un parametro o su velocidad dependiendo de si las animaciones se encuentran activadas.
    P4 - Permite incrementar el angulo de las fuentes de luces direccionales.

`<` - P3 - Permite decrementar un parámetro o su velocidad dependiendo de si las animaciones se encientran activadas.
    P4 - Permite decrementar el ángulo de las fuentes de luces direccionales.

r/R - Permite resetear todos los parámetros de la práctica 3.

f/F - En la práctica 4, permite cambiar entre las fuentes de luces disponibles.

c/C - En la práctica 5, permite cambiar entre las diferentes cámaras.

Flechas - Permiten mover la cámara.

Click Izquierdo - Permite seleccionar el punto de atencion en la práctica 5.

Click Derecho (Arrastrar) - Permite mover la camará con el ratón mientras se mantenga pulsado (práctica 5).


 == Comentarios P1 ==

He realizado la programación del cauce gráfico en los archivos simple_vs.glsl y simple_fs.glsl.
He añadido la posibilidad de colorear un objeto en la clase MallaInd, mediante los métodos fijarColorNodo y setColorVertices.
Se puede cambiar la forma de envio del modo inmediato con la macro MODO_INMEDIATO.

== Comentarios P2 ==

No he utilizado ningún archivo PLY que no se encontrará ya entre los archivos dados.

Se ha realizado una figura básica adicional, CustomCilindro, que representa un cilindro donde las bases no son del mismo tamaño, una de ellas es de tamaño 1 y la otra se le pasa como parámetro (esto se ha utilizado para realizar la cola del muñeco de la practica 3).

== Comentarios P3 ==

Para la realización de la práctica se ha realizado la clase Muneco, Brazo, Cola, SegmentoCola y Ojo.
Además hay dos clases más ejercicio1 y ejercicio2, realizadas para prácticar preguntas realizadas en exámenes de otros años.

He añadido un método a la clase MallaPLY que devuelve el tamaño de la malla, es decir, el máximo en cada eje que alcanza la figura.

== Cambios en el código ==

Se añade un método virtual a Objeto3D llamado 'fijarColorNodo', para proporcionar compatibilidad
en la implementación (recursiva) del método homónimo en NodoGrafoEscena.

== Comentarios P4 ==

Se han añadido una serie de grafos escena, materiales y luces, casi todos comentados, para realizar prácticas y ejercicios de examen. Se han añadido un método a la coleccion de fuentes de luz que permite activar o desactivar una fuente de luz concreta de la fuente.

== Comentarios P5 ==

Se ha utilizado la escena de la práctica 4, junto con 4 cámaras interactivas.
