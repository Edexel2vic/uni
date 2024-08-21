#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME pipolv4

//tal vez añadir que si ya hay uno yendo a por una gema, no vaya otro a no ser que le cueste menos llegar (guardar 
//el size de los movimientos a la gema en un mapa, tal vez))
//****Cuando se busque gemas, una vez encontrada hacer como un bfs desde la gema que se ha encontrado y mirar si tiene
//una unidad diferente a la que la ha detectado más cerca (diferente de id, da igual si es otra unidad mia, asi de mis 
//jugadores no se acercaran a la vez nunca, probar si es worth detectar necromonger también en vez de huir)

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

    map<int, bool> gema_lock;
     
                               
  /**
   * Types and attributes for your player can be defined here.
   */

  /**
   * Play method, invoked once per each round.
   */
queue<Dir> necromonger_huir(Pos origen, int tam){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));

  while(!q.empty()){

    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();
    if(cell(res).id != -1){
      if(unit(cell(res).id).type == Necromonger){
        return actual; 
      }
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      if(n.i < 40 && n.i > -1 && actual.size() < tam && cell(n).type != Rock && visitados.find(n) == visitados.end()){
        if(visitados.find(n) == visitados.end()){
          visitados.insert(n);
          queue<Dir> actualv2 = actual;
          actualv2.push(Dir(i));
          q.push(make_pair(n, actualv2));
        }
      }
    }
  }

  return movimientos;


}

queue<Dir> caza_bfs(Pos origen, int id){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));

  while(!q.empty()){

    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();

    if(cell(res).id != -1){
      int un = cell(res).id;
      if(unit(un).player != me() && unit(un).health > 0 && unit(un).type == Pioneer){
        return actual;
      }
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      if(n.i < 40 && n.i > -1 && cell(n).type != Rock){
        if(cell(n).id != -1){
          if(unit(cell(n).id).player != me()){
            if(visitados.find(n) == visitados.end()){
              visitados.insert(n);
              queue<Dir> actualv2 = actual;
              actualv2.push(Dir(i));
              q.push(make_pair(n, actualv2));
            }
          }
        }
        else{
          if(visitados.find(n) == visitados.end()){
            visitados.insert(n);
            queue<Dir> actualv2 = actual;
            actualv2.push(Dir(i));
            q.push(make_pair(n, actualv2));
          }
        }
        
      }
    }
  }

  return movimientos;


}


queue<Dir> furyans_huir(Pos origen, int tam){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));

  while(!q.empty()){

    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();
    if(cell(res).id != -1){
      if(unit(cell(res).id).player != me() && unit(cell(res).id).health > 0 && unit(cell(res).id).type == Furyan){
        return actual; 
      }
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      if(n.i < 40 && n.i > -1 && actual.size() < tam && cell(n).type != Rock && visitados.find(n) == visitados.end()){
        if(visitados.find(n) == visitados.end()){
          visitados.insert(n);
          queue<Dir> actualv2 = actual;
          actualv2.push(Dir(i));
          q.push(make_pair(n, actualv2));
        }
      }
    }
  }

  return movimientos;


}

bool accesible(Pos origen, int moves, int id){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));
  while(!q.empty()){
    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();

    if(cell(res).id != -1){
        int unidad = cell(res).id;
        if(unit(unidad).type == Pioneer){
            if(unidad == id){
                return true;
            }

            else{
                return false;
            }
        }
      
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      int s = actual.size();
      if(n.i < 40 && n.i > -1 && !daylight(n) && s <= moves){
        if(visitados.find(n) == visitados.end()){
          visitados.insert(n);
          queue<Dir> actualv2 = actual;
          actualv2.push(Dir(i));
          q.push(make_pair(n, actualv2));
        }
      }
    }
  }
    
  return true;


}

