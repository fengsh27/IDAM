/* Author: Qin Ma <maqin@csbl.bmb.uga.edu>, Jan. 22, 2010
 * Usage: This is part of bicluster package. Use, redistribution, modify without limitations
 * show how does the whole program work
 */

/***********************************************************************/

#include "main.h"

extern continuous **arr;
extern discrete **arr_c;
extern discrete *symbols;
extern char **genes;
extern char **conds;
extern int *gene_uber;
extern char **sub_genes;
extern bool *sublist;
extern int rows, cols, sigma;
extern int TFindex;
extern int sub_genes_row;
extern double uber_ratio; 
extern char blast[200];

extern Prog_options* po;

/***********************************************************************/

int main(int argc, char* argv[])
{
	/* Start the timer */
	uglyTime(NULL);
	printf("\nIDAM %.1f: identify functional modules (compiled "__DATE__" "__TIME__")\n\n", VER);
	rows = cols = 0;

	/* get the program options defined in get_options.c */
	get_options(argc, argv);
	
	/*get the size of input expression matrix*/
	get_matrix_size(po->FP);	
	progress("File %s contains %d genes by %d conditions", po -> FN, rows, cols);
	if (rows < 3 || cols < 3)
	{
		/*neither rows number nor cols number can be too small*/
		errAbort("Not enough genes or conditions to make inference");
	}
	genes = alloc2c(rows, LABEL_LEN);
	conds = alloc2c(cols, LABEL_LEN);
	gene_uber = (int *)malloc(sizeof(int)*(rows + 1)); 

	/* Read in the gene names and condition names */
	read_labels(po -> FP);	

    	/* Read in the expression data */
	if (po->IS_DISCRETE)
		read_discrete(po -> FP);
	else
	{
		read_continuous(po -> FP);
		
		/* formatting rules */
		discretize(addSuffix(addSuffix(po->FN, ".rules"), po->TAG));
	}
	fclose(po->FP);

	/*read in the sub-gene list*/
	if (po->IS_list)
	{
		sub_genes = alloc2c(rows, LABEL_LEN);
		read_list (po->FL);
	}

	/*we can do expansion by activate po->IS_SWITCH*/
	if (po->IS_SWITCH)
	{
		read_and_solve_blocks(
			po->FB, addSuffix(
				addSuffix(po->BN, ".expansion"), po->TAG
			)
		);
	}
	else
	{
		/* formatted file */
		write_imported(
			addSuffix(
				addSuffix(po->FN, ".chars"), po->TAG
			)
		);
	
		/* the file that stores all blocks */
		if (po->IS_list)
			make_graph(
				addSuffix(
				    addSuffix(
					    addSuffix(po->FN, po->LN),".modules"
				    ), po->TAG
				)
			);
		else
			make_graph(
				addSuffix(
				    addSuffix(po->FN, ".modules"), po->TAG
				)
			);

   	}/* end of main else */
	free(po);
	free (sublist);
	return 0;
}

/***********************************************************************/
