void settingGame(){
  playI=gameNumber*10;
  gameState=make_question;
  repeatQuestionCounter=0;
  questionI=0;
}

void makeQuestion(){
  if(questionI<questionsPerEx){
    playS(playI);
    startWaitingTime=millis();
    pressedButton=-1;
    gameState=wait_answer;
  }else gameState=end_game;
}

void waitAnswer(){
  if(pressedButton==gameAnswers[gameNumber][questionI]){
    gameState=right_answer;
    questionResult[questionI]=true;
    questionTime[questionI]=(millis()-startWaitingTime)/1000;
    tries[questionI]=repeatQuestionCounter;
    repeatQuestionCounter=0;
    questionI++;
    playI++;
  }else
  if(pressedButton!=gameAnswers[gameNumber][questionI] && pressedButton!=-1){
    gameState=wrong_answer;
    questionResult[questionI]=false;
    questionTime[questionI]=(millis()-startWaitingTime)/1000;
    tries[questionI]=repeatQuestionCounter;
    
  }
  //if(millis()-startWaitingTime > timePerAnsw && repeatQuestionCounter<max_q_repeat){
  if(millis()-startWaitingTime > timePerAnsw){
    //play sound per dire che ripeterò la domanda
    gameState=make_question;
    repeatQuestionCounter++;
  }
//  if(millis()-startWaitingTime > timePerAnsw && repeatQuestionCounter==max_q_repeat){
//    //play sound per dire che ho superato il numero di tentativi e si passa alla prossima domanda
//    gameState=wrong_answer;
//    questionResult[questionI]=false;
//    questionTime[questionI]=0;
//    tries[questionI]=repeatQuestionCounter+1;
//    repeatQuestionCounter=0;
//    questionI++;     
//  }
}

void rightAnswer(){
  int randNumber=rand()%4+make_happy0;
  gameState=mov;
  //if(dir==1)
  playS(4);
  //else
  //playS(5);
  dir=rand()%2;
  startMovement(randNumber); //at the end of makeHappy function there's the gameStateChange
}
void wrongAnswer(){
  //if(dir==1)
  playS(6);
  //else
  //playS(7);
  dir=rand()%2;
  gameState=mov;
  startMovement(make_sad1);
}

void endGame(){
  int nRightAns=countResult();
  if(nRightAns<=2)
  playS(18);
  else if(nRightAns>2 && nRightAns<=5)
  playS(19);
  else if(nRightAns>5 && nRightAns<=8)
  playS(20);
  else if(nRightAns>9 && nRightAns<=10)
  playS(21);
  gameState=no_game;
  interpreterState=choose_modality;
}

int countResult(){
  int n=0;
  for(int i=0;i<questionsPerEx;i++){
    if(questionResult[i]) n++;
    Serial3.print(questionResult[i]); 
    if(i!=questionsPerEx-1) Serial3.print(",");
    else Serial3.print(";");
  }
  return n;
}

void gameModality(){
  switch(gameState){
    case setting:         settingGame();break;
    case make_question:   makeQuestion();break;
    case wait_answer:     waitAnswer();  break;
    case right_answer:    rightAnswer();break;
    case wrong_answer:    wrongAnswer();break;
    case end_game:        endGame();break;
    case mov:             break;
  }
}

