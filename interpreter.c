
/* Import necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_VAR_COUNT 100
#define MAX_STR_LEN 256
#define MAX_INT 99999999

/* Define maximum sizes */
#define MAX_IDENTIFIER_SIZE 10
#define MAX_INT_SIZE 8
#define MAX_STRING_SIZE 256

/* Define token types */
typedef enum {
    IDENTIFIER,                 // Identifiers
    INT_CONST,                  // Integer constants
    OPERATOR,                   // Arithmetic operators
    LEFT_CURLY_BRACKET,         // Left curly bracket
    RIGHT_CURLY_BRACKET,        // Right curly bracket
    STRING_CONST,               // String constants
    KEYWORD,                    // Keywords
    END_OF_LINE,                // End of line marker
    COMMA                       // Comma
} TokenType;

/* Structure for tokens */
typedef struct {
    TokenType type;             // Token type
    char value[MAX_STRING_SIZE];/* Token value */
} Token;

/* Function prototypes */
Token getNextToken(FILE *fp);
void printToken(Token token);

/* Main function */


Token getNextToken(FILE *fp)
{
    Token token;                            // Create a token structure
    char buffer[MAX_STRING_SIZE];          // Temporary storage
    int i = 0;                              // Buffer index
    char c;                                 // Character storage variable

    /* Skip whitespace and comments */
    do {
        c = fgetc(fp);                      // Get the character from the file
        if (c == '/') {                     // If the character is a "/"
            char nextChar = fgetc(fp);      // Get the next character
            if (nextChar == '*') {          // If the next character is ""
                int commentDepth = 1;       // Depth counter to track comment nesting
                while (commentDepth > 0) {  // Loop until the end of the comment
                    char prevChar = c;     // Store the previous character
                    c = fgetc(fp);         // Get the next character
                    if (prevChar == '*' && c == '/')   // End of comment check
                        commentDepth--;     // Found the end of a comment, decrement depth counter
                    else if (prevChar == '/' && c == '*') // Nested comment check
                        commentDepth++;     // Found the start of a nested comment, increment depth counter
                    else if (c == EOF) {   // End of file check
                        printf("Error: Unclosed comment.\n");   // Error message
                        exit(-1);           // Terminate the program
                    }
                }
                c = fgetc(fp);              // Get the character after the comment
            }
            else {
                fseek(fp, -1, SEEK_CUR);   // If not a comment, move the file pointer back
                break; // Exit the loop     
            }
        }
    } while (isspace(c));                  // Skip whitespace characters

    /* End of file check */
    
    /* End of line marker */
    if (c == '.')
    {
        token.type = END_OF_LINE;          // Set token type
        strcpy(token.value, ".");           // Set token value
        return token;                       // Return the token
    }
    
    /* Identifier or Keyword */
    if (isalpha(c))
    {
        buffer[i++] = c;                    // Add character to buffer
        while ((isalnum(c = fgetc(fp)) || c == '_') && i < MAX_IDENTIFIER_SIZE)
        {
            buffer[i++] = c;                // Add character to buffer
        }
        
        if (i == MAX_IDENTIFIER_SIZE && (isalnum(c) || c == '_')) {
            printf("Error: Identifier too long.\n");   // Error message
            exit(-1);                       // Terminate the program
        }
        buffer[i] = '\0';                   // Add null character at the end of the buffer
        fseek(fp, -1, SEEK_CUR);            // Move the file pointer back

        /* Check for defined keywords */
        if (strcmp(buffer, "int") == 0 || strcmp(buffer, "text") == 0 ||
            strcmp(buffer, "is") == 0 || strcmp(buffer, "loop") == 0 ||
            strcmp(buffer, "times") == 0 || strcmp(buffer, "read") == 0 ||
            strcmp(buffer, "write") == 0 || strcmp(buffer, "newLine") == 0)
        {
            token.type = KEYWORD;           // Set token type
        }
        else
        {
            token.type = IDENTIFIER;        // Set token type
        }

        strcpy(token.value, buffer);        // Copy token value
        return token;                       // Return the token
    }

    /* Integer constant */
    if (isdigit(c))
    {
        buffer[i++] = c;                    // Add character to buffer
        while (isdigit(c = fgetc(fp)) && i < MAX_INT_SIZE)
        {
            buffer[i++] = c;                // Add character to buffer
        }
        if (i == MAX_INT_SIZE && isdigit(c)) {
            printf("Error: Integer constant too long.\n");    // Error message
            exit(-1);                       // Terminate the program
        }
        buffer[i] = '\0';                   // Add null character at the end of the buffer
        fseek(fp, -1, SEEK_CUR);            // Move the file pointer back
        token.type = INT_CONST;             // Set token type
        strcpy(token.value, buffer);        // Copy token value
        return token;                       // Return the token
    }

    /* Operators */
    if (c == '+' || c == '-' || c == '*' || c == '/')
    {
        token.type = OPERATOR;              // Set token type
        token.value[0] = c;                 // Set token value
        token.value[1] = '\0';              // Null-terminate the string
        return token;                       // Return the token
    }

    /* Left curly bracket */
    if (c == '{')
    {
        token.type = LEFT_CURLY_BRACKET;   // Set token type
        strcpy(token.value, "{");           // Set token value
        return token;                       // Return the token
    }

    /* Right curly bracket */
    if (c == '}')
    {
        token.type = RIGHT_CURLY_BRACKET;  // Set token type
        strcpy(token.value, "}");           // Set token value
        return token;                       // Return the token
    }

    /* String constant */
    if (c == '"')
    {
        buffer[i++] = c;                    // Add character to buffer
        while ((c = fgetc(fp)) != '"' && c != EOF && i < MAX_STRING_SIZE)
        {
            buffer[i++] = c;                // Add character to buffer
        }
        if (c != '"')
        {
            printf("Error: String constant not terminated.\n");    // Error message
            exit(-1);                       // Terminate the program
        }
        buffer[i++] = c;                    // Add character to buffer
        buffer[i] = '\0';                   // Add null character at the end of the buffer
        token.type = STRING_CONST;          // Set token type
        strcpy(token.value, buffer);        // Copy token value
        return token;                       // Return the token
    }

    /* Comma */
    if (c == ',')
    {
        token.type = COMMA;                // Set token type
        strcpy(token.value, ",");           // Set token value
        return token;                       // Return the token
    }
    
    /* End of file */
    if (c == EOF)
    {
        token.type = END_OF_LINE;          // Set token type
        return token;                      // Return the token
    }

    /* Invalid character */
    printf("Error: Invalid character: %c\n", c);   // Error message
    exit(-1);                       // Terminate the program
}

