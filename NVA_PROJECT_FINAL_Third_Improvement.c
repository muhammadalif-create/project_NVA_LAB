#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int id;
    char title[100];
    char content[500];
    char author[50];
    char date[20];
} Blog;

typedef struct {
    char username[30];
    char password[30];
} User;

Blog blogs[MAX];
int count = 0;
User currentUser;

void saveBlogs() {
    FILE *fp = fopen("blogs.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d\n%s\n%s\n%s\n%s\n",
                blogs[i].id,
                blogs[i].title,
                blogs[i].content,
                blogs[i].author,
                blogs[i].date);
    }
    fclose(fp);
}

void loadBlogs() {
    FILE *fp = fopen("blogs.txt", "r");
    if (!fp) return;

    while (fscanf(fp, "%d\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n",
           &blogs[count].id,
           blogs[count].title,
           blogs[count].content,
           blogs[count].author,
           blogs[count].date) == 5) {
        count++;
    }

    fclose(fp);
}

int registerUser() {
    char uname[30], pass[30];
    printf("Choose Username: ");
    scanf("%s", uname);
    printf("Choose Password: ");
    scanf("%s", pass);

    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        printf("Unable to access users file.\n");
        return 0;
    }
    fprintf(fp, "%s %s\n", uname, pass);
    fclose(fp);
    printf("User registered successfully!\n");
    return 1;
}

int loginUser() {
    char uname[30], pass[30], fileU[30], fileP[30];
    printf("Username: ");
    scanf("%s", uname);
    printf("Password: ");
    scanf("%s", pass);

    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        printf("No users found. Please register first.\n");
        return 0;
    }

    while (fscanf(fp, "%s %s", fileU, fileP) == 2) {
        if (strcmp(uname, fileU) == 0 && strcmp(pass, fileP) == 0) {
            strcpy(currentUser.username, uname);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addBlog() {
    if (count >= MAX) {
        printf("Blog limit reached!\n");
        return;
    }

    printf("Enter Blog Title: ");
    getchar();
    fgets(blogs[count].title, 100, stdin);
    strtok(blogs[count].title, "\n");

    printf("Enter Blog Content: ");
    fgets(blogs[count].content, 500, stdin);
    strtok(blogs[count].content, "\n");

    strcpy(blogs[count].author, currentUser.username);

    printf("Enter Date (YYYY-MM-DD): ");
    fgets(blogs[count].date, 20, stdin);
    strtok(blogs[count].date, "\n");

    blogs[count].id = count + 1;
    count++;

    saveBlogs();
    printf("Blog added successfully!\n");
}

void viewBlogs() {
    for (int i = 0; i < count; i++) {
        if (strcmp(blogs[i].author, currentUser.username) == 0) {
            printf("\nID: %d\nTitle: %s\nContent: %s\nAuthor: %s\nDate: %s\n",
                   blogs[i].id, blogs[i].title, blogs[i].content,
                   blogs[i].author, blogs[i].date);
        }
    }
}

void viewPublicBlogs() {
    printf("\n--- Public Blogs ---\n");
    printf("1. View all\n2. Filter by author\n3. Filter by date\nChoose: ");
    int opt;
    char input[100];
    scanf("%d", &opt);
    getchar();

    switch (opt) {
        case 1:
            for (int i = 0; i < count; i++) {
                printf("\nID: %d\nTitle: %s\nContent: %s\nAuthor: %s\nDate: %s\n",
                       blogs[i].id, blogs[i].title, blogs[i].content,
                       blogs[i].author, blogs[i].date);
            }
            break;
        case 2:
            printf("Enter author to filter: ");
            fgets(input, 100, stdin);
            strtok(input, "\n");
            for (int i = 0; i < count; i++) {
                if (strcmp(blogs[i].author, input) == 0) {
                    printf("\nID: %d\nTitle: %s\nContent: %s\nAuthor: %s\nDate: %s\n",
                           blogs[i].id, blogs[i].title, blogs[i].content,
                           blogs[i].author, blogs[i].date);
                }
            }
            break;
        case 3:
            printf("Enter date to filter (YYYY-MM-DD): ");
            fgets(input, 100, stdin);
            strtok(input, "\n");
            for (int i = 0; i < count; i++) {
                if (strcmp(blogs[i].date, input) == 0) {
                    printf("\nID: %d\nTitle: %s\nContent: %s\nAuthor: %s\nDate: %s\n",
                           blogs[i].id, blogs[i].title, blogs[i].content,
                           blogs[i].author, blogs[i].date);
                }
            }
            break;
        default:
            printf("Invalid option!\n");
    }
}

void searchBlogs() {
    char key[100];
    int found = 0;

    printf("Enter keyword to search (title, author, or date): ");
    getchar();
    fgets(key, 100, stdin);
    strtok(key, "\n");

    for (int i = 0; i < count; i++) {
        if (strcmp(blogs[i].author, currentUser.username) == 0 &&
           (strstr(blogs[i].title, key) || strstr(blogs[i].author, key) || strstr(blogs[i].date, key))) {
            printf("\nID: %d\nTitle: %s\nContent: %s\nAuthor: %s\nDate: %s\n",
                   blogs[i].id, blogs[i].title, blogs[i].content,
                   blogs[i].author, blogs[i].date);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching blogs found.\n");
    }
}

void editBlog() {
    int id, found = 0;
    printf("Enter Blog ID to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (blogs[i].id == id && strcmp(blogs[i].author, currentUser.username) == 0) {
            found = 1;
            printf("\nEditing Blog ID %d\n", id);

            printf("New Title: ");
            getchar();
            fgets(blogs[i].title, 100, stdin);
            strtok(blogs[i].title, "\n");

            printf("New Content: ");
            fgets(blogs[i].content, 500, stdin);
            strtok(blogs[i].content, "\n");

            printf("New Date (YYYY-MM-DD): ");
            fgets(blogs[i].date, 20, stdin);
            strtok(blogs[i].date, "\n");

            saveBlogs();
            printf("Blog updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Blog ID not found or you don't have permission to edit it.\n");
    }
}

void deleteBlog() {
    int id, found = 0;
    printf("Enter Blog ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (blogs[i].id == id && strcmp(blogs[i].author, currentUser.username) == 0) {
            for (int j = i; j < count - 1; j++) {
                blogs[j] = blogs[j + 1];
            }
            count--;
            saveBlogs();
            printf("Blog deleted successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Blog ID not found or you don't have permission to delete it.\n");
    }
}

int main() {
    int choice;

    printf("--- Welcome to Multi-User Blog App ---\n");
    printf("1. Register\n2. Login\nChoose: ");
    scanf("%d", &choice);

    if (choice == 1) {
        registerUser();
        printf("Now login to continue.\n");
        if (!loginUser()) {
            printf("Login failed!\n");
            return 1;
        }
    } else if (choice == 2) {
        if (!loginUser()) {
            printf("Login failed!\n");
            return 1;
        }
    } else {
        printf("Invalid choice!\n");
        return 1;
    }

    printf("Welcome, %s!\n", currentUser.username);
    loadBlogs();

    while (1) {
        printf("\n--- Menu ---\n1. Add Blog\n2. View My Blogs\n3. View Public Blogs\n4. Search My Blogs\n5. Edit Blog\n6. Delete Blog\n7. Exit\nChoose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBlog(); break;
            case 2: viewBlogs(); break;
            case 3: viewPublicBlogs(); break;
            case 4: searchBlogs(); break;
            case 5: editBlog(); break;
            case 6: deleteBlog(); break;
            case 7: printf("Goodbye!\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
