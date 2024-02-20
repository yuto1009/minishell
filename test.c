#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_token {
    char* token;
    struct s_token* next;
} t_token;

typedef struct s_node {
    t_token* commandTokens; // コマンドを構成するトークン
    struct s_node* next;
    struct s_node* prev;
} t_node;

t_node* createCommandNode(t_token* startToken) {
    t_node* newNode = (t_node*)malloc(sizeof(t_node));
    newNode->commandTokens = startToken;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// コマンドとしてトークンを切り出すための関数
t_token* cutCommandTokens(t_token** current, t_token** nextCommandStart) {
    if (!*current) return NULL;

    t_token* start = *current;
    t_token* end = start;

    // パイプを見つけるまで、またはリストの終わりまで進む
    while (end->next!=NULL && end->next->kind!=TK_EOF && (end->next->token, "|") != 0) {
        end = end->next;
    }

    // 次のコマンドの開始トークンを設定
    *nextCommandStart = end->next ? end->next->next : NULL;

    // 現在のコマンドの終わりをマーク
    if (end->next) end->next = NULL;

    return start;
}

t_node* parser(t_token* tokens) {
    t_node* head = NULL;
    t_node* tail = NULL;

    t_token* currentToken = tokens;
    t_token* nextCommandStart = NULL;

    while (currentToken != NULL) {
        t_token* commandStart = cutCommandTokens(&currentToken, &nextCommandStart);

        // 新しいコマンドノードを作成し、リストに追加
        t_node* newCommand = createCommandNode(commandStart);
        if (head == NULL) {
            head = newCommand;
        } else {
            tail->next = newCommand;
            newCommand->prev = tail;
        }
        tail = newCommand;

        // 次のコマンドの処理に移動
        currentToken = nextCommandStart;
    }

    return head;
}

void printCommands(t_node* head) {
    while (head != NULL) {
        t_token* token = head->commandTokens;
        while (token != NULL) {
            printf("%s ", token->token);
            token = token->next;
        }
        printf("\n");
        head = head->next;
    }
}

int main() {
    // テスト用のトークンリスト初期化
    t_token t10 = {"haha", NULL};
    t_token t9 = {"|", &t10};
    t_token t8 = {"world", &t9};
    t_token t7 = {"|", &t8};
    t_token t6 = {"hello", &t7};
    t_token t5 = {"|", &t6};
    t_token t4 = {"hello.txt", &t5};
    t_token t3 = {">", &t4};
    t_token t2 = {"hello", &t3};
    t_token t1 = {"echo", &t2};

    t_node* commands = parser(&t1);
    printCommands(commands);

    // メモリ解放処理は省略していますが、実際のプログラムでは必要です。

    return 0;
}
