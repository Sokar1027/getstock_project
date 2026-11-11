#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const int DIAS = 5;          
const int MAX_PRODUCTOS = 100;

    struct Producto {
        int codigo;
        string nombre;
        string categoria;
        int precio_unitario;
        int cantidad_disponible;
        int cantidad_min;
    };


// funcion de lectura
int cargar_productos(const char * nombre_Archivo, Producto inventario[], int ventas[][DIAS+1]);

// Funciones principales
void registro_ventas_x_dia(Producto inventario[], int n_productos, int ventas[][DIAS + 1]);
void control_existencias(Producto inventario[], int n_productos, int indice[]);
void consultas(Producto inventario[], int n);

// Funcionalidades de la funcion registro_ventas_x_dia
int registrar_venta(Producto inventario[], int n_productos, int ventas[][DIAS + 1]);
void analisis_matriz_ventas(Producto inventario[], int n_productos, int ventas[][DIAS + 1]);

// Funcionalidades de la funcion analisis_matriz_ventas
int total_unidades_vendidas_x_producto(int ventas[][DIAS + 1], int filas);
int total_ventas_x_dia(Producto inventario[], int n_productos, int ventas[][DIAS+1], int dia);
int dia_mayor_cantidad_unidades_vendidas(int ventas[][DIAS+1], int n_productos);
int producto_mayor_cantidad_unidades_vendidas(int ventas[][DIAS+1], int n_productos);

//funcionalidades de la funcion control_existencias
int productos_bajo_inventario(Producto inventario[], int n, int indice[]);
int valor_total_inventario(Producto inventario[], int n_productos);

// funcionalidades de la funcion consultas
void mostrar_productos(Producto inventario[], int n_productos);
int buscar_producto(Producto inventario[], int n_productos);

bool generar_reporte(Producto inventario[], int n_productos, int ventas[][DIAS+1], int indice[]);
bool actualizar_archivo(const char * nombre_Archivo, Producto inventario[], int n_productos);


int main(){

    int n_productos, opcion;

    Producto inventario[MAX_PRODUCTOS];
    int ventas[MAX_PRODUCTOS][DIAS+1]={0}, indice[MAX_PRODUCTOS];

    n_productos = cargar_productos("productos.txt", inventario, ventas);

    do{
        cout << "GESTOCK - Menu principal" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  1. Registro de ventas por dia" << endl;
        cout << "  2. Control de existencias" << endl;
        cout << "  3. Consultas" << endl;
        cout << "  4. Generar reporte" << endl;
        cout << "  5. Salir" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  Seleccione una opcion : ";
        cin >> opcion;

        switch(opcion){
            case 1:{
                registro_ventas_x_dia(inventario, n_productos, ventas);
                break;
            }
            case 2:{
                control_existencias(inventario, n_productos, indice);
                break;
            }
            case 3:{
                consultas(inventario, n_productos);
                break;
            }
            case 4:{
                bool generado;
                generado = generar_reporte(inventario, n_productos, ventas, indice);
                cout<<generado;
                break;
            }
            case 5:{
                actualizar_archivo("productos.txt", inventario, n_productos);
                break;
            }
            default:{
                    cout<<"Opcion invalida, intente de nuevo."<<endl;}    
        }
    }while(opcion != 5);

}
// funcion lectura de archivo
int cargar_productos(const char * nombre_Archivo, Producto inventario[], int ventas[][DIAS+1]){

    string codigo, precio_unitario, cantidad_disponible, cantidad_min;
    int contador=0;

    ifstream in;

    in.open(nombre_Archivo);

    if(in.is_open()){
        while(getline(in, codigo, ',')){
            
            getline(in , inventario[contador].nombre, ',');
            getline(in , inventario[contador].categoria, ',');
            getline(in , precio_unitario, ',');
            getline(in , cantidad_disponible, ',');
            getline(in , cantidad_min);
            inventario[contador].codigo = stoi(codigo);
            inventario[contador].precio_unitario = stoi(precio_unitario);
            inventario[contador].cantidad_disponible = stoi(cantidad_disponible);
            inventario[contador].cantidad_min = stoi(cantidad_min);

            ventas[contador][0] = inventario[contador].codigo;

            contador ++;
        }
    }
    else{
        cout<<"No se encontro el archivo."<<endl;
        return -1;
    }

    in.close();

    cout<<"Se cargaron correctamente "<<contador<<" registros."<<endl;
    return contador;
}