void printToken(Token token)
{
    /* Function to write the token to the output file */

    FILE *outputFile = fopen("code.lex", "a");  // Open the output file
    if (outputFile == NULL)
    {
        printf("Error writing to output file.\n");  // Error message
        exit(-1);                                    // Terminate the program
    }

    switch (token.type)  // Switch case based on token type
    {
    case IDENTIFIER:  // Identifier
        fprintf(outputFile, "Identifier(%s)\n", token.value);  // Write the identifier
        break;
    case INT_CONST:  // Integer constant
        fprintf(outputFile, "IntConst(%s)\n", token.value);  // Write the integer constant
        break;
    case OPERATOR:  // Operator
        fprintf(outputFile, "Operator(%s)\n", token.value);  // Write the operator
        break;
    case LEFT_CURLY_BRACKET:  // Left curly bracket
        fprintf(outputFile, "LeftCurlyBracket\n");  // Write the left curly bracket
        break;
    case RIGHT_CURLY_BRACKET:  // Right curly bracket
        fprintf(outputFile, "RightCurlyBracket\n");  // Write the right curly bracket
        break;
    case STRING_CONST:  // String constant
        fprintf(outputFile, "String(%s)\n", token.value);  // Write the string constant
        break;
    case END_OF_LINE:  // End of line marker
        fprintf(outputFile, "EndOfLine\n");  // Write the end of line marker
        break;
    case COMMA:  // Comma
        fprintf(outputFile, "Comma\n");  // Write the comma
        break;
    case KEYWORD:  // Keyword
        fprintf(outputFile, "Keyword(%s)\n", token.value);  // Write the keyword
        break;
    default:
        fprintf(outputFile, "Error: Unknown token type\n");  // Unknown token type error
        break;
    }
    
    fclose(outputFile);  // Close the output file
}


