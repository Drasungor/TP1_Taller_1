# TP1_Taller_1


TO DO:

- See if functions return bools or ints to indicate errors (correct the ones that return ints if bool are chosen)
- Hacer para socket un handler de cliente y un handler de server
- Arreglar el funcionamiento de sudoku_verify (se fija tambien que no haya casilleros vacíos, los cuales en realidad debería ignorar)
- Agregar static a todas las funciones que no estén declaradas en el .h
- Agregar la primitiva release a los TDA's que no la tengan
- Escribir mensajes de error para que printee cuando se reciba un comando invalido
- Sacar el chequeo de rango de posicion y de valor numerico del sudoku porque lo hace todo el cliente
- Hacer .h con constantes generales para el tp (chequeo de errores y caracteres q importan tanto para el cliente como para el server)
- Agregar el codigo del TIME_WAIT
- Ver si hay que mejorar la forma en la que se inicializa el sudoku con el archivo (como se acceden a los nros en la linea que se saca del archivo)
- Borrar el struct limits_t y hacer la firma de la funcion en distintas lineas
- IMPORTANTE: Agregar static a todas las funciones auxiliares de los tdas
- Imprimir todo el buffer de una con un printf, para eso hay que meterle un \0 al final del buffer que se recibe despues de recibir el numero
- Arreglar la forma en la q se comunica cuando el comando escrito es invalido
- IMPORTANTE: Agregar const a las funciones que reciban punteros que no van a ser modificados
- Ver si hay que borrar limits_t de sudoku.c y poner la firma de la funcion original en distintas lineas
- Ver qué hacer con el valor de retorno de getline cuando se lee de los archivos, no estoy manejando errores de memoria/lectura, investigar qué pasa si encuentra eof en vez de \n
- Revisar partes de codigo que hay que mejorar
- Cambiar el chequeo de devolucion de send en todos los lugares donde se haya usado xq ahora devuelve un int


DUDAS:

- Está mal tener un .h que solo tenga defines que se van a usar en todo el proyecto?
- Init tiene que inicializar todos los elementos del struct? O solo los que yo considere necesarios?
- Por qué se pone const puntero si en vez de eso se puede pasar una copia del tda y así se asegura que no se puede cambiar?
- Estoy mezclando cosas si en el init del sudoku leo el archivo? Lo mismo si devuelvo directamente la misma metriz que quiero imprimir
- Programo las cosas independientemente de lo que se pida? (ej: se chequea que en la inicializacion el sudoku reciba numeros de 0 a 9 aunque no se vaya a recibir un archivo mal hecho)
- Usamos setsockopt para ver si la dirección no está disponible por un TIME_WAIT?
- Las constantes de las bibliotecas van en el .c o en el .h?
- IMPORANTE: Para enviar un dato por un socket siempre hay que castear su dirección a char* y mandar un array de chars (bytes)?
- Está mal incluir un header solo por los defines? (incluyo server.h en client para que solo esten definidos en un único lugar los caracteres de los comandos para no tenes que cambiarlos en distintos lugares en caso de alguna modificación)
- IMPORTANTE: ver duda en verify de server.c
- IMPORTANTE: que conviene: ¿Pasar un array como parametro y modificarlo para que guarde lo que queramos?¿O devolverlo con memoria dinamica? Ver sudoku_get_board como referencia
- Revisar el release del server
- Ver qué hacer con la ejecución del programa si hay un error de sockets o de memoria dinamica
- Ver qué pasa con el getline que recibe los comandos de stdin cuando sudoku.txt no termina en un barra n
