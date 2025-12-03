#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "Axioma.h"
#include "Afirmacion.h"
#define NARCHIVO "AxiomasPrueba23.txt"
#define NPREGUNTA "PreguntaPrueba2.txt"

void imprimirReglasArchivo();
void formalizarAxiomaImprimir(std::string& linea, std::string& reglas);
void leerArchivo(std::vector<Axioma>& baseConocimiento);
void normalizarAxioma(std::string linea, std::vector<Axioma>& baseConocimiento);
void quitarCuantificadores(std::string& linea, char cuantificador);
std::string buscarIndependientes(std::string varCuant, std::string valoresAfirmacion);
void lineaSinCorchete(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones,
    std::vector<Axioma>& baseConocimiento);
void evaluarBiCondicional(std::string& afirmacion, std::string& linea);
void evaluarEntonces(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones,
    std::vector<Axioma>& baseConocimiento);
void lineaConCorchete(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones,
    std::vector<Axioma>& baseConocimiento);
void modificarLineaEntOr(std::string& linea, int posCorcheteCierre, int posCorcheteAfirmacion, std::string& afirmacion);
std::string distributiva(std::string& linea, std::string afirmacion, char conector);
void negacion(std::string& linea);
void negacionCasoRecursivo(std::string contenidoParentesis, std::string& linea);
bool reemplazarStr(std::string cadenaBuscar, std::string contenidoReemplazo, std::string& cadenaFinal, int posIni);
void negarSimple(std::string& linea, std::string contenidoParentesis, bool& negacionCompletada);
void signoNegarAfirmacion(std::string& cadena, std::string& negado);
void negarAfirmacionesOperador(std::string contenidoParentesis,
    std::string& linea, bool& negacionCompletada, char simboloReem,
    char simboloFin);
int cantidadOperadores(std::string cadena);
void negarCompuesto(std::string& cadena);
void negarOperadorAfirmacion(std::string& contenidoParentesis, std::string& cadena);
int posOperador(std::string cadena);
void peluquearParentesisCuadrados(std::string& linea);
std::vector<Afirmacion> crearAfirmaciones(std::string afirmacion, std::vector<Axioma>& baseConocimiento);
std::vector<Afirmacion> crearAfirmacionCompuesta(std::string& afirmacion, std::vector<Axioma>& baseConocimiento);
std::vector<Afirmacion> crearAfirmacionSimple(std::string& afirmacion, std::vector<Axioma>& baseConocimiento);
std::vector<ValorPredicado> crearValores(std::string afirmacion);
void imprimirReglasNormalizadas(std::vector<Axioma>& baseConocimiento);
void leerPregunta(Axioma& preguntaNegada);
void convertirPregunta(std::string linea, Axioma& preguntaNegada);
void inferenciaResolucion(std::vector<Axioma> baseConocimiento, Axioma axiomaSolucion);
void unificacion(Axioma& axioma, Axioma axiomasSolucion);
void reemplazarVarPredicados(std::map<std::string, std::string> variablesValor,
    Axioma& axioma, std::string predicadoIgual);
void imprimirAfirmaciones(std::vector<Afirmacion> afirmaciones);

int main(void) {
    std::vector<Axioma> baseConocimiento;
    Axioma preguntaNegada;
    imprimirReglasArchivo();
    leerArchivo(baseConocimiento);
    imprimirReglasNormalizadas(baseConocimiento);
    leerPregunta(preguntaNegada);
    inferenciaResolucion(baseConocimiento, preguntaNegada);
	return 0;
}

//Lee el archivo con las reglas
void imprimirReglasArchivo() {
    std::string nombre_archivo = NARCHIVO;
    std::ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombre_archivo << std::endl;
        exit(-1);
    }
    std::string linea, reglas;
    while (std::getline(archivo, linea)) {
        formalizarAxiomaImprimir(linea, reglas);
    }
    std::cout << "Reglas iniciales:\n" << reglas << std::endl;
    archivo.close();
}

//Cambia los hechos recibidos, cambia los > por ->, los * por Para todo y @ por Existe un (sin normalizar)
void formalizarAxiomaImprimir(std::string& linea, std::string& reglas) {
    int posEnt = linea.find(">");
    if (posEnt != -1) {
        linea.replace(posEnt, 1, "->"); //Reemplaza el > con un ->
    }
    int posAst = linea.find('*');
	// Reemplaza cada "*" y con "Para todo []"
    while (posAst != -1) {
        int posAstFin = linea.find('*', posAst + 1);
        linea.replace(posAst, posAstFin - posAst + 1, "Para todo [" + linea.substr(posAst + 1, posAstFin - posAst - 1) + "]");
        posAst = linea.find('*');
    }
    posAst = linea.find('@');
	// Reemplaza cada "@" y con "Existe un []"
    while (posAst != -1) {
        int posAstFin = linea.find('@', posAst + 1);
        linea.replace(posAst, posAstFin - posAst + 1, "Existe un [" + linea.substr(posAst + 1, posAstFin - posAst - 1) + "]");
        posAst = linea.find('@');
    }
	// Agrega la línea ya modificada al conjunto de reglas
    reglas += linea + "\n";
}

