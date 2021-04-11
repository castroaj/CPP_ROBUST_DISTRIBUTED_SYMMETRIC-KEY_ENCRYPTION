/*
 * Serial version of the key distribution.
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* uncomment this line to enable t=16 n=24 mode */
//#define SIXTEEN_TWENTYFOUR

int num_keys;        // number of total keys
int n;               // number of total servers
int t;               // number of participating servers (not including initiator)
int num_seats;       // number of seats that each server has for keys
int key_idx;         // index into the key_ids[], keeps track of what keys have already been used when populating
int *omega_col_idxs; // indicates, for each row/server in omega, which column to put the key when populating
int *key_ids;        // array of key ids
int *omega_matrix;   // matrix that holds the end result, of which server has what keys


/*
 * This function calculates the factorial of a number.
 * This is used for "n choose k" formula to calculate the number
 * of seats each server has so hold a key.
*/
unsigned long int factorial(unsigned long int x) {
    if (x == 0) {
        return 1;
    }
    return (x * factorial(x-1));
}

/*
 * Initializes variables and data structures based on if
 * we are in 16-24 mode or not
*/
void initialize_datastructures_variables() {

#   ifdef SIXTEEN_TWENTYFOUR
        n = 24;
        t = 16;
        num_keys = factorial(n) / (factorial(t-1) * factorial(t))
        num_seats = factorial(n-1) / (factorial(t-1) * factorial(n - t));
        key_idx = 0;
        omega_col_idxs = (int*)calloc(n, sizeof(int));
        key_ids = (int*)calloc(num_keys, sizeof(int));
        omega_matrix = (int*)calloc(n*num_seats, sizeof(int));
#   else
        n = 5;
        t = 3;
        num_keys = factorial(n) / (factorial(t-1) * factorial(t));
        num_seats = factorial(n-1) / (factorial(t-1) * factorial(n - t));
        key_idx = 0;
        omega_col_idxs = (int*)calloc(n, sizeof(int));
        key_ids = (int*)calloc(num_keys, sizeof(int));
        omega_matrix = (int*)calloc(n*num_seats, sizeof(int)); // n x num_seats 
#   endif

}


/*
 * Populates key ids into the key_ids[].
 * Hard coded id's for now.
*/
void populate_key_ids() {

    // populate the key_ids[] with id's 1 - (num_keys-1)
    // I'm assuming these keys will be different that just index+1, 
    // and will be read in from somewhere?
    for (int i = 0; i < num_keys; i++) {
            *(key_ids + i) = i + 1;
        }

}


/*
 * This function takes an array of server numbers (corresponding
 * to row numbers in omega table) and the index into the array of keys.
 * to takes the key at that index and populates the all servers in the 
 * omega table with that key id.
 */
void populate_omega_table(int servers[], int key_idx) {

    for (int i = 0; i < t; i++) {
        // for every server, add/update the key id pointed to by key_idx,
        // to the omega matrix, at server-row and...
        // omega_col_idxs
        *(omega_matrix + servers[i]*num_seats + omega_col_idxs[servers[i]]) = key_ids[key_idx];

        // Tell the program that one seat in that server has been filled, so the
        // next seat open is the one after.
        *(omega_col_idxs + servers[i]) += 1;
    }

}


/*
 * This function finds all combos of 't' servers, given a total of 'n' servers.
 * Once a combo is found, it passes the combo of servers and the index into
 * the array of keys, to a function to populate the omega matrix properly.
 * 
 * server_combo[]       -> temp array to hold the combo of servers
 * start_idx & end_idx  -> start and end index into the key_ids[]
 * combo_idx            -> the current index into server_combo[]
 * id_idx               -> the index into key_ids[] to know what id the servers get
 * 
 * NOTE: elements in server_combo correlate to row numbers in the omega table
 *       and each row in the table correlates to an individual server
 */
void find_combos(int server_combo[], int start_num, int combo_idx) {
    
    // server_combo is populated/ready, send info to populate the table
    if (combo_idx == t) {
        //printf("- Server Combo: ");
        //for (int j=0; j<t; j++) {
        //    printf("%d ", server_combo[j]);
        //}
        //printf("gets %d", key_ids[key_idx++]);
        //printf("\n");
        populate_omega_table(server_combo, key_idx++);
        return;
    }

    // replace combo_idx with all possible server numbers. The condition
    // "n-1-server_num+1 >= t-combo_idx" makes sure that including one server number
    // at combo_idx will make a combination with remaining server numbers
    // at remaining positions
    for (int server_num = start_num; server_num<=n-1 && n-1-server_num+1 >= t-combo_idx; server_num++) {
        server_combo[combo_idx] = server_num;
        find_combos(server_combo, server_num+1, combo_idx+1);
    }
}


/*
 * This function, with the help of 2 others, populates the omega matrix.
 */
void generate_omega_matrix() {
    
    // Temp array to store all combinations of t servers
    int server_combo[t];
    // Find combos and populate omega matrix from there
    find_combos(server_combo, 0, 0);

}


int main(int argc, char* argv[])
{

    initialize_datastructures_variables();
    populate_key_ids();
    generate_omega_matrix();

    printf("--- Omega Table ---\n");
    for (int row = 0; row < n; row++) {
      printf("\n");
      for (int col = 0; col < num_seats; col++) {
         printf("%d ", *(omega_matrix + row*num_seats + col));
      }
    }
    printf("\n");

   // ! Pass keys
   // ! ...
 

    // Free memory
    free(key_ids);
    free(omega_col_idxs);
    free(omega_matrix);

}