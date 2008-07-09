
   /*+------- <| --------------------------------------------------------**
    **         A                     Clan                                **       
    **---     /.\   -----------------------------------------------------**    
    **   <|  [""M#                 matrix.c                              **  
    **-   A   | #   -----------------------------------------------------**
    **   /.\ [""M#         First version: 30/04/2008                     **
    **- [""M# | #  U"U#U  -----------------------------------------------**        
         | #  | #  \ .:/    
         | #  | #___| #     
 ******  | "--'     .-"  ******************************************************
 *     |"-"-"-"-"-#-#-##   Clan : the Chunky Loop Analyzer (experimental)     *
 ****  |     # ## ######  *****************************************************
 *      \       .::::'/                                                       *
 *       \      ::::'/     Copyright (C) 2008 Cedric Bastoul                  *
 *     :8a|    # # ##                                                         *
 *     ::88a      ###      This is free software; you can redistribute it     *
 *    ::::888a  8a ##::.   and/or modify it under the terms of the GNU Lesser *
 *  ::::::::888a88a[]:::   General Public License as published by the Free    *
 *::8:::::::::SUNDOGa8a::. Software Foundation, either version 3 of the       *
 *::::::::8::::888:Y8888:: License, or (at your option) any later version.    *
 *::::':::88::::888::Y88a::::::::::::...                                      *
 *::'::..    .   .....   ..   ...  .                                          *
 * This software is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.							      *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with software; if not, write to the Free Software Foundation, Inc.,  *
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA                     *
 *                                                                            *
 * Clan, the Chunky Loop Analyzer                                             *
 * Written by Cedric Bastoul, Cedric.Bastoul@inria.fr                         *
 *                                                                            *
 ******************************************************************************/


# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "../include/clan/clan.h"


/*+****************************************************************************
 *                          Structure display function                        *
 ******************************************************************************/


/**
 * clan_matrix_print_structure function:
 * Displays a clan_matrix_t structure (*matrix) into a file (file, possibly
 * stdout) in a way that trends to be understandable without falling in a deep
 * depression or, for the lucky ones, getting a headache... It includes an
 * indentation level (level) in order to work with others print_structure
 * functions.
 * \param file   File where informations are printed.
 * \param matrix The matrix whose information have to be printed.
 * \param level  Number of spaces before printing, for each line.
 **
 * - 30/04/2008: first version (from CLooG 0.14.0).
 */
void
clan_matrix_print_structure(FILE * file, clan_matrix_p matrix, int level)
{
  int i, j;

  if (matrix != NULL)
  {
    /* Go to the right level. */
    for (j = 0; j < level; j++)
      fprintf(file,"|\t");
    fprintf(file,"+-- clan_matrix_t\n");

    for(j = 0; j <= level; j++)
      fprintf(file,"|\t");
    fprintf(file,"%d %d\n",matrix->NbRows,matrix->NbColumns);

    /* Display the matrix. */
    for (i = 0; i < matrix->NbRows; i++)
    {
      for (j = 0; j <= level; j++)
        fprintf(file,"|\t");

      fprintf(file,"[ ");

      for (j = 0; j < matrix->NbColumns; j++)
      {
        CLAN_print(file,CLAN_FMT,matrix->p[i][j]);
        fprintf(file," ");
      }

      fprintf(file,"]\n");
    }
  }
  else
  {
    /* Go to the right level. */
    for (j = 0; j < level; j++)
      fprintf(file,"|\t");
    fprintf(file,"+-- NULL matrix\n");
  }

  /* The last line. */
  for (j = 0; j <= level; j++)
    fprintf(file,"|\t");
  fprintf(file,"\n");
}


/**
 * clan_matrix_print function:
 * This function prints the content of a clan_matrix_t structure (*matrix) into
 * a file (file, possibly stdout).
 * \param file   File where informations are printed.
 * \param matrix The matrix whose information have to be printed.
 **
 * - 30/04/2008: first version (from CLooG 0.14.0).
 */