//Lee el archivo con las reglas
void leerArchivo(std::vector<Axioma>& baseConocimiento) {
    std::string nombre_archivo = NARCHIVO;
    std::ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombre_archivo << std::endl;
        exit(-1);
    }
    std::string linea;
    while (std::getline(archivo, linea)) {        
        normalizarAxioma(linea, baseConocimiento);
    }
    archivo.close();
}

//Empieza la normalización de los axiomas, primero elimina los cuantificadores y luego los trata segun tenga corchetes o no
void normalizarAxioma(std::string linea, std::vector<Axioma>& baseConocimiento) {
    std::string afirmacion;
    std::vector<Afirmacion> afirmaciones;
    quitarCuantificadores(linea, '*');
    quitarCuantificadores(linea, '@');
    while (linea.size() != 0) {
        if (linea[0] != '[') {
            lineaSinCorchete(afirmacion, linea, afirmaciones, baseConocimiento); //En caso de ser simple
        }
        else {
            lineaConCorchete(afirmacion, linea, afirmaciones, baseConocimiento); //En caso de ser compuesto (corchetes)
        }
    }
}


void quitarCuantificadores(std::string& linea, char cuantificador) {
    char nombreFuncion = 'F';
    int numeroFuncion = 1;
	//Bucle mientras haya cuantificadores
    while (linea.find(cuantificador) != -1) {
        int posCuantIni = linea.find(cuantificador);//Posición del cuantificador
        int posCuantFin = linea.find(cuantificador, posCuantIni + 1) + 1;//Posición del cierre del cuantificador
        //Saca la parte afectada por el cuantificador
		std::string contCuant = linea.substr(posCuantIni, posCuantFin - posCuantIni);
		//Si es un Existe un y hay una coma (quiere decir que el existe es una variable dependiente)
        if (cuantificador == '@' && linea.find(",", posCuantIni, linea.find(")"))) {
            bool constanteEncontrada = false;
			//Posiciones del inicio y fin de los parentesis ()
            int posParINi = linea.find("(", posCuantFin);
            int posParCierr = linea.find(")", posCuantFin);
			//Contenido dentro de los paréntesis
            std::string valoresAfirmacion = linea.substr(posParINi + 1, posParCierr - posParINi - 1);
            std::string funcionCompleta = nombreFuncion + std::to_string(numeroFuncion);
			//Se le da una funcion unica a cada caso Fn
            while (linea.find(funcionCompleta) != -1) {
                numeroFuncion++;
                funcionCompleta = nombreFuncion + std::to_string(numeroFuncion);
            }
			//Saca la variable
            std::string varCuant = contCuant.substr(1, contCuant.length() - 2);
            std::string varsIndep = buscarIndependientes(varCuant, valoresAfirmacion);
            funcionCompleta += "{" + varsIndep + "}";
			//hace el proceso de skolem, poniendo las variables como dependiente
            reemplazarStr(varCuant, funcionCompleta, linea, posParINi);
        }
		//Quita el cuantificador
        linea.replace(posCuantIni, contCuant.size(), "");
    }
}

std::string buscarIndependientes(std::string varCuant, std::string valoresAfirmacion) {
    std::string valoresFinal = "";
    int posComa = valoresAfirmacion.find(",");
    while(posComa != -1){
		//Saca de la variable hasta la coma
        std::string varActual = valoresAfirmacion.substr(0, posComa);
		//Si no es la variable cuantificada, directamente se agrega al final por ser independiente
        if(varCuant != varActual){
            valoresFinal += varActual + ",";
        }
		//El contenido después de la coma
        valoresAfirmacion = valoresAfirmacion.substr(posComa + 1);
		posComa = valoresAfirmacion.find(",");
    }
	//Evalua la última variable por si también es independiente
	if (varCuant != valoresAfirmacion) {
		valoresFinal += valoresAfirmacion;
	}
    return valoresFinal;
}


