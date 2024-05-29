#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "scrabble-game.h"

int main() {
	int num, pilihan;
	
	srand( time(0)); 
    address first = NULL;

    Matriks matriks;
    inisiasiPapan(&matriks);

    LinkedList list;
    list.head = NULL;
    list.tail = NULL;

    char letterPool[POOL_SIZE];
    int letterPoints[POOL_SIZE];
    int poolIndex = 0;

    initializeLetterPool(letterPool, letterPoints);

    pemain player;
    player.skor = 0;

    char boardLetters[POOL_SIZE];
    int boardLetterCount;

    bool keepPlaying = true;
    
    splashScreen();
    system("cls");

    while (keepPlaying) {
        tampilkanMenu();
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1: {
                printf("Masukkan nama pemain: ");
                scanf("%s", player.nama);

                bacaKataDariFile(&first);
                num = rand() % 500 + 1;
    			traversePreorder(root, num, &matriks);
                collectBoardLetters(&matriks, boardLetters, &boardLetterCount);
                shuffleLetterPool(letterPool, POOL_SIZE, boardLetters, boardLetterCount);
                drawLetters(letterPool, letterPoints, POOL_SIZE, player.letters, LETTERS_IN_HAND, &poolIndex);
                displayLetters(player.letters, LETTERS_IN_HAND);

                int row, col;
                char lanjut;
                int totalScore = 0;

                bool keepPlayingInner = true;
                while (keepPlayingInner) {
                    tampilPapan(&matriks);
                    do {
                        inputHuruf(&matriks, &row, &col, player.letters, LETTERS_IN_HAND);
                        tampilPapan(&matriks);
                        displayLetters(player.letters, LETTERS_IN_HAND);

                        printf("Apakah ingin menyisipkan huruf lagi? (y/n): ");
                        scanf(" %c", &lanjut);
                    } while (lanjut == 'y' || lanjut == 'Y');

                    char arah;
                    printf("Apakah kata ditambahkan secara horizontal atau vertikal? (h/v): ");
                    scanf(" %c", &arah);

                    bool horizontal = (arah == 'h' || arah == 'H');
                    checkAndStoreAdjacent(&matriks, &list, row, col, horizontal);

                    printf("Kata yang disimpan dalam linked list:\n");
                    printLinkedList(&list);

                    if (isWordInTree(root, &list)) {
                        printf("Kata tersebut ada di dalam tree.\n");
                        int score = calculateScore(&list);
                        printf("Skor untuk kata ini: %d\n", score);
                        totalScore += score;
                    } else {
                        printf("Kata tersebut tidak ada di dalam tree.\n");
                    }

                    printf("Total skor: %d\n", totalScore);

                    printf("Apakah ingin melanjutkan permainan? (y/n): ");
                    scanf(" %c", &lanjut);
                    if (lanjut == 'n' || lanjut == 'N') {
                        keepPlayingInner = false;
                        player.skor = totalScore;
                        simpanSkor(player.nama, player.skor);
                    } else {
                        refillPlayerHand(letterPool, letterPoints, POOL_SIZE, player.letters, LETTERS_IN_HAND, &poolIndex);
                        displayLetters(player.letters, LETTERS_IN_HAND);
                    }
                }
                break;
            }
            case 2:
                lihatRiwayatSkor();
                break;
            case 3:
                lihatSkorTertinggi();
                break;
            case 4: {
                bool subMenuActive = true;
                while (subMenuActive) {
                    int subPilihan;
                    printf("1. Tambah Kata\n");
                    printf("2. Lihat Daftar Kata\n");
                    printf("3. Kembali ke Menu Utama\n");
                    printf("Pilihan: ");
                    scanf("%d", &subPilihan);

                    if (subPilihan == 1) {
                        tambahKata();
                    } else if (subPilihan == 2) {
                        lihatDaftarKata();
                    } else if (subPilihan == 3) {
                        subMenuActive = false;
                    } else {
                        printf("Pilihan tidak valid.\n");
                    }
                    if (subMenuActive) {
                        while (1) {
                            char choice;
                            printf("Kembali ke Sub Menu Tambah Kata? [Y/N]: ");
                            scanf(" %c", &choice);
                            if (choice == 'y' || choice == 'Y') {
                                system("cls");
                                break;
                            } else if (choice == 'n' || choice == 'N') {
                                subMenuActive = false;
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 5:
                keepPlaying = false;
                break;
            default:
                printf("Pilihan tidak valid.\n");
                break;
        }
        if (keepPlaying) {
            system("cls");
        }
    }

    return 0;
}
