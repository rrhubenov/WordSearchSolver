#include <iostream>

unsigned wordSetCount;
char** wordSet; 

unsigned matrixSize;
char** letterMatrix;

void freeWordSet() {
    for(unsigned i = 0; i < wordSetCount; ++i) {
        delete wordSet[i];
    }

    delete[] wordSet;
}

void freeLetterMatrix() {
    for(unsigned i = 0; i < matrixSize; ++i) {
        delete letterMatrix[i];
    }
    
    delete[] letterMatrix;
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

bool isInField(int row, int column) {
    if(row < 0 || row >= matrixSize) return false;
    if(column < 0 || column >= matrixSize) return false;
    return true;
}

unsigned* findLetterAroundElement(unsigned row, unsigned column, char letter, bool visitedNeighbours[8]) {
    const int y[] = { -1, -1, -1, 0, 1, 1,  1, 0 };
    const int x[] = { -1,  0,  1, 1, 1, 0, -1, -1 };

    for(int i = 0; i < 8; ++i) {
        int currentRow = row + y[i];
        int currentColumn = column + x[i];

        //Insure pos is in matrix
        if(isInField(currentRow, currentColumn)) {
            if(letter == letterMatrix[currentRow][currentColumn] && visitedNeighbours[i] == false) {
                visitedNeighbours[i] = true;

                unsigned* point = new unsigned[2];

                point[0] = currentRow;
                point[1] = currentColumn;

                return point;
            }
        }
    }

    return NULL;
}

bool isWordInMatrix(unsigned row, unsigned column, char* word) {

    if(strlen(word) == 0){
        return true;
    } 

    bool visitedNeighbours[8] = {false};

    while(true) {
        unsigned* point = findLetterAroundElement(row, column, word[0], visitedNeighbours);

        if(point == NULL) return false;
        else {
            unsigned pointRow = point[0];
            unsigned pointColumn = point[1];

            delete[] point;

            char* restOfWord = word + 1;

            if(isWordInMatrix(pointRow, pointColumn, restOfWord)) 
                return true;
        }
    }
}

int main() {
    std::cout << "Enter word set size: ";
    std::cin >> wordSetCount;

    wordSet = new (std::nothrow) char*[wordSetCount];

    if(wordSet == NULL) {
        std::cout << "Memory issue...\n";
        return -1;
    }

    // Input logic
    for(unsigned i = 0; i < wordSetCount; ++i) {
        std::cout << "Enter word " << i << ": ";
        char temp[100];
        std::cin >> temp;

        // Insure input is lex sorted
        if(i == 0) {
            allocAndInsert(temp, 0);
        } else if(strcmp(temp, wordSet[i-1]) > 0) {
            allocAndInsert(temp, i);
        } else {
            std::cout << "Not lex sorted...";
            return 0;
        }
    }

    insertionSortByLength();

    std::cout << "Enter matrix size: ";

    std::cin >> matrixSize;

    letterMatrix = new (std::nothrow) char*[matrixSize];

    if(letterMatrix == NULL) {
        std::cout << "Memory issue...\n";
        return -1;
    }

    for(unsigned column = 0; column < matrixSize; ++column) {
        char* columnString = new (std::nothrow) char[matrixSize];

        if(columnString == NULL) {
            std::cout << "Memory issue...\n";
            return -1;
        }

        letterMatrix[column] = columnString;

        for(unsigned row = 0; row < matrixSize; ++row) {
            char temp;
            std::cin >> temp;
            columnString[row] = temp;
        }
    }

    unsigned wordFoundLength = 0;

    //Iterate over all the words
    for(unsigned i = 0; i < wordSetCount; ++i) {
        char* wordSearched = wordSet[i];

        if(strlen(wordSearched) > matrixSize*matrixSize) {
            std::cout << "Word " << wordSearched << " is longer than matrix, skipping...\n";
            continue;
        }

        std::cout << "Checking for word: " << wordSearched << "\n";
        
        for(unsigned i = 0; i < matrixSize*matrixSize; ++i) {
            unsigned row = (i / matrixSize);
            unsigned column = (i % matrixSize);
            if(wordSearched[0] == letterMatrix[row][column]) {

                std::cout << "Letter '" << wordSearched[0] << "' found on pos: (" << row << "," << column << ")\n" ;

                char* restOfWord = wordSearched + 1;

                if(isWordInMatrix(row, column, restOfWord)) {
                    std::cout << "Word: " << wordSearched << " has been found!\n";
                    wordFoundLength = strlen(wordSearched);
                }
            }
        }
    }

    std::cout << wordFoundLength << "\n";

    freeWordSet();
    freeLetterMatrix();
    return 0;
}