void lineaSinCorchete(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones,
                      std::vector<Axioma>& baseConocimiento) {
    afirmacion = linea.substr(0, linea.find(")") + 1);//Saca la parte de entre los ()
    linea = linea.substr(linea.find(")") + 1); //Actualiza la linea, quitando la afirmacion
	//Si la linea queda vacía despues de quitar la afirmacion
    if (linea.size() == 0) {
        std::vector<Afirmacion> afirmacionesObtenidas = crearAfirmaciones(afirmacion, baseConocimiento);//Se obtienen afirmaciones
        afirmaciones.insert(afirmaciones.end(), afirmacionesObtenidas.begin(), afirmacionesObtenidas.end());//Lista de afirmaciones de lo anterior
        baseConocimiento.push_back(afirmaciones);//Las afirmacines obtenidas se agregan a la base de conocimientos
        linea = "";
		//No es necesario seguir pues la linea quedó vacia
        return;
    }
    char conector = linea[0];
    linea = linea.substr(1);//Quita de la linea el conector
    if (conector == '>') {
        evaluarEntonces(afirmacion, linea, afirmaciones, baseConocimiento); //En caso de que sea un entonces
    }
    else if (conector == '<') {
        evaluarBiCondicional(afirmacion, linea); //En caso de que sea un bicondicional
    }
    else if (conector == '|' && linea[0] == '[') {
        std::string distributivaFinal = distributiva(linea, afirmacion, conector); //En caso de que sea distributiva
    }
}

void evaluarBiCondicional(std::string& afirmacion, std::string& linea) {
    if (afirmacion == "") {
        afirmacion = linea.substr(0, linea.find(")") + 1); //Si no hay afirmación hay que asignarla
    }
    reemplazarStr("<", "", linea, 0); //Quita el <
    int posParCierr = linea.find(")", linea.find(">"));
    int posEnt = linea.find(">");
    std::string afirmacion2 = linea.substr(posEnt+1, (posParCierr + 1) - (posEnt + 1)); //Obtiene la afirmación 2
    peluquearParentesisCuadrados(afirmacion); //Quita los parentesis de las afirmaciones
    peluquearParentesisCuadrados(afirmacion2);
    std::string cadenaFin = "[" + afirmacion + ">" + afirmacion2 + "]&[" + afirmacion2 +
                            ">" + afirmacion + "]"; //Arma los dos entonces
    int iniLinea = 0;
    int finLinea = posParCierr + 2;
    if (linea[0] == '[') {
        iniLinea++;
        finLinea-=2;
    }
    reemplazarStr(linea.substr(iniLinea, finLinea), cadenaFin, linea, 0); //Reemplaza el contenido anterior por el nuevo
}

void evaluarEntonces(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones, 
                     std::vector<Axioma>& baseConocimiento) {
    char conector = '|';
    if (afirmacion[0] == '-') {
        afirmacion = afirmacion.substr(1);//Quita la negacion de la afirmacion
    }
    else {
        afirmacion = "-" + afirmacion;//Agrega la negacion para la transformacion
    }
	//Se crean las afirmaciones dado el procesamiento previo y se agregan a la base de conocimientos
    std::vector<Afirmacion> afirmacionesObtenidas = crearAfirmaciones(afirmacion, baseConocimiento);
    afirmaciones.insert(afirmaciones.end(), afirmacionesObtenidas.begin(), afirmacionesObtenidas.end());
    afirmacionesObtenidas = crearAfirmaciones(linea, baseConocimiento);
    afirmaciones.insert(afirmaciones.end(), afirmacionesObtenidas.begin(), afirmacionesObtenidas.end());
    baseConocimiento.push_back(afirmaciones);
    linea = "";
}

void lineaConCorchete(std::string& afirmacion, std::string& linea, std::vector<Afirmacion>& afirmaciones,
                      std::vector<Axioma>& baseConocimiento) {
    bool biCondicional = false;
	int posCorcheteCierre = linea.find("]");
	if (linea.find("<") != -1) { //Si encuentra que es un bicondicional
        evaluarBiCondicional(afirmacion, linea); //Evalua el bicondicional
        posCorcheteCierre = linea.find(")");
        biCondicional = true;
        std::string contenidoIni = linea.substr(0, linea.find_last_of("]")+1);
        std::string cadenaReemplazar = contenidoIni;
        negacion(cadenaReemplazar);
        reemplazarStr(contenidoIni, cadenaReemplazar, linea, 0); //Reemplaza el contenido con lo obtenido en la linea
    }
    std::string conectorStr = linea.substr(posCorcheteCierre + 1);//La parte posterior al corchete de cierre
    char conector = conectorStr[0];
    if (conector == '>' && !biCondicional) { //Si es un entonces y no un bicondicional
        negacion(linea);
    }
    
    posCorcheteCierre = linea.find("]");//Busca nuevamente el corchete de cierre por si cambió en la negacion
    int posCorcheteAfirmacion = linea.find(")", posCorcheteCierre + 2);
    //Toma la siguiente afirmacion y el que estaba en parentesis, cambiandolos de lugar y lo demas lo deja quieto
    if (conector != '&') {
        modificarLineaEntOr(linea, posCorcheteCierre, posCorcheteAfirmacion, afirmacion);
    }
    peluquearParentesisCuadrados(linea); //Quita los parentesis cuadradps
    //Crea las afimaciones
    std::vector<Afirmacion> afirmacionesObtenidas = crearAfirmaciones(linea, baseConocimiento);
    afirmaciones.insert(afirmaciones.end(), afirmacionesObtenidas.begin(), afirmacionesObtenidas.end());
    baseConocimiento.push_back(afirmaciones);
    linea = "";
}