queue<Dir> gema_cercana(Pos origen, int moves, int id){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));
  while(!q.empty()){
    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();
    if(cell(res).gem){
     int fin_sol = ((round()*2)%80) - 1;
     int dist;
     int s = actual.size();
     if(fin_sol > res.j) dist = (res.j + 79 - fin_sol)/2;
     else dist = (res.j - fin_sol)/2;

     if(dist > s + moves && accesible(res, s+moves, id)) return actual;
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      if(n.i < 40 && n.i > -1 && !daylight(n)){
        if(visitados.find(n) == visitados.end() && cell(n).id == -1){
          visitados.insert(n);
          queue<Dir> actualv2 = actual;
          actualv2.push(Dir(i));
          q.push(make_pair(n, actualv2));
        }
      }
    }
  }

  return movimientos;


}

void detectar_gema(int id){
  Pos p0 = unit(id).pos;
  queue<Dir> gema = gema_cercana(p0, 0, id);

  if(!gema.empty()){
    command(id, gema.front());
  }

  else{
    queue<Dir> ascensor = ascensor_cercano(p0, 21).first;
    if(cell(p0).type == Elevator) command(id, Down);
    else if(!ascensor.empty()) command(id, ascensor.front());

    else{
      if(unit(id).pos.i > 20) command(id, RT);
      else command(id, BR);
    }  
  }

}


bool posible(Pos p, Dir i){
  Pos comp = p + i;
  if(comp.i < 40 && comp.i > -1 && cell(comp).id == -1 && cell(comp).type != Rock) return true;
  return false;
}



void huir(int id, Dir i){
    Pos p = unit(id).pos;
    switch(i){   
        case Bottom:
            if(posible(p, Top)) command(id, Top);
            else if(posible(p, RT)) command(id, RT);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, Left)) command(id, Left);
            else if(posible(p, BR)) command(id, BR);
            else if(posible(p, LB)) command(id, LB);
            else command(id, Bottom);
            break;
        case BR:
            if(posible(p, TL)) command(id, TL);
            else if(posible(p, Left)) command(id, Left);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, RT)) command(id, RT);
            else if(posible(p, LB)) command(id, LB);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, Bottom)) command(id, Bottom);
            else command(id, BR);
            break;
        case LB:
            if(posible(p, RT)) command(id, RT);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, BR)) command(id, BR);
            else if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, Left)) command(id, Left);
            else command(id, LB);
            break;    
        case Right:
            if(posible(p, Left)) command(id, Left);
            else if(posible(p, LB)) command(id, LB);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, RT)) command(id, RT);
            else if(posible(p, BR)) command(id, BR);
            else command(id, Right);
            break;
        case RT:
            if(posible(p, LB)) command(id, LB);
            else if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, Left)) command(id, Left);
            else if(posible(p, BR)) command(id, BR);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, Right)) command(id, Right);
            else command(id, RT);
            break;
        case Left:
            if(posible(p, RT)) command(id, RT);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, BR)) command(id, BR);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, LB)) command(id, LB);
            else command(id, Left);
            break;
        case TL:
            if(posible(p, BR)) command(id, BR);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, RT)) command(id, RT);
            else if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, LB)) command(id, LB);
            else if(posible(p, Top)) command(id, Top);
            else if(posible(p, Left)) command(id, Left);
            else command(id, TL);
            break;
        case Top:
            if(posible(p, Bottom)) command(id, Bottom);
            else if(posible(p, BR)) command(id, BR);
            else if(posible(p, LB)) command(id, LB);
            else if(posible(p, Right)) command(id, Right);
            else if(posible(p, Left)) command(id, Left);
            else if(posible(p, TL)) command(id, TL);
            else if(posible(p, RT)) command(id, RT);
            else command(id, Top);
            break;
    }
    

}

pair<queue<Dir>, Pos> ascensor_cercano(Pos origen, int rango){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);

  q.push(make_pair(origen, movimientos));

  while(!q.empty()){
    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();
    if(cell(res).type == Elevator){
      if(origen.k){
        int fin_sol = ((round()*2)%80) - 1;
        int dist;
        int s = actual.size();
        if(fin_sol > res.j) dist = (res.j + 79 - fin_sol)/2;
        else dist = (res.j - fin_sol)/2;
        if(dist >= s) return make_pair(actual, res);
      }

      else return make_pair(actual, res);
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);

      if(n.i < 40 && n.i > -1 && !daylight(n) && cell(n).type != Rock && visitados.find(n) == visitados.end() && cell(n).id == -1 && actual.size() < rango){
        visitados.insert(n);
        queue<Dir> actualv2 = actual;
        actualv2.push(Dir(i));
        q.push(make_pair(n, actualv2));
      }

    }
  }

  return make_pair(movimientos, origen);
}

