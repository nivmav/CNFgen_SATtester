#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "CNF.h"
/*
 * Lets see,
 * what I want, is a model which randomly assigns true or false
 * to different randomly chosen clauses
 */
int Clause_count = 0;
float cla[20];
float run[20];
int count = 0;
/*
 * Now I'll create a model function, which would randomly assign 
 * true or false
 */
void assign_literal_value(Literal lit, Clause cl, int flag)
{
    int r;
    Literal l;
    int found = 0;
    //srand(time(NULL));

    r = rand() % 2;

    if(flag) {
        if(r) {
            lit->assign = 'F';
        } else {
            lit->assign = 'T';
            cl->assign = 'T';
        }
    } else {
        if(r) {
            lit->assign = 'T';
            cl->assign = 'T';
        }else {
            lit->assign  = 'F';
        }
    }

    cl = cl->next;

    /*
     * If 0 then set to false
     */
    while(cl) {
        l = cl->negated_lit;
        while(l){
            if(strcmp(l->value, lit->value) == 0) {
                if(flag) {
                    l->assign = lit->assign;
                    if(lit->assign == 'T') {
                        cl->assign = 'T';
                    }
                }else {
                    if(lit->assign == 'T'){
                        l->assign = 'F';
                    }else {
                        l->assign = 'T';
                        cl->assign = 'T';
                    }
                }
                found = 1;
                break;
            }
            l = l->next;
        }

        if(!found) {
            l = cl->lit;
            while(l){
                if(strcmp(l->value, lit->value) == 0) {
                    if(!flag) {
                        l->assign = lit->assign;
                        if(lit->assign == 'T') {
                            cl->assign = 'T';
                        }
                    }else {
                        if(lit->assign == 'T'){
                            l->assign = 'F';
                        }else {
                            l->assign = 'T';
                            cl->assign = 'T';
                        }
                    }
                    found = 1;
                    break;
                }
                l = l->next;
            }
        }
        cl = cl->next;
        found = 0;
    }
}


Clause random_assignment()
{
    Clause cl;
    Literal lit;

    cl = root_clause;
    while(cl){
        lit = cl->negated_lit;
        while(lit){
                if((lit->assign) == 'T' || (lit->assign) == 'F'){
                lit = lit->next;
                continue;
            }

            assign_literal_value(lit, cl, 1);
            lit = lit->next;
        }

        lit = cl->lit;
        while(lit){
             if((lit->assign) == 'T' || (lit->assign) == 'F'){
                lit = lit->next;
                continue;
            }
            assign_literal_value(lit, cl, 0);
            lit = lit->next;
        }

        cl = cl->next;
    }
}

int model_satisfies()
{
    Clause cl;
    Literal l;

    cl = root_clause;
    while(cl) {

        /*
        l = cl->negated_lit;
        while(l) {
            if(l->value == 'T'){
                cl->assign = 'T';
                break;
            }
            l = l->next;
        }*/
        if(cl->assign != 'T') {
            return 0;
        }
        cl = cl->next;
    }
    return 1;
}

void total_clauses()
{
    Clause cl;
    Literal lit;

    cl = root_clause;
    while(cl){
        lit = cl->negated_lit;
        while(lit){
            lit = lit->next;
        }

        lit = cl->lit;
        while(lit){
            lit = lit->next;
        }
        cl = cl->next;
        Clause_count++;
    }
}

/*
 * Select a clause at Random
 * but the clause selected should be false
 */
Clause select_clause_atRandom()
{
    int r, i;
    Clause cl;
    int cnt = 0;

    while(1) {
        r = rand() % Clause_count + 1;
        cl = root_clause;
        for(i = 1 ;i < r; i++) {
            cl = cl->next;
        }
        if(cl->assign == 'T') {
            /*
             * do all over again
            */
            continue;
        } else{
            return cl;
        }
    }
}

void clAffected(Clause cl, int flag)
{
    Literal lit;

    lit = cl->negated_lit;
    if(flag) {
        cl->assign_copy = 'F';
    }else{
        cl->assign = 'F';
    }
    while(lit) {
        if(flag) {
            if(lit->assign_copy == 'T') {
                cl->assign_copy = 'T';
                break;
            }
        }else {
            if(lit->assign == 'T') {
                cl->assign = 'T';
                break;
            }
        }
        lit = lit->next;
    }
    lit = cl->lit;
    while(lit) {
        if(flag) {
            if(lit->assign_copy == 'T') {
                cl->assign_copy = 'T';
                break;
            }
        }else {
            if(lit->assign == 'T') {
                cl->assign = 'T';
                break;
            }
        }
        lit = lit->next;
    }
}


