#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "scrabble-game.h"

address root = NULL;

bool isEmpty() {
    return root == NULL;
}

address alokasi(infotype info) {
    address newNode = (address)malloc(sizeof(ElmtList));
    if (newNode != NULL) {
        newNode->hurufKamus = info;
        newNode->fs = NULL;
        newNode->nb = NULL;
        newNode->pr = NULL;
        newNode->hurufAkhir = false;
    }
    return newNode;
}

address createNode(infotype info) {
    return alokasi(info);
}

void insertPreorder(char huruf, address *current) {
    address P = createNode(huruf);
    P->pr = *current;
    (*current)->fs = P;
    *current = P;
}

address searchLevelOrder(char huruf, address root) {
    address temp = root;
    address lastChecked = NULL;
    address prev = NULL;

    while (temp != NULL) {
        lastChecked = temp;
        if (temp->hurufKamus == huruf) {
            return temp;
        }
        prev = temp;
        temp = temp->nb;
    }
    return prev;
}

void createTree(char kata[], address *first) {
    int panjang_kata, i;
    address current, temp, P;
    panjang_kata = strlen(kata);

    if (isEmpty()) {
        root = createNode('-');
        *first = root;
        current = root;
        for (i = 0; i < panjang_kata; i++) {
            insertPreorder(kata[i], &current);
        }
    } else {
        int i = 0;
        current = *first;
        current = current->fs;
        while (i < panjang_kata) {
            temp = searchLevelOrder(kata[i], current);
            current = temp;
            if (temp->hurufKamus == kata[i]) {
                if (current->fs != NULL) {
                    current = current->fs;
                    i++;
                } else {
                    while (i < panjang_kata) {
                        insertPreorder(kata[i], &current);
                        i++;
                    }
                }
            } else {
                P = createNode(kata[i]);
                current->nb = P;
                P->pr = current->pr;
                current = P;
                i++;
                while (i < panjang_kata) {
                    insertPreorder(kata[i], &current);
                    i++;
                }
            }
        }
    }
    current->hurufAkhir = true;
}

void initStack(Stack *s) {
    s->top = -1;
}

bool isStackEmpty(Stack *s) {
    return s->top == -1;
}

bool isStackFull(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

void push(Stack *s, address node) {
    if (!isStackFull(s)) {
        s->nodes[++s->top] = node;
    }
}

address pop(Stack *s) {
    if (!isStackEmpty(s)) {
        return s->nodes[s->top--];
    }
    return NULL;
}

void storeWordFromStack(Stack *s, Matriks *matriks, int startRow, int startCol) {
    int i;
    for (i = 0; i <= s->top; i++) {
        matriks->matriks[startRow][startCol + i].info.huruf = s->nodes[i]->hurufKamus;
    }
}

void traversePreorder(address root, int kataKe, Matriks *matriks) {
    Stack stack;
    initStack(&stack);
    address current = root;
    int i = 0;
    
    while (current != NULL || !isStackEmpty(&stack)) {
        if (current != NULL) {
            if (current->hurufKamus != '-' && !isStackFull(&stack)) {
                push(&stack, current);
            }

            if (current->hurufAkhir && current->fs == NULL) {
                i++;
                if (i == kataKe) {
                    storeWordFromStack(&stack, matriks, BOARD_SIZE / 2, (BOARD_SIZE - stack.top - 1) / 2);
                    return;
                }
            }

            if (current->hurufAkhir && current->fs != NULL) {
                i++;
                if (i == kataKe) {
                    storeWordFromStack(&stack, matriks, BOARD_SIZE / 2, (BOARD_SIZE - stack.top - 1) / 2);
                    return;
                }
                current = current->fs;
            } else {
                current = current->fs;
            }
        } else {
            current = pop(&stack);
            if (current != NULL) {
                current = current->nb;
                while (current != NULL && current->fs == NULL) {
                    if (current->hurufKamus != '-') {
                        push(&stack, current);
                    }
                    if (current->hurufAkhir) {
                        i++;
                        if (i == kataKe) {
                            storeWordFromStack(&stack, matriks, BOARD_SIZE / 2, (BOARD_SIZE - stack.top - 1) / 2);
                            return;
                        }
                    }
                    current = current->nb;
                }
            }
        }
    }
}

void inisiasiPapan(Matriks *matriks) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            matriks->matriks[i][j].info.huruf = ' ';
            matriks->matriks[i][j].poinBonus = 0;
        }
    }
}

