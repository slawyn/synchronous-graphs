#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define log(f_, ...) printf((f_), ##__VA_ARGS__);

/*Data structures*/
#define namemaxlength 4
typedef struct{
    char name[namemaxlength];
    int gamma;
}node;

typedef struct {
    int p_rate;
    int c_rate;
    node* source;
    node* destination;
    int token;
}edge;


/* Greatest Common Divisor*/
int gcd(int number0, int number1){
  return number1==0?number0:gcd(number1, number0%number1);
}


/* Least Common Multiple*/
int lcm(int number0, int number1){
    return (number0*number1)/gcd(number0, number1);
}



/*Initializes the Node*/
void initnode(node* n, char *name){
    long i = 0;
    while(i < (namemaxlength-1) && name[i] != '\0'){
         n->name[i] = name[i];
         ++i;
    }
    n->name[i] = '\0';
    n->gamma = 0;
}

/*Initializes the edge between the nodes*/
void initedge(edge* e,node* source, node* dest, int p_rate, int c_rate){
    e->p_rate = p_rate;
    e->c_rate = c_rate;
    e->source = source;
    e->destination = dest;
    e->token = 0; //d.c
}

/* Define local graph */
int genlocalgraph(node* nodes[], edge* edges[], int* nodecount, int* edgecount){
    // define number of nodes and edges

    /**/
    // Graph 1
    *nodecount = 3;
    *edgecount = 5;

    // allocate memory and assign the base
    *nodes = (node*)malloc(sizeof(node)*(*nodecount));
    *edges = (edge*)malloc(sizeof(edge)*(*edgecount));

    // define nodes
    initnode(&(*nodes)[0], "a");
    initnode(&(*nodes)[1], "b");
    initnode(&(*nodes)[2], "c");


    // define edges
    initedge(&(*edges)[0], &(*nodes)[0], &(*nodes)[1], 1, 2);
    initedge(&(*edges)[1], &(*nodes)[1], &(*nodes)[0], 2, 1);
    initedge(&(*edges)[2], &(*nodes)[1], &(*nodes)[1], 1, 1);
    initedge(&(*edges)[3], &(*nodes)[1], &(*nodes)[2], 3, 2);
    initedge(&(*edges)[4], &(*nodes)[2], &(*nodes)[1], 2, 3);

    /*
     // Graph 2
    *nodecount = 5;
    *edgecount = 4;

    // allocate memory and assign the base
    *nodes = (node*)malloc(sizeof(node)*(*nodecount));
    *edges = (edge*)malloc(sizeof(edge)*(*edgecount));

    // define nodes
    initnode(&(*nodes)[0], "a");
    initnode(&(*nodes)[1], "b");
    initnode(&(*nodes)[2], "c");
    initnode(&(*nodes)[3], "d");
    initnode(&(*nodes)[4], "e");

    // define edges
    initedge(&(*edges)[0], &(*nodes)[0], &(*nodes)[1], 3, 251);
    initedge(&(*edges)[1], &(*nodes)[1], &(*nodes)[2], 5, 233);
    initedge(&(*edges)[2], &(*nodes)[2], &(*nodes)[3], 7, 227);
    initedge(&(*edges)[3], &(*nodes)[3], &(*nodes)[4], 3, 271);*/

    return 0;
}