//Cambia el entonces por un or y hace la distributiva
void modificarLineaEntOr(std::string& linea, int posCorcheteCierre, int posCorcheteAfirmacion, std::string& afirmacion) {
    linea = linea.substr(posCorcheteCierre + 2, posCorcheteAfirmacion - (posCorcheteCierre + 1)) + "|" + linea.substr(0, posCorcheteCierre + 1) + linea.substr(posCorcheteAfirmacion + 1);
    afirmacion = linea.substr(0, linea.find(")") + 1);
    linea = linea.substr(linea.find(")") + 1);
    linea = linea.substr(1);
    linea = distributiva(linea, afirmacion, '|');
}

void negacion(std::string& linea) {
    bool negacionCompletada = false;
    std::string contenidoParentesis;
    std::string parentesisNegado = "";
    while (!negacionCompletada) {
		//Contenido antes de la implicacion
        contenidoParentesis = linea.substr(1, linea.find_last_of(">") - 1);
		// Se elimina cualquier corchete adicional al final del contenido
        while (contenidoParentesis[contenidoParentesis.size() - 1] == ']') {
            contenidoParentesis = contenidoParentesis.substr(0, contenidoParentesis.size() - 1);
        }
		// Si el contenido de los paréntesis es una expresión compuesta
        if (contenidoParentesis[0] == '[' && contenidoParentesis.find(">") != -1) {
            negacionCasoRecursivo(contenidoParentesis, linea);
        }
        else if (cantidadOperadores(linea) >= 2) {
            negarCompuesto(linea);
            negacionCompletada = true;
        }
        else {
            negarSimple(linea, contenidoParentesis, negacionCompletada);
			//Si hay otra implicacion se niega otra vez
            if (linea.find(">") != -1) {
                negacion(linea);
            }
        }
    }
}


void negacionCasoRecursivo(std::string contenidoParentesis, std::string& linea) {
	// Se extrae el contenido dentro de los paréntesis
    std::string contenidoRecursion = contenidoParentesis.substr(0, linea.find("]", linea.find(">")));
    std::string contenidoRecIni = contenidoRecursion;
    negacion(contenidoRecursion);
    reemplazarStr(contenidoRecIni, contenidoRecursion, linea, 0);
}

bool reemplazarStr(std::string cadenaBuscar, std::string contenidoReemplazo, std::string& cadenaFinal, int posIni) {
    int posicionRecursion = cadenaFinal.find(cadenaBuscar, posIni);
    if (posicionRecursion != -1) {
		// Se reemplaza la cadena a buscar con el contenido de reemplazo en la cadena final
        cadenaFinal.replace(posicionRecursion, cadenaBuscar.size(), contenidoReemplazo);
        return true;
    }
    return false;
}

//Si se niega una negacion, simplemente se quitan las negaciones
void signoNegarAfirmacion(std::string& cadena, std::string& negado) {
    if (cadena[0] == '-') {
        negado = "";
        cadena = cadena.substr(1);
        return;
    }
}

void negarSimple(std::string& linea, std::string contenidoParentesis, bool& negacionCompletada) {
    int posParentesisCierre = linea.find(")") + 1;
    if (linea[posParentesisCierre] == '&') {
        negarAfirmacionesOperador(contenidoParentesis, linea, negacionCompletada, '&', '|');
    }
    else if (linea[posParentesisCierre] == '|') {
        negarAfirmacionesOperador(contenidoParentesis, linea, negacionCompletada, '|', '&');
    }
    else if (linea[posParentesisCierre] == '>') {
		//Se extrae la afirmación y el consecuente de la implicacion
        int posIniAf = 0;
        std::string afirmacion = contenidoParentesis.substr(posIniAf, contenidoParentesis.find(")") - posIniAf + 1);
        //Se niega o quita la negacion segun sea
		if (afirmacion[0] == '-') {
            afirmacion = afirmacion.substr(1);
        }
        else {
            afirmacion = "-" + afirmacion;
        }
        std::string consecuente = contenidoParentesis.substr(contenidoParentesis.find(">") + 1);
		// Se reemplaza la expresión original por la negación de la implicación
        linea.replace(1, linea.find("]") - 1, afirmacion + "|" + consecuente);
        
        negacionCompletada = true;
    }
}

