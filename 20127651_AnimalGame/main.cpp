#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    string key;
    Node* pLeft;
    Node* pRight;
};

Node* createNode(string key) {
    Node* p = new Node;
    p->key = key;
    p->pLeft = NULL;
    p->pRight = NULL;
    return p;
}

void freeTree(Node*& N) {
    if (N) {
        freeTree(N->pLeft);
        freeTree(N->pRight);
        delete N;
    }
}

void insert(Node*& N, fstream& f)  {
    string type, content;
    getline(f, type, ' ');
    getline(f, content);

    N = createNode(content);

    if (type == "#Q") {
        insert(N->pLeft, f);
        insert(N->pRight, f);
    }
}

void loadFromFile(Node*& N, string filename) {
    fstream f(filename, ios::in);
    insert(N, f);
    f.close();
}

void printTree(Node* N, fstream& f) {
    if (N == NULL) return;
    if (N->pLeft != NULL) {
        f << "#Q " << N->key << endl;
    }
    else {
        f << "#A " << N->key << endl;
    }
    printTree(N->pLeft, f);
    printTree(N->pRight, f);
}

void saveToFile(Node* N, string filename) {
    fstream f(filename, ios::out);
    printTree(N, f);
    f.close();
}

bool answer(Node* N) {
    string ans;
    do {
        cout << N->key << "  (y/n): ";
        cin >> ans;
        if (ans != "y" && ans != "n") {
            cout << "Try again!" << endl;
        }
    } while (ans != "y" && ans != "n");
    if (ans == "y") return 1;
    return 0;
}

void expand(Node* N) {
    string ans;
    cout << "\nWould you like to expand the game tree (y/n): ";
    cin.ignore();
    getline(cin, ans);

    if (ans == "y") {
        system("color B0 ");
        string guess, question;
        cout << "\nEnter a new animal in the form of a question" << endl;
        cout << "e.g., 'Is it a whale?': ";
        getline(cin, guess);
        cout << "\nNow enter a question for which the answer is 'yes' for your new animal," << endl;
        cout << "and which does not contradict your previous answers: ";
        getline(cin, question);

        N->pLeft = createNode(guess);
        N->pRight = createNode(N->key);
        N->key = question;
    }
}

void guess(Node* N) {
    if (answer(N)) {
        system("color AF");
        cout << "\nAY! I guessed your animal!\n" << endl;
        system("pause");
    }
    else {
        system("color CF");
        cout << "\nI give up!" << endl;
        expand(N);
        system("pause");
    }
}

void question(Node* N) {
    string reply;
    if (N->pLeft != NULL && N->pRight != NULL) {
        if (answer(N)) {
            question(N->pLeft);
        }
        else {
            question(N->pRight);
        }
    }
    else {
        guess(N);
    }
}

void menu(Node*& N) {
    while (true) {
        system("color E0");
        cout << "* * * * ANIMAL GAME * * * *" << endl;
        cout << "*                         *" << endl;
        cout << "* 1) Play the game        *" << endl;
        cout << "*                         *" << endl;
        cout << "* 2) Save the game file   *" << endl;
        cout << "*                         *" << endl;
        cout << "* 3) Quit                 *" << endl;
        cout << "*                         *" << endl;
        cout << "* * * * * * * * * * * * * *" << endl;

        int slt;
        do {
            cout << "\nYour selection: ";
            cin >> slt;
            cin.ignore();
            if (slt != 1 && slt != 2 && slt != 3) {
                cout << "Try Again!" << endl;
            }
        } while (slt != 1 && slt != 2 && slt != 3);

        if (slt == 1) {
            system("cls");
            question(N);
        }
        else if (slt == 2) {
            system("color B0");
            saveToFile(N, "Output.txt");
            cout << "\nSave Successfully!\n" << endl;
            system("pause");
        }
        else {
            system("cls");
            system("color DF");
            cout << "SEE YOU LATER, BYE BYE :D\n" << endl;
            break;
        }
        system("cls");
    }
}

int main() {
    Node* N = NULL;
    loadFromFile(N, "Data.txt");
    menu(N);
    saveToFile(N, "Data.txt");
    freeTree(N);
    system("pause");
    return 0;
}