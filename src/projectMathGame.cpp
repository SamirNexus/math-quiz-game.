#include <iostream>
#include <ctime>
#include <string>
using namespace std;

// ========================
// Enumerations
// ========================
enum enQuestionsLevel { EasyLevel = 1, MedLevel = 2, HardLevel = 3, Mix = 4 };
enum enOperationType { Add = 1, Sub = 2, Mult = 3, Div = 4, MixOp = 5 };

// ========================
// Structures
// ========================
struct stQuestion {
    int Number1 = 0;
    int Number2 = 0;
    enOperationType OperationType;
    enQuestionsLevel QuestionLevel;
    int CorrectAnswer = 0;
    int PlayerAnswer = 0;
    bool AnswerResult = false;
};

struct stQuizz {
    stQuestion QuestionList[100]; // Max 100 questions
    short NumberOfQuestions;
    enQuestionsLevel QuestionsLevel;
    enOperationType OpType;
    short NumberOfWrongAnswers = 0;
    short NumberOfRightAnswers = 0;
    bool isPass = false;
};

// ========================
// Utility Functions
// ========================
int RandomNumber(int From, int To) {
    return rand() % (To - From + 1) + From;
}

short ReadHowManyQuestions() {
    short NumberOfQuestions;
    do {
        cout << "How Many Questions do You Want to answer? (1-10): ";
        cin >> NumberOfQuestions;
    } while (NumberOfQuestions < 1 || NumberOfQuestions > 10);
    return NumberOfQuestions;
}

enQuestionsLevel ReadQuestionLevel() {
    short QuestionLevel = 0;
    do {
        cout << "Enter Question Level [1] Easy, [2] Med, [3] Hard, [4] Mix: ";
        cin >> QuestionLevel;
    } while (QuestionLevel < 1 || QuestionLevel > 4);
    return (enQuestionsLevel)QuestionLevel;
}

enOperationType ReadOpType() {
    short OpType;
    do {
        cout << "Enter Operation Type [1] Add, [2] Sub, [3] Mul, [4] Div, [5] Mix: ";
        cin >> OpType;
    } while (OpType < 1 || OpType > 5);
    return (enOperationType)OpType;
}

enOperationType GetRandomOperationType() {
    return (enOperationType)RandomNumber(1, 4);
}

// ========================
// Core Logic
// ========================
int SimpleCalculator(int Number1, int Number2, enOperationType OpType) {
    switch (OpType) {
    case Add: return Number1 + Number2;
    case Sub: return Number1 - Number2;
    case Mult: return Number1 * Number2;
    case Div:
        if (Number2 == 0) Number2 = 1; // Safety check
        return Number1 / Number2;
    default: return Number1 + Number2;
    }
}

stQuestion GenerateQuestion(enQuestionsLevel QuestionLevel, enOperationType OpType) {
    stQuestion Question;

    // Handle mixed levels or operations
    if (QuestionLevel == Mix)
        QuestionLevel = (enQuestionsLevel)RandomNumber(1, 3);
    if (OpType == MixOp)
        OpType = GetRandomOperationType();

    Question.OperationType = OpType;

    // Generate numbers based on level
    switch (QuestionLevel) {
    case EasyLevel:
        Question.Number1 = RandomNumber(1, 10);
        Question.Number2 = RandomNumber(1, 10);
        break;
    case MedLevel:
        Question.Number1 = RandomNumber(10, 50);
        Question.Number2 = RandomNumber(10, 50);
        break;
    case HardLevel:
        Question.Number1 = RandomNumber(50, 100);
        Question.Number2 = RandomNumber(50, 100);
        break;
    }

    // Prevent division by zero
    if (OpType == Div && Question.Number2 == 0)
        Question.Number2 = 1;

    Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, OpType);
    Question.QuestionLevel = QuestionLevel;

    return Question;
}

void GenerateQuizzQuestions(stQuizz& Quizz) {
    for (short i = 0; i < Quizz.NumberOfQuestions; i++)
        Quizz.QuestionList[i] = GenerateQuestion(Quizz.QuestionsLevel, Quizz.OpType);
}

