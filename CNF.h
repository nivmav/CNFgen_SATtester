#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include<string.h>


/*
 * New thinking this should work
 * Lets create a linked list for clauses, 
 * the next would point to another clause, 
 * no more discretion based on the relationshipi.
 * Then each clause points to a linked list which 
 * contains a linked list that has literals divided into 
 * positive and negative literals.
 */

typedef struct literal *Literal;
typedef struct clause *Clause;

/*
 * The structure which will hold the literals 
 * for each clause.
 * It will have a list for negative literals 
 * and positive literals
 */

struct literal{
    char *value;
    Literal next;
    char assign;
    char assign_copy;
};


struct clause{
    int rel_type;
    char *clause;
    Literal lit;
    Literal negated_lit;
    int no_add;
    char assign;
    char assign_copy;
    Clause next;
    Clause end_ptr;
};

Clause root_clause;

void reconstruct(Clause cl);
void free_Clause(Clause cl);
/*
 * Initialization of each of the clause
 */

Clause initialize_clause()
{
    Clause temp;
    temp = malloc(sizeof(struct clause));
    memset(temp, 0, sizeof(struct clause));
    return temp;
}

/*
 * Initialization of each literal
 */

Literal initialize_literal()
{
    Literal temp;
    temp = malloc(sizeof(struct literal));
    memset(temp, 0, sizeof(struct literal));
    return temp;
}
/*
 * Function which checks for duplicate literals
 *
 */

int check_ifLiteral_present(Clause cl, char* val, int flag)
{
    Literal lit;

    if(flag){
        lit = cl->negated_lit;
    }else{
        lit = cl->lit;
    }
    while(lit) {
        if(strcmp(val, lit->value) == 0){
            return 1;
        }
        lit = lit->next;
    }
    return 0;
}

void add_Literal(Clause cl)
{
    char *tok;
    char *neg;
    char *copy = (char *)malloc((strlen(cl->clause)+1) * sizeof(char *));
    Literal temp, prev;
    int res = 0;
    int recons = 0;


    strcpy(copy, cl->clause);
    tok = strtok(copy, "v");
    while(tok){
        neg = strstr(tok, "-");
        if(neg) {
            ++neg;
            res = check_ifLiteral_present(cl, neg, 1);
            if(res){
                tok = strtok(NULL, "v");
                recons = 1;
                continue;
            }

            temp = initialize_literal();
            if(!cl->negated_lit) {
                cl->negated_lit = temp;
                prev = temp;
            } else {
                prev->next = temp;
            }
            temp->value = (char *)malloc(sizeof(char *) * 5);
            strcpy(temp->value, neg);
        } else {
            res = check_ifLiteral_present(cl, tok, 0);
            if(res) {
                tok = strtok(NULL, "v");
                recons = 1;
                continue;
            }
            temp = initialize_literal();
            if(!cl->lit) {
                cl->lit = temp;
                prev = temp;
            } else {
                prev->next = temp;
            }
            temp->value = (char *)malloc(sizeof(char *) * 5);
            strcpy(temp->value, tok);
        }
        tok = strtok(NULL, "v");
    }
    if(recons) {
        reconstruct(cl);
    }
    free(copy);
}

/*
 * Reconstruction of clause from literals
 */

void reconstruct(Clause cl)
{
    Literal lit;
    int neg = 0;
    
    lit = cl->negated_lit;
    if(lit) {
        neg = 1;
        strcpy(cl->clause,"-");
        while(lit) {
            strcat(cl->clause, lit->value);
            if(lit->next){
                strcat(cl->clause, "v-");
            }
            lit = lit->next;
        }
    }

    lit = cl->lit;
    if(neg && lit) {
        strcat(cl->clause,"v");
        strcat(cl->clause, lit->value);
        lit = lit->next;
    } else if(lit) {
        strcpy(cl->clause, lit->value);
        lit = lit->next;
    }
    while(lit) {
        strcat(cl->clause, "v");
        strcat(cl->clause, lit->value);
        lit = lit->next;
    }

}