void tampilPapan(Matriks *matriks) {
    int i, j;
    printf("    ");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i);
        for (j = 0; j < BOARD_SIZE; j++) {
            if (matriks->matriks[i][j].info.huruf == ' ') {
                printf(" . ");
            } else {
                printf(" %c ", matriks->matriks[i][j].info.huruf);
            }
        }
        printf("\n");
    }
}

void inputHuruf(Matriks *matriks, int *row, int *col, char letters[], int numLetters) {
    int i, j;
	char huruf;
    bool validLetter = false;

    while (!validLetter) {
        printf("Masukkan huruf yang ingin disisipkan (dari huruf yang tersedia): ");
        scanf(" %c", &huruf);

        for (i = 0; i < numLetters; i++) {
            if (huruf == letters[i]) {
                validLetter = true;
                // Remove the letter from the player's hand
                for (j = i; j < numLetters - 1; j++) {
                    letters[j] = letters[j + 1];
                }
                letters[numLetters - 1] = ' ';
                break;
            }
        }

        if (!validLetter) {
            printf("Huruf tidak valid. Silakan masukkan huruf yang tersedia.\n");
        }
    }

    printf("Masukkan koordinat baris: ");
    scanf("%d", row);
    printf("Masukkan koordinat kolom: ");
    scanf("%d", col);

    if (*row >= 0 && *row < BOARD_SIZE && *col >= 0 && *col < BOARD_SIZE) {
        matriks->matriks[*row][*col].info.huruf = huruf;
    } else {
        printf("Koordinat tidak valid.\n");
    }
}

void checkAndStoreAdjacent(Matriks *matriks, LinkedList *list, int row, int col, bool horizontal) {
    list->head = NULL;
    list->tail = NULL;
    if (horizontal) {
        int c = col;
        // Kiri
        while (c >= 0 && matriks->matriks[row][c].info.huruf != ' ') {
            NodeAddress newNode = (NodeAddress)malloc(sizeof(Node));
            newNode->huruf = matriks->matriks[row][c].info.huruf;
            newNode->next = list->head;
            list->head = newNode;
            if (list->tail == NULL) {
                list->tail = newNode;
            }
            c--;
        }
        // Kanan
        c = col + 1;
        while (c < BOARD_SIZE && matriks->matriks[row][c].info.huruf != ' ') {
            NodeAddress newNode = (NodeAddress)malloc(sizeof(Node));
            newNode->huruf = matriks->matriks[row][c].info.huruf;
            newNode->next = NULL;
            if (list->head == NULL) {
                list->head = newNode;
                list->tail = newNode;
            } else {
                list->tail->next = newNode;
                list->tail = newNode;
            }
            c++;
        }
    } else { // Vertical check
        int r = row;
        // Atas
        while (r >= 0 && matriks->matriks[r][col].info.huruf != ' ') {
            NodeAddress newNode = (NodeAddress)malloc(sizeof(Node));
            newNode->huruf = matriks->matriks[r][col].info.huruf;
            newNode->next = list->head;
            list->head = newNode;
            if (list->tail == NULL) {
                list->tail = newNode;
            }
            r--;
        }
        // Bawah
        r = row + 1;
        while (r < BOARD_SIZE && matriks->matriks[r][col].info.huruf != ' ') {
            NodeAddress newNode = (NodeAddress)malloc(sizeof(Node));
            newNode->huruf = matriks->matriks[r][col].info.huruf;
            newNode->next = NULL;
            if (list->head == NULL) {
                list->head = newNode;
                list->tail = newNode;
            } else {
                list->tail->next = newNode;
                list->tail = newNode;
            }
            r++;
        }
    }
}

void printLinkedList(LinkedList *list) {
    NodeAddress current = list->head;
    while (current != NULL) {
        printf("%c -> ", current->huruf);
        current = current->next;
    }
    printf("NULL\n");
}

bool isWordInTree(address root, LinkedList *list) {
    NodeAddress currentNode = list->head;
    address currentTree = root->fs;
    
    while (currentNode != NULL) {
        currentTree = searchLevelOrder(currentNode->huruf, currentTree);
        if (currentTree == NULL || currentTree->hurufKamus != currentNode->huruf) {
            return false;
        }
        currentNode = currentNode->next;
        if (currentNode != NULL) {
            currentTree = currentTree->fs;
        }
    }
    return currentTree != NULL && currentTree->hurufAkhir;
}