typedef enum { INT, TEXT } VarType;

typedef struct {
    char name[32];
    VarType type;
    union {
        int intValue;
        char strValue[MAX_STR_LEN];
    };
} Variable;

Variable variables[MAX_VAR_COUNT];
int varCount = 0;

void execute(const char* line);

Variable* getVariable(const char* name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

char* concatStrings(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1); // +1 for the null-terminator
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

char* subtractStrings(const char* str, const char* sub) {
    char* pos = strstr(str, sub);
    if (!pos) return strdup(str); // sub not found in str

    size_t len = strlen(str) - strlen(sub);
    char* result = (char*)malloc(len + 1); // +1 for the null-terminator

    strncpy(result, str, pos - str); // copy part before sub
    result[pos - str] = '\0';   
    strcat(result, pos + strlen(sub)); // copy part after sub

    return result;
}

void declareVariable(const char* name, VarType type, const char* value) {
    if (varCount >= MAX_VAR_COUNT) {
        printf("Error: Too many variables declared.\n");
        exit(1);
    }

    strcpy(variables[varCount].name, name);
    variables[varCount].type = type;

    if (type == INT) {
        if (value != NULL) {
            int intValue;
            sscanf(value, "IntConst(%d)", &intValue);
            variables[varCount].intValue = intValue;
        } else {
            variables[varCount].intValue = 0;
        }
        printf("Declared variable: %s of type INT with value %d\n", name, variables[varCount].intValue);
    } else {
        if (value != NULL) {
            char buffer[MAX_STR_LEN];
            sscanf(value, "String(\"%255[^\"]\")", buffer);
            strcpy(variables[varCount].strValue, buffer);
        } else {
            variables[varCount].strValue[0] = '\0';
        }
        printf("Declared variable: %s of type TEXT with value \"%s\"\n", name, variables[varCount].strValue);
    }

    varCount++;
}

void handleDeclaration(const char* line) {
    char* token = strtok((char*)line, " ,.");
    VarType type;

    // Determine the type of the variables
    if (strcmp(token, "Keyword(int)") == 0) {
        type = INT;
    } else if (strcmp(token, "Keyword(text)") == 0) {
        type = TEXT;
    } else {
        printf("Error: Unrecognized type keyword %s.\n", token);
        return;
    }

    // Process each variable declaration
    while ((token = strtok(NULL, " ,.")) != NULL) {
        if (strncmp(token, "Identifier(", 11) == 0) {
            char varName[32];
            sscanf(token, "Identifier(%31[^)])", varName);

            char* value = NULL;
            token = strtok(NULL, " ,.");
            if (token != NULL && strcmp(token, "Keyword(is)") == 0) {
                token = strtok(NULL, " ,.");
                if (token != NULL && (strncmp(token, "IntConst(", 9) == 0 || strncmp(token, "String(", 7) == 0)) {
                    if (strncmp(token, "String(", 7) == 0) {
                        // Allocate enough space for the entire string
                        size_t valueLen = strlen(token);
                        value = (char*)malloc(valueLen + 1);
                        strcpy(value, token);

                        char* endQuote = strstr(token, "\")");
                        while (endQuote == NULL) {
                            token = strtok(NULL, " ,");
                            if (token == NULL) {
                                printf("Error: Unterminated string.\n");
                                free(value);
                                return;
                            }
                            valueLen += strlen(token) + 1;
                            value = (char*)realloc(value, valueLen + 1);
                            strcat(value, " ");
                            strcat(value, token);
                            endQuote = strstr(token, "\")");
                        }

                        // Null-terminate the final string
                        value[valueLen] = '\0';
                    } else {
                        value = (char*)malloc(strlen(token) + 1);
                        strcpy(value, token);
                    }

                    token = strtok(NULL, " ,."); // Move to the next token
                } else {
                    printf("Error: Invalid assignment for variable %s.\n", varName);
                    return;
                }
            }

            // Declare the variable with or without an initial value
            declareVariable(varName, type, value);

            if (value) {
                free(value);
            }
        }

        if (token == NULL || strcmp(token, "Comma") != 0) {
            break;
        }
    }
}



void handleAssignment(const char* varName, const char* expression) {
    Variable* var = getVariable(varName);
    if (var == NULL) {
        // Yeni bir değişken oluştur
        if (varCount >= MAX_VAR_COUNT) {
            printf("Error: Too many variables declared.\n");
            exit(1);
        }
        strcpy(variables[varCount].name, varName);

        // String içerip içermediğini kontrol et
        if (strstr(expression, "String(") != NULL) {
            variables[varCount].type = TEXT;
            var = &variables[varCount];
            varCount++;
        } else {
            variables[varCount].type = INT;
            var = &variables[varCount];
            varCount++;
        }
    }

    if (var->type == INT) {
        int result = 0;
        char* exprCopy = strdup(expression);
        char* token = strtok(exprCopy, " ");
        int lastOperation = 1; // 1: ekleme, -1: çıkarma

        while (token != NULL) {
            if (strncmp(token, "IntConst(", 9) == 0) {
                int num;
                sscanf(token, "IntConst(%d)", &num);
                result += lastOperation * num;
            } else if (strncmp(token, "Identifier(", 11) == 0) {
                char otherVarName[32];
                sscanf(token, "Identifier(%31[^)])", otherVarName);
                Variable* other = getVariable(otherVarName);
                if (other != NULL && other->type == INT) {
                    result += lastOperation * other->intValue;
                } else {
                    printf("Error: Invalid integer assignment for %s.\n", otherVarName);
                    free(exprCopy);
                    return;
                }
            } else if (strcmp(token, "Operator(/)") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL && strncmp(token, "IntConst(", 9) == 0) {
                    int divisor;
                    sscanf(token, "IntConst(%d)", &divisor);
                    if (divisor != 0) {
                        result /= divisor;
                    } else {
                        printf("Error: Division by zero.\n");
                        free(exprCopy);
                        return;
                    }
                } else {
                    printf("Error: Invalid expression.\n");
                    free(exprCopy);
                    return;
                }
            } else if (strcmp(token, "Operator(+)") == 0) {
                lastOperation = 1; // Sonraki işlemi toplama olarak ayarla
            } else if (strcmp(token, "Operator(-)") == 0) {
                lastOperation = -1; // Sonraki işlemi çıkarma olarak ayarla
            } else {
                printf("Error: Unexpected token %s.\n", token);
                free(exprCopy);
                return;
            }
            token = strtok(NULL, " ");
        }
        var->intValue = result < 0 ? 0 : (result > MAX_INT ? MAX_INT : result);
        printf("Assigned %d to variable %s\n", var->intValue, varName);
        free(exprCopy);
    } 
    else if (var->type == TEXT) {
        // Handle assignment to TEXT variable
        char result[MAX_STR_LEN] = ""; // Sonuç string'i başlat
        char* exprCopy = strdup(expression); // İfadeyi kopyala
        char* token = strtok(exprCopy, " "); // İfadeyi token'lara ayır
        int isAddition = 1; // Başlangıçta toplama işlemi varsayılsın

        while (token != NULL) { // Tüm token'ları işleyene kadar devam et
            if (strcmp(token, "Keyword(is)") == 0) {
                // 'is' keywordünü atla ve bir sonraki token'a geç
                token = strtok(NULL, " ");
                continue;
            } else if (strcmp(token, "Operator(+)") == 0) {
                // Eğer operatör + ise sonuca ekleme işlemi yap
                isAddition = 1;
            } else if (strcmp(token, "Operator(-)") == 0) {
                // Eğer operatör - ise sonuca çıkarma işlemi yap
                isAddition = 0;
            } else if (strncmp(token, "String(", 7) == 0) {
                char str[MAX_STR_LEN] = "";
                // String'in tamamını al
                size_t len = strlen(token);
                if (token[len - 1] != ')') {
                    // Eğer token ')' ile bitmiyorsa, devamındaki token'ları birleştir
                    strcat(str, token + 7); // "String(" kısmını atla
                    while ((token = strtok(NULL, " ")) != NULL) {
                        len = strlen(token);
                        strcat(str, " ");
                        strcat(str, token);
                        if (token[len - 1] == ')') {
                            str[strlen(str) - 1] = '\0'; // Kapanış parantezini kaldır
                            break;
                        }
                    }
                } else {
                    // Eğer token ')' ile bitiyorsa, sadece string içeriğini al
                    sscanf(token, "String(\"%255[^\"]", str); // String değerini al
                }

                // Son string'deki " karakterlerini kaldır
                char tempStr[MAX_STR_LEN] = "";
                char* p = str;
                char* q = tempStr;
                while (*p) {
                    if (*p != '\"') {
                        *q++ = *p;
                    }
                    p++;
                }
                *q = '\0';

                if (isAddition) {
                    strcat(result, tempStr); // Sonuca ekle
                } else {
                    char* removedStr = subtractStrings(result, tempStr); // Sonucu güncelle
                    strcpy(result, removedStr);
                    free(removedStr);
                }
            } else if (strncmp(token, "Identifier(", 11) == 0) {
                char otherVarName[32];
                sscanf(token, "Identifier(%31[^)])", otherVarName); // Diğer değişken ismini al
                Variable* other = getVariable(otherVarName); // Diğer değişkeni getir
                if (other != NULL && other->type == TEXT) {
                    if (isAddition) {
                        strcat(result, other->strValue); // Diğer değişkenin değerini sonuca ekle
                    } else {
                        char* removedStr = subtractStrings(result, other->strValue); // Sonucu güncelle
                        strcpy(result, removedStr);
                        free(removedStr);
                    }
                } else {
                    printf("Error: Invalid text assignment for %s.\n", otherVarName);
                    free(exprCopy); // Hatalı durumda belleği temizle ve çık
                    return;
                }
            } else {
                printf("Error: Unexpected token %s.\n", token);
                free(exprCopy); // Beklenmeyen token durumunda çık
                return;
            }
            token = strtok(NULL, " "); // Bir sonraki token'a geç
        }
        strcpy(var->strValue, result); // Sonucu değişkene ata
        printf("Assigned \"%s\" to variable %s\n", var->strValue, varName);
        free(exprCopy); // Belleği temizle
    }
    
    else {
        printf("Error: Unsupported variable type for %s.\n", varName);
    }
}

