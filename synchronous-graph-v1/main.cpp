#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.hpp"
#define log(f_, ...) printf((f_), ##__VA_ARGS__);


/*Data structures*/
typedef struct{
    char name[4];
    int gamma;      // we assume 1, because 0 cannot exist in the production system
}node;

typedef struct {
    int p_rate;
    int c_rate;
    node* source;
    node* destination;
    int token;
}edge;


/*Initializes the Node*/
void initnode(node* n, char *name){
    int len = strlen(name);
    name[len-1] = '\0';
    snprintf(n->name, 4, "%s", name);       // Replace new line, so the names of nodes match with the mentioned node names in the edges
    n->gamma = 1;

}

/*Initializes the edge between the nodes*/
void initedge(edge* e,node* source, node* dest, int p_rate, int c_rate){
    e->p_rate = p_rate;    e->c_rate = c_rate;
    e->source = source;
    e->destination = dest;
    e->token = 0; //d.c

}

/* Searches for a defined node based on name*/
node * findnode(node * nodes, int nodecount, char *nodename){
    node * found = NULL;
    int idx = 0;

    while(idx<nodecount && found == NULL){
        //log("%d) %d %s >< %s\n", idx, nodecount, nodes[idx].name, nodename)

        if(strcmp(nodes[idx].name, nodename) == 0){
            found = &nodes[idx];
        }
        ++idx;
    }

    return found;
}



/* Generates graph from file */
int gengraph(FILE *fp, node* nodes[], edge* edges[], int* nodecount, int* edgecount){
    char *stoken0=NULL,*stoken1=NULL,*line=NULL;
    int pvalue, cvalue;
    ssize_t read;

    size_t len = 0;
    int serror = 0;
    int nidx = 0;
    int eidx = 0;

    node *node0 = NULL;
    node *node1 = NULL;

    // is it a graph file
    if((read = getline(&line, &len, fp)) != -1){
        log(line)

        // Tokenize the header and get number of nodes and edges. If something is wrong, file won't be parsed
        stoken0 = tokenize(line);
        if(strstr(stoken0,"graph")!=NULL && !setint(nodecount, getnexttoken()) && !setint(edgecount, getnexttoken())){

            // Allocate memory for arrays
            *nodes = (node*)malloc(sizeof(node)*(*nodecount));
            *edges = (edge*)malloc(sizeof(edge)*(*edgecount));

            // Read file if it is a graph file
            while ((read = getline(&line, &len, fp)) != -1 && !serror){
                log(line)

                // Tokenize the string and init structures
                stoken0 = tokenize(line);


                if(stoken0 != NULL){
                    if(strstr(stoken0,"node:")!=NULL){
                        if(nidx == *nodecount){
                            log("log: Failed. Node count and node definitions don't match")
                            serror = 3;
                        }else{
                            initnode(&(*nodes)[nidx], getnexttoken());
                            ++nidx;
                        }

                    }else if(strstr(stoken0,"edge:")!=NULL){
                        if(eidx == *edgecount){
                            log("log: Failed. Edge count and edge definitions don't match")
                            serror = 4;
                        }else{
                             // NB! Could be very dangerous. Need to read all the values beforehand into the variables, because the function call could be scheduled out of order!!!
                             // But this is very fast
                             stoken0 = getnexttoken();
                             stoken1 = getnexttoken();
                             pvalue = getnextinttoken();
                             cvalue = getnextinttoken();

                             node0 =findnode(*nodes,*nodecount, stoken0);
                             node1 =findnode(*nodes,*nodecount, stoken1);

                             // Sanity check
                             if(NULL == node0 ||NULL == node1){
                                log("log: Failed. One or more nodes couldn't be found");
                                serror = 5;
                             }else{
                                initedge(&(*edges)[eidx], node0, node1, pvalue, cvalue);
                                ++eidx;
                             }

                        }
                    }
                    else{
                        //log("log:Unknown entry\n")
                    }
                }
            }
        }else{
            log("log: Header has an error")
            serror = 2;
        }
    }else{
        log("log: File is empty")
        serror = 1;
    }

    return serror;
}

void calculategamma(node* nodes, edge* edges, int nodecount, int edgecount){
    edge* cedge;
    node* node0;
    node* node1;
    int tint;
    int factor0;
    int factor1;
    int idx = 0;
    // iterate through edges and calculate the gamma for the nodes
    while(idx < edgecount){
        cedge = &edges[idx];
        node0 = cedge->source;
        node1 = cedge->destination;

        // Calculate common max number
        tint = kgV(node0->gamma * cedge->p_rate, node1->gamma * cedge->c_rate);

        factor0 = tint / (node0->gamma * cedge->p_rate);
        factor1 = tint / (node1->gamma * cedge->c_rate);

        // we need to update the current nodes, and start anew, so the gamma values propagate through the whole node network
        if(factor0>1 || factor1 >1){
            node0->gamma *=factor0;
            node1->gamma *=factor1;
            idx = 0;

        }else{
            ++idx;
        }
    }
}


/* Main function*/
int main(int argc, char *argv[])
{
    FILE *fp;
    node *nodes = NULL;
    edge *edges = NULL;
    int nodecount;
    int edgecount;
    int serror;

    //argc = 2;

    if(argc != 2){
        log("Usage: program.exe <filename>\n")

    }else{
        fp = fopen(argv[1],"r");//fopen("C:\\Users\\Unixt\\OneDrive\\Desktop\\pushe\\mohi\\bin\\Debug\\graph.txt","r");

        if(fp != NULL){
            log("log:Successfully opened file. Generating graph..\n")
            serror = gengraph(fp, &nodes, &edges,&nodecount, &edgecount);

            log("\nlog: Closing file\n")
            fclose(fp);

            if(!serror){
                log("log: Graph generated: Nodes(%d) Edges(%d)\n",nodecount,edgecount)

                // Calculate gamma
                log("log: Calculating Gamma for the nodes\n");
                calculategamma(nodes,edges, nodecount, edgecount);

                // Printing Gamma values
                for(int i=0;i<nodecount;i++){
                    log("\t%s: %d\n",nodes[i].name,nodes[i].gamma)
                }
            }else{
                log("log: Parsing failed");
            }

            // Free memory
            free(edges);
            free(nodes);
        }






    }
}