void negarAfirmacionesOperador(std::string contenidoParentesis, std::string& linea, 
                               bool& negacionCompletada, char simboloReem, char simboloFin){
    std::string antecedenteInicial = contenidoParentesis;
    peluquearParentesisCuadrados(contenidoParentesis);
    std::string negado = "-";
    int posOperador = contenidoParentesis.find(simboloReem);
    std::string afirmacion1 = contenidoParentesis.substr(0, posOperador);//Primera afirmacion
    signoNegarAfirmacion(afirmacion1, negado);
    std::string negadoConsecuente = "-";
    std::string afirmacion2 = contenidoParentesis.substr(posOperador+1);//Segunda afirmacion
    signoNegarAfirmacion(afirmacion2, negadoConsecuente);
    int posCierreContenido = contenidoParentesis.find(")") + 1;
	// Se construye la expresion negada con las afirmaciones y el operador a poner
    std::string parentesisNegado = negado + afirmacion1 + simboloFin + negadoConsecuente + afirmacion2;
    reemplazarStr(antecedenteInicial, parentesisNegado, linea, 0);
    reemplazarStr(">", "|", linea, 0);
    negacionCompletada = true;
}


// Elimina los [] de las lineas
void peluquearParentesisCuadrados(std::string& linea) {
    int posParCuad = 0, posParCuadCierr = 0;
    do {} 
    while (reemplazarStr("[", "", linea, 0) != false || 
           reemplazarStr("]", "", linea, 0) != false);
}

//Cuenta los operadores
int cantidadOperadores(std::string cadena) {
    int cant = 0;
    for (int i = 0; i < cadena.size(); i++) {
        if (cadena[i] == '&' || cadena[i] == '|') {
            cant++;
        }
    }
    return cant;
}

void negarCompuesto(std::string& cadena) {
    bool negacionCompleta = false;
    std::string contenidoParentesis = cadena.substr(cadena.find("[")+1, cadena.find("]")-1);
    while (!contenidoParentesis.empty()) {
		//Se eliminan los corchetes adicionales
        while (contenidoParentesis[0] == ']') {
            contenidoParentesis = contenidoParentesis.substr(1);
        }
		// Si hay corchete es compuesta
        if (contenidoParentesis[0] == '[') {
            std::string contenidoParIni = contenidoParentesis;
            negacion(contenidoParentesis);
            contenidoParentesis = contenidoParentesis.substr(1);//Elimina [] que pudieran surgir de la negacion
            reemplazarStr(contenidoParIni, contenidoParentesis, cadena,0);//Se reemplaza la expresión original con la expresión negada en la cadena
            contenidoParentesis = cadena.substr(cadena.find("]") + 1);//Se actualiza el contenido del parentesis con el restante de la cadena
        }
        else {
            negarOperadorAfirmacion(contenidoParentesis, cadena);
        }
    }
    int posEnt = cadena.find(">");
    if (posEnt != -1) {
        cadena.replace(posEnt, 1, "|");
    }
}

void negarOperadorAfirmacion(std::string& contenidoParentesis, std::string& cadena) {
    std::string negada = "";
    std::string contenidoParentesisIni = contenidoParentesis;
    if (contenidoParentesis[0] == '&') {
        negada = '|';
        contenidoParentesis = contenidoParentesis.substr(1);
    }
    else if (contenidoParentesis[0] == '|') {
        negada = '&';
        contenidoParentesis = contenidoParentesis.substr(1);
    }
    std::string negado = "-";
    if (reemplazarStr("-", "", contenidoParentesis, contenidoParentesis.find(">") + 2)) {
        negado = "";
    }
	//Se construye la expresion negada
    negada += negado + contenidoParentesis;
	//Se reemplaza la expresión original con la expresion negada en la cadena
    reemplazarStr(contenidoParentesisIni, negada, cadena, 0);
	//Si se encuentra un proximo operador, se actualiza el contenido con lo que resta de la cadena
    int posSigOperador = posOperador(contenidoParentesis);
    if (posSigOperador != -1) {
        contenidoParentesis = contenidoParentesis.substr(posSigOperador);
    }
    else {
        contenidoParentesis = "";
    }
}

int posOperador(std::string cadena) {
    for (int i = 0; i < cadena.size(); i++) {
        if (cadena[i] == '&' || cadena[i] == '|') {
            return i;
        }
    }
    return -1;
}