void
clan_matrix_print(FILE * file, clan_matrix_p matrix)
{ 
  clan_matrix_print_structure(file,matrix,0);
}


/**
 * clan_matrix_expression_element function:
 * This function returns a string containing the printing of a value (possibly
 * an iterator or a parameter with its coefficient or a constant).
 * \param val   The coefficient or constant value.
 * \param first Pointer to a boolean set to 1 if the current value is the first
 *              of an expresion, 0 otherwise (this function may update it).
 * \param cst   A boolean set to 1 if the value is a constant, 0 otherwise.
 * \param name  String containing the name of the iterator or of the parameter.
 **
 * - 03/05/2008: first version (from CLooG 0.14.0, glorious pprint_val).
 */
char *
clan_matrix_expression_element(Value val, int * first, int cst, char * name)
{
  char * sval, * body, * temp;

  temp = (char *)malloc(CLAN_MAX_STRING * sizeof(char));
  body = (char *)malloc(CLAN_MAX_STRING * sizeof(char));
  sval = (char *)malloc(CLAN_MAX_STRING * sizeof(char));
  body[0] = '\0';
  sval[0] = '\0';

  /* statements for the 'normal' processing. */
  if (CLAN_notzero_p(val) && (!cst))
  {
    if ((*first) || CLAN_neg_p(val))
    {
      if (CLAN_one_p(val))           /* case 1 */
        sprintf(sval,"%s",name);
      else
      {
        if (CLAN_mone_p(val))        /* case -1 */
          sprintf(sval,"-%s",name);
	else                         /* default case */
	{
	  CLAN_sprint(sval,CLAN_FMT_TXT,val);
	  sprintf(temp,"*%s",name);
	  strcat(sval,temp);
        }
      }
      *first = 0;
    }
    else
    {
      if (CLAN_one_p(val))
        sprintf(sval,"+%s",name);
      else
      {
        sprintf(sval,"+");
	CLAN_sprint(temp,CLAN_FMT_TXT,val);
	strcat(sval,temp);
	sprintf(temp,"*%s",name);
	strcat(sval,temp);
      }
    }
  }
  else
  {
    if (cst)
    {
      if ((CLAN_zero_p(val) && (*first)) || CLAN_neg_p(val))
        CLAN_sprint(sval,CLAN_FMT_TXT,val);
      if (CLAN_pos_p(val))
      {
        if (!(*first))
        {
	  CLAN_sprint(sval,"+"CLAN_FMT_TXT,val); /* Block macro ! */
	}
	else
          CLAN_sprint(sval,CLAN_FMT_TXT,val);
      }
    }
  }
  free(temp);
  free(body);
  
  return(sval);
}
 

/**
 * clan_matrix_expression function:
 * This function returns a string corresponding to an affine expression
 * stored at the "row"^th row of the matrix pointed by "matrix".
 * \param matrix        A set of linear expressions.
 * \param row           The row of the matrix corresponding to the expression.
 * \param nb_iterators  The number of iterators for the considered statement.
 * \param iterators     An array containing iterator names for the statement.
 * \param nb_parameters The number of parameters in the SCoP.
 * \param parameters    An array containing all parameters names.
 **
 * - 03/05/2008: first version (from CLooG 0.14.0, glorious pprint_val).
 */