int calculateScore(LinkedList *list) {
    int score = 0;
    NodeAddress current = list->head;
    while (current != NULL) {
        // Assign points to each letter (example: A=1, B=3, C=3, ..., Z=10)
        switch (current->huruf) {
            case 'a': case 'e': case 'i': case 'l': case 'n': case 'o': case 'r': case 's': case 't': case 'u':
                score += 1;
                break;
            case 'd': case 'g':
                score += 2;
                break;
            case 'b': case 'c': case 'm': case 'p':
                score += 3;
                break;
            case 'f': case 'h': case 'v': case 'w': case 'y':
                score += 4;
                break;
            case 'k':
                score += 5;
                break;
            case 'j': case 'x':
                score += 8;
                break;
            case 'q': case 'z':
                score += 10;
                break;
            default:
                score += 0;
                break;
        }
        current = current->next;
    }
    return score;
}

void initializeLetterPool(char letterPool[], int letterPoints[]) {
    int i;
	// Initialize the letter pool and their corresponding points
    const char letters[] = "aaaaaaaaabbccddddeeeeeeeeeeeeffggghhiiiiiiiijkllllmmnnnnooooooooppqrrrrrrssssttttttuuuuvvwwxyyz";
    const int points[] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1,  // a
        3, 3,                       // b
        3, 3,                       // c
        2, 2, 2, 2,                 // d
        1, 1, 1, 1, 1, 1, 1, 1, 1,  // e
        4, 4,                       // f
        2, 2, 2,                    // g
        4, 4,                       // h
        1, 1, 1, 1, 1, 1, 1, 1,     // i
        8,                          // j
        5,                          // k
        1,                          // l
        3, 3,                       // m
        1, 1,                       // n
        1, 1, 1, 1, 1,              // o
        3, 3,                       // p
        10,                         // q
        1, 1, 1, 1,                 // r
        1, 1, 1,                    // s
        1, 1, 1, 1, 1, 1,           // t
        1, 1, 1,                    // u
        4, 4,                       // v
        4, 4,                       // w
        8,                          // x
        4, 4,                       // y
        10                          // z
    };

    for (i = 0; i < POOL_SIZE; i++) {
        letterPool[i] = letters[i];
        letterPoints[i] = points[i];
    }
}

void collectBoardLetters(Matriks *matriks, char boardLetters[], int *boardLetterCount) {
    int i, j;
	*boardLetterCount = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (matriks->matriks[i][j].info.huruf != ' ') {
                boardLetters[(*boardLetterCount)++] = matriks->matriks[i][j].info.huruf;
            }
        }
    }
}

void shuffleLetterPool(char letterPool[], int poolSize, char boardLetters[], int boardLetterCount) {
    // Merge the letter pool with the letters from the board
    int i, j;
    int mergedSize = poolSize + boardLetterCount;
    char mergedPool[mergedSize];
    memcpy(mergedPool, letterPool, poolSize);
    memcpy(mergedPool + poolSize, boardLetters, boardLetterCount);

    // Shuffle the merged pool
    for (i = mergedSize - 1; i > 0; i--) {
        j = rand() % (i + 1);
        char temp = mergedPool[i];
        mergedPool[i] = mergedPool[j];
        mergedPool[j] = temp;
    }

    // Copy back to letterPool
    memcpy(letterPool, mergedPool, poolSize);
}

void drawLetters(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex) {
    int i;
	for (i = 0; i < handSize; i++) {
        if (*poolIndex < poolSize) {
            hand[i] = letterPool[*poolIndex];
            (*poolIndex)++;
        } else {
            hand[i] = ' ';
        }
    }
}

void refillLetters(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex) {
    int i;
	for (i = 0; i < handSize; i++) {
        if (hand[i] == ' ' && *poolIndex < poolSize) {
            hand[i] = letterPool[*poolIndex];
            (*poolIndex)++;
        }
    }
}

void refillPlayerHand(char letterPool[], int letterPoints[], int poolSize, char hand[], int handSize, int* poolIndex) {
    int i;
	for (i = 0; i < handSize; i++) {
        if (hand[i] == ' ' && *poolIndex < poolSize) {
            hand[i] = letterPool[*poolIndex];
            (*poolIndex)++;
        }
    }
}

void displayLetters(char hand[], int handSize) {
    int i;
	printf("Huruf yang tersedia: ");
    for (i = 0; i < handSize; i++) {
        if (hand[i] != ' ') {
            printf("%c ", hand[i]);
        }
    }
    printf("\n");
}

void bacaKataDariFile(address *first) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    char kata[100];
    while (fscanf(file, "%s", kata) != EOF) {
        createTree(kata, first);
    }
    fclose(file);
}