void add_Clause(int rel, int A, int B, int tables)
{
    Clause temp, prev;
    char buf[10];
    int i, j, n;


    if(!root_clause) {
        root_clause = initialize_clause();
        temp = root_clause;
    } else {
        temp = root_clause;
        while(temp!=NULL){
            prev = temp;
            temp = temp->next;
        }
        temp = initialize_clause();
        prev->next = temp;
    }

    if(rel == 3) {
        temp->rel_type = 3;
        temp->clause = (char *)malloc(12);
        strcpy(temp->clause, "-A1v-A2");
        prev = temp;
        add_Literal(temp);
        temp = initialize_clause();
        prev->next = temp;
        temp->rel_type = 3;
        temp->clause = (char *)malloc(12);
        strcpy(temp->clause, "A1vA2");
        add_Literal(temp);
    }

    /*
     * A person sits in only one table
     */
    /*
     * first person
     */
    if(rel == 4) {
        temp->rel_type = 4;
        temp->clause = (char *) malloc((7 * tables));
        strcpy(temp->clause, "x");
        sprintf(buf, "%d", A);
        strcat(temp->clause, buf);
        strcat(temp->clause, "0");

        for(i = 1 ; i< tables; i++) {
            strcat(temp->clause, "vx");
            sprintf(buf, "%d", A);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", i);
            strcat(temp->clause, buf);
        }
        prev = temp;
        add_Literal(temp);
        if(tables != 1) {
            temp = initialize_clause();
            prev->next = temp;
        }

        for(i = 0; i<tables; i++) {
            for(j = i+1; j<tables; j++) {
                temp->clause = (char *) malloc( 12);
                strcpy(temp->clause, "-x");
                sprintf(buf, "%d", A);
                strcat(temp->clause, buf);
                sprintf(buf, "%d", i);
                strcat(temp->clause, buf);
                strcat(temp->clause, "v-x");
                sprintf(buf, "%d", A);
                strcat(temp->clause, buf);
                sprintf(buf, "%d", j);
                strcat(temp->clause, buf);
                add_Literal(temp);
                prev = temp;
                if((i+2 != tables)){
                    temp = initialize_clause();
                    prev->next = temp;
                }

            }
        }
    }

    if(rel == 1) {
        for(i = 0; i< tables; i++){
            temp->rel_type = rel;
            temp->clause = (char *)malloc(12);
            strcpy(temp->clause, "-x");
            sprintf(buf, "%d", A);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", i);
            strcat(temp->clause, buf);
            strcat(temp->clause, "vx");
            sprintf(buf, "%d", B);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", i);
            strcat(temp->clause, buf);
            add_Literal(temp);
            prev = temp;
            if(i + 1 != tables){
                temp = initialize_clause();
                prev->next = temp;
            }
        }
    }else if(rel == 2) {
        for(i = 0; i< tables; i++) {
            temp->rel_type = rel;
            temp->clause = (char *)malloc(12);
            strcpy(temp->clause, "-x");
            sprintf(buf, "%d", A);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", i);
            strcat(temp->clause, buf);
            strcat(temp->clause, "v-x");
            sprintf(buf, "%d", B);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", i);
            strcat(temp->clause, buf);
            add_Literal(temp);
            prev = temp;
            if(i + 1 != tables){
                temp = initialize_clause();
                prev->next = temp;
            }
        }
    } else if (rel == 0) {
        for(i = 0; i < 2; i++) {
            temp->clause = (char *)malloc(2* (tables +1));
            strcpy(temp->clause, "x");
            sprintf(buf, "%d", A);
            strcat(temp->clause, buf);
            sprintf(buf, "%d", 0);
            strcat(temp->clause, buf);
            for(j = 1; j< tables; j++) {
                strcat(temp->clause, "v");
                strcat(temp->clause, "x");
                if(i == 0){
                    sprintf(buf, "%d", A);
                } else {
                    sprintf(buf, "%d", B);
                }
                strcat(temp->clause, buf);
                sprintf(buf, "%d", j);
                strcat(temp->clause, buf);
            }
            add_Literal(temp);
            prev = temp;
            if(i + 1 != 2){
                temp = initialize_clause();
                prev->next = temp;
            }
        }
    }
}



     
/*
 * The CNF generator
 */

void CNF_GEN(int num_ppl, int tables, float e, float f) 
{
    int i, j;
    float r;
    int fr = 0 , en = 0;

    for(i = 0; i<num_ppl; i++) {
        add_Clause(4, i, 0, tables);
    }

    if(e>f){
        for(i = 0; i < num_ppl; i++) {
            for(j = i+1; j <num_ppl; j++) {
                r = rand()/(RAND_MAX + 1.0);
                if(r < f) {
                    add_Clause(1, i, j, tables);
                    fr++;
                }
                if(r > f && r <= (e+f)) {
                    add_Clause(2, i, j, tables);
                    en++;
                }
            }
        }
    }else{
        for(i = 0; i < num_ppl; i++) {
            for(j = i+1; j <num_ppl; j++) {
                r = rand()/(RAND_MAX + 1.0);
                if(r <= e) {
                    add_Clause(2, i, j, tables);
                    en++;
                }
                if(r >e && r <= (e+f)) {
                    add_Clause(1, i, j, tables);
                    fr++;
                }
            }
        }
    }

}

/*
 * This function is present to print the CNF literals 
 */

void clear_CNF()
{

    Clause temp, temp1;
    int cnt = 0;

    temp = root_clause;
    temp1 = temp;

    //printf("The clauses for a given sentence: \n");   
    while (temp != NULL) {
        //printf("%s \n", temp->clause);
        temp = temp->next;
        free_Clause(temp1);
        temp1 = temp;
        cnt++;
    }
}



void free_Clause(Clause cl)
{
    Literal lit1, lit;
    int root = 0;

    if(root_clause == cl){
        root = 1;
    }

    free(cl->clause);
    lit = cl->negated_lit;
    while(lit){
        lit1 = lit;
        lit = lit->next; 
        free(lit1->value);
        free(lit1);
    }

    lit = cl->lit;
    while(lit){
        lit1 = lit;
        lit = lit->next;
        free(lit1->value);
        free(lit1);
    }
    free(cl);
    if(root) {
        root_clause = NULL;
    }
}

void status_print(Clause cl)
{
    Clause temp;
    int cnt = 0;

    temp = cl;
    while(temp) {
        cnt++;
        printf("\n CL #%d = %s\n", cnt, temp->clause);
        temp = temp->next;
    }
}