/* Algorithm: https://www.geeksforgeeks.org/program-for-rank-of-matrix/*/
int calculatematrixrank(int rows, int columns, int *ppmatrixdata){
    #define getmatrixat(i_,j_) (*(ppmatrixdata+i_*columns + j_))
    #define setmatrixat(i_,j_,val) (*(ppmatrixdata+i_*columns + j_)) =val
    int rank = columns;

    for (int row = 0; row < rank; row++)
    {
        // Before we visit current row 'row', we make
        // sure that mat[row][0],....mat[row][row-1]
        // are 0.

        // Diagonal element is not zero
        if (getmatrixat(row,row))
        {
           for (int col = 0; col < rows; col++)
           {
               if (col != row)
               {
                 // This makes all entries of current
                 // column as 0 except entry 'mat[row][row]'
                 double mult = (double)getmatrixat(col,row) / getmatrixat(row,row);
                 for (int i = 0; i < rank; i++){
                    int temp = getmatrixat(col,i);
                    setmatrixat(col,i,temp- mult*getmatrixat(row,i));
                 }
              }
           }
        }

        // Diagonal element is already zero. Two cases
        // arise:
        // 1) If there is a row below it with non-zero
        //    entry, then swap this row with that row
        //    and process that row
        // 2) If all elements in current column below
        //    mat[r][row] are 0, then remvoe this column
        //    by swapping it with last column and
        //    reducing number of columns by 1.
        else
        {
            bool reduce = true;

            /* Find the non-zero element in current
                column  */
            for (int i = row + 1; i < rows;  i++)
            {
                // Swap the row with non-zero element
                // with this row.
                if (getmatrixat(i,row))
                {

                    // swap
                    for (int idx = 0; idx < rank; idx++)
                    {

                        int temp = getmatrixat(row,idx);

                        setmatrixat(row, idx, getmatrixat(i, idx));
                        setmatrixat(i, idx, temp);
                    }

                    reduce = false;
                    break ;
                }
            }

            // If we did not find any row with non-zero
            // element in current columnm, then all
            // values in this column are 0.
            if (reduce)
            {
                // Reduce number of columns
                rank--;

                // Copy the last column here
                for (int i = 0; i < rows; i ++)
                    setmatrixat(i,row, getmatrixat(i,rank));
            }

            // Process this row again
            row--;
        }

        /*
        for (int irow = 0; irow < rows; irow++){
            for (int jcol = 0; jcol < columns; jcol++)
                printf("  %d", getmatrixat(irow,jcol));
            printf(" \n");
        }
        printf(":---------------\n")*/
    }

    return rank;
}

/* Unfinished
   Refer to
   "An Introduction to Synchronous Data Flow Model" Dr. HaitaoWei CAPSLatUDEL #page 26 */
int periodicscheduleconsitency(node* nodes, edge* edges, int nodecount, int edgecount, int* rank){
    #define matrixmemsz (sizeof(int)*edgecount*nodecount)
    #define findindex(currentnode) (currentnode-nodes)
    int inconsistent = 1;
    edge* cedge;
    node* cnode;

    // Allocate memory for matrix, and zero it
    int *pmatrixmemory = (int*) malloc(matrixmemsz);
    memset(pmatrixmemory,0,matrixmemsz);

    // Fill matrix,
    // Assumption: heap memory addresses increase for a continuous memory block
    for(long i=0,j=0;i<edgecount;i++){
        cedge = &edges[i];

        // Source
        cnode = cedge->source;
        j = findindex(cnode);
        pmatrixmemory[i*nodecount+j] = cedge->p_rate;

        // Sink
        cnode = cedge->destination;
        j = findindex(cnode);
        pmatrixmemory[i*nodecount+j] = -cedge->c_rate;
    }

    // Row Reduction
    // Trick to avoid fractions:
    // https://www.wyzant.com/resources/blogs/448435/trick_for_row_echelon_reduction_of_integer_matrices_eliminates_fractions
    // Calculate rank, Rank has to be s-1 where "s" is the number of nodes
    *rank = calculatematrixrank(edgecount,nodecount,pmatrixmemory);
    if(*rank>0 && (*rank) == (nodecount-1)){
      inconsistent = 0;
    }
    return inconsistent;
}

/* Update repetition vector*/
void updaterepetitions(node *nodes, int nodecount, int multiplier){
    for(int x=0;x<nodecount;++x){
        nodes[x].gamma *=multiplier;
    }
}



