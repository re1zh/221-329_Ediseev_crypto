#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024 // Размер буфера для чтения/записи

void readFromFile(const char* fileName) {
    HANDLE hFile = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file for reading\n");
        return;
    }

    // Выделение памяти под буфер для чтения
    BYTE buffer[BUFFER_SIZE];
    DWORD bytesRead;

    // Чтение из файла
    while (ReadFile(hFile, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {
        // Вывод содержимого в терминал
        printf("%.*s", bytesRead, buffer);
    }

    CloseHandle(hFile);
}

void writeToFile(const char* fileName, const char* data) {
    HANDLE hFile = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Failed to open file for writing\n");
        return;
    }

    DWORD bytesWritten;

    // Запись данных в файл
    if (!WriteFile(hFile, data, strlen(data), &bytesWritten, NULL)) {
        printf("Failed to write to file\n");
    }

    CloseHandle(hFile);
}

int main() {
    char fileName[] = "C:\\Users\\user\\Desktop\\file.cryptLab2"; // Имя тестового файла
    //char dataToWrite[] = "Hello Klim!                                                                                     "; // Строка для записи
    char dataToWrite[] = "Lorem ipsum dolor sit ametconsectetur adipiscing eli Nulla nec elit at est commodo 1234567 a sia";
    printf("Choose an operation:\n");
    printf("1. Read from file\n");
    printf("2. Write to file\n");

    int choice;
    scanf_s("%d", &choice);

    switch (choice) {
    case 1:
        readFromFile(fileName);
        break;
    case 2:
        writeToFile(fileName, dataToWrite);
        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    printf("\n");
    system("pause");

    return 0;
}