// Funcionalidades de la funcion registro_ventas_x_dia
// Submenu de ventas: registrar venta o analizar la matriz
void registro_ventas_x_dia(Producto inventario[], int n_productos, int ventas[][DIAS + 1]){

    int opcion;

    do{

        cout<< "Registro de ventas por dia:" << endl;
        cout<<"--------------------------------------------------"<<endl;
        cout<< "  1. Registrar venta" << endl;
        cout<< "  2. Analizar matriz de ventas" << endl;
        cout<< "  3. Volver al menu principal" << endl;
        cout<<"--------------------------------------------------"<<endl;
        cout<<"Seleccione una opcion: ";
        cin>>opcion;
        cout<<endl;

        switch(opcion){
            case 1: 
                int valor_pagar;
                valor_pagar = registrar_venta(inventario, n_productos, ventas);
                break;
            case 2:
                analisis_matriz_ventas(inventario, n_productos, ventas);
                break;
            case 3:
                break;
            default:
                cout<<"Opcion invalida, intente de nuevo."<<endl;   
        }

    }while(opcion != 3);



   

}

int registrar_venta(Producto inventario[], int n_productos, int ventas[][DIAS + 1]){

    int codigo, cantidad, dia, valor_pagar, pos;
    cout << "Ingrese los datos de la compra:" << endl;
    cout << "--------------------------------------------------" << endl;
    cout << "  Codigo del producto : ";
    cin >> codigo;
    cout << "  Cantidad            : ";
    cin >> cantidad;
    cout << "  Dia                 : ";
    cin>>dia;
    cout << "--------------------------------------------------" << endl;

    pos = codigo-101;


    if(inventario[pos].cantidad_disponible >= cantidad){
        valor_pagar = cantidad * inventario[pos].precio_unitario;
        inventario[pos].cantidad_disponible = inventario[pos].cantidad_disponible - cantidad;
        ventas[pos][dia] += cantidad;

        cout<<"El valor a pagar es de: "<<valor_pagar;
        return valor_pagar;
    }
    else{
        cout<<"No hay disponibilidad.";
        return -1;
    }
}

 // Submenu de analisis: total de unidades vendidas por producto, total de ventas por dia, dia de mayor cantidad de unidades vendidas, producto de mayor cantidad de unidades vendidas
void analisis_matriz_ventas(Producto inventario[], int n_productos, int ventas[][DIAS + 1]){
    int opcion;

    do{
        cout << "Analisis de la matriz de ventas:" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  1. Total de unidades vendidas por producto" << endl;
        cout << "  2. Total de ventas por dia" << endl;
        cout << "  3. Dia con mayor cantidad de unidades vendidas" << endl;
        cout << "  4. Producto con mayor cantidad de unidades vendidas" << endl;
        cout << "  5. Volver" << endl;
        cout << "--------------------------------------------------" << endl;
        cout<<"Seleccione una opcion: ";
        cin >> opcion;
        cout<<endl;

        switch(opcion){
            case 1: {
                int unidades_vendidas_producto, codigo;
                cout<<"Digite el codigo del producto que desea saber la cantidad de unidades vendidas: ";
                cin>>codigo;
                cout<<endl;
                unidades_vendidas_producto = total_unidades_vendidas_x_producto(ventas, codigo);
                cout<<"La cantidad de unidades vendidas del producto "<<codigo<<" es de: "<<unidades_vendidas_producto<<endl;
                break;}
            case 2:{
                int dia, total_vendido_x_dia;
                cout<<"Digite el dia que desea saber la cantidad total vendida: ";
                cin>>dia;
                total_vendido_x_dia = total_ventas_x_dia(inventario, n_productos, ventas, dia);
                cout<<"la cantidad total vendida en el dia "<<dia<<" fue de: "<<total_vendido_x_dia;
                break;}
            case 3:{
                int dia_max;
                cout<<"En caso de que el dia de mayor cantidad de unidades vendidas sea 0, significa que no hubo ventas durante la semana."<<endl;
                dia_max = dia_mayor_cantidad_unidades_vendidas(ventas, n_productos);
                cout<<"El dia con mayor cantidad de unidades vendidas es: "<<dia_max<<endl;
                break;}
            case 4:{
                int producto_vendido;
                cout<<"En caso de que el codigo del producto con mayor cantidad de unidades vendidas sea -1, signigica no hubo ventas durante la semana."<<endl;
                producto_vendido = producto_mayor_cantidad_unidades_vendidas(ventas, n_productos);
                cout<<"El producto con mayor cantidad de unidades vendidas es: "<<producto_vendido<<endl;
                break;}
            case 5:{
                break;}
            default:{
                cout<<"Opcion invalida, intente de nuevo."<<endl;}
        }

    }while(opcion !=5 );
}

