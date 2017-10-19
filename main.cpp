#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;

class Question{
public:
    char * question, * answer;
    Question( char * questionStart, char * answerStart ){
        question = new char[strlen(questionStart)];
        strcpy( question, questionStart );
        answer = new char[strlen(answerStart)];
        strcpy( answer, answerStart );
    }
    virtual ~Question(){
        delete [] question;
        delete [] answer;
    }
    void show(){
        cout << question << endl;
    }
    bool check( char * providedAnswer ){
        return ( strcmp( providedAnswer, answer ) == 0 );
    }
};

class QuestionManager{
public:
    vector<Question*> questions;
    int loaded;

    QuestionManager(){
        loaded = 0;
    }

    void loadQuestions(){
        char question[256];
        char buffer[256];
        ifstream fin( "questions.txt", ios::in );
        while ( !fin.eof() ){
            fin.getline(buffer,256);
            switch ( buffer[0] ){
            case '#':
                strcpy ( question, buffer+2 );
                break;
            case '$':
                questions.push_back( new Question(question,buffer+2) );
                loaded++;
                //questions.back()->show();
                break;
            default:;
            }
        }
    }
};

int main () {
    QuestionManager manager;
    manager.loadQuestions();


    char buffer[256];
    Question * question;
    int right;
    while ( manager.questions.size() ){
        question = manager.questions.back();
        question->show();
        cin.getline(buffer,256);
        strlwr(buffer);
        if ( question->check(buffer) ){
            cout << "Correct!" << endl << endl;
            right++;
        }
        else
            cout << "Wrong!" << endl << endl;
        delete question;
        manager.questions.pop_back();
    }

    cout << "You've got " << right << " questions correct out of " << manager.loaded << endl;

    return 0;
}