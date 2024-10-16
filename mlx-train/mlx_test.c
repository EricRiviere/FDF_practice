/*
   - Intruccion basica para uso de la libreria minilibx:
   		Siguiendo este articulo: 
			https://medium.com/@jalal92/understanding-the-minilibx-a-practical-dive-into-x-window-programming-api-in-c-cb8a6f72bec3

	MiniLibX es una pequeña biblioteca gráfica utilizada en los proyectos de 42, incluyendo 42 Paris.
	Esta librería se utiliza principalmente para aprender conceptos básicos de gráficos en 2D, 
	crear ventanas, y manejar eventos de teclado y ratón. Es especialmente útil en proyectos como 
	"Cub3D" o "FDF", donde los estudiantes deben implementar gráficos básicos en un entorno controlado

	- Características principales de MiniLibX:
		
		- Creación de ventanas: 
	
		Te permite abrir una ventana donde se pueden dibujar píxeles, líneas, formas, etc.

		- Manejo de eventos: 

		Puedes gestionar entradas de teclado y ratón para interactuar con la ventana.

		- Dibujo de gráficos simples: 

		Proporciona funciones para dibujar píxeles, formas básicas como líneas y rectángulos, 
		así como para cargar imágenes y manipular buffers de imágenes.
		Compatibilidad limitada: MiniLibX está diseñada para funcionar en sistemas UNIX como Linux 
		y macOS. En el caso de Linux, suele requerir que instales bibliotecas como X11 y Xext para 
		el manejo de gráficos y eventos.

		- Ejemplos de funciones importantes:

			-mlx_init: Inicializa la conexión con el servidor de ventanas.
			-mlx_new_window: Crea una nueva ventana.
			-mlx_pixel_put: Dibuja un píxel en una coordenada específica de la ventana.
			-mlx_key_hook y mlx_mouse_hook: Manejan eventos de teclado y ratón.
			-mlx_loop: Inicia el bucle de eventos que mantiene la ventana abierta y activa la gestión 
			de eventos.

		- Limitaciones:

			- Limitada en funcionalidad: 

			MiniLibX no es comparable a bibliotecas gráficas más completas 
			como SDL o OpenGL. Es muy básica y está destinada a enseñar fundamentos, no a crear 
			aplicaciones gráficas complejas.

			- Documentación escasa: No tiene mucha documentación oficial, por lo que aprender a usarla
			generalmente implica estudiar ejemplos o consultar a otros estudiantes.
			
		En resumen, MiniLibX es una herramienta clave en los proyectos de gráficos 42, diseñada 
		para ayudar a los estudiantes a entender conceptos básicos de gráficos sin la complejidad 
		de bibliotecas más grandes.

   - Se puede descargar la libreria de 42 Paris aqui: 
		https://github.com/42Paris/minilibx-linux/tree/7dc53a411a7d4ae286c60c6229bd1e395b0efb82

   - Una vez descargada podemos ojear las distintas funciones presentes listandolas 
   		(es una libreria para usar la libreria X11 que es mucho mas grande)

   - MinilibX es una "mini libreria" disenada para empezar en el mundo de la programacion X-Window
   	Se trata de una API basada en el sistema X-Window que seria la base para que los sistemas operativos 
	linux tengan GUIs (Graphycal User Interface - Interfaz grafica de usuario).
	Es un intermediario entre tus inputs (ya sea teclado, raton, etc) y los outputs visuales en pantalla 
	(permite interaccionar y ver el resultado en pantalla).

	- Ahora que ya se entiende que es, veamos como funciona.
	Primero de todo hay que establecer una conexion con el servidor X-Window, y para ello usamos mlx_init()
	
	-MLX_INIT():

	-Esta es la funcion mlx_init() de minilibx:

	#include	"mlx_int.h"

	void	*mlx_init()
	{
		t_xvar	*xvar;

		if (!(xvar = malloc(sizeof(*xvar))))
			return ((void*)0);
		if ((xvar->display = XOpenDisplay("")) == 0)
		{
			free(xvar);
			return ((void*)0);
		}
		xvar->screen = DefaultScreen(xvar->display);
		xvar->root = DefaultRootWindow(xvar->display);
		xvar->cmap = DefaultColormap(xvar->display,xvar->screen);
		xvar->depth = DefaultDepth(xvar->display,xvar->screen);
		if (mlx_int_get_visual(xvar)==-1)
		{
			printf(ERR_NO_TRUECOLOR);
			exit(1);
		}
		xvar->win_list = 0;
		xvar->loop_hook = 0;
		xvar->loop_param = (void *)0;
		xvar->do_flush = 1;
		xvar->wm_delete_window = XInternAtom (xvar->display, "WM_DELETE_WINDOW", False);
		xvar->wm_protocols = XInternAtom (xvar->display, "WM_PROTOCOLS", False);
		mlx_int_deal_shm(xvar);
		if (xvar->private_cmap)
			xvar->cmap = XCreateColormap(xvar->display,xvar->root,
					 xvar->visual,AllocNone);
		mlx_int_rgb_conversion(xvar);
		xvar->end_loop = 0;
		return (xvar);
	}

	*Esta funcion devuelve una estructura (xvar).
	Para establecer la conexion, se crea un puntero a una variable de tipo xvar que contiene todo lo 
	necesario para ello, y esta es su estructura:

	typedef struct	s_xvar
	{
		Display		*display;
		Window		root;
		int			screen;
		int			depth;
		Visual		*visual;
		Colormap	cmap;
		int			private_cmap;
		t_win_list	*win_list;
		int			(*loop_hook)();
		void		*loop_param;
		int			use_xshm;
		int			pshm_format;
		int			do_flush;
		int			decrgb[6];
		Atom		wm_delete_window;
		Atom		wm_protocols;
		int 		end_loop;
	}				t_xvar;


	*Como la funcion mlx_init, reserva espacio para esta estructura xvar, cuando establecemos la conexion,
	debemos asegurarnos de liberar todo antes de finalizar el programa.
	Para ello usamos la funcion mlx_destroy_display (para destruir el display) y luego liberaremos el puntero
	creado con free.
	Aqui un pequeno ejemplo de inicializar la conexion y cerrarla sin que nada pase:

		#include <stdlib.h>
		#include "minilibx_linux/mlx.h"

		int	main(void)
		{
			void	*mlx_ptr;//Creamos el puntero

			mlx_ptr = mlx_init();//El puntero apunta a la estructura (xvar) devuelta por mlx_init
			if (!mlx_ptr)//Controlamos el posible fallo en la reserva de memoria
			return (1);

			mlx_destroy_display(mlx_ptr);//Liberamos el display
			free(mlx_ptr);//Liberamos el puntero
			return (0);
		}

	*Para compilar un archivo de test usando la libreria son necesarios varios flags
	Ejemplo:

	cc test.c -Lminilibx_linux -lmlx-Linux -lx11 -lXent

	cc: compila
	test.c: nombre archivo de prueba
	-Lminilibx_Linux: Especifica el directorio para buscar las librerias
	-lmlx_Linux: Especifica a cc que linkee a las librerias de Linux (libmlx_Linux.a o libmlx_Linux.so)
	-lX11: Especifica que linkee con la libreria X11 (GUI para SO Linux)
	-Xext: Especifica que linkee con la libreria Xent que prvee extensiones para el sistema X Window

	Asi pues, le decimos donde encontrar los archivos de las librerias y que librerias linkear

	Importante entender el concepto de DISPLAY, en este esquema donde la libreria minilibx nos permite
	conectar hardware de inputs (teclado, mouse, etc) y hardware de outputs (pantalla), cuando llamamos
	a la funcion molx_init(), establecemos esta conexion y le decimos "esta conexion es para imprimir 
	cosas por pantalla", por eso hablamos de "display".
	-------------

	Una vez entendido que hay que establecer una conexion, el siguiente concepto a profundizar es el
	de 'window' o ventana.
	Para abrir una ventana, usamos la funcion mlx_new_window() de minilibx.
	Este es el prototipo de la funcion: 
		- void	*mlx_new_window(t_xvar *xvar,int size_x,int size_y,char *title)
			mlx_new_window(puntero conexion, alto, ancho, nombre de la ventana).

	Podemos ver la estructura en el archivo mlx_new_window.c dentro de la libreria minilibx:

	void	*mlx_new_window(t_xvar *xvar,int size_x,int size_y,char *title)
	{
		t_win_list				*new_win;
		...
		...
		return (new_win);
	}

	Esta funcion crea un puntero a una estructura t_win_list y la devuelve, esta estructura se puede
	ver en el archivo mlx_int.h:

	typedef struct	s_win_list
	{
		Window				window;
		GC					gc;
		struct s_win_list	*next;
		int					(*mouse_hook)();
		int					(*key_hook)();
		int					(*expose_hook)();
		void				*mouse_param;
		void				*key_param;
		void				*expose_param;
		t_event_list		hooks[MLX_MAX_EVENT];
	}				t_win_list;

	La funcion mlx_new_window() tambien reserva espacio en memoria para la estructura de tipo t_win_list 
	el puntero de la cual devuelve (new_win).
	El primer parametro de la funcion es el puntero a la conexion display, por lo que si no la hemos
	establecido no podremos crear esta ventana.
	El concepto VENTANA, si que nos es mas familiar, porque es literalmente una ventana dentro de nuestra
	pantalla.
	Como la reserva de memoria para la estructura necesaria para crear la ventana (t_win_list) puede fallar,
	debemos protejerla y liberar el display (mlx_destroy_display()) y el puntero (con free).

	Aqui un ejemplo de programa para establecer la conexion y crear una ventana:

		#include <stdlib.h>
		#include "minilibx-linux/mlx.h"
		#define WIDTH	500
		#define HEIGHT	500

		int main(void)
		{
			void	*mlx_connection;
			void	*mlx_window;

			mlx_connection = mlx_init();
			if (!mlx_connection)
				return (1);

			mlx_window = mlx_new_window(mlx_connection, HEIGHT, WIDTH, "Mi ventana");
			if (!mlx_window)
			{
				mlx_destroy_display(mlx_connection);
				free(mlx_connection);
				return (1);
			}

			//mlx_loop(mlx_connection);
			mlx_destroy_window(mlx_connection, mlx_window);
			mlx_destroy_display(mlx_connection);
			free(mlx_connection);
			return (0);
		}

	Si compilamos y ejecutamos tal y como esta con los mismos flags que antes, veremos como se crea la
	ventana y se cierra automaticamente.
	Por eso es importante la funcion comentada "mlx_loop()"
	-	MLX_LOOP() es una funcion para mantener nuestro programa activo a la espera de eventos.
	Si lo descomentamos, compilamos y ejecutamos de nuevo, la ventana permanecera abierta hata que 
	hagamos cntl + C (pero cerrarla asi, nos generara leaks, podemos comprobarlo, compilando y ejecutando
	"valgrind ./a.out" y cerrando con cntl + 'C').
	Para evitar estos leaks, necesitamos una funcion para gestionar los eventos (que vendrian a ser los
	inputs que mandamos mediante el hardware).

	Antes de profundizar en el concepto de Eventos, mencionar que es posible crear multiples ventanas a 
	la vez. Si lo hacemos es importante en la parte de control de la reserva de memoria, tener en cuenta
	las ventanas que hemos creado anteriormente y liberarlas con mlx_destroy_window().

	-----------------

	EVENTOS:
	
	Para tener una idea inicial de en que consisten los eventos, revisar la documentacion de
	minilibx en: https://harm-smits.github.io/42docs/libs/minilibx/events.html


		Eventos: Son acciones o interacciones que ocurren mientras la aplicación gráfica está en 
		ejecución, como presionar una tecla, mover el ratón o cerrar una ventana.
		Estos eventos son detectados por la biblioteca y se pueden manejar con código específico.

	HOOKS:

	Para tener una idea inicial sobre los hooks revisar la documentacion de hooks en:
	https://harm-smits.github.io/42docs/libs/minilibx/hooks.html#hooking-into-key-events

		Hooks: Son funciones que asocias a ciertos eventos. Cuando un evento ocurre (por ejemplo, 
		presionar una tecla), la función "hookeada" se ejecuta para manejarlo.
		En MiniLibX, usas funciones como mlx_key_hook o mlx_mouse_hook para conectar eventos con 
		tus propias funciones de respuesta.
	
	Teniendo una idea inicial de lo que son los eventos y los hooks, se puede entender, que con
	minilibx, podriamos por ejemplo:

		- Mediante la funcion mlx_key_hook, decirle a la biblioteca que cuando se presione una tecla,
		se llame a una funcion que se haya definido para gestionar ese evento.

		- Mediante la funcion mlx_mouse_hook, decirle a la biblioteca que cuando se realice una
		accion con el mouse (click izquierdo, click derecho, click en la rueda o scroll up/down),
		se llame a una funcion que se haya definido para gestionar ese evento.

		*El Keysym es parte del sistema de ventanas X11 en Linux y macOS, y se utiliza para traducir
		una tecla física (un código de tecla, o keycode) a un símbolo comprensible para el programa,
		como 'a', 'A', o una tecla especial como 'Escape'.
		https://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h

		*Para el raton existe una numeracion que puede variar en los distintos sistemas operativos
		pero en el caso de linux:
			-Click izquierdo:1
			-Click en rueda:2
			-Clich derecho:3
			-Scroll arriba:4
			-Srcoll abajo:5

	Veamos un ejemplo de como usar EVENTOS y HOOKS con minilibx:

	#include "minilibx-linux/mlx.h"
	#include <X11/keysym.h>//Incluimos la libreria keysym para el teclado
	#include <stdlib.h>//Para malloc
	#include <stdio.h>//Para printf

	#define MLX_ERROR		1
	#define	WINDOW_WIDTH	1000
	#define	WINDOW_HEIGHT	1000

	typedef struct	s_mlx_data//Creamos una estructura con punteros a la conexion y la ventana
	{
		void	*mlx_ptr;
		void	*win_ptr;
	}			t_mlx_data;//Esta nos servira para destruir conexion y ventana en un evento

	
	//Funcion propia para manejar los eventos de teclado. Recibe el int de keysym y la data
	int	handle_key_input(int keysym, t_mlx_data *data)
	{
		//Si la ecla es "esc" destruimos la ventana, la conexion,liberamos el puntero y salimos
		if (keysym == XK_Escape)
		{
			printf("The %d key has been pressed\n\n", keysym);
			mlx_destroy_window(data->mlx_ptr, data->win_ptr);
			mlx_destroy_display(data->mlx_ptr);
			free(data->mlx_ptr);
			exit(1);
		}
		else if (keysym == XK_Delete)//Este es un ejemplo, se puede hacer un caso para cada letra
			printf("The 'delete' key has been pressed\n\n");
		else
			printf("The %d key has been pressed\n\n", keysym);
		return (0);
	}

	//Funcion para manejar los eventos del mouse
	int	handle_mouse_input(int movement, int x, int y, t_mlx_data *data)
	{
		if (movement == 1)
			printf("Se ha apretado el boton izquierdo del mouse en (%d, %d)\n", x, y);
		if (movement == 2)
			printf("Se ha apretado la rueda de scroll del mouse en (%d, %d)\n", x, y);
		if (movement == 3)
			printf("Se ha apretado el boton derecho del mouse en (%d, %d)\n", x, y);
		if (movement == 4)
			printf("Se ha scrolleado hacia arriba\n");
		if (movement == 5)
			printf("Se ha scrolleado hacia abajo\n");
		return (0);
	}

	int	main(void)
	{
		t_mlx_data	data;

		//Iniciamos la conexion con X
		data.mlx_ptr = mlx_init();
		if (!data.mlx_ptr)
			return (MLX_ERROR);
		//Creamos la ventana
		data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "Test de eventos");
		if (!data.win_ptr)
		{
			mlx_destroy_display(data.mlx_ptr);
			free(data.mlx_ptr);
			return (MLX_ERROR);
		}

		//Funciones para manejar los hooks de teclado y mouse
		mlx_key_hook(data.win_ptr, handle_key_input, &data);
		mlx_mouse_hook(data.win_ptr, handle_mouse_input, &data);
		//Funcion para mantener el programa escuchando
		mlx_loop(data.mlx_ptr);
	}

	Si compilamos y ejecutamos el programa podemos testear los distintos eventos y hooks

	Se pueden hacer muchas cosas mas con la libreria minilibx, el siguiente ejemplo es sobre como 
	usar un par mas de funciones de la libreria:
	
		- mlx_string_put(): 
			
			int		mlx_string_put(t_xvar *xvar,t_win_list *win,
			       int x,int y,int color,char *string)

			Escribe un string en la ventana, en unas cordinadas concretas.

		- mlx_loop_hook():

			int	mlx_loop_hook(t_xvar *xvar,int (*funct)(),void *param)

			Aplica una funcion siempre que no se disparen eventos.
	
	
		*En este ejemplo, escribimos un texto con un color en una posicion en la pantalla.
		Tenemos un hook que ejecuta una funcion en bucle mientras no se disparen eventos
		(esta funcion cambiara el texto de color).
		Cuando se presione una tecla, se disparara un evento que imprime la tecla en consola
		y detiene el programa durante 2 segundos (no escuchara eventos ni ejecutara mas codigo):


		#include "minilibx-linux/mlx.h"
	#include <stdio.h>
	#include <unistd.h>

	typedef struct s_data
	{
		void	*mlx_ptr;
		void	*win_ptr;
		int		color;
	}			t_data;

	int	sleep_event(int keysym, t_data *data)//Funcion para alargar el evento 2 segundos
	{
		printf("Pressed %d\n", keysym);
		sleep(2);
		return (1);
	}

	int	change_color(t_data *data)//Funcion que se aplicara siempre que no se disparen eventos
	{
		mlx_string_put(data->mlx_ptr, data->win_ptr, 150, 250, data->color, "Hello World!");
	
		if (data->color == 0xFF0000)//Si es rojo
			data->color = 0x00FF00;//cambia a verde
		else if (data->color == 0x00FF00)//Si es verde
			data->color = 0x0000FF;//cambia a azul
		else
			data->color = 0xFF0000;//Sino cambia a rojo
		return (0);
	}

	int	main(void)
	{
		t_data	data;

		data.mlx_ptr = mlx_init();
		data.win_ptr = mlx_new_window(data.mlx_ptr, 500, 500, "Ventana texto de color");
		data.color = 0xFF0000;

		mlx_key_hook(data.win_ptr, sleep_event, &data);//Hook de gestion de teclado
		mlx_loop_hook(data.mlx_ptr, change_color, &data);//Hook de gestion de bucle
		mlx_loop(data.mlx_ptr);
		return(0);
	}

	-----------------------------

	Lo siguiente que veremos es como pintar los pixeles de la pantalla.
	Para ello usamos la funcion mlx_pixel_put (mlx_pixel_put.c):
		
		int		mlx_pixel_put(t_xvar *xvar,t_win_list *win,
			      int x,int y,int color)
	
		- Argumentos:
			conexion, ventana, cordenada x, cordenada y, color a pintar
	
	El ejemplo que se comparte a continuacion es mas complejo de lo que podria ser porque
	se crean estructuras de mas, pero esta explicado en los comentarios:


	#include "minilibx-linux/mlx.h"
	#include <stdio.h>
	#include <unistd.h>
	#include <X11/keysym.h>
	#include <string.h>
	#define SIDE_LEN	800

	//Estructura para acceder a los colores
	struct s_col_name
	{
		char	*name;
		int		color;
	};

	//Estructura con los colores
	struct s_col_name mlx_col_name[] = {
		{"red", 0xFF0000},
		{"green", 0x00FF00},
		{"blue", 0x0000FF},
		{"white", 0xFFFFFF},
		{"black", 0x000000},
		{NULL, 0}
	};

	//Estructura para la conexion y la ventana
	typedef struct s_var
	{
		void	*mlx;
		void	*win;
	}			t_var;

	//Funcion para obtener el color de la estructura
	int	get_color_by_name(const char *name)
	{
		int	i = 0;
	
		while (mlx_col_name[i].name != NULL)
		{
			if (strcmp(mlx_col_name[i].name, name) == 0)
				return (mlx_col_name[i].color);
			i++;
		}
		return (-1);
	}

	//Funcion para colorear la pantalla
	void	color_screen(t_var *data, int color)
	{
		for (int y = 0; y < SIDE_LEN; ++y)
		{
			for (int x = 0; x < SIDE_LEN; ++x)
			{
				mlx_pixel_put(data->mlx, data->win, x, y, color);
			}
		}
	}
	
	//Ni las estructuras de colores ni la funcion para obtener el color serian necesarias si en la funcion
	//f llamamos al color por su valor int hexadecimal
	//Funcion para gestionar el evento
	int	f(int keysym, t_var *data)
	{
		if (keysym == XK_r)
			color_screen(data, get_color_by_name("red"));
		else if (keysym == XK_g)
			color_screen(data, get_color_by_name("green"));
		else if (keysym == XK_b)
			color_screen(data, get_color_by_name("blue"));
		else if (keysym == XK_w)
			color_screen(data, get_color_by_name("white"));
		else if (keysym == XK_space)
			color_screen(data, get_color_by_name("black"));
	
		return (0);
	}
	
	int	main(void)
	{
		t_var	data;
	
		data.mlx = mlx_init();//Conexion
		data.win = mlx_new_window(data.mlx, SIDE_LEN, SIDE_LEN, "Ventana texto de color");//Ventana
	
		mlx_key_hook(data.win, f, &data);//Hook de gestion de teclado
		mlx_loop(data.mlx);//Loop para eventos
		return(0);
	}

	-----------------------

	Para optimizar el pintado de la ventana introducimos el concepto de IMAGEN:
		https://harm-smits.github.io/42docs/libs/minilibx/images.html

	-IMAGEN:
	
	El concepto de imagen se refiere a un bloque de memoria que representa una imagen en formato 
	crudo (raw), donde cada píxel se almacena en función de su color. 
	Cuando trabajamos con imágenes, no manipulamos directamente los píxeles con funciones como 
	mlx_pixel_put, sino que podemos acceder a la memoria de la imagen directamente utilizando punteros 
	a los datos de la imagen. Esto nos permite modificar muchos píxeles de manera eficiente sin 
	necesidad de realizar múltiples llamadas a la API gráfica.

	Para acceder a los píxeles de una imagen, utilizamos la función mlx_get_data_addr, que nos da 
	un puntero al bloque de memoria donde están almacenados los píxeles de la imagen en formato crudo. 
	Luego, podemos modificar los valores de estos píxeles directamente.

	- Funciones involucradas en el uso de imágenes con MiniLibX:

		-mlx_new_image: 
			
			void *mlx_new_image(void *mlx_ptr, int width, int height);
			
			Crea una nueva imagen en memoria que puedes modificar antes de mostrarla en la ventana.
		
		-mlx_get_data_addr:
		
			char *mlx_get_data_addr(void *img_ptr, int *bits_per_pixel, int *size_line, int *endian);

			Te proporciona un puntero al bloque de memoria donde están los píxeles de la imagen.

			-Argumentos:
				-img_ptr: Un puntero a la imagen que se creó con mlx_new_image.
				-bits_per_pixel: Número de bits usados para representar el color de un solo píxel.
				-size_line: Número de bytes utilizados para una fila completa de píxeles.
				-endian: Especifica el orden de los bytes en el valor del color 
				(si es little-endian o big-endian).
				-Retorno: Devuelve un puntero a la dirección de memoria donde comienza la imagen.

		-mlx_put_image_to_window:

			int mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr, int x, int y);
			
			Coloca la imagen en una ventana.
			Dibuja la imagen en las coordenadas especificadas dentro de la ventana.

		-mlx_destroy_image:

			int mlx_destroy_image(void *mlx_ptr, void *img_ptr);

			Libera la memoria de la imagen una vez que ya no es necesaria.

	
	Veamos un ejemplo de uso:

*/

