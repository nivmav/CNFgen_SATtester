#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "CNF.h"

/*
 * This program implements the PL-Resolution algorithm
 */


/*
 * This function checks if there exists a clause
 * which contains a literal and its complementary.
 * If so, this clause would always retrun true, so we will
 * discard it.
 */

Clause new_clauseRes(Clause new_clause)
{
    Clause temp;
    Literal nlit, lit, lit1;
    int found = 0;
    Clause cl;

    nlit = new_clause->negated_lit;
    lit = new_clause->lit;

    while(nlit){
        while(lit){
            if(strcmp(nlit->value, lit->value) == 0){
                found = 1;
                break;
            }
            lit = lit->next;
        }
        if(found) {
            break;
        }
        nlit = nlit->next;
        lit = new_clause->lit;
    }
    if(found) {
        /*
         * Delete the node
         */
        new_clause->no_add = 1;
    }
    return new_clause;
}

/*
 * This function would create new clauses
 * The calling function is the resolve function
 * the arguments for this function are the 
 * cl1 & cl2: two clauses which were resolved, 
 * lit: the literal which is getting removed,
 * new: the root of the new clause link 
 * flag: if zero says the literal exists as a negated one in cl2
 *       if one says the literal exists as negated in cl1 clause 
 * return value:
 * true: if new_clause was formed
 * false: If an empty clause
 */

Clause create_new_clause(Clause cl1, Clause cl2, Literal lit, int *res, Clause new, int flag)
{
    char *copy_cl1 = (char *)malloc((strlen(cl1->clause) + 1) * sizeof(char *));
    char *copy_cl2 = (char *)malloc((strlen(cl2->clause) + 1) * sizeof(char *));
    char *tok, *copy_tok, *tok1;
    char value[10];
    int len = 0;
    Clause new_clause, temp, prev;

    if(new == NULL) {
        new = initialize_clause();
        temp = new;
    } else if(new->next == NULL){
        temp = initialize_clause();
        new->end_ptr = temp;
        new->next = temp;
    } else{
        temp = initialize_clause();
        new->end_ptr->next = temp;
        new->end_ptr = temp;
    }
    new_clause = temp;

    /*
     * need to add the new clause
     * and this clause should not have the literal 
     * which is being sent as an argument
     */

    
    new_clause->clause = (char*)malloc(((strlen(cl1->clause) + strlen(cl2->clause))) * sizeof(char *));
    *new_clause->clause = '\0';
    strcpy(copy_cl1, cl1->clause);


    /*
     * Second half of first clause
     */

    tok1 = copy_cl1;

    /*
     * Check if the negated literal is present
     * then we nedd to search fo "-"literal -that
     * is  -literal
     */

    if(flag) {
        strcpy(value,"-");
        strcat(value, lit->value);
    } else {
        strcpy(value, lit->value);
    }

    /*
     * Keep searching till we find the exact string
     */
    while(1) {
        tok = strstr(tok1, value);

        /*
         * Capture the value for the 
         * first half
         */
        tok1 = tok;
        len = 0;
        while(len < strlen(value)) {
            ++tok;
            len++;
        }
        if(*tok == 'v' || *tok == '\0'){
            if(*tok == 'v') {
                ++tok;
            }
            break;
        } else {
            tok1 = tok;
            continue;
        }
    }
    copy_tok = (char*)malloc((strlen(cl1->clause)) * sizeof(char *));
    strcpy(copy_tok, tok);

    /*
     * first half
     */

    *tok1 = '\0';
    
    if(*copy_cl1 != '\0' && *copy_tok == '\0'){
        tok = copy_cl1;
        len = 0;
        while(len < (strlen(copy_cl1) - 1)) {
            ++tok;
            len++;
        }
        *tok = '\0';
    }

    if(*copy_cl1 != '\0') {
        strcpy(new_clause->clause, copy_cl1);
        if(*copy_tok != '\0'){
            strcat(new_clause->clause, copy_tok);
        }
    }else if (*copy_tok != '\0'){
        strcpy(new_clause->clause, copy_tok);
    }
    free(copy_tok);

    /*
     * second clause
     * second half
     */

    strcpy(copy_cl2, cl2->clause);
    if(!flag) {
        strcpy(value,"-");
        strcat(value, lit->value);
    } else {
        strcpy(value, lit->value);
    }
    tok1 = copy_cl2;
    /*
     * Keep searching till we find the exact string
     */
    while(1) {
        tok = strstr(tok1, value);

        /*
         * Capture the value for the 
         * first half
         */
        tok1 = tok;
        len = 0;
        while(len < strlen(value)) {
            ++tok;
            len++;
        }
        if(*tok == 'v' || *tok == '\0'){
            if(*tok == 'v') {
                ++tok;
            }
            break;
        } else {
            tok1 = tok;
            continue;
        }
    }
    copy_tok = (char*)malloc((strlen(cl2->clause)) * sizeof(char *));
    strcpy(copy_tok, tok);

    /*
     * First half
     */
    *tok1 = '\0';
    
     if(*copy_cl2 != '\0' && *copy_tok == '\0'){
        tok = copy_cl2;
        len = 0;
        while(len < (strlen(copy_cl2) - 1)) {
            ++tok;
            len++;
        }
        *tok = '\0';
    }

    if(*new_clause->clause == '\0' && *copy_cl2 == '\0' && *copy_tok == '\0'){
        *res = 0;
        return new;
    }else if(*new_clause->clause == '\0'){
        if(*copy_cl2 != '\0'){
            strcpy(new_clause->clause, copy_cl2);
            if(*copy_tok != '\0') {
                strcat(new_clause->clause, copy_tok);
            }
        }else if(*copy_tok != '\0'){
            strcat(new_clause->clause, "v");
            strcpy(new_clause->clause, copy_tok);
        }
    }else {
        if(*copy_cl2 != '\0'){
            strcat(new_clause->clause, "v");
            strcat(new_clause->clause, copy_cl2);
            if(*copy_tok != '\0') {
                strcat(new_clause->clause, copy_tok);
            }
        }else if(*copy_tok != '\0'){
            strcat(new_clause->clause, "v");
            strcat(new_clause->clause, copy_tok);
        }
    }
    add_Literal(new_clause);

    /*
     * Check if a clause has a literal and its complementary
     * both
     */
    new_clause = new_clauseRes(new_clause);
    //printf("\n\nThis is all clauses are not added yet \n\n");
    //status_print(new);
    //print_status(new);
    free(copy_cl1);
    free(copy_cl2);
    free(copy_tok);

    return new;
}