char *
clan_matrix_expression(clan_matrix_p matrix, int row,
                       int nb_iterators,  char ** iterators,
                       int nb_parameters, char ** parameters)
{
  int i, first = 1;
  char * sline, * sval;
  
  sline = (char *)malloc(CLAN_MAX_STRING * sizeof(char)) ;
  sline[0] = '\0' ;
  
  /* First the iterator part. */
  for (i = 1; i <= nb_iterators; i++)
  {
    sval = clan_matrix_expression_element(matrix->p[row][i],&first,0,
                                          iterators[i-1]);
    strcat(sline,sval);
    free(sval);
  }

  /* Next the parameter part. */
  for (i = nb_iterators + 1; i <= nb_iterators + nb_parameters; i++)
  {
    sval = clan_matrix_expression_element(matrix->p[row][i],&first,0,
                                          parameters[i - nb_iterators - 1]);
    strcat(sline,sval);
    free(sval);
  }

  /* Finally the constant part (yes, I reused i). */
  sval = clan_matrix_expression_element(matrix->p[row][i],&first,1,NULL);
  strcat(sline,sval);
  free(sval);
  
  return sline;
}


/**
 * clan_matrix_print_dot_scop function:
 * This function prints the content of a clan_matrix_t structure (*matrix) into
 * a file (file, possibly stdout) for the .scop format.
 * \param file          File where informations are printed.
 * \param matrix        The matrix whose information have to be printed.
 * \param type          A bit of semantic about this matrix (domain, access...).
 * \param nb_iterators  The number of iterators for the considered statement.
 * \param iterators     An array containing iterator names for the statement.
 * \param nb_parameters The number of parameters in the SCoP.
 * \param parameters    An array containing all parameters names.
 * \param nb_arrays     The number of arrays accessed in the SCoP.
 * \param arrays        An array containing all accessed array names.
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_print_dot_scop(FILE * file, clan_matrix_p matrix, int type,
                           int nb_iterators,  char ** iterators,
                           int nb_parameters, char ** parameters,
			   int nb_arrays,     char ** arrays)
{
  int i, j, k;
  char * expression;
  
  if (matrix == NULL)
  {
    fprintf(file,"0 %d\n",nb_iterators+nb_parameters+2);
    return;
  }
  
  fprintf(file,"%d %d\n",matrix->NbRows,matrix->NbColumns);
  
  for (i = 0; i < matrix->NbRows; i++)
  {
    for (j = 0; j < matrix->NbColumns; j++)
    {
      CLAN_print(file,CLAN_FMT,matrix->p[i][j]);
      fprintf(file," ");
    }
    
    if (type == CLAN_TYPE_DOMAIN)
    {
      expression = clan_matrix_expression(matrix,i,nb_iterators,iterators,
                                          nb_parameters,parameters);
      fprintf(file,"   ## %s",expression);
      free(expression);
      if (CLAN_zero_p(matrix->p[i][0]))
        fprintf(file," == 0");
      else
        fprintf(file," >= 0");
    }
    
    if (type == CLAN_TYPE_SCATTERING)
    {
      expression = clan_matrix_expression(matrix,i,nb_iterators,iterators,
                                          nb_parameters,parameters);
      fprintf(file,"   ## %s",expression);
      free(expression);
    }

    if (type == CLAN_TYPE_ACCESS)
    {
      if (CLAN_notzero_p(matrix->p[i][0]))
      {      
	fprintf(file,"   ## %s",arrays[CLAN_get_si(matrix->p[i][0]) - 1]);
	k = i;
	do
	{
	  expression = clan_matrix_expression(matrix,k,nb_iterators,iterators,
                                              nb_parameters,parameters);
          fprintf(file,"[%s]",expression);
          free(expression);
	  k++;
	}
	while ((k < matrix->NbRows) && CLAN_zero_p(matrix->p[k][0]));
      }
      else
        fprintf(file,"   ##");
    }
   
    fprintf(file,"\n");
  }
}


/*+****************************************************************************
 *                    Memory allocation/deallocation function                 *
 ******************************************************************************/


/**
 * clan_matrix_malloc function:
 * This function allocates the memory space for a clan_matrix_t structure and
 * sets its fields with default values. Then it returns a pointer to the
 * allocated space.
 * \param NbRows    The number of row of the matrix to allocate.
 * \param NbColumns The number of columns of the matrix to allocate.
 **
 * - 30/04/2008: first version (from PipLib 1.4.0).
 */
