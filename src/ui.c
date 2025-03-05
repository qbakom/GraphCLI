#include <stdio.h>
#include "ui.h"

void displayWelcomeMessage() {
    printf("===================================\n");
    printf("      GRAPH GENERATOR PROGRAM      \n");
    printf("===================================\n");
    printf("This program allows you to create and visualize graphs\n\n");
}

void showMainMenu() {
    printf("\nMAIN MENU:\n");
    printf("1. Create Random Graph\n");
    printf("2. Create User-Specified Graph\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void displayErrorMessage(const char* message) {
    printf("\nERROR: %s\n", message);
}

void displaySuccessMessage(const char* message) {
    printf("\nSUCCESS: %s\n", message);
}