queue<Dir> hellhound_cercano(Pos origen){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);

  q.push(make_pair(origen, movimientos));

  while(!q.empty()){
    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();
    if(cell(res).id != -1 && unit(cell(res).id).type == Hellhound){
      return actual;
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);

      if(n.i < 40 && n.i > -1 && actual.size() < 6 && cell(n).type != Rock && visitados.find(n) == visitados.end()){
        visitados.insert(n);
        queue<Dir> actualv2 = actual;
        actualv2.push(Dir(i));
        q.push(make_pair(n, actualv2));
      }

    }
  }

  return movimientos;
}

queue<Dir> pintar_cerca(Pos origen){
  set<Pos> visitados;
  queue<pair<Pos, queue<Dir>>> q;
  queue<Dir> movimientos;
  visitados.insert(origen);
  q.push(make_pair(origen, movimientos));

  while(!q.empty()){
    Pos res = q.front().first;
    queue<Dir> actual = q.front().second;
    q.pop();

    if(cell(res).type == Cave && cell(res).owner != me()){
      return actual;
      break;
    }

    for(int i = 0; i < 8; ++i){
      Pos n = res + Dir(i);
      if(n.i < 40 && n.i > -1 && cell(n).id == -1 && actual.size() < 5 && cell(n).type != Rock && visitados.find(n) == visitados.end()){
        visitados.insert(n);
        queue<Dir> actualv2 = actual;
        actualv2.push(Dir(i));
        q.push(make_pair(n, actualv2));
      }

    }
  }

  return movimientos;
}


void move_pioneers() {
  vector<int> P = pioneers(me());
  for (int id : P) {
      Pos p = unit(id).pos;

      if(p.k){
        detectar_gema(id);
      }

      
      else{
        queue <Dir> hell = hellhound_cercano(p);
        queue <Dir> furys = furyans_huir(p, 3);
        
        if(!hell.empty() && hell.size() <= 3){
          huir(id, hell.front());
        }
       
        else if(!furys.empty() && furys.size() <= 2){
          huir(id, furys.front());
        }

        

        else{
          pair<queue <Dir>, Pos> ascensor = ascensor_cercano(p, 20);
          int moves = ascensor.first.size();
          Pos arriba1 = ascensor.second;
          arriba1.k = 1;
          if(!gema_cercana(arriba1, moves, id).empty()){
            if(cell(p).type == Elevator) command(id, Up);
            else{
              command(id, ascensor.first.front());
            }
          }
          else{
            queue<Dir> pint = pintar_cerca(p);
            if(!pint.empty()) command(id, pint.front());
            else command(id, ascensor.first.front());
            
          }
        }
       

        
      
    }
  }
}

void move_furyans() {
  vector<int> F = furyans(me());
  for(int id : F){
    Pos p = unit(id).pos;
    queue<Dir> r;
    queue<Dir> v = furyans_huir(p, 2);
    r = hellhound_cercano(p);

    if(!v.empty() && v.size() <= 2){
        Dir q = v.front();
        Pos enem = p;
        if(v.size() == 1) command(id, v.front());
        
        else{
        while(!v.empty()){
          enem = enem + v.front();
          v.pop();
        }
        
        if(unit(cell(enem).id).health > unit(id).health){
          huir(id, q);
        }
        else if(!r.empty() && r.size() <= 3){
          huir(id, r.front());
        }
        else{
            r = caza_bfs(p, id);
            if(!r.empty()){
              command(id, r.front());
            }
            else{
              command(id, Dir(random(0, 7)));
            }
        }
        }
    
    }

    else if(!r.empty() && r.size() <= 3){
      huir(id, r.front());
    }
    

    else{

        r = caza_bfs(p, id);
        if(!r.empty()){
          command(id, r.front());
        }
        else{
          command(id, Dir(0));
        }
    }
  }


}




  virtual void play () {
    move_furyans();
    move_pioneers();
    
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);