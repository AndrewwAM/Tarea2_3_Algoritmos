int bruteForce(string &s1, int idx1, string &s2, int idx2){
    /*
    Estamos editando el String X para llegar a Y
    Al encontrarnos en el elemento n, tenemos 4 casos:
    Xn , Yn   -> tenemos 4 opciones
        > borramos Xn.                  sub: idx1 + 1, idx2
        > insertamos b delante de Xn    sub: idx1, idx2 + 1
        > sustituimos X con b           sub: idx1 + 1, idx2 + 1 
        > transponemos Xn con X(n+1)    sub: idx1 + 2, idx2 + 2
    */
    
    if(s1.size() == 0 && s2.size() == 0){
        return 0;
    }
    int caso1, caso2, caso3, caso4;
    // Casos base
    if (idx1 >= s1.size()){
        // Solo podriamos insertar lo que resta de S2
        int costo_ins_total = 0;
        for (int i = idx2; i < s2.size(); i++){
            // Sumamos iterativamente
            costo_ins_total += costo_ins(s2[i]);
        }
        return costo_ins_total;
    } else if (idx2 >= s2.size()){
        // Solo podriamos borrar lo que resta de S1
        int costo_del_total = 0;
        for (int i = idx1; i < s1.size(); i++){
            costo_del_total += costo_del(s1[i]);
        }
        return costo_del_total;
    }


    // Caso borrar
    caso1 = costo_del(s1[idx1]) + bruteForce(s1, idx1 + 1, s2, idx2);

    // Caso insertar
    caso2 = costo_ins(s2[idx2]) + bruteForce(s1, idx1, s2, idx2 + 1);

    // Caso sustituir
    caso3 = costo_sub(s1[idx1], s2[idx2]) + bruteForce(s1, idx1 + 1, s2, idx2 + 1);

    // Caso transponer
    caso4 = INT_MAX;
    if((idx1 + 1 < s1.size() && idx2 + 1 < s2.size()) && (s1[idx1+1] == s2[idx2] && s2[idx1+1] == s1[idx1])){
        caso4 = costo_trans(s1[idx1], s1[idx1+1]) + bruteForce(s1, idx1 + 2, s2 , idx2 + 2);
    }

    result = min({caso1, caso2, caso3, caso4});
    return result;
}