/*
 * This function would resolve the clauses
 * cl1 and cl2. 
 * It checks if there are any literal and its negated
 * literal is present in the clauses given. If so, calls
 * create_new_clause. If there is nothing to resolve then 
 * I would just ignore those clauses and nothing new to add
 * IMP : the res bool value does not reflect on this.
 * The return value: false if we encounter an empty set, after resolving
 * else return true;
 */

Clause resolve(Clause cl1, Clause cl2, int *res, Clause new)
{
    Literal n_lit, lit;
    
    *res = 1;
    /*
     * Comparing the negated literals of clause one
     * with the non negated literals of clause two
     */

    n_lit = cl1->negated_lit;
    lit = cl2->lit;
    while(n_lit){
        while(lit){
            if(strcmp(n_lit->value, lit->value) == 0) {
                new = create_new_clause(cl1,cl2,n_lit, res, new, 1);
                if(*res == 0) {
                    return new;
                }
                break;
            }
            lit = lit->next;
        }
        lit = cl2->lit;
        n_lit = n_lit->next;
    }

    /* 
     * Comparing the negated literals of clause 2
     * with non-negated literals of clause 1
     */
    n_lit = cl2->negated_lit;
    lit = cl1->lit;
    while(n_lit){
        while(lit){ 
            if(strcmp(n_lit->value, lit->value) == 0) {
                new = create_new_clause(cl1,cl2,n_lit, res, new, 0);
                if(*res == 0) {
                    return new;
                }
                break;
            }
            lit = lit->next;
        }
        lit = cl1->lit;
        n_lit = n_lit->next;
    }
    return new;
}