/* Integer only*/
int calculategamma(node* nodes, edge* edges, int nodecount, int edgecount){
    edge* cedge;
    node* srcnode;
    node* dstnode;
    int reductionfactor;
    int p_factor;
    int c_factor;
    int status = 0;

    // Iterate through edges and calculate the gamma for the nodes
    for(long idx=0;idx<edgecount && !status;++idx){
        cedge = &edges[idx];
        srcnode = cedge->source;
        dstnode = cedge->destination;

        // A couple of cases depending on the gamma
        // General formula:
        // p * gamma0 = c * gamma1
        // p/c * gamma0  =  gamma1   |*multiplier

        // Case 1: Destination Gamma is not set
        if(dstnode->gamma == 0){

            // If Source gamma has not been set, we set it to 1
            srcnode->gamma = srcnode->gamma == 0?1:srcnode->gamma;

            // set locals and calculate gcd
            p_factor = cedge->p_rate*srcnode->gamma;
            c_factor = cedge->c_rate;
            reductionfactor = gcd(p_factor,c_factor);

            // Reduce factors to the smallest amount
            // Useful for finding smallest required multiplier
            if(reductionfactor>1){
               p_factor /=reductionfactor;
               c_factor /=reductionfactor;
            }

            // We update the repetition vector and assign p_factor to destination
            // Assume 1 for source if no previous value has been set for any of the gammas
            updaterepetitions(nodes,nodecount,c_factor);
            dstnode->gamma = p_factor;                  // Assign afterwards so it doesn't get multiplied with the rest

        // Case 2: Destination gamma has been set
        }else if(dstnode->gamma){
              if(srcnode->gamma){
                  // Here we have two gammas, we need to check if the nodes are balanced
                  // if not the graph is not consistent
                  p_factor = cedge->p_rate*srcnode->gamma;
                  c_factor = cedge->c_rate*dstnode->gamma;
                  if(p_factor != c_factor){
                    log("log: Graph is not consistent: edge(%ld) source-gamma(%d) destination-gamma(%d)\n",idx,srcnode->gamma,dstnode->gamma);
                    status = 1;
                  }
              // source node has not been set
              }else{
                    p_factor = cedge->p_rate;
                    c_factor = cedge->c_rate*dstnode->gamma;

                    reductionfactor = gcd(p_factor,c_factor);

                    // Reduce factors to the smallest amount
                    // Useful for finding smallest required multiplier
                    if(reductionfactor>1){
                       p_factor /=reductionfactor;
                       c_factor /=reductionfactor;
                    }

                    // We update the repetition vector and assign c_factor to source
                    // Assume 1 for source if no previous value has been set for any of the gammas
                    updaterepetitions(nodes,nodecount,p_factor);
                    srcnode->gamma = c_factor;                  // Assign afterwards so it doesn't get multiplied with the rest
              }
        }

        // Check for overflow
        for(long jdx=0;jdx<nodecount;++jdx){
            if(nodes[jdx].gamma<0){
                status = 2;
                log("log: Integer overflowed at edge(%ld) node(%ld)\n",idx,jdx);
            }
        }
    }

    return status;
}

/* Main function*/
int main(int argc, char *argv[]){
    node *nodes = NULL;
    edge *edges = NULL;
    int nodecount;
    int edgecount;
    int rank;

    genlocalgraph(&nodes,&edges,&nodecount,&edgecount);
    log("log: Graph generated: Nodes(%d) Edges(%d)\n",nodecount,edgecount)

    // Check rank
    if(!periodicscheduleconsitency(nodes, edges, nodecount, edgecount, &rank)){
      log("log: Graph's rank(%d) assumes graph is consistent\n",rank);
    }else{
       log("log: Graph is not consistent according to rank(%d)\n",rank);
    }

    // Print gamma values if the calculation went okay
    log("log: Calculating Gamma for the nodes\n");
    if(!calculategamma(nodes,edges, nodecount, edgecount)){
        for(int i=0;i<nodecount;i++){
            log("\t%s: %d\n",nodes[i].name,nodes[i].gamma)
        }
    }else{
        log("log: Could not calculate gamma\n");
    }

    free(edges);
    free(nodes);
}