std::string distributiva(std::string& linea, std::string afirmacion, char conector) {
    int posFinParentesis = linea.find(']') - 1;
	//Se extrae el contenido dentro de los corcehtes
    std::string contenidoParentesis = linea.substr(1, posFinParentesis);
    std::string distributivaFinal = "";
    while (contenidoParentesis.size() != 0) {
		//Se extrae la siguiente afirmación dentro del contenido
        std::string afirmacion2 = contenidoParentesis.substr(0, contenidoParentesis.find("&"));
        if (contenidoParentesis.find("&") != -1) {
			//Si hay operador and, se construye la parte distributiva de la expresión final y se añade al resultado
            distributivaFinal += "[" + afirmacion + conector + afirmacion2 + "]&";
			//Se actualiza el contenido eliminando la afirmacion procesada y el operador "&"
            contenidoParentesis = contenidoParentesis.substr(contenidoParentesis.find("&") + 1);
        }
		//Si no hay operador and, se construye y añade
        else {
            distributivaFinal += "[" + afirmacion + conector + afirmacion2 + "]";
            contenidoParentesis = "";
        }
    }
	// Se actualiza la línea eliminando la parte procesada de los corchetes y el operador siguiente
    linea = linea.substr(posFinParentesis + 2);
    return distributivaFinal;
}

//Procesa las afirmaciones
std::vector<Afirmacion> crearAfirmaciones(std::string afirmacion, std::vector<Axioma>& baseConocimiento) {
    if (afirmacion[0] == '[' || cantidadOperadores(afirmacion) > 0) {
        return crearAfirmacionCompuesta(afirmacion, baseConocimiento);
    }
    return crearAfirmacionSimple(afirmacion, baseConocimiento);
}

std::vector<Afirmacion> crearAfirmacionCompuesta(std::string& afirmacion, std::vector<Axioma>& baseConocimiento) {
    std::vector<Afirmacion> afirmaciones;
    peluquearParentesisCuadrados(afirmacion);
    while (!afirmacion.empty()) {
		// Si el operador es un and
        if (afirmacion[0] == '&') {
            int posFin = afirmacion.find("&", 1);
			//Si no se encuentra más un and, se asigna la longitud total de la afirmación como la posición final
            if (posFin == -1) {
                posFin = afirmacion.size();
            }
			//Se crea la afirmación hasta el próximo and &
            baseConocimiento.push_back(crearAfirmaciones(afirmacion.substr(1, posFin), baseConocimiento));
            //Se actualiza la afirmación eliminando la parte procesada
			afirmacion = afirmacion.substr(posFin);
            continue;
        }
		//Si es un or
        else if (afirmacion[0] == '|') {
            afirmacion = afirmacion.substr(1);
        }
		//Se agregan las afirmaciones a la base de conocimientos separando por )
        std::vector<Afirmacion> afirmacionesObtenidas = crearAfirmaciones(afirmacion.substr(0, afirmacion.find(")") + 1), baseConocimiento);
        afirmacion = afirmacion.substr(afirmacion.find(")") + 1);
        afirmaciones.insert(afirmaciones.end(), afirmacionesObtenidas.begin(), afirmacionesObtenidas.end());
    }
    return afirmaciones;
}

std::vector<Afirmacion> crearAfirmacionSimple(std::string& afirmacion, std::vector<Axioma>& baseConocimiento) {
    std::vector<Afirmacion> afirmaciones;
    bool negado = false;
	//Identifica si está negado
    if (afirmacion[0] == '-') {
        negado = true;
        afirmacion = afirmacion.substr(1);
    }
    int posFinPred = afirmacion.find("(");
    std::string predicado = afirmacion.substr(0, posFinPred);
    afirmacion = afirmacion.substr(posFinPred+1, afirmacion.size() - posFinPred - 2);//Extrae los valores de los argumentos
    Afirmacion afirmacionClase(predicado, crearValores(afirmacion), negado);
    afirmaciones.push_back(afirmacionClase);
    return afirmaciones;
}

std::vector<ValorPredicado> crearValores(std::string afirmacion) {
    std::vector<ValorPredicado> valores;
    std::string nombreConstVar, valorVar = "";
    while (afirmacion.size() != 0) {
        int posComa = afirmacion.find(",");
		//Por si es una variable
        if (posComa == 1) {
            nombreConstVar = afirmacion[0];
            afirmacion = afirmacion.substr(2);
        }
		//último o unica variable o constante
        else if (afirmacion.find(",") == -1) {
            nombreConstVar = afirmacion;
            afirmacion = "";
        }
		//Asigna el nombre seg´un lo que haya entre las comas
        else {
            nombreConstVar = afirmacion.substr(0, afirmacion.find(","));
            afirmacion = afirmacion.substr(afirmacion.find(",") + 1);
        }
        ValorPredicado valor(nombreConstVar, valorVar);
        valores.push_back(valor);
    }
    return valores;
}

