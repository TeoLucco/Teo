#define timePerAnsw 50000
#define maxQuestionsPerEx 10
#define maxAnswersPerEx 4
#define maxScenariosPerEx 4
#define GamesNumber 4

int currentQuestionI = 0;
int currentNumberAnswered = 0;
bool currentGivenAnswers[N_HEAD_SENSORS]  = {false, false, false, false};

struct Scenario {
  int n_questions;
  int audio_indices[maxQuestionsPerEx] ;
  bool correct_answers[maxQuestionsPerEx][N_HEAD_SENSORS];
  int n_answers[maxQuestionsPerEx];
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
        "SCENARIO1"
      },
      { // scenario 1
        5,
        {    10,          11,       12,        13,        14},
        { {true, false, true, false} , {true, true, true, false}, {true, false, true, false}, {true, false, false, false}, {true, false, true, false} },
        {     0,          0,        0,          0,        0       },
        "SCENARIO2"
      }
    }
  },
  { 1,//numero scenari gioco 2
    { 5,
      {    10,          11,       12,        13,        14},
      { {true, false, false, false} , {true, false, true, false}, {true, false, true, true}, {true, false, false, true}, {true, false, true, true} },
      {     0,          0,        0,          0,        0       },
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
  Serial3.println(s.Id);
  for (int i = 0; i < s.n_questions; i++) {
    for (int j = 0; j < N_HEAD_SENSORS; j++) {
      if (s.correct_answers[i][j]) {
        games[currentGameI].scenarios[currentScenarioI].n_answers[i]++;
      }
    }
    Serial3.println( s.n_answers[i]);
  }
}

void settingGame() {
  t_Scenario s = games[currentGameI].scenarios[currentScenarioI];
  setupScenario(s);
  currentQuestionI = 0;
  gameState = make_question;
}

void makeQuestion() {
  t_Scenario s = games[currentGameI].scenarios[currentScenarioI];
  Serial3.print("currentQuestionI:  " + currentQuestionI);
  Serial3.print("s.n_questions: " + s.n_questions);
  if (currentQuestionI < s.n_questions) {                                   //se devo fare ancora domande
    Serial3.println("currentQuestionI < s.n_questions");
    Serial3.println("played audio:  " + s.audio_indices[currentQuestionI]);
    playS(s.audio_indices[currentQuestionI]);                                 //faccio la domanda I
    Serial3.println("Reset del vettore risposte fino ad ora");
    Serial3.println("currentGivenAnswers: ");
    for (int i = 0; i < N_BODY_SENSORS; i++) {
      currentGivenAnswers[i] = false;                                         //azzero il vettore delle risposte date fino ad ora
      Serial3.print(currentGivenAnswers[i] + " ");
    }
    Serial3.println();
    currentNumberAnswered = 0;                                               //azzero il contatore delle risposte date fino ad ora
    gameState = wait_answer;                                                  //vado in stato wait answer
    startWaitingTime = millis();
    Serial3.println("wait answer begin");
  } else {                                                                   //se invece ho finito le domande
    gameState = end_game;                                                     //vado a stato end game
  }
}
//  //  boolean moreQuestion = checkIfMoreQuestions(gameNumber, scenarioNumber, questionI);
//  //  if (moreQuestion) {
//  //
//  //    pressedButton = -1;
//  //
//  //  } else
//}
//
//boolean checkIfMoreQuestions(int gameN, int scenarioN, int questionN) {
//  if (gameAnswers[gameN][scenarioN][questionN][0] == -1) return false;
//  else return true;
//}
//
//int uguali, diversi;
//boolean checkIfAllRightAnswers() {
//  confrontaArray (gameAnswers[gameNumber][scenarioNumber][questionI], answerTillNow, maxAnswersPerEx, &uguali, &diversi);
//  if (uguali)
//    return false;
//  else return true;
//}
//
//boolean checkIfAllRightAnswers2() {
//  sortArray(gameAnswers[gameNumber][scenarioNumber][questionI], maxAnswersPerEx);
//  sortArray(answerTillNow, maxAnswersPerEx);
//  return compareArrays(gameAnswers[gameNumber][scenarioNumber][questionI], answerTillNow, maxAnswersPerEx);
//}
//
//boolean compareArrays(int array1[], int array2[], int dim) {
//  int n = 0;
//  for (int i = 0; i < dim; i++) {
//    if (array1[i] == array2[i]) n++;
//  }
//  if (n == dim) return true;
//  else return false;
//}
//
//void confrontaArray (int array1[], int array2[], int dim, int *ptrUguali, int *ptrDiversi) {
//  int i, j, trovato;
//  // Confronto dei due array
//  *ptrUguali = 1;   /* inizialmente a VERO (1) viene posta a FALSO (0) se un elemento
//
//                   compare nel primo array ma non nel secondo */
//
//  *ptrDiversi = 1;    /* inizialmente a VERO (1) viene posta a FALSO (0) quando un elemento
//
//                   del promo array viene trovato anche nel secondo array */
//
//  /* la condizione (uguali || diversi) implica che il ciclo prosegua
//
//    finchè gli elementi del primo array o compaiono sempre anche nel
//
//    secondo array o non vi compaiono mai */
//
//  for (i = 0; i < dim && (*ptrUguali || *ptrDiversi); i++) {
//    trovato = 0; /* il ciclo prosegue finchè non viene trovato il valore uguale
//
//                (trovato assume valore 1) o termina l'array (j assume valore DIM) */
//
//    for (j = 0; j < dim && !trovato; j++)
//
//      if (array1[i] == array2[j])
//
//        trovato = 1;
//
//    if (trovato)
//
//      *ptrDiversi = 0;
//
//    else
//
//      *ptrUguali = 0;
//  }
//  return;
//}
//
//boolean linearSearch(int *array1, int target, int sizeArr) {
//  for (int i = 0; i < sizeArr; i++) {
//    if (array1[i] == target) {
//      return true;
//    }
//  }
//  return false;
//}
//
//void resetArray(int *array1, int sizeA) {
//  for (int i; i < sizeA; i++) {
//    array1[i] = -1;
//  }
//}





void waitAnswer() {
  Serial3.println("Attivo capacitivi Testa");
  previousWorkingCapacitives = workingCapacitives;
  workingCapacitives = head;
  t_Scenario s = games[currentGameI].scenarios[currentScenarioI];
  Serial3.println("currentNumberAnswered " + currentNumberAnswered);
  Serial3.println("n answers " + s.n_answers[currentQuestionI]);
  if (currentNumberAnswered < s.n_answers[currentQuestionI]) {//se ci sono ancora risposte da dare
    Serial3.println("currentNumberAnswered < s.n_answers[currentQuestionI]");
    if (currentNumberAnswered > 0) {  //se ho già dato una risposta
      //say:e poi??
      Serial3.println("E poi???");
      timedPlayS(77, 15000);
    }
    Serial3.println("Pressed Button:  " + pressedButton);
    if (pressedButton != -1) { //resto in attesa che venga premuto un bottone
      int a = pressedButton;//viene premuto un bottone
      Serial3.println("currentGivenAnswers: ");
      for (int i = 0; i < N_HEAD_SENSORS; i++) {
        Serial3.print(currentGivenAnswers[i] + " ");
      }
      Serial3.println();
      if (currentGivenAnswers[a]) {//se il bottone è già stato premuto per questa domanda
        Serial3.println("Bottone Gia premuto per questa domanda!");
        playS(18);// say: hai già dato questa risposta, provane un'altra
      } else {//se invece è la prima volta che prova un bottone
        Serial3.println("Prima volta che premi il bottone!");
        currentGivenAnswers[a] = true;  //mi salvo il bottone che ha premuto
        Serial3.println("currentGivenAnswers: ");
        for (int i = 0; i < N_HEAD_SENSORS; i++) {
          Serial3.print(currentGivenAnswers[i] + " ");
        }
        Serial3.println();
        if (s.correct_answers[currentQuestionI][a]) {  //se il bottone premuto è una risposta corretta
          Serial3.println("s.correct_answers[currentQuestionI][a];  " + s.correct_answers[currentQuestionI][a]);
          Serial3.println("risposta corretta");
          // correct
          currentNumberAnswered++;      //incremento il contatore delle risposte corrette
          Serial3.println("currentNumberAnswered incremented at:  " + currentNumberAnswered);
          rightAnswer();                //eseguo movimento right answer
        } else {                      //se invece la riposta è sbagliata
          Serial3.println("risposta scorretta");
          wrongAnswer();//say risposta sbagliata  ed esegui movimento
          // wrong
        }
        //gameState = mov;                //TODO da decommentare
      }

    }
  } else {            //se non ci sono più risposte da dare
    currentQuestionI++;     //incrementa l'indice della domanda
    gameState = make_question; //vado in stato make question
    Serial3.print("wait answer end");
  }
}

//
//    boolean pressedBottonInAnswers = linearSearch(gameAnswers[gameNumber][scenarioNumber][questionI], pressedButton, maxAnswersPerEx);
//    if (pressedBottonInAnswers) { //se la risposta fa parte di quelle corrette
//      answerTillNow[answerI] = pressedButton;
//      answerI++;
//      if (checkIfAllRightAnswers()) { //controllo se ha dato tutte le risposte necessarie
//        gameState = right_answer;   //date tutte le risposte, faccio movimento felice e passo a domanda successiva
//        questionI++;
//        playI++;
//      } else {
//        playS(36);
//        startWaitingTime = millis();
//        gameState = right_answer;   //numero risposte incompleto,si potrebbe fare altro stato 'oneMoreAnswer'
//      }
//    } else {                        //risposta scorretta, svuoto array delle risposte date fino ad ora e si riparte da capo
//      gameState = wrong_answer;
//      resetArray(answerTillNow, maxAnswersPerEx);
//      answerI = 0;
//    }
//  }
//    if (millis() - startWaitingTime > timePerAnsw) {
//      //play sound per dire che ripeterÃ² la domanda
//      gameState = make_question;
//    }
//  }

void rightAnswer() {
  int randNumber = rand() % 4 + make_happy0;
  Serial3.println("Disattivo Capacitivi");
  previousWorkingCapacitives = workingCapacitives;
  if (interpreterState == game_modality)workingCapacitives = noOne;
  else if (interpreterState == fam_modality)workingCapacitives = body;
  if (dir == 1)
    playS(4);
  else
    playS(5);
  dir = rand() % 2;
  bodyLedUpdate(color_wipe, greenC);
  //startMovement(randNumber); //at the end of makeHappy function there's the gameStateChange
}
void wrongAnswer() {
  Serial3.println("Disattivo Capacitivi");
  previousWorkingCapacitives = workingCapacitives;
  if (interpreterState == game_modality)workingCapacitives = noOne;
  else if (interpreterState == fam_modality)workingCapacitives = body;
  if (dir == 1)
    playS(6);
  else
    playS(7);
  dir = rand() % 2;
  bodyLedUpdate(color_wipe, redC);
  //startMovement(make_sad1);
}

void endGame() {
  //  int nRightAns = countResult();
  //  if (nRightAns <= 2)
  //    playS(18);
  //  else if (nRightAns > 2 && nRightAns <= 5)
  //    playS(19);
  //  else if (nRightAns > 5 && nRightAns <= 8)
  //    playS(20);
  //  else if (nRightAns > 9 && nRightAns <= 10)
  playS(1); // gioco finito
  gameState = no_game;
  interpreterState = choose_modality;
}

int countResult() {
  int n = 0;
  for (int i = 0; i < maxQuestionsPerEx; i++) {
    if (questionResult[i]) n++;
    Serial3.print(questionResult[i]);
    if (i != maxQuestionsPerEx - 1) Serial3.print(",");
    else Serial3.print(";");
  }
  return n;
}
boolean color_Game=false;

void colorGameSetup() {
  if (!color_Game) {
    color_Game = true;
    int color = rand() % 4;
    playS(DOMANDA_COLORE_LED_AUDIO);
    switch(color) {
    case 0: headLedUpdate(red, color_wipe); break;
    case 1: headLedUpdate(green, color_wipe); break;
    case 2: headLedUpdate(blue, color_wipe); break;
    case 3: headLedUpdate(yellow, color_wipe); break;
    }
    buttonToTouch = color;
    CapacitivesUpdate(head);
  }
}

void gameModality() {
  switch (gameState) {
    case setting:         Serial3.println("Setting game begin"); settingGame(); Serial3.println("Setting game end"); break;
    case make_question:   Serial3.println("make question begin"); makeQuestion(); Serial3.println("make question end"); break;
    case wait_answer:     waitAnswer(); break;
    case mov:             break;
    case end_game:        Serial3.println("end game"); endGame(); break;
  }
}