clan_matrix_p
clan_matrix_malloc(unsigned NbRows, unsigned NbColumns)
{
  clan_matrix_p matrix;
  Value ** p, * q;
  int i, j;

  matrix = (clan_matrix_p)malloc(sizeof(clan_matrix_t));
  if (matrix == NULL) 	
  {
    fprintf(stderr, "[Clan] Memory Overflow.\n");
    exit(1);
  }
  matrix->NbRows = NbRows;
  matrix->NbColumns = NbColumns;
  matrix->p_Init_size = NbRows * NbColumns;
  if (matrix->p_Init_size == 0) 
  {
    matrix->p = NULL;
    matrix->p_Init = NULL;
  }  
  else 
  {
    p = (Value **)malloc(NbRows*sizeof(Value *));
    if (p == NULL) 
    {
      fprintf(stderr, "[Clan] Memory Overflow.\n");
      exit(1);
    }
    q = (Value *)malloc(NbRows * NbColumns * sizeof(Value));
    if (q == NULL) 
    {
      fprintf(stderr, "[Clan] Memory Overflow.\n");
      exit(1);
    }
    matrix->p = p;
    matrix->p_Init = q;
    for (i = 0; i < NbRows; i++) 
    {
      *p++ = q;
      for (j = 0; j < NbColumns; j++)   
        CLAN_init_set_si(*(q+j),0);
      q += NbColumns;
    }
  }
  return matrix;
}


/**
 * candl_matrix_free_inside function:
 * This function frees the allocated memory for the inside of a
 * clan_matrix_t structure, i.e. only p and p_Init.
 * \param matrix The pointer to the matrix we want to free.
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_free_inside(clan_matrix_p matrix)
{ 
  int i;
  Value * p;

  if (matrix == NULL)
    return;
  
  p = matrix->p_Init;
  for (i = 0; i < matrix->p_Init_size; i++) 
    CLAN_clear(*p++);

  if (matrix->p_Init != NULL)
    free(matrix->p_Init);
 
  if (matrix->p != NULL)
    free(matrix->p);
}


/**
 * candl_matrix_free function:
 * This function frees the allocated memory for a clan_matrix_t structure.
 * \param matrix The pointer to the matrix we want to free.
 **
 * - 30/04/2008: first version.
 * - 02/05/2008: now uses clan_matrix_free_inside.
 */
void
clan_matrix_free(clan_matrix_p matrix)
{ 
  if (matrix == NULL)
    return;

  clan_matrix_free_inside(matrix);
  free(matrix);
}


/*+****************************************************************************
 *                            Processing functions                            *
 ******************************************************************************/


/**
 * clan_matrix_ncopy function:
 * this functions builds and returns a "hard copy" (not a pointer copy) of a
 * clan_matrix_t data structure such that the copy is restricted to the "n"
 * first rows of the matrix.
 * \param matrix The pointer to the matrix we want to copy.
 * \param n      The number of row of the matrix we want to copy.
 **
 * - 02/05/2008: first version.
 */
clan_matrix_p
clan_matrix_ncopy(clan_matrix_p matrix, int n)
{
  int i, j;
  clan_matrix_p copy;

  if (matrix == NULL)
    return NULL;

  if (n > matrix->NbRows)
  {
    fprintf(stderr,"[Clan] Error: not enough rows in the matrix\n");
    exit(1);  
  }

  copy = clan_matrix_malloc(n,matrix->NbColumns);
  
  for (i = 0; i < n; i++)
    for (j = 0; j < matrix->NbColumns; j++)
      CLAN_assign(copy->p[i][j],matrix->p[i][j]);
  
  return copy;
}


/**
 * clan_matrix_copy function:
 * this functions builds and returns a "hard copy" (not a pointer copy) of a
 * clan_matrix_t data structure.
 * \param matrix The pointer to the matrix we want to copy.
 **
 * - 30/04/2008: first version (from CLooG 0.14.0).
 * - 02/05/2008: no uses clan_matrix_ncopy.
 */