#include "minilibx-linux/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <string.h>
#define SIDE_LEN	800

//Estructura para la conexion, la ventana y la imagen
typedef struct s_var
{
	void	*mlx;
	void	*win;
	void	*img;//Puntero a la imagen
	char	*addr;//Puntero a la direccion de los pixeles de la imagen
	int		bpp;//Bits por pixel
	int		line_length;//Tamano de la fila de la imagen
	int		endian;//Endian de la imagen
}			t_var;

//Funcion para obtener el color de la estructura
int	get_color_by_name(const char *name)
{
	if (strcmp(name, "red") == 0)
		return (0xFF0000);
	if (strcmp(name, "green") == 0)
		return (0x00FF00);
	if (strcmp(name, "blue") == 0)
		return (0x0000FF);
	if (strcmp(name, "white") == 0)
		return (0xFFFFFF);
	if (strcmp(name, "black") == 0)
		return (0x000000);
	return (-1);
}

//Funcion ara poner color en la direccion de la imagen
void	my_mlx_pixel_put(t_var *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bpp/8));
	*(unsigned int *)dst = color;
}
/*
   Esta funcion es la novedad en este caso y la que merece ser explicada mejor:
   Sirve para calcular la direccion en memoria de la imagen donde queremos escribir el pixel.
   	1 data->addr:
		addr es un puntero que apunta a la PRIMERA DIRECCION DE MEMORIA de la imagen (donde
		se almacenan los pixeles).
	2 y * data->line_length:
		Esto calcula la posicion (coordenada) vertical (y) en la imagen.
		Como cada fila tiene un numero determinado de bytes (representado por line_length),
		multiplicamos y por line_length para saltar a la fila adecuada.
	3 x * (data->bpp /8):
		Esto calcula la posicion (coordenada) horizontal (x) dentro de la file. El valor bpp
		(bits por pixel) nos indica cuantos bits se usan para cada pixel.
		Dividimos por 8 para obtener cuantos bytes ocupa cada pixel, ya que 1 byte = 8 bits.
		Luego multiplicamos por x para movernos al pixel correcto dentro de la fila.
	4 dst = data->addr + (y * data->line_length + x * (data->bpp/8));:
		Esta linia combina las dos operaciones anteriores y calcula la DIRECCION EXACTA EN MEMORIA 
		donde se encuentra el pixel en la posicion (x, y).
	5 *(unsigned int *)dst = color:
		Aqui convertimos dst en un puntero a un valor de tipo unsigned int y le asignamos el color.
		Es decir, almacenamos el valor de color en la memoria correspondiente a ese pixel.

	Porque declaramos dst como puntero a array de caracteres y luego lo casteamos a unsigned int?
	Usamos char* porque necesitamos movernos byte a byte (1 char es 1 byte), por la memoria para
	calcular la direccion correcta del pixel, independientemente del numero de bytes que ocupe
	cada pixel.
	Luego, casteamos a unsigned int* solo en el momento de escribir el color, porque estamos seguros
	de que queremos tratar ese bloque especifico de memoria como un valor completo (4 bytes en este caso)
*/

