#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct Word{
    char vietnamese[100];
    char english[100];
} Word;

typedef struct Node{
    Word word;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct QueueNode{
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue{
    QueueNode* front;
    QueueNode* rear;
} Queue;

void initQueue(Queue** q){
    *q = (Queue*)malloc(sizeof(Queue));
    (*q)->front = NULL;
    (*q)->rear = NULL;
}

Node* createNode(Word word){
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->word.english, word.english);
    strcpy(newNode->word.vietnamese, word.vietnamese);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void enqueue(Queue** q, Node* data){
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->treeNode = data;
    newNode->next = NULL;
    if((*q)->front == NULL){
        (*q)->front = newNode;
        (*q)->rear = newNode;
    }
    else{
        (*q)->rear->next = newNode;
        (*q)->rear = newNode;
    }
}

Node* dequeue(Queue** q){
    if((*q)->front == NULL) return NULL;
    QueueNode* temp = (*q)->front;
    Node* tempData = temp->treeNode;
    (*q)->front = (*q)->front->next;
    if((*q)->front == NULL) (*q)->rear = NULL;
    free(temp);
    return tempData;
}

Node* addWord(Node* root, Word word){
    if(root == NULL) return createNode(word);
    Queue* q;
    initQueue(&q);
    enqueue(&q, root);
    while(q->front){
        Node* current = dequeue(&q);
        if(current->left == NULL){
            current->left = createNode(word);
            break;
        }
        else{
            enqueue(&q, current->left);
        }
        if(current->right == NULL){
            current->right = createNode(word);
            break;
        }
        else{
            enqueue(&q, current->right);
        }
    }
    return root;
}

void toLowerCase(char word[]){
    for(int i = 0; i < strlen(word); i++){
        word[i] = tolower(word[i]);
    }
}

void searchMean(Node* root, char wordSearch[]){
    if(root == NULL){
        printf("Danh sach trong\n");
        return;
    }
    int found = 0;
    char wordCopy[100];
    Queue* q;
    initQueue(&q);
    enqueue(&q, root);
    while(q->front){
        Node* current = dequeue(&q);
        strcpy(wordCopy, current->word.english);
        toLowerCase(wordCopy);
        if(strcmp(wordSearch, wordCopy) == 0){
            found = 1;
            printf("%s\n", current->word.vietnamese);
            break;
        } 
        if(current->left) enqueue(&q, current->left);
        if(current->right) enqueue(&q, current->right);
    }
    if(!found) printf("Khong tim thay tu\n");
}

Node* updateWord(Node* root, char wordSearch[]){
    if(root == NULL){
        printf("Danh sach trong\n");
        return NULL;
    }
    char wordCopy[100];
    int found = 0;
    Queue* q;
    initQueue(&q);
    enqueue(&q, root);
    while(q->front){
        Node* current = dequeue(&q);
        strcpy(wordCopy, current->word.english);
        toLowerCase(wordCopy);
        if(strcmp(wordSearch, wordCopy) == 0){
            printf("Nhap nghia moi: ");
            fgets(current->word.vietnamese, sizeof(current->word.vietnamese), stdin);
            current->word.vietnamese[strcspn(current->word.vietnamese, "\n")] = '\0';
            printf("Cap nhat nghia thanh cong\n");
            found = 1;
            break;
        }
        if(current->left) enqueue(&q, current->left);
        if(current->right) enqueue(&q, current->right);
    }
    if(found == 0) printf("Khong tim thay\n");
    return root;
}

void viewWord(Node* root){
    if(root == NULL) return;
    viewWord(root->left);
    viewWord(root->right);
    printf("%s - %s\n", root->word.english, root->word.vietnamese);
}

int main(){
    Word word;
    char wordSearch[100];
    Node* root = NULL;
    int choice, found = 0;
    do{
        printf("1. Them tu\n");
        printf("2. Tra nghia\n");
        printf("3. Hien thi\n");
        printf("4. Cap nhat\n");
        printf("5. Thoat\n");
        printf("Lua chon: ");
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                printf("Nhap tu tieng anh: ");
                fgets(word.english, sizeof(word.english), stdin);
                word.english[strcspn(word.english, "\n")] = '\0';
                printf("Nhap nghia tieng viet: ");
                fgets(word.vietnamese, sizeof(word.vietnamese), stdin);
                word.vietnamese[strcspn(word.vietnamese, "\n")] = '\0';
                root = addWord(root, word);
                printf("Them thanh cong\n");
                break;
            case 2:
                printf("Nhap tu can tra nghia: ");
                fgets(wordSearch, sizeof(wordSearch), stdin);
                wordSearch[strcspn(wordSearch, "\n")] = '\0';
                toLowerCase(wordSearch);
                searchMean(root, wordSearch);
                break;
            case 3:
                if(root == NULL) printf("Danh sach trong\n");
                else viewWord(root);
                break;
            case 4:
                found = 0;
                printf("Nhap tu can cap nhat nghia: ");
                fgets(wordSearch, sizeof(wordSearch), stdin);
                wordSearch[strcspn(wordSearch, "\n")] = '\0';
                toLowerCase(wordSearch);
                root = updateWord(root, wordSearch);
                break;
            case 5:
                printf("Da thoat\n");
                break;
            default:
                printf("Nhap sai chuc nang\n");
                break;
        }
    }
    while(choice != 5);
    return 0;
}