void change_values(Literal lit, int flag)
{
    Clause cl;
    Literal l;
    int found = 0;

    cl = root_clause;
    while(cl) {
        l = cl->negated_lit;
        while(l){
            if(strcmp(l->value, lit->value) == 0) {
                if(flag) {
                    /*
                    * assign the same value;
                    * also check would this affect the 
                    * clause
                    */
                    l->assign = lit->assign;
                }else {
                        if(lit->assign == 'F') {
                            l->assign = 'T';
                        } else {
                            l->assign = 'F';
                        }
                }
                found = 1;
                break;
            }
            l = l->next;
        }

        if(!found) {
            l = cl->lit;
            while(l){
                if(strcmp(l->value, lit->value) == 0) {
                    if(flag) {
                    /*
                     * if flag is set then negate the value
                     */
                        if(lit->assign == 'T') {
                            l->assign = 'F';
                        } else{
                            l->assign = 'T';
                        }
                    } else {
                        l->assign = lit->assign;
                    }
                    found = 1;
                    break;
                }
                l = l->next;
            }
        } 
        clAffected(cl, 0);
        cl = cl->next;
        found = 0;
    }
}
int flip_values(float prob, Clause cl)
{
    int r1, r2, pos = 0, neg = 0, i;
    float p;
    Literal lit;
    int cnt = 0, cnt1 = 0, cnt2 = 0;

    lit = cl->negated_lit;
    while(lit) {
        lit = lit->next;
        cnt1++;
    }

    lit = cl->lit;
    while(lit) {
        lit = lit->next;
        cnt2++;
    }

    cnt = cnt1 + cnt2;

    r1 = rand() % cnt + 1;
    if(r1 > cnt1) {
        lit = cl->lit;
        r1 = r1 - cnt1;
    } else{
        neg = 1;
        lit = cl->negated_lit;
    }

    for(i = 1; i< r1; i++){
        lit = lit->next;
    }

    while(1) {
        r2 = rand() % 100 + 1;
        p = (prob * 100);
        if(r2 < p) {
            /*
            * Lets flip the literal value
            */
            if(!lit) {
                if(cl->negated_lit) {
                    lit = cl->negated_lit;
                    neg = 1;
                } else {
                    lit = cl->lit;
                }
            }
            if(lit->assign == 'T'){
                lit->assign = 'F';
            } else {
                lit->assign = 'T';
            }
            /*
            * Also now, we need to change the value of
            * this symbol in all the other clauses where it
            * is present either as itself or as its complementary
            */
            if(neg) {
                /*
                * If we have changed the negated value then 
                * if we find the complementary of that then the
                * value of that symbol should be negated, hence the flag 1 
                */
                change_values(lit, 1);
            } else{
                change_values(lit, 0);
            }
            return 1;
        } else if(r2 == p) {
            /*
            * do it all over again
            */
            continue;
        } else {
            return 0;
        }
    }
}


int num_TrueClauses(int flag)
{
    int n = 0, n_cp = 0;
    Clause cl;

    cl = root_clause;

    while(cl) {
        if(cl->assign == 'T' && !flag) {
            n++;
        }else if(flag && cl->assign_copy == 'T') {
            n_cp++;
        }
        cl = cl->next;
    }
    if(flag){
        return n_cp;
    }else{
        return n;
    }
}

int find_affectedClauses(Literal lit, int flag)
{
    Clause cl;
    Literal l;

    cl = root_clause;
    l = cl->negated_lit;

    while(cl){
        l = cl->negated_lit; 
        while(l){
            if(strcmp(lit->value, l->value) == 0) {
                if(flag) {
                    l->assign_copy = lit->assign_copy;
                }else{
                    if(lit->assign_copy == 'T'){
                        l->assign_copy = 'F';
                    }else{
                        l->assign_copy = 'T';
                    }
                }
            } else {
                l->assign_copy = l->assign;
            }
            l = l->next;
        }

        l = cl->lit;
        while(l){
            if(strcmp(l->value, lit->value) == 0) {
                if(flag) {
                    /*
                     * if flag is set then negate the value
                     */
                    if(lit->assign_copy == 'T') {
                        l->assign_copy = 'F';
                    } else{
                        l->assign_copy = 'T';
                    }
                } else {
                    l->assign_copy = lit->assign_copy;
                }
            } else{
                l->assign_copy = l->assign;
            }
            l = l->next;
        }
        clAffected(cl, 1);
        cl = cl->next;
    }
    return num_TrueClauses(1);
}