int total_unidades_vendidas_x_producto(int ventas[][DIAS + 1], int codigo){
    int total=0;
    for(int i=1; i<=DIAS;i++){
        total += ventas[codigo-101][i];
    }
    return total;
}

int total_ventas_x_dia(Producto inventario[], int n_productos, int ventas[][DIAS+1], int dia){
    int total = 0;

    for(int i=0; i<n_productos; i++){
        int cantidad_x_producto, precio_x_producto;
        cantidad_x_producto = ventas[i][dia];
        precio_x_producto = inventario[i].precio_unitario;
        total += cantidad_x_producto * precio_x_producto; 
    }

    return total;
}

int dia_mayor_cantidad_unidades_vendidas(int ventas[][DIAS+1], int n_productos){
    int dia_max=0, n_ventas_dia=0;

    for(int i=1; i<=DIAS; i++){
        int contador=0;
        for(int j=0; j<n_productos;j++){
            contador += ventas[j][i];
        }

        if(contador>n_ventas_dia){
            n_ventas_dia = contador;
            dia_max = i;
        }

    }

    return dia_max;
}

int producto_mayor_cantidad_unidades_vendidas(int ventas[][DIAS+1], int n_productos){
    int producto_max=0, n_ventas_producto=0;

    for(int i=0; i<n_productos; i++){
        int contador=0;
        for(int j=1; j<=DIAS;j++){
            contador += ventas[i][j];
        }

        if(contador>n_ventas_producto){
            n_ventas_producto = contador;
            producto_max = i;
        }
    }

    if(n_ventas_producto==0){
        return -1;
    }

    return producto_max+101;
}

// Funcionalidades control de existencia: productos bajo inventario y valor total de inventario
void control_existencias(Producto inventario[], int n_productos, int indice[]){
    int opcion;

    do{
        cout << "Control de existencias:" << endl;
        cout << "--------------------------------------------------" << endl;
        cout << "  1. Productos con bajo inventario" << endl;
        cout << "  2. Valor total del inventario" << endl;
        cout << "  3. Volver al menu principal" << endl;
        cout << "--------------------------------------------------" << endl;
        cout<<"Seleccione una opcion: ";
        cin>>opcion;
        cout<<endl;
        
        switch(opcion){
            case 1:{ 
                int n_productos_bajo_inventario;
                cout<<"La funcion rellenara el arreglo de productos bajo de inventario con los codigos de los productos, ordenados de manera ascendete y el numero de productos bajo inventario."<<endl;
                n_productos_bajo_inventario = productos_bajo_inventario(inventario, n_productos, indice);
                cout<<"El numero de productos bajos de inventario es de: "<<n_productos_bajo_inventario<<endl;
                break;}
            case 2:
                {int valor_inventario;
                valor_inventario =  valor_total_inventario(inventario, n_productos);
                cout<<"El valor total del inventario al momento de la carga del archivo es de: "<<valor_inventario<<endl;
                break;}
            case 3:{
                break;}
            default:
                cout<<"Opcion invalida, intente de nuevo."<<endl;
        } 
    }while(opcion != 3);

}

int productos_bajo_inventario(Producto inventario[], int n_productos, int indice[]){
    int contador = 0;
    for(int i=0; i<n_productos; i++){
        if(inventario[i].cantidad_disponible <= inventario[i].cantidad_min){
            indice[contador] = inventario[i].codigo;
            contador +=1;
        }
    }

    for(int i=0;i<contador;i++){
        int aux_producto;
        for(int j=1;j<contador-i;j++){

            if(inventario[(indice[j-1])-101].cantidad_disponible > inventario[(indice[j])-101].cantidad_disponible){
                aux_producto = indice[j-1];
                indice[j-1] = indice[j];
                indice[j] = aux_producto;
            }
        }
    }    

    return contador;

}

int valor_total_inventario(Producto inventario[], int n_productos){
    int total_inventario=0;
    for(int i=0; i<n_productos;i++){
       
        total_inventario += inventario[i].cantidad_disponible * inventario[i].precio_unitario;
    }

    return total_inventario;
}

// funcionalidades de consulta: mostrar los productos y buscar producto
void consultas(Producto inventario[], int n_productos){
    int opcion;
    do{
        cout<<endl;
        cout<<"Consultas:"<<endl;
        cout<<"--------------------------------------------------"<<endl;
        cout<<"  1. Mostrar productos"<<endl;
        cout<<"  2. Buscar producto"<<endl;
        cout<<"  3. Volver al menu principal"<<endl;
        cout<<"--------------------------------------------------"<<endl;
        cout<<"  Seleccione una opcion : ";
        cin>>opcion;
        cout<<endl;

        switch(opcion){
            case 1:{
                mostrar_productos(inventario, n_productos);
                break;
            }
            case 2:{
                cin.ignore(); //limpia el Enter que dejo cin >> opcion
                buscar_producto(inventario, n_productos);
                break;}
            case 3:{
                break;}
            default:{
                cout<<"Opcion invalida, intente de nuevo."<<endl;}
        } 

    }while(opcion != 3);

}

