#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const char* keywords[] = {
    "int", "float", "char", "double", "if", "else", "while", "for",
    "return", "break", "continue", "void", "class", "public", "private", "bool", "true", "false"
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

bool isKeyword(const char* word) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool isDelimiter(char ch) {
    return ch == ' ' || ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
}

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>' || ch == '%';
}

bool isInteger(const char* str) {
    int i = 0;
    if (str[0] == '-') i = 1;
    for (; str[i]; i++) {
        if (!isdigit(str[i])) return false;
    }
    return i > 0;
}

bool isFloat(const char* str) {
    int i = 0, dotCount = 0;
    if (str[0] == '-') i = 1;
    for (; str[i]; i++) {
        if (str[i] == '.') dotCount++;
        else if (!isdigit(str[i])) return false;
    }
    return dotCount == 1;
}

bool isIdentifier(const char* str) {
    if (!(isalpha(str[0]) || str[0] == '_')) return false;
    for (int i = 1; str[i]; i++) {
        if (!(isalnum(str[i]) || str[i] == '_')) return false;
    }
    return true;
}

void extractTokens(const char* str) {
    int i = 0, j = 0;
    char token[100];

    while (str[i] != '\0') {
        if (str[i] == '"') {
            // Handle string literal
            i++;
            j = 0;
            while (str[i] != '"' && str[i] != '\0') {
                token[j++] = str[i++];
            }
            token[j] = '\0';
            if (str[i] == '"') {
                printf("[String Literal] \"%s\"\n", token);
                i++; // Skip closing quote
            } else {
                printf("[Invalid Token] \"%s (missing closing quote)\"\n", token);
            }
            continue;
        }

        if (str[i] == '\'') {
            // Handle character literal
            i++;
            j = 0;
            while (str[i] != '\'' && str[i] != '\0') {
                token[j++] = str[i++];
            }
            token[j] = '\0';
            if (str[i] == '\'') {
                printf("[Character Literal] '%s'\n", token);
                i++; // Skip closing quote
            } else {
                printf("[Invalid Token] '%s (missing closing quote)'\n", token);
            }
            continue;
        }

        if (isDelimiter(str[i]) || isOperator(str[i])) {
            if (j != 0) {
                token[j] = '\0';
                if (isKeyword(token)) printf("[Keyword]        %s\n", token);
                else if (isInteger(token)) printf("[Integer Const]  %s\n", token);
                else if (isFloat(token)) printf("[Float Const]    %s\n", token);
                else if (isIdentifier(token)) printf("[Identifier]     %s\n", token);
                else printf("[Invalid Token]  %s\n", token);
                j = 0;
            }

            if (isOperator(str[i])) printf("[Operator]       %c\n", str[i]);
            else if (isDelimiter(str[i]) && str[i] != ' ') printf("[Delimiter]      %c\n", str[i]);
        } else {
            token[j++] = str[i];
        }

        i++;
    }

    if (j != 0) {
        token[j] = '\0';
        if (isKeyword(token)) printf("[Keyword]        %s\n", token);
        else if (isInteger(token)) printf("[Integer Const]  %s\n", token);
        else if (isFloat(token)) printf("[Float Const]    %s\n", token);
        else if (isIdentifier(token)) printf("[Identifier]     %s\n", token);
        else printf("[Invalid Token]  %s\n", token);
    }
}

int main() {
    char code[1000];
    printf("Enter your code (end with a line containing only 'END'):\n");

    char line[256];
    code[0] = '\0';

    while (1) {
        fgets(line, sizeof(line), stdin);
        if (strncmp(line, "END", 3) == 0)
            break;
        strcat(code, line);
    }

    printf("\n---- Tokens Detected ----\n");
    char* line_ptr = strtok(code, "\n");
    while (line_ptr) {
        extractTokens(line_ptr);
        line_ptr = strtok(NULL, "\n");
    }

    return 0;
}
