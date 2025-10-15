#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

enum MathOperator { Addition = 1, Subtraction, Multiplication, Division, MixedOperations };
enum DifficultyLevel { Easy = 1, Medium, Hard, MixedDifficulty };

struct MathQuestion {
    int FirstNumber;
    int SecondNumber;
    float UserAnswer;
    MathOperator Operation;
    float CorrectAnswer;
};

struct GameSession {
    int TotalQuestions;
    DifficultyLevel Difficulty;
    MathOperator OperationType;
    MathQuestion CurrentQuestion;
    int CorrectAnswersCount = 0;
    int IncorrectAnswersCount = 0;
};

int GetNumberOfQuestionsFromUser() {
    int questionCount;
    cout << "How Many Questions do you want to answer? ";
    cin >> questionCount;
    while (questionCount <= 0) {
        cout << "Invalid input! Please enter a positive number greater than zero: ";
        cin >> questionCount;
    }
    return questionCount;
}

DifficultyLevel SelectDifficultyLevel() {
    int difficultyChoice;
    cout << "Enter Questions Level [1] Easy, [2] Medium, [3] Hard, [4] Mixed: ";
    cin >> difficultyChoice;
    while (difficultyChoice < 1 || difficultyChoice > 4) {
        cout << "Invalid input! Please enter a number between (1 - 4): ";
        cin >> difficultyChoice;
    }
    return (DifficultyLevel)difficultyChoice;
}

MathOperator SelectOperationType() {
    int operationChoice;
    cout << "Enter Operation Type [1] Addition, [2] Subtraction, [3] Multiplication, [4] Division, [5] Mixed: ";
    cin >> operationChoice;
    while (operationChoice < 1 || operationChoice > 5) {
        cout << "Invalid input! Please enter a number between (1 - 5): ";
        cin >> operationChoice;
    }
    return (MathOperator)operationChoice;
}

int GenerateRandomNumber(short minValue, short maxValue) {
    return rand() % (maxValue - minValue + 1) + minValue;
}

int GenerateNumberForMixedDifficulty() {
    switch (GenerateRandomNumber(1, 3)) {
    case Easy: return GenerateRandomNumber(0, 20);
    case Medium: return GenerateRandomNumber(20, 80);
    case Hard: return GenerateRandomNumber(80, 150);
    default: return 0;
    }
}

string GetOperatorSymbol(MathOperator operation) {
    switch (operation) {
    case Addition: return "+";
    case Subtraction: return "-";
    case Division: return "/";
    case Multiplication: return "*";
    default: return "";
    }
}

float CalculateAnswer(GameSession& gameSession) {
    MathOperator selectedOperator;

    if (gameSession.OperationType == MixedOperations) {
        selectedOperator = (MathOperator)GenerateRandomNumber(1, 4);
        gameSession.CurrentQuestion.Operation = selectedOperator;
    }
    else {
        selectedOperator = gameSession.OperationType;
        gameSession.CurrentQuestion.Operation = gameSession.OperationType;
    }

    switch (selectedOperator) {
    case Addition:
        return gameSession.CurrentQuestion.FirstNumber + gameSession.CurrentQuestion.SecondNumber;
    case Subtraction:
        return gameSession.CurrentQuestion.FirstNumber - gameSession.CurrentQuestion.SecondNumber;
    case Multiplication:
        return gameSession.CurrentQuestion.FirstNumber * gameSession.CurrentQuestion.SecondNumber;
    case Division:
        if (gameSession.CurrentQuestion.SecondNumber == 0) {
            gameSession.CurrentQuestion.SecondNumber = 1; // Prevent division by zero
        }

        if (gameSession.CurrentQuestion.FirstNumber % gameSession.CurrentQuestion.SecondNumber != 0) {
            gameSession.CurrentQuestion.FirstNumber = gameSession.CurrentQuestion.SecondNumber *
                GenerateRandomNumber(1, 10);
        }

        return (float)gameSession.CurrentQuestion.FirstNumber / gameSession.CurrentQuestion.SecondNumber;
    default:
        return 0;
    }
}

