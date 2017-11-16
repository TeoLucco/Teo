#define timePerAnsw 50000
#define maxQuestionsPerEx 10
#define maxAnswersPerEx 4
#define maxScenariosPerEx 4
#define GamesNumber 4

int currentQuestionI = 0;
int currentCorrectAnswers = 0;
//int currentAnswers = 0;
bool currentGivenAnswers[N_HEAD_SENSORS]  = {false, false, false, false};

struct Scenario {
  int n_questions;
  int audio_indices[maxQuestionsPerEx] ;
  bool correct_answers[maxQuestionsPerEx][N_HEAD_SENSORS];
  int n_answers[maxQuestionsPerEx];
  unsigned long int questionResponseTime[maxQuestionsPerEx];
  boolean givenAnswersReport[maxQuestionsPerEx];

  String Id;
};

typedef struct Scenario t_Scenario;

struct Game {
  int n_senarios;
  t_Scenario scenarios[maxScenariosPerEx];
};

typedef struct Game t_Game;

t_Game games[] =
{ { // game 0
    2, //numero scenari
    {
      { // scenario 0
        5,//Number questions
        {    10,          11,       12,        13,        14},
        { {true, false, false, false} , {true, false, true, false}, {true, false, true, true}, {true, false, false, true}, {true, false, true, true} },
        {     0,          0,        0,          0,        0       },
        {     0,          0,        0,          0,        0       },
        {   false,      false,    false,      false,    false     },
        "SCENARIO1"
      },
      { // scenario 1
        5,
        {    10,          11,       12,        13,        14},
        { {true, false, true, false} , {true, true, true, false}, {true, false, true, false}, {true, false, false, false}, {true, false, true, false} },
        {     0,          0,        0,          0,        0       },
        {     0,          0,        0,          0,        0       },
        {   false,      false,    false,      false,    false     },
        "SCENARIO2"
      }
    }
  },
  { 1,//numero scenari gioco 2
    { 5,
      {    10,          11,       12,        13,        14},
      { {true, false, false, false} , {true, false, true, false}, {true, false, true, true}, {true, false, false, true}, {true, false, true, true} },
      {     0,          0,        0,          0,        0       },
      {     0,          0,        0,          0,        0       },
      {   false,      false,    false,      false,    false     },
      "SCENARIO1"
    }
  }
}
;


int answerTillNow[maxAnswersPerEx];
int answerI = 0;

//struct Scenario & getCurrentScenario() {
//  t_Game & g = games[currentGameI].scenarios[currentScenarioI];
//  t_Scenario & s = games[currentGameI].scenarios[currentScenarioI];
//  return s;
//}

void setupScenario(struct Scenario &s) {
  //Serial3.println(s.Id);
  for (int i = 0; i < s.n_questions; i++) {
    s.questionResponseTime[i] = 5;
    s.givenAnswersReport[i] = true;
    for (int j = 0; j < N_HEAD_SENSORS; j++) {
      if (s.correct_answers[i][j]) {
        games[currentGameI].scenarios[currentScenarioI].n_answers[i]++;
      }
    }
    //Serial3.println( s.n_answers[i]);
  }
}

void settingGame() {
  t_Scenario &s = games[currentGameI].scenarios[currentScenarioI];
  setupScenario(s);
  currentQuestionI = 0;
  gameState = make_question;
}

void makeQuestion() {
  t_Scenario &s = games[currentGameI].scenarios[currentScenarioI];
  Serial3.print("currentQuestionI:  "); Serial3.println(currentQuestionI);
  //  Serial3.print("s.n_questions: "); Serial3.println(s.n_questions);
  if (currentQuestionI < s.n_questions) {                                   //se devo fare ancora domande
    //    Serial3.println("currentQuestionI < s.n_questions");
    Serial3.print("played audio: "); Serial3.println(s.audio_indices[currentQuestionI]);
    playS(s.audio_indices[currentQuestionI]);                                 //faccio la domanda I
    //    Serial3.println("Reset del vettore risposte fino ad ora");
    //    Serial3.println("currentGivenAnswers: ");
    for (int i = 0; i < N_HEAD_SENSORS; i++) {
      currentGivenAnswers[i] = false;                                         //azzero il vettore delle risposte date fino ad ora
      //      Serial3.print(currentGivenAnswers[i]);Serial3.print(" ");
    }
    //    Serial3.println();
    currentCorrectAnswers = 0;                                               //azzero il contatore delle risposte corrette date fino ad ora
    gameState = wait_answer;                                                  //vado in stato wait answer
    startWaitingTime = millis();
    Serial3.println("startWaitingTime: " + String(startWaitingTime));
    Serial3.println("wait answer begin");
  } else {                                                                   //se invece ho finito le domande
    gameState = end_game;                                                     //vado a stato end game
  }
}