clan_matrix_p
clan_matrix_copy(clan_matrix_p matrix)
{
  if (matrix == NULL)
    return NULL;
    
  return clan_matrix_ncopy(matrix,matrix->NbRows);
}


/**
 * clan_matrix_replace_vector function:
 * this function replaces the "row"^th row of a matrix "matrix" with the
 * vector "vector". It directly updates "matrix".
 * \param matrix The matrix we want to change a row.
 * \param vector The vector that will replace a row of the matrix.
 * \param row    The row of the matrix to be replaced.
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_replace_vector(clan_matrix_p matrix, clan_vector_p vector, int row)
{
  int i;
 
  if ((matrix == NULL) || (vector == NULL) ||
      (matrix->NbColumns != vector->Size) ||
      (row >= matrix->NbRows) || (row < 0))
  {
    fprintf(stderr,"[Clan] Error: vector cannot replace a matrix row\n");
    exit(1);
  }   
    
  for (i = 0; i < vector->Size; i++)
    CLAN_assign(matrix->p[row][i],vector->p[i]);
}


/**
 * clan_matrix_insert_vector function:
 * this function adds a new row corresponding to the vector "vector" to
 * the matrix "matrix" by inserting it at the "row"^th row. It directly
 * updates "matrix". If "vector" (or "matrix") is NULL, the matrix is left
 * unmodified.
 * \param matrix The matrix we want to extend.
 * \param vector The vector that will be added matrix.
 * \param row    The row where to insert the vector (CLAN_END means at the end).
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_insert_vector(clan_matrix_p matrix, clan_vector_p vector, int row)
{
  int i, j;
  clan_matrix_p new;

  if ((vector == NULL) || (matrix == NULL))
    return;

  if (row == CLAN_END)
    row = matrix->NbRows;

  if ((matrix->NbColumns != vector->Size) ||
      (row > matrix->NbRows) || (row < 0))
  {
    fprintf(stderr,"[Clan] Error: vector cannot be inserted\n");
    exit(1);
  }
  
  /* We use a temporary matrix just to reuse existing functions. Cleaner. */
  new = clan_matrix_malloc(matrix->NbRows+1,matrix->NbColumns);
  
  for (i = 0; i < row; i++)
    for (j = 0; j < matrix->NbColumns; j++)
      CLAN_assign(new->p[i][j],matrix->p[i][j]);

  clan_matrix_replace_vector(new,vector,row);

  for (i = row+1; i < matrix->NbRows; i++)
    for (j = 0; j < matrix->NbColumns; j++)
      CLAN_assign(new->p[i][j],matrix->p[i-1][j]);
  
  clan_matrix_free_inside(matrix);
  
  /* Replace the inside of matrix */
  matrix->NbRows = new->NbRows;
  matrix->NbColumns = new->NbColumns;
  matrix->p = new->p;
  matrix->p_Init = new->p_Init;
  /* Free the new "shell" */
  free(new);
}


/**
 * clan_matrix_from_vector function:
 * this function converts a vector "vector" to a matrix with a single row
 * and returns a pointer to that matrix.
 * \param vector The vector to convert to a matrix.
 **
 * - 02/05/2008: first version.
 */
clan_matrix_p
clan_matrix_from_vector(clan_vector_p vector)
{
  clan_matrix_p matrix;

  if (vector == NULL)
    return NULL;

  matrix = clan_matrix_malloc(1,vector->Size);
  clan_matrix_replace_vector(matrix,vector,0);
  return matrix;
}


