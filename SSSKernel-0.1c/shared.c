#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **loadStrings(char *filename, int* len, int *nStr)
{
  int **S;
  char *str, *linetemp, *line, *next_elem;
  int i, j, c;
  int elem;
  FILE *inpfile;
  inpfile = fopen ( filename, "r" );
  nStr[0] = MAXNSTR;
  S = (int **)malloc(nStr[0]*sizeof(int *));
  if ( inpfile )
  {
     line = (char *)malloc(STRMAXLEN*sizeof(char));
     i = 0;
     while( fgets( line, STRMAXLEN, inpfile ) )
     {
        c = 0;
        linetemp = (char *)malloc(STRMAXLEN*sizeof(char));
        strcpy(linetemp, line);
        next_elem = strtok(linetemp," ");
        while (next_elem != NULL)
        {
          c++;
          next_elem = strtok(NULL," ");
        }
        free(linetemp);
        len[i] = c;
        S[i] = (int *)malloc(len[i]*sizeof(int));
        linetemp = (char *)malloc(STRMAXLEN*sizeof(char));
        strcpy(linetemp, line);
        c = 0;
        next_elem = strtok(linetemp," ");
        while (next_elem != NULL)
        {
          elem = atoi( next_elem );
          S[i][c] = elem;
          c++;
          next_elem = strtok(NULL," ");
        }
        free(linetemp);
        i++;
     }
     nStr[0] = i;
     fclose(inpfile);
     free(line);
  }
  else
  {
    perror( filename );   
  }
  return S;
}


int *cntsrtna(int *sx, int k, int r, int na)
{
    int *sxc = (int *)malloc(na*sizeof(int));
    int *bc  = (int *)malloc(na*sizeof(int));
    int *sxl = (int *)malloc(r*sizeof(int));
    int *cc  = (int *)malloc(r*sizeof(int));
    int *regroup = (int *)malloc(r*sizeof(int));
    int i, j;
    for (i = 0; i < r; ++i)
       regroup[i]=i;
    for (j = k-1; j >= 0; --j)
    {
       for(i = 0; i < na; ++i)
         sxc[i]=0;
       for (i = 0; i < r; ++i)
       {
                cc[i]=sx[regroup[i]+j*r];
		sxc[ cc[i] ]++;
       }
       bc[0]=0;
       for (i = 1;i < na; ++i)
           bc[i]=bc[i-1]+sxc[i-1];
       for (i = 0; i < r; ++i)
            sxl[bc[ cc[i] ]++] = regroup[i];
       for (i = 0; i < r; ++i)
            regroup[i] = sxl[i];
    }
    free(sxl); free(bc); free(sxc); free(cc);
    return regroup;
}

void countAndUpdate(int *outK, int *sx, int *g, int k, int r, int nStr)
{
   char same;
   int i, j; 
   int cu;
   long int ucnt;
   long int startInd, endInd, j1;
   int *curfeat = (int *)malloc(k*sizeof(int));
   int *ucnts= (int *)malloc(nStr*sizeof(int));
   int *updind = (int *)malloc(nStr*sizeof(int));
 
   /*printf("r=%d,nStr=%d,k=%d\n", r, nStr, k);*/
   i = 0;
   ucnt = 0;
   while (i<r)
   {
     for (j = 0; j < k; ++j)
	curfeat[j]=sx[i+j*r];
     same=1;
     for (j = 0;j < k; ++j)
	if (curfeat[j]!=sx[i+j*r])
	{
	   same=0;
	   break;
	}
     startInd=i;
     while (same && i<r)
     {
	i++;
	if (i >= r) break;
        same = 1;
	for (j = 0; j < k; ++j)
	   if (curfeat[j]!=sx[i+j*r])
	   {
	   	same=0;
	   	break;
	   }
     }
     endInd= (i<r) ? (i-1):(r-1);
     ucnt++;
     if ((long int)endInd-startInd+1>2*nStr)	
     {
	for (j = 0; j < nStr; ++j) ucnts[j]=0;
        for (j = startInd;j <= endInd; ++j)  ucnts[g[j]]++;
	cu=0;
	for (j=0;j<nStr;j++)
	{
	   if (ucnts[j]>0)
	   {
	     updind[cu] = j;
             cu++;
	   }
	}
	for (j=0;j<cu;j++)
	  for (j1=0;j1<cu;j1++)
	    outK[updind[j]+updind[j1]*nStr]+=ucnts[updind[j]]*ucnts[updind[j1]];
     }
     else
     {
        for (j = startInd;j <= endInd; ++j)
          for (j1 = startInd;j1 <= endInd; ++j1)
            outK[ g[j]+nStr*g[j1] ]++;
     }   
  }
  free(updind);
  free(ucnts);
  free(curfeat);
}


double nchoosek(double n, double k)
{
  int i;
  double *nums, *dens;
  double prod;
  if (k > n/2) k=n-k;
  if (k==0) return 1;
  if (k==1) return n;
  if (k > 1)
  { 
    nums = malloc(k*sizeof(double));
    dens = malloc(k*sizeof(double));
    for (i = 0; i < k; ++i)
    {
       nums[i] = n-k+1+i;
       dens[i] = i+1;
    }  
    prod = 1;
    for (i = 0; i < k; ++i)
    {
      nums[i] = nums[i] / dens[i];
      prod = prod * nums[i];
    }
    free(nums); free(dens);
  }
  return prod;
}


void getCombinations(const int *elems, unsigned int n, unsigned int k, int *pos, unsigned int depth, unsigned int margin, int *cnt_comb, int *out, int num_comb)
{
 int j;
 if (depth >= k)
 {
    for (j = 0; j < k; ++j)
       out[cnt_comb[0] + j*num_comb] = elems[pos[j]];
    cnt_comb[0]++;    
    return; 
 }
 for (j = margin; j < n; ++j)
 {
    pos[depth]=j;
    getCombinations(elems, n, k, pos, depth + 1, j + 1, cnt_comb, out, num_comb);    
 }
}