//Funcion para colorear la pantalla
void	color_screen(t_var *data, int color)
{
	for (int y = 0; y < SIDE_LEN; ++y)//loopeamos por filas
	{
		for (int x = 0; x < SIDE_LEN; ++x)//loopeamos por columnas
		{
			my_mlx_pixel_put(data, x, y, color);//En cada coordenada (x, y) pintamos de color
		}
	}
	//Despues de modificar la imagen en memoria, la mostramos en la ventana
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

//Funcion para gestionar el evento
int	f(int keysym, t_var *data)
{
	if (keysym == XK_r)
		color_screen(data, get_color_by_name("red"));
	else if (keysym == XK_g)
		color_screen(data, get_color_by_name("green"));
	else if (keysym == XK_b)
		color_screen(data, get_color_by_name("blue"));
	else if (keysym == XK_w)
		color_screen(data, get_color_by_name("white"));
	else if (keysym == XK_space)
		color_screen(data, get_color_by_name("black"));

	return (0);
}

int	main(void)
{
	t_var	data;

	data.mlx = mlx_init();//Conexion
	data.win = mlx_new_window(data.mlx, SIDE_LEN, SIDE_LEN, "Ventana texto de color");//Ventana
	data.img = mlx_new_image(data.mlx, SIDE_LEN, SIDE_LEN);//Crea la imagen

	//Obtiene la direccion de la imagen. 
	data.addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);

	mlx_key_hook(data.win, f, &data);//Hook de gestion de teclado
	mlx_loop(data.mlx);//Loop para eventos
	mlx_destroy_image(data.mlx, data.img);//Liberamos la memoria de la imagen
	return(0);
}