int GenerateNumberBasedOnDifficulty(DifficultyLevel difficulty) {
    if (difficulty == MixedDifficulty)
        return GenerateNumberForMixedDifficulty();

    switch (difficulty) {
    case Easy: return GenerateRandomNumber(0, 20);
    case Medium: return GenerateRandomNumber(20, 80);
    case Hard: return GenerateRandomNumber(80, 150);
    default: return 0;
    }
}

void CreateNewQuestion(GameSession& gameSession) {
    gameSession.CurrentQuestion.FirstNumber = GenerateNumberBasedOnDifficulty(gameSession.Difficulty);

    do {
        gameSession.CurrentQuestion.SecondNumber = GenerateNumberBasedOnDifficulty(gameSession.Difficulty);

        if ((gameSession.OperationType == Division ||
            (gameSession.OperationType == MixedOperations && gameSession.CurrentQuestion.Operation == Division)) &&
            gameSession.CurrentQuestion.SecondNumber == 0) {
            continue; 
        }

        if ((gameSession.OperationType == Division ||
            (gameSession.OperationType == MixedOperations && gameSession.CurrentQuestion.Operation == Division)) &&
            gameSession.CurrentQuestion.FirstNumber < gameSession.CurrentQuestion.SecondNumber) {
            swap(gameSession.CurrentQuestion.FirstNumber, gameSession.CurrentQuestion.SecondNumber);
        }

    } while (gameSession.OperationType == Division && gameSession.CurrentQuestion.SecondNumber == 0);

    gameSession.CurrentQuestion.CorrectAnswer = CalculateAnswer(gameSession);
}

void PresentQuestionAndCheckAnswer(GameSession& gameSession) {
    cout << gameSession.CurrentQuestion.FirstNumber << " "
        << GetOperatorSymbol(gameSession.CurrentQuestion.Operation) << " "
        << gameSession.CurrentQuestion.SecondNumber << " : ";
    cin >> gameSession.CurrentQuestion.UserAnswer;

    if (fabs(gameSession.CurrentQuestion.UserAnswer - gameSession.CurrentQuestion.CorrectAnswer) < 0.001) {
        system("color 20");
        cout << "Correct! Answer is " << gameSession.CurrentQuestion.CorrectAnswer << endl;
        gameSession.CorrectAnswersCount++;
    }
    else {
        system("color 40");
        cout << "Incorrect! Correct answer is " << gameSession.CurrentQuestion.CorrectAnswer << endl;
        gameSession.IncorrectAnswersCount++;
    }
}

void DisplayFinalResults(GameSession gameSession) {
    string finalResult;
    if (gameSession.CorrectAnswersCount >= (gameSession.TotalQuestions * 0.5)) {
        finalResult = "Pass";
        system("color 20");
    }
    else {
        finalResult = "Fail";
        system("color 40");
    }

    cout << "\nGame Over\n";
    cout << "Number of Questions: " << gameSession.TotalQuestions << "\n";
    cout << "Correct answers: " << gameSession.CorrectAnswersCount << endl;
    cout << "Incorrect answers: " << gameSession.IncorrectAnswersCount << endl;
    cout << "Result: " << finalResult << endl;
}

void RunMathGame() {
    GameSession currentGame;
    char playAgainChoice;

    do {
        currentGame.TotalQuestions = GetNumberOfQuestionsFromUser();
        currentGame.Difficulty = SelectDifficultyLevel();
        currentGame.OperationType = SelectOperationType();

        for (int questionNumber = 1; questionNumber <= currentGame.TotalQuestions; questionNumber++) {
            CreateNewQuestion(currentGame);
            PresentQuestionAndCheckAnswer(currentGame);
        }

        DisplayFinalResults(currentGame);
        cout << "Want to play again (Y/N)? ";
        cin >> playAgainChoice;
    } while (playAgainChoice == 'y' || playAgainChoice == 'Y');
}

int main() {
    srand((unsigned int)time(0));
    RunMathGame();
    return 0;
}