// scrabble-game.h
#ifndef scrabble_h
#define scrabble_h

#define BOARD_SIZE 15
#define MAX_STACK_SIZE 100
#define POOL_SIZE 108
#define LETTERS_IN_HAND 7
#define FILE_NAME "kata.txt"
#define SCORE_FILE "scores.txt"

typedef char infotype;

typedef struct huruf {
    infotype huruf;
    int poin;
} huruf;

typedef struct papan {
    huruf info;
    int poinBonus;
} papan;

typedef struct Matriks {
    papan matriks[BOARD_SIZE][BOARD_SIZE];
} Matriks;

typedef struct Elmt *address;
typedef struct Elmt {
    infotype hurufKamus;
    address fs;
    address nb;
    address pr;
    bool hurufAkhir;
} ElmtList;

typedef struct Node *NodeAddress;
typedef struct Node {
    infotype huruf;
    NodeAddress next;
} Node;

typedef struct LinkedList {
    NodeAddress head;
    NodeAddress tail;
} LinkedList;

typedef struct {
    address nodes[MAX_STACK_SIZE];
    int top;
} Stack;

typedef struct pemain {
    char nama[100];
    int skor;
    char letters[LETTERS_IN_HAND];
} pemain;

extern address root; // Declare root as extern

bool isEmpty();

//***************Modul Stack**************//
void initStack(Stack *s);
bool isStackEmpty(Stack *s);
bool isStackFull(Stack *s);
void push(Stack *s, address node);
address pop(Stack *s);
void storeWordFromStack(Stack *s, Matriks *matriks, int startRow, int startCol);

//***************Modul Trie**************//
address alokasi(infotype info);
address createNode(infotype info);
void insertPreorder(char huruf, address *current);
address searchLevelOrder(char huruf, address root);
void createTree(char kata[], address *first);
void traversePreorder(address root, int kataKe, Matriks *matriks);

//***************Modul Game**************//
bool isWordInTree(address root, LinkedList *list);
void inisiasiPapan(Matriks *matriks);
void tampilPapan(Matriks *matriks);
void inputHuruf(Matriks *matriks, int *row, int *col, char letters[], int numLetters);
void checkAndStoreAdjacent(Matriks *matriks, LinkedList *list, int row, int col, bool horizontal);
void printLinkedList(LinkedList *list);
int calculateScore(LinkedList *list);
void initializeLetterPool(char letterPool[], int letterPoints[]);
void collectBoardLetters(Matriks *matriks, char boardLetters[], int *boardLetterCount);
void shuffleLetterPool(char letterPool[], int poolSize, char boardLetters[], int boardLetterCount);
void drawLetters(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex);
void refillLetters(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex);
void refillPlayerHand(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex);
void displayLetters(char hand[], int handSize);
void tambahKata();
void lihatDaftarKata();
void bacaKataDariFile(address *first);
void simpanSkor(const char *nama, int skor);
void lihatRiwayatSkor();
void lihatSkorTertinggi();
void tampilkanMenu();
void splashScreen();

#endif