//Simplemente imprime las reglas
void imprimirReglasNormalizadas(std::vector<Axioma>& baseConocimiento) {
    std::cout << "Reglas normalizadas:\n";
    for (int i = 0; i < baseConocimiento.size(); i++) {
        imprimirAfirmaciones(baseConocimiento[i].getAfirmaciones());
        std::cout << "\n";
    }
    std::cout << "\n";
}

//Lee la pregunta que será la hipótesis
void leerPregunta(Axioma& preguntaNegada) {
    std::string nombre_archivo = NPREGUNTA;
    std::ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << nombre_archivo << std::endl;
        exit(-1);
    }
    std::string linea;
    std::getline(archivo, linea); //Se asume que solo es una linea para la pregunta
    convertirPregunta(linea, preguntaNegada);
    archivo.close();
}

void convertirPregunta(std::string linea, Axioma& preguntaNegada) {
    std::vector<Axioma> axiomasTemp; //Variable temporal para que sea compatible con la funcion de crearAfirmaciones
    std::cout << "Pregunta:\n" << linea << "\n\n";
    axiomasTemp.push_back(crearAfirmaciones(linea, axiomasTemp));
    preguntaNegada = axiomasTemp[0]; //Asigna el axioma obtenido
    if (preguntaNegada.getAfirmaciones()[0].getNegado()) {
        preguntaNegada.getAfirmaciones()[0].setNegado(false);
        return;
    }
    preguntaNegada.getAfirmaciones()[0].setNegado(true);
}

void inferenciaResolucion(std::vector<Axioma> baseConocimiento, Axioma axiomaSolucion) {
    bool afirmacionNegadaEncontrada = true;
    std::cout << "Inferencia:\n";
	//Mientras haya axiomas por resolver o se encuentre la negada
    while (!axiomaSolucion.getAfirmaciones().empty() && afirmacionNegadaEncontrada) {
        afirmacionNegadaEncontrada = false;
		//Se intenta unificar el axioma resultante que al principio será la misma hipótesis con los axiomas de la base de conocimientos
        for (int i = 0; i < baseConocimiento.size() && !axiomaSolucion.getAfirmaciones().empty(); i++) {
            unificacion(baseConocimiento[i], axiomaSolucion);
            if (!axiomaSolucion.isAxiomaVarCompleto()) {
                unificacion(axiomaSolucion, baseConocimiento[i]);
            }
            for (int k = 0; k < baseConocimiento[i].getAfirmaciones().size() && !axiomaSolucion.getAfirmaciones().empty(); k++) {
                std::vector<Afirmacion> afAxiomAnt = axiomaSolucion.getAfirmaciones();
                std::vector<Afirmacion> afirmacionesExcluidas;
                if (axiomaSolucion.encontrarYEliminarAfirmacionesNegadas(baseConocimiento[i].getAfirmaciones(), k, afirmacionesExcluidas)) {
                    std::cout << "Solucion: ";
                    imprimirAfirmaciones(afAxiomAnt);
                    std::cout << " Base de conocimiento: ";
                    imprimirAfirmaciones(baseConocimiento[i].getAfirmaciones());
                    std::cout << "\n";
					//Se insertan las afirmaciones separadas en el axioma de solución
                    afirmacionesExcluidas.insert(afirmacionesExcluidas.end(), axiomaSolucion.getAfirmaciones().begin(), axiomaSolucion.getAfirmaciones().end());
                    axiomaSolucion.insertarAfirmacionesMenosUnas(baseConocimiento[i].getAfirmaciones(), afirmacionesExcluidas);
                    afirmacionNegadaEncontrada = true;
                    break;
                }
            }
            if (afirmacionNegadaEncontrada) {
                break;
            }
        }
    }
	//Si no se resolvió, o sea, no se encontró un vacío, entonces no se pudo demostrar por inferencia
    if (!axiomaSolucion.getAfirmaciones().empty()) {
        std::cout << "La pregunta es falsa\n";
        return;
    }
    std::cout << "La pregunta es verdadera\n";
}

