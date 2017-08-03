void resetButtons(){
  triangolo=false;
  quadrato=false;
  cerchio=false;
  croce=false;
  startbutton=false;
  select=false;
}

void FromRobotToApp(){
  if (Serial.available()){
    b =  Serial.read();
    // mirror the commands back to the serial monitor
    // makes it easy to follow the commands
    Serial.write(b);
    Serial3.write(b);
  }
}

void famMod(){
  if (Serial3.available()){
    b = Serial3.read();
    switch (b){
      case '0':
        //Serial.println("0");
        resetButtons();
        if(actual_movement==no_movement && prec_movement!=follow && prec_movement!=autonomous_movement){
          triskar.stop2();
          move=false;
        }else if(actual_movement==no_movement && prec_movement==follow){
          actual_movement=follow;
          prec_movement=no_movement;
        }
        else if(actual_movement==no_movement && prec_movement==autonomous_movement){
          actual_movement=autonomous_movement;
          prec_movement=no_movement;
        }
        break;

      case '1':
        Serial.println("1-SU");
        if(!veryclose_front_obstacle && actual_movement!=dontwonna){
          move=true;
          triskar.run(speed_trg,0.0);
        }else if(veryclose_front_obstacle && actual_movement!=dontwonna)  startMovement(dontwonna);
        break;
      case '2':
        Serial.println("2-GIU'");
        if(!veryclose_back_obstacle && actual_movement!=dontwonna){
          move=true;
          triskar.run(-speed_trg,0.0);
        }else if(veryclose_back_obstacle && actual_movement!=dontwonna)  startMovement(dontwonna);
        break;

      case '3':
        Serial.println("3-SINISTRA");
        move=true;
        if(actual_movement==no_movement){
          triskar.run(0.0,speed_trg/robot_radius);  
        }else if(actual_movement==autonomous_movement){
          prec_movement=autonomous_movement;
          actual_movement=no_movement;
          triskar.run(0.0,speed_trg/robot_radius);  
        }else if(actual_movement==follow){
          prec_movement=follow;
          actual_movement=no_movement;
          triskar.run(0.0,speed_trg/robot_radius);  
        }
        break;

      case '4':
        Serial.println("4-DESTRA");
        move=true;
        if(actual_movement==no_movement){
          triskar.run(0.0,-speed_trg/robot_radius);  
        }else if(actual_movement==autonomous_movement){
          prec_movement=autonomous_movement;
          actual_movement=no_movement;
          triskar.run(0.0,-speed_trg/robot_radius);  
        }else if(actual_movement==follow){
          prec_movement=follow;
          actual_movement=no_movement;
          triskar.run(0.0,-speed_trg/robot_radius);  
        }
        break;
        //Code when RIGHT key is pressed

      case '5':
        Serial.println("5-SELECT");
        if(!select){
          select=true;
          speed_trg -= 3;
          if(speed_trg<0)     speed_trg=0;
          Serial.print("Speed trg:  ");
          Serial.println(speed_trg);
        }
        break;
        
      case '6':
        Serial.println("6-START");
        if(!startbutton){
          startbutton=true;
          speed_trg += 3;
          if(speed_trg>400)   speed_trg=400;
          Serial.print("Speed trg:  ");
          Serial.println(speed_trg);
        }
        break;
        
      case '7':
        Serial.println("7-TRIANGOLO");
        if(!triangolo){
          triangolo=true;
          //startMovement(make_happy[2]);
   
            //CODICE PER ATTIVARE FOLLOWING
            //startMovement(follow);
            //actual_obstacle=none;
            //last_obstacle=none;
            
          //CODICE PER ATTIVARE AUTONOMOUS MOVEMENT
          if(actual_movement==no_movement || actual_movement==follow){
            Serial.println("start Autonomous Movement");
            startMovement(autonomous_movement);
          }else{
            Serial.println("start Following");
            stopMovement();
            startMovement(follow);
            actual_obstacle=none;
            last_obstacle=none;
          }
          
        }
        break;
      case '8':
        Serial.println("8-QUADRATO");
        if(!quadrato){
          quadrato=true;
          startMovement(next_movement);
          if(next_movement<make_sad1)
            next_movement++;
          else next_movement=make_circle;
        }
        break;
        
      case '9':
        Serial.println("9-X");
        if(!croce){
          croce=true;
          startMovement(make_happy2);
          //startMovement(makeCircle);
          //startMovement(angrymov);
        }
        break;
      case 'A':
        Serial.println("A-CERCHIO");
        if(!cerchio){
          cerchio=true;
          //startMovement(make_happy[3]);
          //CODICE PER DISATTIVARE FOLLOWING
          stopMovement();
          prec_movement=no_movement;
          last_obstacle=none;
          
          //CODICE PER DISATTIVARE AUTONOMOUS MOVEMENT
         /* autonomous_movement=false;
          move=false;
          aut=false;*/
        }
        break;
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}
void chooseModality(){
  if (Serial3.available()){
    b = Serial3.read();
    switch (b){
      case '0':
          resetButtons();
          break;
        
      case '7':
        Serial.println("7-TRIANGOLO");
        if(!triangolo){
          triangolo=true;
          interpreterState=choose_game;
          playS(02);
        }
        break;
      case '8':
        Serial.println("8-QUADRATO");
        if(!quadrato){
          quadrato=true;
          interpreterState=fam_modality;
        }
        break;
        
      default:
        break;
    }

  }
  FromRobotToApp();
}
void chooseGame(){
  if (Serial3.available()){
    b = Serial3.read();
    switch (b){
      case '0':
          resetButtons();
          break;
        
      case '7':
        Serial.println("7-TRIANGOLO");
        if(!triangolo){
          triangolo=true;
          interpreterState=sg_waiting;
          gameNumber=colorGame;
          playS(03);
        }
        break;
      case '8':
        Serial.println("8-QUADRATO");
        if(!quadrato){
          quadrato=true;
          interpreterState=sg_waiting;
          gameNumber=animalGame;
          playS(03);
        }
        break;
        
      case '9':
        Serial.println("9-X");
        if(!croce){
          croce=true;
          interpreterState=sg_waiting;
        }
        break;
      case 'A':
        Serial.println("A-CERCHIO");
        if(!cerchio){
          cerchio=true;
          interpreterState=sg_waiting;
        }
        break;
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp();
}

void sgWaiting(){
  if (Serial3.available()){
    b = Serial3.read();
    switch (b){
      case '0':
          resetButtons();
          break;
            
      case '9':
        Serial.println("9-X");
        if(!croce){
          croce=true;
          interpreterState=game_modality;
          gameState=setting;
        }
        break;
      
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp(); 
}

void gameMod(){
  if (Serial3.available()){
    b = Serial3.read();
    switch (b){
      case '0':
          resetButtons();
          break;
      
      default:
        // default code (should never run)
        break;
    }

  }
  FromRobotToApp(); 
}

void disCharge(){
  FromRobotToApp();
}

void btInterpreter(){
  switch(interpreterState){
    case choose_modality: chooseModality();break;
    case choose_game:     chooseGame();break;
    case sg_waiting:      sgWaiting();break;
    case game_modality:   gameMod();break;
    case fam_modality:    famMod();break;
    case discharge:       disCharge();break;
  }
}


