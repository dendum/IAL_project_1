/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Function to determine the precedence of an operator.
 *
 * This function takes an operator character as input and returns its precedence value.
 * Precedence values: '+' and '-' have a precedence of 1, '*' and '/' have a precedence of 2,
 * and all other characters have a precedence of -1.
 *
 * @param operator The operator character for which precedence needs to be determined.
 * @return The precedence value of the operator.
 */
int precedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return -1;
    }
}


/**
 * Function to check if a character is an operator.
 * This function takes a character as input and checks
 * if it is one of the four supported operators: '+', '-', '*', or '/'.
 *
 * @param ch The character to be checked.
 * @return true if the character is an operator, false otherwise.
 */
bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

/**
 * Function to check whether a character is a letter (lowercase or uppercase) or a digit (0-9).
 *
 * @param c The character to be checked.
 * @return true if the character is alphanumeric, false otherwise.
 */
bool isAlphanumeric(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar(Stack *stack, char *postfixExpression, int *postfixExpressionLength) {
    char top;
    Stack_Top(stack, &top);
    while (!Stack_IsEmpty(stack) && top != '(') {
        postfixExpression[(*postfixExpressionLength)++] = top;
        Stack_Pop(stack);
        if (!Stack_IsEmpty(stack)) {
            Stack_Top(stack, &top);
        }
    }
    Stack_Pop(stack);
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation(Stack *stack, char c, char *postfixExpression, int *postfixExpressionLength) {
    char top;
    Stack_Top(stack, &top);
    while (!Stack_IsEmpty(stack) && precedence(top) >= precedence(c)) {
        postfixExpression[(*postfixExpressionLength)++] = top;
        Stack_Pop(stack);
        if (!Stack_IsEmpty(stack)) {
            Stack_Top(stack, &top);
        }
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix(const char *infixExpression) {
    int i, j;
    char top;

    char *postfix = (char *) malloc(sizeof(char) * MAX_LEN);
    if (postfix == NULL) {
        return NULL;
    }

    Stack _stack;
    Stack *stack = &_stack;
    Stack_Init(stack);

    for (i = 0, j = 0; infixExpression[i]; ++i) {
        if (infixExpression[i] == ' ' || infixExpression[i] == '\t')
            continue;

        if (isAlphanumeric(infixExpression[i])) {
            postfix[j++] = infixExpression[i];
        } else if (infixExpression[i] == '(') {
            Stack_Push(stack, infixExpression[i]);
        } else if (infixExpression[i] == ')') {
            untilLeftPar(stack, postfix, &j);
        } else if (isOperator(infixExpression[i])) {
            if (!Stack_IsEmpty(stack)) {
                doOperation(stack, infixExpression[i], postfix, &j);
            }
            Stack_Push(stack, infixExpression[i]);
        }
    }

    while (!Stack_IsEmpty(stack)) {
        Stack_Top(stack, &top);
        postfix[j++] = top;
        Stack_Pop(stack);
    }
    postfix[j++] = '=';
    postfix[j] = '\0';

    Stack_Dispose(stack);

    return postfix;
}

/**
 * This function searches for a variable with a specified name in the provided array of VariableValue structures
 * and returns its corresponding value.
 *
 * @param values An array of VariableValue structures.
 * @param numValues The number of VariableValue structures in the array.
 * @param variableName The name of the variable to find.
 * @return The value of the variable if found; otherwise, it returns 0.
 */
int findVariableValue(const VariableValue values[], int numValues, char variableName) {
    for (int i = 0; i < numValues; i++) {
        if (values[i].name == variableName) {
            return values[i].value;
        }
    }
    return 0;
}

/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push(Stack *stack, int value) {
    stack->topIndex++;
    int *intPtr = (int *) &stack->array[(stack->topIndex) * sizeof(int)];
    *intPtr = value;
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop(Stack *stack, int *value) {
    if (stack->topIndex >= 0) {
        int *intPtr = (int *) &stack->array[stack->topIndex * sizeof(int)];
        *value = *intPtr;
        stack->topIndex--;
    } else {
        *value = 0;
    }
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval(const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value) {
    char *postfix = infix2postfix(infixExpression);
    if (postfix == NULL) {
        return false;
    }

    Stack _stack;
    Stack *stack = &_stack;
    Stack_Init(stack);

    for (int i = 0; postfix[i]; i++) {
        if (postfix[i] == '=')
            continue;

        if (isAlphanumeric(postfix[i])) {
            if ((postfix[i] >= '0' && postfix[i] <= '9')) {
                expr_value_push(stack, postfix[i] - '0');
            } else {
                expr_value_push(stack, findVariableValue(variableValues, variableValueCount, postfix[i]));
            }
        } else {
            int operand2;
            int operand1;
            expr_value_pop(stack, &operand2);
            expr_value_pop(stack, &operand1);

            switch (postfix[i]) {
                case '+':
                    expr_value_push(stack, operand1 + operand2);
                    break;
                case '-':
                    expr_value_push(stack, operand1 - operand2);
                    break;
                case '*':
                    expr_value_push(stack, operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) {
                        Stack_Dispose(stack);
                        free(postfix);
                        return false;
                    }
                    expr_value_push(stack, operand1 / operand2);
                    break;
            }
        }
    }

    expr_value_pop(stack, value);
    Stack_Dispose(stack);
    free(postfix);

    return true;
}

/* Konec c204.c */