void simpanSkor(const char *nama, int skor) {
    FILE *file = fopen(SCORE_FILE, "a");
    if (file == NULL) {
        printf("Gagal membuka file skor.\n");
        return;
    }
    fprintf(file, "%s %d\n", nama, skor);
    fclose(file);
}

void lihatRiwayatSkor() {
    FILE *file = fopen(SCORE_FILE, "r");
    if (file == NULL) {
        printf("Gagal membuka file skor.\n");
        return;
    }
    char nama[100];
    int skor;
    printf("========================================\n");
    printf("               SCRABBLE GAME            \n");
    printf("========================================\n");
    printf("Riwayat Skor\n\n");
    printf("|%-10s|%-10s|\n", "Nama", "Skor");
    printf("------------------------\n");

    while (fscanf(file, "%s %d", nama, &skor) != EOF) {
        printf("|%-10s|%-10d|\n", nama, skor);
    }
    printf("\n");
    while (1) {
        char choice;
        printf("Kembali ke Menu Utama? [Y/N]: ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            system("cls");
            break;
        }
    }
    fclose(file);
}

void lihatSkorTertinggi() {
    FILE *file = fopen(SCORE_FILE, "r");
    if (file == NULL) {
        printf("Gagal membuka file skor.\n");
        return;
    }
    char nama[100];
    int skor;
    char namaTertinggi[100];
    int skorTertinggi = 0;
    bool found = false;

    while (fscanf(file, "%s %d", nama, &skor) != EOF) {
        if (!found || skor > skorTertinggi) {
            strcpy(namaTertinggi, nama);
            skorTertinggi = skor;
            found = true;
        }
    }
    fclose(file);

    if (found) {
        printf("========================================\n");
        printf("               SCRABBLE GAME            \n");
        printf("========================================\n");
        printf("Skor Tertinggi\n\n");
        printf("%s\n", namaTertinggi);
        printf("%d Poin\n", skorTertinggi);
    } else {
        printf("Tidak ada skor yang ditemukan.\n");
    }

    printf("\n");
    while (1) {
        char choice;
        printf("Kembali ke Menu Utama? [Y/N]: ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            system("cls");
            break;
        }
    }
}

void tampilkanMenu() {
    printf("\n");
    printf("========================================\n");
    printf("               SCRABBLE GAME            \n");
    printf("========================================\n");
    printf("Pilih nomor dibawah ini :\n");
    printf(" 1. Mulai Permainan\n");
    printf(" 2. Riwayat Skor\n");
    printf(" 3. Skor tertinggi\n");
    printf(" 4. Tambah kata\n");
    printf(" 5. Keluar\n");
    printf("Pilihan: ");
}

void splashScreen() {
    printf("========================================\n");
    printf("=           SCRABBLE GAME              =\n");
    printf("========================================\n");
    printf("=                                      =\n");
    printf("=        SELAMAT DATANG DI GAME        =\n");
    printf("=           SCRABBLE TERBAIK           =\n");
    printf("=                                      =\n");
    printf("=       Siapkan strategi terbaikmu     =\n");
    printf("=      dan menangkan permainannya!     =\n");
    printf("=                                      =\n");
    printf("========================================\n\n");
    printf("Tekan Enter untuk melanjutkan...");
    getchar();
}

bool kataSudahAda(const char* kata) {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return false;
    }

    char kataDariFile[100];
    while (fscanf(file, "%s", kataDariFile) != EOF) {
        if (strcmp(kata, kataDariFile) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

void tambahKata() {
    char kata[100];
    printf("Masukkan kata yang ingin ditambahkan: ");
    scanf("%s", kata);

    if (kataSudahAda(kata)) {
        printf("Kata '%s' sudah ada di dalam kamus.\n", kata);
    } else {
        FILE *file = fopen(FILE_NAME, "a");
        if (file == NULL) {
            printf("Gagal membuka file.\n");
            return;
        }
        fprintf(file, "%s\n", kata);
        fclose(file);
        printf("Kata berhasil ditambahkan.\n");
    }
}

void lihatDaftarKata() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    char hurufAwal;
    printf("Masukkan huruf awal untuk mencari kata: ");
    scanf(" %c", &hurufAwal);

    char kata[100];
    bool adaKata = false;
    printf("Daftar kata yang dimulai dengan '%c':\n", hurufAwal);
    while (fscanf(file, "%s", kata) != EOF) {
        if (kata[0] == hurufAwal) {
            printf("%s\n", kata);
            adaKata = true;
        }
    }

    if (!adaKata) {
        printf("Tidak ada kata yang dimulai dengan '%c'.\n", hurufAwal);
    }

    fclose(file);
}
