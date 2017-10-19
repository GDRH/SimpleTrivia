#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
using namespace std;

/** Stores question and answer strings and has basic operations like showing the question and checking the provided answer */
class Question{
public:
    /** Constructor that takes pointers to question and answer strings */
    Question( char * questionStart, char * answerStart ){
        question = new char[strlen(questionStart)]; // allocate enough memory for the question string
        strcpy( question, questionStart ); // copy string from paramter to local buffer
        answer = new char[strlen(answerStart)]; // allocate enough memory for the answer string
        strcpy( answer, answerStart ); // copy string from parameter to local buffer
    }
    /** Destructor */
    virtual ~Question(){
        delete [] question; // deallocate question string
        delete [] answer; // deallocate answer string
    }
    /** Displays the question in the terminal */
    void show(){
        cout << question << endl;
    }
    /** Compares providedAnswer with the correct answer */
    bool check( char * providedAnswer ){
        return ( strcmp( providedAnswer, answer ) == 0 );
    }

private:
    /** Pointers to the question and answer strings */
    char * question, * answer;
};

/** Provides support for loading multiple question-answer pairs in a clean way */
class QuestionManager{
public:
    /** Question Pointer Dynamic Vector */
    vector<Question*> questions;

    /** The number of loaded question-answer pairs */
    int loadedPairs;

    QuestionManager(){
        loadedPairs = 0;
    }

    /** Loads question-answer pairs from a file ( parses input and stores data in a vector ) */
    void loadQuestions(){
        char question[256]; // Buffer to store the question
        char buffer[256]; // Buffer to store temporary data coming from ifstream::getline(...)

        ifstream fin( "questions.txt", ios::in ); // Open file in input mode

        while ( !fin.eof() ){ // while the code hasn't reached the end of file...
            fin.getline(buffer,256); // getLine into the buffer
            switch ( buffer[0] ){ // switch on the first character of each line
            case '#': // if it starts with a # it is a question
                strcpy ( question, buffer+2 ); // save it for later
                break;
            case '$': // if it starts with a $ it is an answer
                // create question-answer pair with the question saved earlier, and push it to the vector
                strlwr(buffer+2); // but lowercase answer for non-case-sensitive-ness
                questions.push_back( new Question(question,buffer+2) );

                loadedPairs++; // increment this ...
                //questions.back()->show(); // this is for debugging mostly
                break;
            default:; // treats default cases ( => no compiler warnings )
            }
        }
    }
};

int main () {
    QuestionManager manager; // Create manager
    manager.loadQuestions(); // Load the questions

    char buffer[256]; // buffer for reading input from the keyboard
    Question * question; // pointer to current question
    int correctQuestions = 0; // number of correct questions

    while ( manager.questions.size() ){ // while there are questions left in the trivia game
        question = manager.questions.back(); // get the last question from the vector

        question->show(); // show it to the user

        cin.getline(buffer,256); // get user input
        strlwr(buffer); // lowercase transform on the buffer ( for non-case-sensitive-ness )

        if ( question->check(buffer) ){ // if the answer is correct
            cout << "Correct!" << endl << endl;
            correctQuestions++; // increment this for the final score
        }
        else
            cout << "Wrong!" << endl << endl;

        delete question; // deallocate memory
        manager.questions.pop_back(); // remove last pointer
    }

    // Display a final score at the end
    cout << "You've got " << correctQuestions << " questions correct out of " << manager.loadedPairs << endl;

    return 0;
}