/**
 * clan_matrix_replace_matrix function:
 * this function replaces some rows of a matrix "m1" with the rows of
 * the matrix "m2". It begins at the "row"^th row of "m1". It directly
 * updates "m1".
 * \param m1  The matrix we want to change some row1.
 * \param m2  The matrix containing the new rows.
 * \param row The first row of the matrix m1 to be replaced.
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_replace_matrix(clan_matrix_p m1, clan_matrix_p m2, int row)
{
  int i, j;
 
  if ((m1 == NULL) || (m2 == NULL) ||
      (m1->NbColumns != m1->NbColumns) ||
      ((row + m2->NbRows) > m1->NbRows) || (row < 0))
  {
    fprintf(stderr,"[Clan] Error: vector cannot replace a matrix row\n");
    exit(1);
  }   
    
  for (i = 0; i < m2->NbRows; i++)
    for (j = 0; j < m2->NbColumns; j++)
      CLAN_assign(m1->p[i+row][j],m2->p[i][j]);
}


/**
 * clan_matrix_insert_matrix function:
 * this function adds new rows corresponding to the matrix "m1" to
 * the matrix "m2" by inserting it at the "row"^th row. It directly
 * updates "m1". If "m2" (or "m1") is NULL, the matrix is left
 * unmodified.
 * \param m1  The matrix we want to extend.
 * \param m2  The matrix to be inserted.
 * \param row The row where to insert the matrix (CLAN_END means at the end).
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_insert_matrix(clan_matrix_p m1, clan_matrix_p m2, int row)
{
  int i, j;
  clan_matrix_p new;

  if ((m1 == NULL) || (m2 == NULL))
    return;

  if (row == CLAN_END)
    row = m1->NbRows;

  if ((m1->NbColumns != m2->NbColumns) ||
      (row > m1->NbRows) || (row < 0))
  {
    fprintf(stderr,"[Clan] Error: matrix cannot be inserted\n");
    exit(1);
  }
  
  /* We use a temporary matrix just to reuse existing functions. Cleaner. */
  new = clan_matrix_malloc(m1->NbRows+m2->NbRows,m1->NbColumns);
  
  for (i = 0; i < row; i++)
    for (j = 0; j < m1->NbColumns; j++)
      CLAN_assign(new->p[i][j],m1->p[i][j]);

  clan_matrix_replace_matrix(new,m2,row);

  for (i = row + m2->NbRows; i < m1->NbRows; i++)
    for (j = 0; j < m1->NbColumns; j++)
      CLAN_assign(new->p[i][j],m1->p[i-m2->NbRows][j]);
  
  clan_matrix_free_inside(m1);
  
  /* Replace the inside of matrix */
  m1->NbRows = new->NbRows;
  m1->NbColumns = new->NbColumns;
  m1->p = new->p;
  m1->p_Init = new->p_Init;
  
  /* Free the new "container" */
  free(new);
}

/**
 * clan_matrix_concat function:
 * this function builds a new matrix as the concatenation of the rows of
 * two other matrices sent as parameters.
 * \param m1  The first matrix.
 * \param m2  The second matrix.
 **
 * - 02/05/2008: first version.
 */
clan_matrix_p
clan_matrix_concat(clan_matrix_p m1, clan_matrix_p m2)
{
  clan_matrix_p new;

  if (m1 == NULL)
    return clan_matrix_copy(m2);

  if (m2 == NULL)
    return clan_matrix_copy(m1);

  if (m1->NbColumns != m2->NbColumns)
  {
    fprintf(stderr,"[Clan] Error: matrices cannot be concatenated\n");
    exit(1);
  }
  
  new = clan_matrix_malloc(m1->NbRows+m2->NbRows,m1->NbColumns);
  clan_matrix_replace_matrix(new,m1,0);
  clan_matrix_replace_matrix(new,m2,m1->NbRows);
  
  return new;
}


/**
 * clan_matrix_tag_array function:
 * this function tags a matrix to explicit it is describing the array index of
 * a given array. This means using SCoP representation that the very first
 * element of the very first row will be the array number instead of zero.
 * It updates directly the matrix provided as parameter.
 * \param matrix The matrix to tag.
 * \param array  The array number.
 **
 * - 02/05/2008: first version.
 */