void maximize_clause(Clause cl)
{
    Literal lit;
    int num[100];
    int i = 1, j, k, max = 0;
    int flag = 0, elem = 0;
    int neg =0;
    int found = 0;

    num[0] = num_TrueClauses(0);
    /*
     * lets see how many clauses will be affected if we make 
     * a symbol true or false
     * lets start with the first symbol
     */

    lit = cl->negated_lit;
    while(lit) {
        if(lit->assign == 'T'){
            lit->assign_copy = 'F';
        } else{
            lit->assign_copy = 'T';
        }
        num[i] = find_affectedClauses(lit, 1);
        i++;
        lit = lit->next;
    }
    lit = cl->lit;
    while(lit) {
         if(lit->assign == 'T'){
            lit->assign_copy = 'F';
        } else{
            lit->assign_copy = 'T';
        }
        num[i] = find_affectedClauses(lit, 0);
        i++;
        lit = lit->next;
    }

    for(j = 1; j<i; j++) {
        if(num[j] > max) {
            max = num[j];
            elem = j;
        } 
    }

    /*
     * Now we know that by changing the 
     * literal elemth element we get maximum 
     * number of clause satisfiable
     */
    
    i = 0;

    lit = cl->negated_lit;
    while(lit) {
        i++;
        if(i == elem) {
            found = 1;
            neg = 1;
            break;
        }
        lit = lit->next;
    }

    if(!found){
        lit = cl->lit;
        while(lit){
            i++;
            if (i == elem) {
                break;
            }
            lit = lit->next;
        }
    }

    if(lit->assign == 'T'){
        lit->assign = 'F';
    } else {
        lit->assign = 'T';
    }

    if(neg) {
        change_values(lit, 1);
    } else{
        change_values(lit, 0);
    }
}

/*
 * WalkSAT function will now run in a loop for max_flips
 * number of times
 */

int walkSAT(int max_flips, float prob)
{
    int i;
    Clause cl;
    
    Clause_count = 0;
    total_clauses();

    for(i = 0; i< max_flips; i++){
        /*
         * Check if the model is satisfied
         * else randomly choose a clause
         */

        if(model_satisfies()) {
            /*
             * As the sentence is satisfiable 
             * return true;
             */
            cla[count] = Clause_count;
            run[count] = i+1;
            return i+1;
        }else{
            cl = select_clause_atRandom();
        }
        /*
         * Once a clause is selected at random
         * we will try fliping a randomly selected symbol
         * of its, with a probability of 0.5
         * If we donnot flip, the function 
         * returns false.
         */
        if(flip_values(prob, cl)){
            continue;
        } else {
            /*
             * If we did not flip, then now comes the tricky part
             * we need to or flip the value of that clause which would 
             * make the clause satisfiable and also increase the number of 
             * satisfiable clauses.
             */
            maximize_clause(cl);
        }
    }
    return 0;
}

int main()
{
    int rel, i, j, sat = 0, nsat = 0;
    float prob, sum = 0, sum_it = 0;
    int res, ppl, tables, max_flips;
    srand(time(NULL));
    FILE *fid;

    /*
     * need to first randomly assign true/false values
     * to the clauses
     */

    printf("Enter the number of tables: \n");
    scanf("%d", &tables);

    printf("Enter the number of people\n");
    scanf("%d", &ppl);

    printf("Enter the max flips value\n");
    scanf("%d", &max_flips);

    printf("Please find the output file at WalkSAT_Output_experiment3.txt ,under the current directory\n");
    fid = fid = fopen("WalkSAT_Output_experiment3.txt", "w");
    fprintf(fid, "\nGiven %d people and %d tables enemies with a probability of 0.02 and friends with a probability of 0.02\n", ppl, tables);
    fprintf(fid, "Generating....\n\n");
    while(sat < 20){
        CNF_GEN(ppl, tables, 0.02,0.02);
        random_assignment();
        res = walkSAT(max_flips, 0.5);
        if(!res) {
            nsat++;
        } else {
            fprintf(fid, "The sentence is satisfiable \n and the runtime is %d\n", res);
            sat++;
        }
        fprintf(fid, "The number of Satisfiable sentences %d\n\n", sat);
        clear_CNF();
        count = sat;
     }
    for(i = 0; i< 20; i++){
        sum = sum +cla[i];
        sum_it= sum_it+run[i];
    }

    fprintf(fid, "average clause /symbol = %f \n\n", sum/(20*ppl*tables));
    fprintf(fid, "average iterations = %f \n\n", sum_it/20);
    fclose(fid);
    return 1;
}