bool is_literals_equal(Clause main_clause, Clause new_clause)
{
    Literal nlit_m, lit_m, nlit_n, lit_n;

    if(strlen(main_clause->clause) != strlen(new_clause->clause)){
        return false;
    }

    nlit_m = main_clause->negated_lit;
    nlit_n = new_clause->negated_lit;

    while(nlit_m){
        while(nlit_n){
            if(strcmp(nlit_m->value, nlit_n->value) && !nlit_n->next){
                return false;
            }else if(strcmp(nlit_m->value, nlit_n->value) == 0){
                break;
            }
            nlit_n = nlit_n->next;
        }
        nlit_m = nlit_m->next;
        nlit_n = new_clause->negated_lit;
    }

    lit_m = main_clause->lit;
    lit_n = new_clause->lit;
    while(lit_m) {
        while(lit_n){
            if(strcmp(lit_m->value, lit_n->value) && !lit_n->next){
                return false;
            } else if(strcmp(lit_m->value, lit_n->value) == 0){
                break;
            }
            lit_n = lit_n->next;
        }
        lit_m = lit_m->next;
        lit_n = new_clause->lit;
    }

    return true;
}

/*
 * This function would check the new_clauses and
 * deletes those that already exist and rest gets appeneded
 * to the main list of clauses
 */

bool add_new_clause(Clause new)
{
    Clause temp, prev;
    Clause temp_main;
    Literal lit, lit1;

    temp = new;
    prev = temp;
    temp_main = root_clause;
    while(temp_main->next){
        temp_main = temp_main->next;
    }
    while(temp) {

        if(temp->no_add == 1){
            if(temp == new) {
                new = temp->next;
                
                free(temp->clause);
                lit = temp->negated_lit;
                while(lit){
                    lit1 = lit;
                    lit = lit->next;
                    free(lit1->value);
                    free(lit1);
                }
                lit = temp->lit;
                while(lit){
                    lit1 = lit;
                    lit = lit->next;
                    free(lit1->value);
                    free(lit1);
                }
                free(temp);

                temp = new;
            }else {
                prev->next = temp->next;
                
                free(temp->clause);
                lit = temp->negated_lit;
                while(lit){
                    lit1 = lit;
                    lit = lit->next;
                    free(lit1->value);
                    free(lit1);
                }
                lit = temp->lit;
                while(lit){
                    lit1 = lit;
                    lit = lit->next;
                    free(lit1->value);
                    free(lit1);
                }
                free(temp);

                temp = prev->next;
            }
        } else {
            prev = temp;
            temp = temp->next;
        }
    }

    if(!new){
        return true;
    } else {
        temp_main->next = new;
        new->end_ptr = NULL;
        //status_print(new);
        return false;
    }
}


/*
 * This function compares the existing clauses with the newly created clauses
 * if there are no new clauses to add then it returns true
 */

bool check_new_clauses(Clause new)
{
    Clause main_clause, new_clause;
    bool res = false;

    if(!new) {
        return true;
    }

    main_clause = root_clause;
    new_clause = new;
    
    while(main_clause) {
        while(new_clause) {
            if(new_clause->no_add) {
                new_clause = new_clause->next;
                continue;
            }
            res = is_literals_equal(main_clause, new_clause);
            if(res){
                /*
                 * the clause is marked as not to be added
                 */
                new_clause->no_add = 1;
            }
            new_clause = new_clause->next;
        }
        main_clause = main_clause->next;
        new_clause = new;
    }
    /*
     * add the clause which are new to the main link
     */
    res = add_new_clause(new);

    if(res) {
        /*
         * No new clause found return true
         */
        return true;
    }
    return false;
}


/*
 * The function compares every clause pair that can
 * be formed and calls the resolve function
 * the "res" value gets filled by two functions
 * res: is set to false if we come across any empty set
 * res: is set to true if we find no new clauses at the end of
 * comparing all the sets
 */