string GetOpTypeSymbol(enOperationType OpType) {
    switch (OpType) {
    case Add: return "+";
    case Sub: return "-";
    case Mult: return "*";
    case Div: return "/";
    default: return "Mix";
    }
}

void PrintTheQuestion(stQuizz Quizz, short QuestionNumber) {
    cout << "\nQuestion [" << QuestionNumber + 1 << "/" << Quizz.NumberOfQuestions << "]\n";
    cout << Quizz.QuestionList[QuestionNumber].Number1 << " "
        << GetOpTypeSymbol(Quizz.QuestionList[QuestionNumber].OperationType) << " "
        << Quizz.QuestionList[QuestionNumber].Number2 << " = ?" << endl;
}

int ReadQuestionAnswer() {
    int answer = 0;
    cin >> answer;
    return answer;
}

void CorrectTheQuestionAnswer(stQuizz& Quizz, short QuestionNumber) {
    if (Quizz.QuestionList[QuestionNumber].PlayerAnswer != Quizz.QuestionList[QuestionNumber].CorrectAnswer) {
        Quizz.QuestionList[QuestionNumber].AnswerResult = false;
        Quizz.NumberOfWrongAnswers++;
        cout << "Wrong Answer :-(\n";
        cout << "The right answer is " << Quizz.QuestionList[QuestionNumber].CorrectAnswer << "\n";
    }
    else {
        Quizz.QuestionList[QuestionNumber].AnswerResult = true;
        Quizz.NumberOfRightAnswers++;
        cout << "Right Answer :-)\n";
    }
}

string GetFinalResultsText(bool Pass) {
    return Pass ? "PASS :-)" : "Fail :-(";
}

string GetQuestionLevelText(enQuestionsLevel QuestionLevel) {
    string arr[4] = { "Easy", "Med", "Hard", "Mix" };
    return arr[QuestionLevel - 1];
}

void PrintQuizzResults(stQuizz Quizz) {
    cout << "\n____________________________\n";
    cout << "Final Result: " << GetFinalResultsText(Quizz.isPass) << "\n";
    cout << "Number Of Questions: " << Quizz.NumberOfQuestions << endl;
    cout << "Questions Level: " << GetQuestionLevelText(Quizz.QuestionsLevel) << endl;
    cout << "OpType: " << GetOpTypeSymbol(Quizz.OpType) << endl;
    cout << "Right Answers: " << Quizz.NumberOfRightAnswers << endl;
    cout << "Wrong Answers: " << Quizz.NumberOfWrongAnswers << endl;
    cout << "____________________________\n";
}

void AskAndCorrectQuestionListAnswer(stQuizz& Quizz) {
    for (short i = 0; i < Quizz.NumberOfQuestions; i++) {
        PrintTheQuestion(Quizz, i);
        Quizz.QuestionList[i].PlayerAnswer = ReadQuestionAnswer();
        CorrectTheQuestionAnswer(Quizz, i);
    }
    Quizz.isPass = (Quizz.NumberOfRightAnswers >= Quizz.NumberOfWrongAnswers);
}

// ========================
// Game Flow
// ========================
void PlayMathGame() {
    stQuizz Quizz;
    Quizz.NumberOfQuestions = ReadHowManyQuestions();
    Quizz.QuestionsLevel = ReadQuestionLevel();
    Quizz.OpType = ReadOpType();

    GenerateQuizzQuestions(Quizz);
    AskAndCorrectQuestionListAnswer(Quizz);
    PrintQuizzResults(Quizz);
}

void ResetScreen() {
    system("cls");  // For Windows
    system("color 0F");
}

void StartGame() {
    char PlayAgain = 'Y';
    do {
        ResetScreen();
        PlayMathGame();
        cout << "\nDo You Want to Play again? Y | N: ";
        cin >> PlayAgain;
    } while (PlayAgain == 'Y' || PlayAgain == 'y');
}

// ========================
// Main
// ========================
int main() {
    srand((unsigned)time(NULL));
    StartGame();
    return 0;
}
