# TP1_Taller_1


TO DO:

- See if functions return bools or ints to indicate errors (correct the ones that return ints if bool are chosen)
- Achicar verify_block, leer comentarios de todo el codigo escrito
- Ver si se pueden generalizar los verify en uno solo
- Ver si es mejor devolver memoria dinamica en vez de pasar una matriz por parametro a sudoku_get_board



DUDAS:

- Está mal no poner un init o destroy si sé que no lo no lo necesita el TDA
- Está mal tener un .h que solo tenga defines que se van a usar en todo el proyecto?
- Init tiene que inicializar todos los elementos del struct? O solo los que yo considere necesarios?
- Está bien hacer un iterador interno privado para un tda? O solo ensuciaría el código del .c? (así no escribo muchas veces el doble for )
- Esta bien poner returns y breaks en las iteraciones?
- Otras funciones en un TDA que no sean primitivas son consideradas globales?
- Está mal usar memset en algo que no sea strings?
- Por qué se pone const puntero si en vez de eso se puede pasar una copia del tda y así se asegura que no se puede cambiar?