void
clan_matrix_tag_array(clan_matrix_p matrix, int array)
{
  if ((matrix == NULL) || (matrix->NbRows == 0))
  {
    fprintf(stderr,"[Clan] Error: matrix cannot be array-tagged\n");
    exit(1);
  }

  CLAN_set_si(matrix->p[0][0],array);
}


/**
 * clan_matrix_scheduling function:
 * this function builds the scheduling matrix for the clan_statement_t
 * structures thanks to the parser current state of parser_scheduling (rank)
 * and parser_depth (depth). The "rank" vector gives the "position" of the
 * statement for every loop depth (see Feautrier's demonstration of existence
 * of a schedule for any SCoP or CLooG's manual for original scattering
 * function to understand if necessary). This function just "expands" this
 * vector to a (2*n+1)-dimensional schedule for a statement at depth n and
 * returns it.
 * \param rank  The position of the statement at every loop depth.
 * \param depth The depth of the statement.
 **
 * - 01/05/2008: First version.
 */
clan_matrix_p
clan_matrix_scheduling(int * rank, int depth)
{ 
  int i, j, nb_rows, nb_columns;
  clan_matrix_p scheduling;
  
  nb_rows    = 2 * depth + 1;
  nb_columns = CLAN_MAX_DEPTH + CLAN_MAX_PARAMETERS + 2;
  scheduling = clan_matrix_malloc(nb_rows,nb_columns);
  
  j = 0;
  for (i = 0; i < depth; i++)
  {
    CLAN_set_si(scheduling->p[j][nb_columns-1],rank[i]);
    CLAN_set_si(scheduling->p[j+1][i+1],1);
    j += 2;
  }
  CLAN_set_si(scheduling->p[nb_rows-1][nb_columns-1],rank[depth]);
  
  return scheduling;
}


/**
 * clan_matrix_compact function:
 * This function compacts a matrix such that it uses the right number
 * of columns (during construction we used CLAN_MAX_DEPTH and
 * CLAN_MAX_PARAMETERS to define matrix and vector sizes). It modifies
 * directly the matrix provided as parameter.
 * \param matrix        The matrix to compact.
 * \param nb_iterators  The true number of iterators for this matrix.
 * \param nb_parameters The true number of parameters in the SCoP.
 **
 * - 02/05/2008: first version.
 * - 24/05/2008: nice bug fixed (p_Init_size was not copied, segfaulting later).
 */
void
clan_matrix_compact(clan_matrix_p matrix, int nb_iterators, int nb_parameters)
{ 
  int i, j, nb_columns;
  clan_matrix_p compacted;

  if (matrix == NULL)
    return;

  nb_columns = nb_iterators + nb_parameters + 2;
  compacted = clan_matrix_malloc(matrix->NbRows,nb_columns);

  for (i = 0; i < matrix->NbRows; i++)
  {
    /* We copy the equality/inequality tag and the iterator coefficients */
    for (j = 0; j <= nb_iterators; j++)
      CLAN_assign(compacted->p[i][j],matrix->p[i][j]);

    /* Then we copy the parameter coefficients */
    for (j = 0; j < nb_parameters; j++)
      CLAN_assign(compacted->p[i][j + nb_iterators + 1],
                  matrix->p[i][j + CLAN_MAX_DEPTH + 1]);
    
    /* Lastly the scalar coefficient */
    CLAN_assign(compacted->p[i][nb_columns - 1],
                matrix->p[i][matrix->NbColumns - 1]);
  }
  
  clan_matrix_free_inside(matrix);
  
  /* Replace the inside of matrix */
  matrix->NbRows      = compacted->NbRows;
  matrix->NbColumns   = compacted->NbColumns;
  matrix->p           = compacted->p;
  matrix->p_Init      = compacted->p_Init;
  matrix->p_Init_size = compacted->p_Init_size;
  
  /* Free the compacted "container" */
  free(compacted);
}
