#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Request{
    char username[100];
    char issue[100];
    char timestamp[100];
} Request;

typedef struct Node{
    Request request;
    struct Node* next;
} Node;

typedef struct Queue{
    Node* front;
    Node* rear;
} Queue;

typedef struct Stack{
    Node* top;
} Stack;

typedef struct LinkedList{
    Node* head;
} LinkedList;

void initQueue(Queue** q){
    *q = (Queue*)malloc(sizeof(Queue));
    (*q)->front = NULL;
    (*q)->rear = NULL;
}

void initStack(Stack** s){
    *s = (Stack*)malloc(sizeof(Stack));
    (*s)->top = NULL;
}

void initLinkedList(LinkedList** list){
    *list = (LinkedList*)malloc(sizeof(LinkedList));
    (*list)->head = NULL;
}

Node* createNode(Request data){
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->request.issue, data.issue);
    strcpy(newNode->request.timestamp, data.timestamp);
    strcpy(newNode->request.username, data.username);
    newNode->next = NULL;
    return newNode;
}

void enqueue(Queue** q, Request data){
    Node* newNode = createNode(data);
    if((*q)->front == NULL){
        (*q)->front = newNode;
        (*q)->rear = newNode;
    }
    else{
        (*q)->rear->next = newNode;
        (*q)->rear = newNode;
    }
}

void dequeue(Queue** q){
    if((*q)->front == NULL) return;
    Node* temp = (*q)->front;
    (*q)->front = (*q)->front->next;
    if((*q)->front == NULL) (*q)->rear = NULL;
    free(temp);
}

void push(Stack** s, Request data){
    Node* newNode = createNode(data);
    if((*s)->top == NULL){
        (*s)->top = newNode;
    }
    else{
        newNode->next = (*s)->top;
        (*s)->top = newNode;
    }
}

void pop(Stack** s){
    if((*s)->top == NULL) return;
    Node* temp = (*s)->top;
    (*s)->top = (*s)->top->next;
    free(temp);
}
    
void procressRequest(Queue** requestProcess, Stack** undo, LinkedList** history){
    if((*requestProcess)->front == NULL){
        printf("Khong co yeu cau nao can xu li");
        return;
    }
    Request temp = (*requestProcess)->front->request;
    push(undo, temp);
    dequeue(requestProcess);
    printf("Xu li thanh cong yeu cau %s\n");
    // Luu lich su
    Node* newNode = createNode(temp);
    if((*history)->head == NULL){
        (*history)->head = newNode;
    }
    else{
        newNode->next = (*history)->head;
        (*history)->head = newNode;
    }
} 

void viewRequestProcess(Queue* requestProcess){
    if(requestProcess->front == NULL){
        printf("Khong co yeu cao nao can xu li\n");
        return;
    }
    Node* current = requestProcess->front;
    printf("----- Cac yeu cau cho xu li -----\n");
    while(current){
        printf("Ten nguoi gui: %s\n", current->request.username);
        printf("Noi dung: %s\n", current->request.issue);
        printf("Thoi gian: %s\n", current->request.timestamp);
        printf("--------------------\n");
        current = current->next;
    }
}

void viewHistory(LinkedList* history){
    if(history->head == NULL){
        printf("Lich su trong\n");
        return;
    }
    Node* current = history->head;
    printf("----- Lich su -----\n");
    while(current){
        printf("Ten nguoi gui: %s\n", current->request.username);
        printf("Noi dung: %s\n", current->request.issue);
        printf("Thoi gian: %s\n", current->request.timestamp);
        printf("--------------------\n");
        current = current->next;
    }
} 

void undoRequest(Queue** requestProcess, Stack** undo){
    if((*undo)->top == NULL){
        printf("Khong the undo\n");
        return;
    }
    Request temp = (*undo)->top->request;
    enqueue(requestProcess, temp);
    pop(undo);
    printf("Da hoan tac xu li yeu cau\n");
}

void clearHistory(LinkedList** history){
    if((*history)->head == NULL){
        printf("Lich su trong\n");
        return;
    }
    Node* current = (*history)->head;
    while(current){
        Node* temp = current->next;
        free(current);
        current = temp;
    }
    (*history)->head = NULL;
    printf("Xoa lich su thanh cong\n");
}

void freeQueue(Queue** q){
    Node* current = (*q)->front;
    while(current){
        Node* temp = current->next;
        free(current);
        current = temp;
    }
    (*q)->front = NULL;
    (*q)->rear = NULL;    
}

void freeStack(Stack** s){
    Node* current = (*s)->top;
    while(current){
        Node* temp = current->next;
        free(current);
        current = temp;
    }
    (*s)->top = NULL;
}

int main(){
    int choice;
    Request requets;
    Queue* requestProcess;
    initQueue(&requestProcess);
    Stack* undo;
    initStack(&undo);
    LinkedList* history;
    initLinkedList(&history);
    do{
        printf("1. Create Request\n");
        printf("2. Process\n");
        printf("3. Undo\n");
        printf("4. View\n");
        printf("5. History\n");
        printf("6. Clear\n");
        printf("7. Exit\n");
        printf("Lua chon: ");
        scanf("%d", &choice);
        getchar();
        switch(choice){
            case 1:
                printf("Nhap ten nguoi gui yeu cau: ");
                fgets(requets.username, sizeof(requets.username), stdin);
                requets.username[strcspn(requets.username, "\n")] = '\0';
                printf("Nhap noi dung yeu cau: ");
                fgets(requets.issue, sizeof(requets.issue), stdin);
                requets.issue[strcspn(requets.issue, "\n")] = '\0';
                printf("Nhap thoi gian: ");
                fgets(requets.timestamp, sizeof(requets.timestamp), stdin);
                requets.timestamp[strcspn(requets.timestamp, "\n")] = '\0';
                enqueue(&requestProcess, requets);
                printf("Them thanh cong yeu cau\n");
                break;
            case 2:
                procressRequest(&requestProcess, &undo, &history);
                break;
            case 3:
                undoRequest(&requestProcess, &undo);
                break;
            case 4:
                viewRequestProcess(requestProcess);
                break;
            case 5:
                viewHistory(history);
                break;
            case 6:
                clearHistory(&history);
                break;
            case 7:
                freeQueue(&requestProcess);
                freeStack(&undo);
                clearHistory(&history);
                break;
            default:
                printf("Nhap sai chuc nang\n");
                break;
        }
    }
    while(choice != 7);
    return 0;
}