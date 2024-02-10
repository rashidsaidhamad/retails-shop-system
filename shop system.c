#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    char name[100];
    int quantity;
    float buyingPrice;
    float sellingPrice;
    int inventoryQuantity;  // New field to store total inventory quantity
    struct Product* next;
    struct Product* prev;
};

// Function prototypes
void addOrUpdateProduct(struct Product** head);
void deleteProduct(struct Product** head, char name[]);
void traverseList(struct Product* head);
void sellProduct(struct Product* head, char name[], int quantity);
void generateReport(struct Product* head);
void displayMenu();

// Function prototypes for main menu
void displayMainMenu();
void inventoryMenu(struct Product** head);

int main() {
    int isLoggedIn = 0;
    char username[20];
    char password[20];

    while (!isLoggedIn) {
        printf("Login\n");
        printf("Username: ");
        fgets(username, sizeof(username), stdin);
        printf("Password: ");
        fgets(password, sizeof(password), stdin);

        if (strcmp(username, "admin\n") == 0 && strcmp(password, "password\n") == 0) {
            isLoggedIn = 1;
            printf("Login successful!\n");
        } else {
            printf("Invalid username or password. Please try again.\n");
        }
    }

    struct Product* productList = NULL;

    int mainChoice;
    do {
        displayMainMenu();
        scanf("%d", &mainChoice);

        switch (mainChoice) {
            case 1:
                inventoryMenu(&productList);
                break;
            case 2:
                printf("\nProgram Description:\n");
                // Add your program description here
                break;
            case 3:
                isLoggedIn = 0;
                printf("\nLogout successful!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (isLoggedIn);

    return 0;
}

void addOrUpdateProduct(struct Product** head) {
    float buyingPrice;
    float sellingPrice;
    char name[100];
    int quantity;

    printf("Enter details for the product:\n");
    printf("Name: ");
    scanf("%s", name);

    struct Product* current = *head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            printf("Product already exists. Updating buying and selling prices.\n");
            printf("Enter new Buying Price: ");
            scanf("%f", &buyingPrice);
            printf("Enter new Selling Price per unit: ");
            scanf("%f", &sellingPrice);

            current->buyingPrice = buyingPrice;
            current->sellingPrice = sellingPrice;
            printf("Buying and selling prices for product '%s' updated.\n", name);
            return;
        }
        current = current->next;
    }

    printf("Product does not exist. Adding new product.\n");
    printf("Buying Price: ");
    scanf("%f", &buyingPrice);
    printf("Selling Price per unit: ");
    scanf("%f", &sellingPrice);
    printf("Quantity: ");
    scanf("%d", &quantity);

    struct Product* newProduct = (struct Product*)malloc(sizeof(struct Product));
    strcpy(newProduct->name, name);
    newProduct->quantity = quantity;
    newProduct->buyingPrice = buyingPrice;
    newProduct->sellingPrice = sellingPrice;
    newProduct->inventoryQuantity = quantity;  // Initialize inventory quantity

    current = *head;

    if (*head == NULL || (*head)->buyingPrice >= buyingPrice) {
        newProduct->next = *head;
        newProduct->prev = NULL;
        if (*head != NULL) {
            (*head)->prev = newProduct;
        }
        *head = newProduct;
    } else {
        while (current->next != NULL && current->next->buyingPrice < buyingPrice) {
            current = current->next;
        }
        newProduct->next = current->next;
        newProduct->prev = current;
        if (current->next != NULL) {
            current->next->prev = newProduct;
        }
        current->next = newProduct;
    }

    printf("Product added successfully!\n");
}

void deleteProduct(struct Product** head, char name[]) {
    struct Product* current = *head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (current->prev != NULL) {
                current->prev->next = current->next;
            } else {
                *head = current->next;
            }
            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            free(current);
            printf("Product '%s' deleted successfully.\n", name);
            return;
        }
        current = current->next;
    }

    printf("Product '%s' not found in the store.\n", name);
}

void traverseList(struct Product* head) {
    struct Product* current = head;
    printf("Products in ascending order of buying price:\n");
    while (current != NULL) {
        printf("Name: %s, Buying Price: %.2f, Quantity: %d, Selling Price: %.2f\n",
               current->name, current->buyingPrice, current->quantity, current->sellingPrice);
        current = current->next;
    }
    printf("\nProducts in descending order of buying price:\n");
    current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    while (current != NULL) {
        printf("Name: %s, Buying Price: %.2f, Quantity: %d, Selling Price: %.2f\n",
               current->name, current->buyingPrice, current->quantity, current->sellingPrice);
        current = current->prev;
    }
}

void sellProduct(struct Product* head, char name[], int quantity) {
    struct Product* current = head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (current->inventoryQuantity >= quantity) {
                current->inventoryQuantity -= quantity;
                printf("Sold %d units of product '%s'.\n", quantity, name);
            } else {
                printf("Insufficient quantity of product '%s' in the inventory.\n", name);
            }
            return;
        }
        current = current->next;
    }

    printf("Product '%s' not found in the store.\n", name);
}

void generateReport(struct Product* head) {
    struct Product* current = head;
    float totalProfit = 0;
    float totalLoss = 0;

    printf("Product Report:\n");
    printf("%-20s%-15s%-15s%-15s%-15s%-15s\n", "Name", "Initial Qty", "Sold Qty", "Remaining Qty", "Profit", "Loss");

    while (current != NULL) {
        int soldQuantity = current->quantity - current->inventoryQuantity;
        float profit = soldQuantity * (current->sellingPrice - current->buyingPrice);
        float loss = (current->quantity - soldQuantity) * current->buyingPrice;

        totalProfit += profit;
        totalLoss += loss;

        printf("%-20s%-15d%-15d%-15d%-15.2f%-15.2f\n", current->name, current->quantity, soldQuantity,
               current->inventoryQuantity, profit, loss);

        current = current->next;
    }

    printf("\nTotal Profit: %.2f\n", totalProfit);
    printf("Total Loss: %.2f\n", totalLoss);
}

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Add or Update Product\n");
    printf("2. Delete Product\n");
    printf("3. View Product List\n");
    printf("4. Sell Product\n");
    printf("5. Generate Report\n");
    printf("6. Logout\n");
    printf("Enter your choice: ");
}

void displayMainMenu() {
    printf("\nMain Menu:\n");
    printf("1. Inventory\n");
    printf("2. Program Description\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");
}

void inventoryMenu(struct Product** head) {
    int inventoryChoice;
    do {
        displayMenu();
        scanf("%d", &inventoryChoice);

        switch (inventoryChoice) {
            case 1:
                addOrUpdateProduct(head);
                break;
            case 2: {
                char productToDelete[100];
                printf("Enter the name of the product to delete: ");
                scanf("%s", productToDelete);

                deleteProduct(head, productToDelete);
                break;
            }
            case 3:
                printf("\nProduct list:\n");
                traverseList(*head);
                break;
            case 4: {
                char productName[100];
                int sellQuantity;

                printf("Enter the name of the product to sell: ");
                scanf("%s", productName);

                printf("Enter the quantity to sell: ");
                scanf("%d", &sellQuantity);

                sellProduct(*head, productName, sellQuantity);
                break;
            }
            case 5:
                generateReport(*head);
                break;
            case 6:
                printf("\nReturning to Main Menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (inventoryChoice != 6);
}

