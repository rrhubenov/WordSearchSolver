#include <iostream>

unsigned wordSetCount;
char** wordSet; 

unsigned matrixSize;

void freeWordSet(char** wordSet, const unsigned wordSetCount) {
    for(unsigned i = 0; i < wordSetCount; ++i) {
        delete wordSet[i];
    }

    delete[] wordSet;
}

void allocAndInsert(const char* str, const unsigned pos) {
    char* word = new char[strlen(str)];
    strcpy(word, str);
    wordSet[pos] = word;
}

void insertionSortByLength() {
    for(unsigned i = 0; i < wordSetCount; i++) {
        unsigned j = i;
        while(j > 0 && strlen(wordSet[j-1]) < strlen(wordSet[j])) {
            char* temp = wordSet[j-1];
            wordSet[j-1] = wordSet[j];
            wordSet[j] = temp;
            --j;
        }
    }
}

bool isInField(unsigned row, unsigned column) {
    if(row < 0 || row >= matrixSize) return false;
    if(column < 0 || column >= matrixSize) return false;
    return true;
}

unsigned[] findLetterAroundElement(unsigned row, unsigned column, char letter, char** letterMatrix) {
    const int y[] = { -1, -1, -1,  1, 1, 1,  0, 0 };
    const int x[] = { -1,  0,  1, -1, 0, 1, -1, 1 };

    for(int i = 0; i < 8; ++i) {
        unsigned currentRow = row + y[i];
        unsigned currentColumn = column + x[i];
        if(isInField(currentRow, currentColumn)) {
            if(letter == letterMatrix[currentRow][currentColumn]) {
                unsigned point = { row, column };
                return point;
            }
        }
    }

    return NULL;
}

bool isWordInMatrix(char* word, char letterMatrix[][matrixSize]) {

    char firstLetter = word[0];

    for(unsigned i = 0; i < matrixSize*matrixSize; ++i) {
        if(firstLetter == letterMatrix[i]) {
            unsigned row = (i / matrixSize) + 1;
            unsigned column = (i % matrixSize) + 1;

            //Make a copy of the matrix
            char matrixCopy[matrixSize][matrixSize];
            for(unsigned x = 0; x < matrixSize; ++x) {
                for(unsigned y = 0; y < matrixSize; ++y) {
                    matrixCopy[x][y] = letterMatrix[x][y];
                }
            }

            for(unsigned j = 1; j < strlen(word); ++j) {
                unsigned rowAndColumn[2] = findLetterAroundElement(row, column, word[j], letterMatrix);

                if(next == NULL) {
                    break;
                }
                else if(j == strlen(word)) {
                    return true;
                }
                else {
                    row = rowAndColumn[0];
                    column = rowAndColumn[1];
                    letterMatrix[row][column] = '0';
                }
            }
        }
    }

    return false;
}

int main() {
    std::cout << "Enter word set size: ";
    std::cin >> wordSetCount;

    wordSet = new char*[wordSetCount];

    for(unsigned i = 0; i < wordSetCount; ++i) {
        std::cout << "Enter word " << i << ": ";
        char temp[100];
        std::cin >> temp;

        if(i == 0) {
            allocAndInsert(temp, 0);
        } else if(strcmp(temp, wordSet[i-1]) > 0) {
            allocAndInsert(temp, i);
        } else {
            std::cout << "Not lex sorted...";
            return -1;
        }
    }

    insertionSortByLength();

    std::cout << "Enter matrix size: ";

    std::cin >> matrixSize;

    char letterMatrix[matrixSize][matrixSize];

    for(unsigned column = 0; column < matrixSize; ++column) {
        for(unsigned row = 0; row < matrixSize; ++row) {
            char temp;
            std::cin >> temp;
            letterMatrix[column][row] = temp;
        }
    }

    unsigned wordFoundLength = 0;

    //Iterate over all the words
    for(unsigned i = 0; i < wordSetCount; ++i) {
        char* wordSearched = wordSet[i];

        if(strlen(wordSearched) > matrixSize*matrixSize)
            continue;

        if(isWordInMatrix(wordSearched, letterMatrix)) {
            wordFoundLength = strlen(wordSearched);
        }
    }

    std::cout << wordFoundLength << "\n";


    freeWordSet(wordSet, wordSetCount);
    return 0;
}