void unificacion(Axioma& axioma, Axioma axiomasSolucion) {
    //Contiene las variables con su respectivo valor al encontrarlo según la pregunta
	std::map<std::string, std::string> variablesValor;
    std::string predicadoIgual;
	//Se recorren las afirmaciones del primer axioma
    for (int i = 0; i < axioma.sizeAfirmaciones(); i++) {
		for (int j = 0; j < axiomasSolucion.sizeAfirmaciones(); j++) {
			// Si los predicados son iguales y tienen la misma cantidad de valores
			if (axioma.getPredicado(i) == axiomasSolucion.getPredicado(j) &&
				axioma.sizeValores(i) == axiomasSolucion.sizeValores(j)) {
					//Se recorren los valores de las afirmaciones
				for (int k = 0; k < axioma.sizeValores(i); k++) {
                    predicadoIgual = axioma.getPredicado(i);
					//Si la variable no ha sido unificada y no es independiente
					if (std::isupper(axioma.getNombreVal(i, k)[0]) &&
						axioma.getNombreVal(i, k).find("}") == -1 &&
                        variablesValor.find(axioma.getNombreVal(i, k)) == variablesValor.end()) {
                        std::string valorVariable = axiomasSolucion.getNombreVal(j, k);
						//Verifica que sea una variable y sea igual a la del axioma solucion
                        if ((std::isupper(valorVariable[0]) && valorVariable == axioma.getNombreVal(i, k)) ||
                            std::islower(valorVariable[0])) {
								//Si es variable y no una constante se reemplaza con el nombre en el axioma solucion
                            if (std::isupper(valorVariable[0])) {
                                valorVariable = axiomasSolucion.getValorVar(j, k);
                            }
                            axioma.setValorVar(i, k, valorVariable);//Se actualiza el valor de la variable en el axioma original
                            variablesValor.insert(std::make_pair(axioma.getNombreVal(i, k), axioma.getValorVar(i, k)));//Se guarda la unificación en el map
                        }
					}
					//Hay variables independientes de cuando se quito un Existe un
					else if (std::isupper(axioma.getNombreVal(i, k)[0]) &&
						axioma.getNombreVal(i, k).find("}") != -1 && 
                        variablesValor.find(axioma.getNombreVal(i, k)) == variablesValor.end()) {
						std::string nombreValIni = axioma.getNombreVal(i, k);
						axioma.setNombreVal(i, k, axiomasSolucion.getNombreVal(j, k));
                        axioma.setValorVar(i, k, axiomasSolucion.getNombreVal(j, k));
						variablesValor.insert(std::make_pair(nombreValIni, axioma.getNombreVal(i, k)));//Se guarda la unificación en el map
					}
				}
			}
		}
	}
	reemplazarVarPredicados(variablesValor, axioma, predicadoIgual);
}

//Reemplaza las variables de los argumentos en los predicados
void reemplazarVarPredicados(std::map<std::string, std::string> variablesValor, 
                             Axioma& axioma, std::string predicadoIgual) {
    for (int i = 0; i < axioma.sizeAfirmaciones(); i++) {
        for (int j = 0; j < axioma.sizeValores(i); j++) {
			//Verifica si el nombre de la variable está en el map
            if (variablesValor.find(axioma.getNombreVal(i, j)) != variablesValor.end()) {
				//Verifica si es independiente o no
                if (variablesValor[axioma.getNombreVal(i, j)].find("}") == -1) {
                    axioma.setValorVar(i, j, variablesValor[axioma.getNombreVal(i, j)]);
                }
                else {
                    axioma.setNombreVal(i, j, variablesValor[axioma.getNombreVal(i, j)]);
                }
            }
        }
    }
}

//Imprime las afirmaciones pasadas con el formato adecuado
void imprimirAfirmaciones(std::vector<Afirmacion> afirmaciones) {
    for (int i = 0; i < afirmaciones.size(); i++) {
        std::string negado = "";
        if (afirmaciones[i].getNegado()) {
            negado = "-";
        }
        std::cout << negado << afirmaciones[i].getPredicado() << "(";
        for (int j = 0; j < afirmaciones[i].getValores().size(); j++) {
            if (std::isupper(afirmaciones[i].getValores()[j].getnombreVarConst()[0])) {
            	//Imprime el nombre o valor dependiendo si está asignado o no y es Variable
                if (afirmaciones[i].getValores()[j].getvalorVar() != "") {
                    std::cout << afirmaciones[i].getValores()[j].getvalorVar();
                }
                else {
                    std::cout << afirmaciones[i].getValores()[j].getnombreVarConst();
                }
            }
            else {
            	//Si no es variable simplemente imprime el nombre
                std::cout << afirmaciones[i].getValores()[j].getnombreVarConst();
            }
            if (j != afirmaciones[i].getValores().size() - 1) {
            	//Si no es el último valor de la afirmación imprime la coma
                std::cout << ",";
            }
            else {
            	//Si es el último valor de la afirmación imprime el parentesis de cierre
                std::cout << ")";
            }
        }
        if (i != afirmaciones.size() - 1) {
        	//Si no es la última afirmación imprime el or correspondiente
            std::cout << " | ";
        }
    }
}
