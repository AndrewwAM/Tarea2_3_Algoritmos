void dynamic(string &s1, string &s2, matrix &cache){
    /*
    Estamos editando el String X para llegar a Y
    Al encontrarnos en el elemento n, tenemos 4 casos:
    Xn = Yn    ->   no hacemos nada, ya está listo.
    (a = b)
    Xn != Yn   -> tenemos 4 opciones
        > borramos Xn.                  sub: X(n-1), Yn
        > insertamos b delante de Xn    sub: Xn, Y(n-1)
        > sustituimos X con b           sub: X(n-1), Y(n-1) 
        > transponemos Xn con X(n-1)    sub: X(n-2), Y(n-2)
    
    Tabla caché:
        > coord 1: ultimo elemento de S1 que fue añadido a la solución
        > coord 2: ultimo elemento de S2 que fue añadido a la solución

    llenado de la tabla caché:
        casos base:
        > i == j == 0 (nada de nada) -> costo 0
        > i == 0 (no hay elementos en S1): -> solo se puede insertar
            > cache[i][j] = costo_insert(S2[j])
        > j == 0 (no hay elementos en S2): -> solo se puede borrar
            > cache[i][j] = costo_delete(S2[i])

        condición de recurrencia:
        > cache[i][j] min(caso1, caso2, caso3, caso4)
        > caso1 (borrar) -> seguimos evaluando S2[j], bajamos a S1[i-1]
            > caso1 = costo_borrar(S1[i]) + cache[i-1][j]
        > caso2 (insertar) -> seguimos evaluando S1[i], bajamos a S2[j-1]
            > caso2 = costo_insertar(S2[j]) + cache[i][j-1]
        > caso3 (sustituir) -> bajamos a S1[i-1] y S2[j-1]
            > caso3 = costo_sustituir(S1[i], S2[j]) + cache[i-1][j-1]
        > caso4 (transponer) -> bajamos a S1[i-2] y S2[j-2]
            > caso4 = costo_transponer(S1[i], S1[i-1]) + cache[i-2][j-2]
    */
    int caso1, caso2, caso3, caso4, i, j, a, b;
    for(int i = 0; i <= s1.size(); i++){
        a = i-1; // ajuste de indice
        for(int j = 0; j <= s2.size(); j++){
            b = j-1; // ajuste de indice
            if(i == 0 && j == 0){
                cache[i][j] = 0; 
                continue;
            }        
            if(i == 0){
                // no hay elementos en S1. j > 0
                cache[i][j] = costo_ins(s2[j-1]) * j;
                continue;
            } 
            if(j == 0){
                // no hay elementos en S2. i > 0
                cache[i][j] = costo_del(s1[i-1]) * i;
                continue;
            } 
            // Borrar
            caso1 = costo_del(s1[a]) + cache[i-1][j];
            // Insertar
            caso2 = costo_ins(s2[b]) + cache[i][j-1];
            // Sustituir
            caso3 = costo_sub(s1[a], s2[b]) + cache[i-1][j-1];
            // Transponer
            caso4 = INT_MAX;
            if((i > 1 && j > 1) && (s1[a] == s2[b-1] && s1[a-1] == s2[b])){
                caso4 = costo_trans(s1[a], s1[a-1]) + cache[i-2][j-2];
            }

            cache[i][j] = min({caso1, caso2, caso3, caso4});
        }
    }
    result = cache[s1.size()][s2.size()];
    return;
}


vector<string> solutionReconstructor(string &s1, string &s2){
    int optimo;
    int caso1, caso2, caso3, caso4;
    int i = s1.size();
    int j = s2.size();
    int a, b;
    int casoMin; // 1-4
    int costoMin;
    // cout << "s1: " << s1 << " s2: " << s2 << endl;
    vector<string> pasos;
    while(true){
        /*
        pregunta: de las cuatro opciones, cual me dió el optimo?
        */
        a = i - 1;
        b = j - 1;
        if (i == 0 && j == 0) casoMin = -1;
        else if (i == 0) casoMin = 2; 
        else if (j == 0) casoMin = 1;
        else {
            optimo = cache[i][j];
            casoMin = -1;
            // Borrar
            caso1 = costo_del(s1[a]) + cache[i-1][j];
            if(caso1 == optimo){
                casoMin = 1;
            } 
            // Insertar
            caso2 = costo_ins(s2[b]) + cache[i][j-1];
            if(caso2 == optimo){
                casoMin = 2;
            } 
            // Sustituir
            caso3 = costo_sub(s1[a], s2[b]) + cache[i-1][j-1];
            if(caso3 == optimo){
                casoMin = 3; 
            } 
            // Transponer
            caso4 = INT_MAX;
            if((i > 1 && j > 1) && (s1[a] == s2[b-1] && s1[a-1] == s2[b])){
                caso4 = costo_trans(s1[a], s1[a-1]) + cache[i-2][j-2];
            }
            if(caso4 == optimo){
                casoMin = 4;
            }
        }


        // cout << "i: " << i << " j: " << j << endl;
        string paso = "";
        switch (casoMin)
        {
        case 1:
            // Borrar fue el óptimo
            // cout << "caso1: " << caso1 << endl;
            paso = "Borrar ";
            paso += s1[a];
            paso += " (pos " + to_string(a);
            paso += ")";
            paso += " costo: ";
            paso += to_string(costo_del(s1[a]));
            pasos.push_back(paso);
            i--;
            break;
        
        case 2:
            // Insertar fue el óptimo
            // cout << "caso2: " << caso2 << endl;
            paso = "Insertar ";
            paso += s2[b];
            paso += " (pos " + to_string(a);
            paso += ")";
            paso += " costo: ";
            paso += to_string(costo_ins(s2[b]));
            pasos.push_back(paso);
            j--;
            break;
        case 3:
            // Sustituir fue el óptimo
            // cout << "caso3: " << caso3 << endl;
            paso = "Sustituir ";
            paso += s1[a];
            paso += " -> ";
            paso += + s2[b];
            paso += " costo: ";
            paso += to_string(costo_sub(s1[a], s2[b]));
            i--;
            j--;
            if(s1[a] == s2[b]) break;
            pasos.push_back(paso);
            break;
        case 4:
            // Tranponer fue el óptimo
            // cout << "caso4: " << caso4 << endl;
            paso = "Transponer ";
            paso += s1[a];
            paso += " con ";
            paso += s1[a-1];
            paso += " costo: ";
            paso += to_string(costo_trans(s1[a], s2[b]));
            pasos.push_back(paso);
            i = i-2;
            j = j-2;
            break;
        default:
            // cout << "default: " << casoMin << endl;
            paso = "whoops";
            i--;
            j--;
        }

        // cout << paso << endl;
        if(j < 0 || i < 0){
            break;
        }
    }
    cout << "\nPasos: " << endl;
    for(int i = pasos.size()-1; i >= 0; i--){
        cout << pasos[i] << endl;
    }
    return pasos;
}