bool compare()
{
    Clause cl1;
    Clause cl2;
    Clause new = NULL;
    int res = 0;
    bool result = false;

    /*
     * Keep executing the loop till we come across either 
     * empty clause or we have no new clause.
     */
    while(1) {
        cl1 = root_clause;
        cl2= root_clause->next;
        while(cl1){
            while(cl2){
                new =  resolve(cl1, cl2, &res, new);

                /*
                 * Empty clause encountered
                 * sentence not satisfiable
                 */
                if(!res){
                    return false;
                }
                cl2 = cl2->next;
            }
            cl1 = cl1->next;
            if(cl1->next == NULL){
                break;
            }
            cl2 = cl1->next;
        }


        result = check_new_clauses(new);

        /*
         * No new clauses found so the sentence 
        * is satisfiable
        */

        if(result){
            return true;
        }else {
            new = NULL;
            continue;
        }
    }

}





int main()
{
    /*
     * I ll call the CNF generator for some random sentence 
     */
    Clause temp;
    int rel;
    bool res;
    int i,j, num_ppl, tables, r_sen;
    int sat = 0;
    int nsat = 0;
    float pf, pe;
    int ans;
    srand(time(NULL));
    FILE *fid;



    /*
     * Ask the User if he wants to generate a random sentence
     */

    printf("\nIf you want to generate a single sentence press 1 else press 2 to generate random sentences?\n");
    scanf("%d", &ans);

    if(ans == 1){
        printf("Enter the number of people you wish to have in the wedding\n");
        scanf("%d", &num_ppl);
        
        printf("Enter the number of tables you wish to have in the wedding\n");
        scanf("%d", &tables);

        printf("Enter the percentage probability (please enter only numbers out of 100) of two people being friends\n");
        scanf("%f", &pf);
        if(pf > 1){
            pf = pf/100;
        }

        printf("Enter the percentage probability (please enter only numbers out of 100) of two people being enemies\n");
        scanf("%f", &pe);
        if(pe > 1){
            pe = pe/100;
        }

        printf("Encoding ...\n");

        CNF_GEN(num_ppl, tables, pe, pf);
        res = compare();
        if(res){
            printf("Its satisfiable \n");
            sat++;
        } else {
            printf("Not Satisfiable \n");
            nsat++;
        }
        clear_CNF();

    } else if(ans == 2) {
        printf("Creating automated random sentences\n");
        printf("Please enter the number of Random sentenses to be generated\n");
        scanf("%d", &r_sen);
        printf("Please enter the percentage probability (enter only numbers out of 100) of two ppl being enemies\n");
        scanf("%f", &pe);
        if(pe > 1){
            pe = pe/100;
        }
        printf("Assuming friends = 0, number of people = 16 and table =2\nTesting satisfiability\n");
        printf("PL-Resolution is a long running proccess please wait...\n");
        /*
        * Creating a random sequences of
        * sentences for e varying from 
        * 0.02 to .2
        */
        fid = fopen("PL-ResolutionOutput.txt", "w");
        fprintf(fid, "\nGiven two people are enemies with a probability %f\n", pe);
        fprintf(fid, "Generating....\n");
        for(j = 1; j <= r_sen; j++ ) {
            fprintf(fid, "Case%d:\n", j);
            CNF_GEN(16,2,pe,0.0);
            res = compare();

            if(res){
                fprintf(fid, "Its satisfiable \n");
                sat++;
            } else {
                fprintf(fid, "Not Satisfiable \n");
                nsat++;
            }
            clear_CNF();
        }
        fprintf(fid, "The number of Satisfiable sentences %d\n\n", sat);
        fprintf(fid, "The number of Not-Satisfiable sentences %d\n\n", nsat);
        fclose(fid);
        printf("Please find the out put in the file PL-ResolutionOutput.txt, int the current directory\n");
    }else{
        printf("Please answer either yes or no\n");
    }
    return 1;
}