void handleIO(const char* command, const char* line) {
    // Eğer komut "Keyword(write)" ise
    if (strcmp(command, "Keyword(write)") == 0) {
        // Satırı parçalara ayır
        char* token = strtok((char*)line, " ,.");
        // Parçaları tara
        while (token != NULL) {
            // Eğer parça "String(" ile başlıyorsa
            if (strncmp(token, "String(", 7) == 0) {
                // String değerini al
                char str[MAX_STR_LEN];
                if (sscanf(token, "String(\"%255[^\"])\"", str) == 1) {
                    printf("%s", str);
                } else {
                    printf(" ");
                }

            // Eğer parça "Identifier(" ile başlıyorsa
            } else if (strncmp(token, "Identifier(", 11) == 0) {
                // Değişken adını al
                char varName[32];
                sscanf(token, "Identifier(%31[^)])", varName);
                // Değişkeni al
                Variable* var = getVariable(varName);
                // Eğer değişken varsa
                if (var != NULL) {
                    // Eğer değişken türü INT ise
                    if (var->type == INT) printf("%d", var->intValue);
                    // Değilse, string ise
                    else printf("%s", var->strValue);
                } else {
                    // Hata: Tanımlanmamış değişken
                    printf("Error: Tanımlanmamış değişken %s.\n", varName);
                }
            }
            // Bir sonraki parçaya geç
            token = strtok(NULL, " ,.");
        }
        // Yeni satıra geç
        printf("\n");

    // Eğer komut "Keyword(read)" ise
    } else if (strcmp(command, "Keyword(read)") == 0) {
        // İstenen bilgiyi ve değişken adını al
        char prompt[MAX_STR_LEN], varName[32];
        sscanf(line, "String(\"%255[^\"]\") Identifier(%31[^)])", prompt, varName);
        // Değişkeni al
        Variable* var = getVariable(varName);
        // Eğer değişken yoksa
        if (var == NULL) {
            // Hata: Tanımlanmamış değişken
            printf("Error: Tanımlanmamış değişken %s.\n", varName);
            return;
        }
        // Kullanıcıya istenen bilgiyi göster
        printf("%s", prompt);
        // Eğer değişken türü INT ise
        if (var->type == INT) {
            // INT tipinde giriş al
            if (scanf("%d", &var->intValue) != 1) {
                // Uyarı: Geçersiz giriş
                printf("Warning: Geçersiz tam sayı girişi. %s için 0 atandı.\n", varName);
                var->intValue = 0;
            }
        // Değilse, string ise
        } else {
            scanf("%s", var->strValue);
        }

    // Eğer komut "Keyword(newLine)" ise
    } else if (strcmp(command, "Keyword(newLine)") == 0) {
        // Yeni satıra geç
        printf("\n");
    }
}