void waitAnswer() {
  if (workingCapacitives != head) {
    //Serial3.println("Attivo capacitivi Testa");
    //CapacitivesUpdate(head);
  }
  t_Scenario &s = games[currentGameI].scenarios[currentScenarioI];
  if (currentCorrectAnswers < s.n_answers[currentQuestionI]) {    //se ci sono ancora risposte da dare
    if (currentCorrectAnswers > 0) {                                //se ho già dato una risposta
      //Serial3.println("E poi???");                                    //say:e poi??
      timedPlayS(77, 15000);
    }
    if (pressedButton != -1) {                                        //se viene premuto un bottone
      int a = pressedButton;
      if (currentGivenAnswers[a]) {                                     //se il bottone è già stato premuto per questa domanda
        if (s.correct_answers[currentQuestionI][a]) {                     //se il bottone premuto è una risposta corretta
           headLedUpdate(green, color_wipe);                                     
        } else {                                                       
           headLedUpdate(red, color_wipe);                                                
          // wrong
        }   
        //Serial3.println("Bottone Gia premuto per questa domanda!");
        playS(18);                                                        // say: hai già dato questa risposta, provane un'altra
      } else {                                                          //se invece è la prima volta che prova un bottone
        //Serial3.println("Prima volta che premi il bottone!");
        currentGivenAnswers[a] = true;                                    //mi salvo il bottone che ha premuto
        if (s.correct_answers[currentQuestionI][a]) {                     //se il bottone premuto è una risposta corretta
          //Serial3.print("s.correct_answers[currentQuestionI][a]: ");
          Serial3.println("risposta corretta");                             // correct
          currentCorrectAnswers++;                                          //incremento il contatore delle risposte corrette
          rightAnswer();                                                    //eseguo movimento right answer
        } else {                                                          //se invece la riposta è sbagliata
          Serial3.println("risposta scorretta");
          s.givenAnswersReport[currentQuestionI] = false;
          wrongAnswer();                                                    //say risposta sbagliata  ed esegui movimento
          // wrong
        }
        gameState = mov;                //TODO da testare############################################################
      }

    }
  } else {                                                        //se non ci sono più risposte da dare
    s.questionResponseTime[currentQuestionI] = (millis() - startWaitingTime);
    Serial3.println("Millis: " + String(millis()));
    Serial3.println("startWaitingTime: " + String(startWaitingTime));
    Serial3.println("Response Time question number " + String(currentQuestionI) + ": " + String(s.questionResponseTime[currentQuestionI]));
    Serial3.println("Report question number " + String(currentQuestionI) + ": " + String(s.givenAnswersReport[currentQuestionI]));
    
    currentQuestionI++;                                                                       //incrementa l'indice della domanda
    gameState = make_question;                                                                //vado in stato make question
    Serial3.println("wait answer end");
  }
}

void rightAnswer() {
  int randNumber = rand() % 4 + make_happy0;
  //Serial3.println("Disattivo Capacitivi");
  previousWorkingCapacitives = workingCapacitives;
  if (interpreterState == game_modality)workingCapacitives = noOne;
  else if (interpreterState == fam_modality)workingCapacitives = body;
  if (dir == 1)
    playS(4);
  else
    playS(5);
  dir = rand() % 2;
//  setLedTimer(2000);
  bodyLedUpdate(color_wipe, greenC,2000);
  headLedUpdate(green, color_wipe);
  startMovement(randNumber); //at the end of makeHappy function there's the gameStateChange
}
void wrongAnswer() {
  //Serial3.println("Disattivo Capacitivi");
  previousWorkingCapacitives = workingCapacitives;
  if (interpreterState == game_modality)workingCapacitives = noOne;
  else if (interpreterState == fam_modality)workingCapacitives = body;
  if (dir == 1)
    playS(6);
  else
    playS(7);
  dir = rand() % 2;
  bodyLedUpdate(color_wipe, redC,2000);
  headLedUpdate(red, color_wipe);
  startMovement(make_sad1);
}

void endGame() {
  Serial3.print("End Game");
  int nRightAns = countResult();
  if (nRightAns <= 2)
    playS(18);
  else if (nRightAns > 2 && nRightAns <= 5)
    playS(19);
  else if (nRightAns > 5 && nRightAns <= 8)
    playS(20);
  else if (nRightAns > 9 && nRightAns <= 10)
    playS(1); // gioco finito
  gameState = no_game;
  interpreterState = choose_modality;

  Serial3.print("Correct answers number:"); Serial3.println(nRightAns);
  t_Scenario &s = games[currentGameI].scenarios[currentScenarioI];
  
  Serial3.println("*G");
  for (int i = 0; i < s.n_questions - 1; i++) {
    Serial3.print(s.questionResponseTime[i]); Serial3.print(",");
  }
  Serial3.print(s.questionResponseTime[s.n_questions - 1]); Serial3.println("*");
  
  Serial3.println("*H");
  for (int i = 0; i < s.n_questions - 1; i++) {
    if (s.givenAnswersReport[i]) Serial3.print("t");
    else Serial3.print("f");
    Serial3.print(",");
  }
  if (s.givenAnswersReport[s.n_questions - 1]) Serial3.print("t");
  else Serial3.print("f");
  Serial3.println("*");

}

int countResult() {
  int n = 0;
  t_Scenario s = games[currentGameI].scenarios[currentScenarioI];
  for (int i = 0; i < maxQuestionsPerEx; i++) {
    if (s.givenAnswersReport[i])n++;
  }
  return n;
  //  int n = 0;
  //  for (int i = 0; i < maxQuestionsPerEx; i++) {
  //    if (questionResult[i]) n++;
  //    Serial3.print(questionResult[i]);
  //    if (i != maxQuestionsPerEx - 1) Serial3.print(",");
  //    else Serial3.print(";");
  //  }
  //  return n;
}
boolean color_Game = false;

void colorGameSetup() {
  if (!color_Game) {
    color_Game = true;
    int color = rand() % 4;
    playS(DOMANDA_COLORE_LED_AUDIO);
    switch (color) {
      case 0: headLedUpdate(red, color_wipe); break;
      case 1: headLedUpdate(green, color_wipe); break;
      case 2: headLedUpdate(blue, color_wipe); break;
      case 3: headLedUpdate(yellow, color_wipe); break;
    }
    buttonToTouch = color;
    //CapacitivesUpdate(head);
  }
}

void gameModality() {
  switch (gameState) {
    case no_game:         break;
    case setting:         settingGame(); break;
    case make_question:   makeQuestion(); break;
    case wait_answer:     waitAnswer(); break;
    case mov:             break;
    case end_game:        endGame(); break;
  }
}