void mostrar_productos(Producto inventario[], int n_productos){
    cout<<left<< setw(10) << "Codigo"<< setw(25) << "Nombre"<< setw(20) << "Categoria"<< setw(12) << "Precio"<< setw(10) << "Cantidad" << endl;

    cout << string(77, '-') << endl;
    for(int i=0; i<n_productos;i++){
          cout<<left
             << setw(10) << inventario[i].codigo
             << setw(25) << inventario[i].nombre
             << setw(20) << inventario[i].categoria
             << setw(12) << inventario[i].precio_unitario
             << setw(10) << inventario[i].cantidad_disponible << endl;
    }
}
int buscar_producto(Producto inventario[], int n_productos){

    string nombre_producto;
    int pos_producto; 

    cout<<"Ingrese el nombre del producto que desea buscar: ";
    getline(cin, nombre_producto);

    for(int i=0; i<n_productos;i++){
        if(inventario[i].nombre == nombre_producto){
            cout<<left<< setw(10) << "Código"<< setw(25) << "Nombre"<< setw(20) << "Categoría"<< setw(12) << "Precio"<< setw(10) << "Cantidad" << endl;
            cout<<left
             << setw(10) << inventario[i].codigo
             << setw(25) << inventario[i].nombre
             << setw(20) << inventario[i].categoria
             << setw(12) << inventario[i].precio_unitario
             << setw(10) << inventario[i].cantidad_disponible << endl;
            
            pos_producto = i;

            return pos_producto;
        }
    }

    cout<<"El producto no fue encontrado.";
    return -1;
}

// Funciones de generamiento de reporte y actualizacion de archivo

bool generar_reporte(Producto inventario[], int n_productos, int ventas[][DIAS+1], int indice[]){

    ofstream out;

    int valor_total, producto_mas_vendido, dia_mas_ventas, n_productos_bajo_inventario;


    valor_total = valor_total_inventario(inventario, n_productos);
    producto_mas_vendido = producto_mayor_cantidad_unidades_vendidas(ventas, n_productos);
    dia_mas_ventas = dia_mayor_cantidad_unidades_vendidas(ventas, n_productos);
    n_productos_bajo_inventario = productos_bajo_inventario(inventario, n_productos, indice);


    out.open("reporte_inventario.txt");
    const string linea(50, '=');
    const string sub(50, '-');

    out << linea << endl;
    out << setw(33) << "REPORTE DE INVENTARIO" << endl;
    out << linea << endl << endl;

    out << left << fixed << setprecision(2);
    out << setw(28) << "Cantidad de productos:" << n_productos << endl;
    out << setw(28) << "Valor total inventario:" << "$" << valor_total << endl;
    out << setw(28) << "Producto mas vendido:" << producto_mas_vendido<< endl;
    out << setw(28) << "Dia con mas ventas:" << dia_mas_ventas << endl;

    out << sub << endl;
    out << "PRODUCTOS CON BAJO INVENTARIO (menor o igual a la cantidad minima)" << endl;
    out << sub << endl;
    out<<left<< setw(10) << "Codigo"<< setw(25) << "Nombre"<< setw(20) << "Categoria"<< setw(12) << "Precio"<< setw(10) << "Cantidad" << endl;


    for(int i=0; i<n_productos_bajo_inventario;i++){
        int pos = indice[i] - 101;
                out<<left
                << setw(10) << inventario[pos].codigo
                << setw(25) << inventario[pos].nombre
                << setw(20) << inventario[pos].categoria
                << setw(12) << inventario[pos].precio_unitario
                << setw(10) << inventario[pos].cantidad_disponible << endl;
            }

    cout<<"Se ha generado el reporte correctamente."<<endl;
    out.close();

    return true;
}    
bool actualizar_archivo(const char * nombre_Archivo, Producto inventario[], int n_productos){
    ofstream out;

    out.open(nombre_Archivo);

    for(int i=0; i<n_productos;i++){
        out<<inventario[i].codigo<<','<<inventario[i].nombre<<','<<inventario[i].categoria<<','<<inventario[i].precio_unitario<<','<<
        inventario[i].cantidad_disponible<<','<<inventario[i].cantidad_min<<endl;
    }

    return true;
}