// handleLoop fonksiyonu
void handleLoop(int loopCount, const char* loopBody) {
    char loopBodyCopy[4096];
    strcpy(loopBodyCopy, loopBody);
    int incrementValue = 1; // Varsayılan artış miktarı

    // Döngü gövdesini tarayarak 'i' değişkeninin artış miktarını belirle
    char* token = strtok(loopBodyCopy, " ");
    while (token != NULL) {
        if (strncmp(token, "Identifier(i)", 13) == 0) {
            char* nextToken = strtok(NULL, " ");
            if (nextToken != NULL && strcmp(nextToken, "Keyword(is)") == 0) {
                nextToken = strtok(NULL, " ");
                if (nextToken != NULL && strncmp(nextToken, "Identifier(i)", 13) == 0) {
                    nextToken = strtok(NULL, " ");
                    if (nextToken != NULL && strcmp(nextToken, "Operator(+)") == 0) {
                        nextToken = strtok(NULL, " ");
                        if (nextToken != NULL && strncmp(nextToken, "IntConst(", 9) == 0) {
                            sscanf(nextToken, "IntConst(%d)", &incrementValue);
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }

    for (int i = 0; i < loopCount; i++) {
        printf("Loop iteration %d:\n", i + 1);

        strcpy(loopBodyCopy, loopBody); // loopBodyCopy'yi tekrar doldur
        char* token = strtok(loopBodyCopy, " ");
        char statement[1024] = {0};
        int inStatement = 0;
        int inInnerLoop = 0;
        int innerLoopActive = 0;
        char innerLoopBody[4096] = {0};
        int innerLoopCount = 0;
        int loopKeywordEncountered = 0; // iç döngü bayrağı

        while (token != NULL) {
            if (innerLoopActive) {
                if (strcmp(token, "LeftCurlyBracket") == 0) {
                    inInnerLoop = 1;
                } else if (strcmp(token, "RightCurlyBracket") == 0) {
                    inInnerLoop = 0;
                    innerLoopActive = 0;
                    handleLoop(innerLoopCount, innerLoopBody); // İç döngüyü işle
                    innerLoopBody[0] = '\0'; // İç döngü gövdesini sıfırla
                    token = strtok(NULL, " ");
                    continue;
                }
                strcat(innerLoopBody, token);
                strcat(innerLoopBody, " ");
            } else if (strncmp(token, "Keyword(loop)", 13) == 0) {

                loopKeywordEncountered = 1; // iç döngü bayrağını etkinleştir
                token = strtok(NULL, " ");
                if (token != NULL && strncmp(token, "IntConst(", 9) == 0) {
                    sscanf(token, "IntConst(%d)", &innerLoopCount);
                    // Ardından Keyword(times) tokenini bekle
                    token = strtok(NULL, " ");
                    if (token != NULL && strcmp(token, "Keyword(times)") == 0) {
                        innerLoopActive = 1;
                        token = strtok(NULL, " ");
                        continue;
                    }
                }
            } else if (strncmp(token, "Keyword(times)", 14) == 0) {
                // İç döngü bayrağını kontrol et
                if (loopKeywordEncountered) {
                    loopKeywordEncountered = 0; // iç döngü bayrağını devre dışı bırak
                    inStatement = 1; // iç döngü gövdesini toplamaya başla
                }
            } else if (strncmp(token, "LeftCurlyBracket", 16) == 0) {
                inInnerLoop = 1;
            } else if (strncmp(token, "Keyword", 7) == 0) {
                inStatement = 1;
            }

            if (inStatement && !inInnerLoop && !innerLoopActive) {
                strcat(statement, token);
                strcat(statement, " ");
                if (strcmp(token, "EndOfLine") == 0) {
                    execute(statement);
                    statement[0] = '\0';
                    inStatement = 0;
                }
            }

            token = strtok(NULL, " ");
        }

        if (inStatement && !innerLoopActive) {
            execute(statement);
        }

        // Her iterasyon sonunda Identifier(i) değerini incrementValue kadar artır
        Variable *var = getVariable("i");
        if (var != NULL) {
            var->intValue += incrementValue;
        } else {
            printf("Error: Variable 'i' is not declared.\n");
        }
    }
}

void handleIsStatement(const char* line) {
    char varName[32], expression[256];
    sscanf(line, " Identifier(%31[^)]) Operator(is) %255[^\n]", varName, expression);
    handleAssignment(varName, expression);
}


void execute(const char* line) {

    if (strncmp(line, "Keyword(write)", 14) == 0) {
        handleIO("Keyword(write)", line + 14);
    } else if (strncmp(line, "Keyword(int)", 12) == 0) {
        handleDeclaration(line);
    } else if (strncmp(line, "Keyword(text)", 13) == 0) {
        handleDeclaration(line);
    } else if (strncmp(line, "Keyword(read)", 13) == 0) {
        char command[32], rest[256];
        sscanf(line, "%31s %255[^\n]", command, rest);
        handleIO(command, rest);
    } else if (strncmp(line, "Keyword(newLine)", 16) == 0) {
        handleIO("Keyword(newLine)", NULL);
    } else if (strncmp(line, "Keyword(loop)", 13) == 0) {
        int loopCount;
        const char* loopCountStart = strstr(line, "IntConst(") + strlen("IntConst(");
        sscanf(loopCountStart, "%d", &loopCount);

        const char* loopBodyStart = strstr(line, "LeftCurlyBracket") + strlen("LeftCurlyBracket");
        const char* loopBodyEnd = strrchr(line, 'R'); // "RightCurlyBracket" yerine son 'R' karakterini bulma
        if (loopBodyEnd != NULL) {
            char loopBody[4096];
            strncpy(loopBody, loopBodyStart, loopBodyEnd - loopBodyStart - 1); // Sağ küme parantezden bir önceki karaktere kadar al
            loopBody[loopBodyEnd - loopBodyStart - 1] = '\0';

            handleLoop(loopCount, loopBody);
        } else {
            printf("Error: Missing RightCurlyBracket in loop.\n");
        }
    } else if (strncmp(line, "Keyword(is)", 11) == 0) {
        handleIsStatement(line + 11);
    } else if (strstr(line, "Keyword(is)") != NULL) {
        char varName[32], expression[256];
        sscanf(line, "Identifier(%31[^)]) Keyword(is) %255[^\n]", varName, expression);
        handleAssignment(varName, expression);
    } else {
        printf("Error: Unrecognized keyword.\n");
    }
}



void removeComments(char* line) {
    char* start = strstr(line, "/*");
    while (start != NULL) {
        char* end = strstr(start + 2, "*/");
        if (end != NULL) {
            memmove(start, end + 2, strlen(end + 2) + 1);
        } else {
            *start = '\0';
        }
        start = strstr(line, "/*");
    }
}

int Interpreter() {
    FILE* file = fopen("code.lex", "r");
    if (!file) {
        printf("Error: Could not open file code.lex.\n");
        return 1;
    }

    char token[256];
    char line[1024] = {0};
    int inLoop = 0;
    char loopBody[4096] = {0};

    while (fgets(token, sizeof(token), file)) {
        removeComments(token);
        strtok(token, "\n");
        if (strlen(token) == 0) continue;

        if (strncmp(token, "Keyword(loop)", 13) == 0) {
            inLoop = 1;
            strcat(line, token);
        } else if (inLoop) {
            strcat(loopBody, " ");
            strcat(loopBody, token);
            if (strstr(token, "RightCurlyBracket") != NULL) {
                inLoop = 0;
                strcat(line, " ");
                strcat(line, loopBody);
                execute(line);
                line[0] = '\0';
                loopBody[0] = '\0';
            }
        } else {
            if (strcmp(token, "EndOfLine") == 0) {
                if (strlen(line) > 0) {
                    execute(line);
                    line[0] = '\0';
                }
            } else {
                if (strlen(line) > 0) strcat(line, " ");
                strcat(line, token);
            }
        }
    }

    if (strlen(line) > 0) {
        execute(line);
    }

    fclose(file);
    return 0;
}


int main() {
    /* Open input and output files */
    FILE *inputFile = fopen("code.sta", "r");    // Open input file
    FILE *outputFile = fopen("code.lex", "w");   // Open output file

    if (inputFile == NULL || outputFile == NULL) {   // File opening error check
        printf("Error opening files.\n");     // Error message
        return -1;                            // Exit with error code
    }

    /* Read tokens and write to output file */
    Token token;
    int endOfLineCount = 0;   // End of line counter
    do {
        token = getNextToken(inputFile);   // Get the next token
        if (token.type != END_OF_LINE) {   // If the token is not an end of line
            if (endOfLineCount > 0) {      // If the previous token was an end of line
                printToken((Token){END_OF_LINE, ""});   // Write the end of line token
            }
            printToken(token);              // Write the token
            endOfLineCount = 0;            // Reset the end of line counter
        } else {
            endOfLineCount++;              // If the token is an end of line, increment the counter
        }
    } while (!feof(inputFile));            // Continue until the end of the file

    if (endOfLineCount > 0) {              // If there is an end of line at the end of the file
        printToken((Token){END_OF_LINE, ""});   // Write the end of line token
    }

    /* Close files */
    fclose(inputFile);                     // Close the input file
    fclose(outputFile);                    // Close the output file

    Interpreter();

    return 0;                              // Exit successfully
}