#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shared.h"

#define ARG_REQUIRED 3

Features *extractDouble(int **S, int *len, int nStr, int d1)
{
  int i, j;
  int n, nfeat;
  int *group;
  int *features;
  int *s;
  int c;
  Features *F;

  nfeat = 0;
  for (i = 0; i < nStr; ++i)
    nfeat += len[i] - d1;
  group = (int *)malloc(nfeat*sizeof(int));
  features = (int *)malloc(nfeat*2*sizeof(int *));
  c = 0;
  for (i = 0; i < nStr; ++i)
  {
    n = len[i] - d1;
    s = S[i];
    for (j = 0; j < n; ++j)
    {
      features[c] = s[j];
      features[c+nfeat] = s[j+d1];
      group[c] = i;
      c++;
    }
  }
  if (nfeat!=c)
    printf("Something is wrong...\n");
  F = (Features *)malloc(sizeof(Features));
  (*F).features = features;
  (*F).group = group;
  (*F).n = nfeat;
  return F;
}

int help()
{
  printf("Usage: doubleKernel <Sequence-file> <d> <Alphabet-size>\n");
  printf("\t Sequence-file : file with sequence data\n");
  printf("\t d : kernel parameter (max. distance between samples), d>0\n");
  printf("\t Alphabet-size: size of the sequence alphabet, >0\n");
  printf("\t IMPORTANT: sequence elements are assumed to be\n\tin the range [0,AlphabetSize - 1]\n");
  return 1;
}

int main (int argc, char **argv)
{
  
  char *filename;
  int d;
  int na;
  int d1;
  int i, j;
  int nStr;
  int nfeat;
  int *K;
  int *len;
  int **S;
  int *sortIdx;
  int *features_srt;
  int *group_srt;
  int *feat;
  int *g;
  int maxIdx;
  char isVerbose;
  Features *features;

  isVerbose = 0;
  if (argc != ARG_REQUIRED+1)
  {
    return help();
  }

  filename = argv[1];
  d = atoi(argv[2]);
  na = atoi(argv[3]);
  if (d <= 0 || na <=0)
    return help();
  
  len = (int *)malloc(MAXNSTR*sizeof(int));
  printf("Reading %s\n",filename);
  S = loadStrings(filename, len, &nStr);
  printf("Read %d strings\n", nStr);
  K = (int *)malloc(nStr*nStr*sizeof(int));
  for (i=0; i<nStr*nStr; ++i)
    K[i]=0;
  for (d1 = 1; d1 <= d; ++d1)
  {
      if (isVerbose)
      {
        printf("Extracting features...");
        fflush( stdout );
      }
      features = extractDouble(S,len,nStr,d1);
      nfeat = (*features).n;
      printf("d=%d: %d features\n", d1, nfeat);
      maxIdx = 0;
      for (i = 0; i < nfeat*2; ++i)
        maxIdx = maxIdx > (*features).features[i] ? maxIdx : (*features).features[i];
      if (na < maxIdx+1)
      {
        printf("WARNING: Sequence elements are outside the specified range [0,%d]\n",na);
        printf("\tUsing [0,%d] instead\n", maxIdx+1);
        na = maxIdx+1;
      }
      if (isVerbose)
      {
        printf("done.\n");
        printf("Sorting...");
        fflush( stdout );
      }
      sortIdx = cntsrtna((*features).features,2,(*features).n,na);
      if (isVerbose)
      {
        printf("done.\n");
      }
      features_srt = (int *)malloc(nfeat*2*sizeof(int *));
      group_srt = (int *)malloc(nfeat*sizeof(int));
      for (i = 0; i < nfeat; ++i)
      {
        features_srt[i]=(*features).features[sortIdx[i]];
        features_srt[i+nfeat]=(*features).features[sortIdx[i]+nfeat];
        group_srt[i] = (*features).group[sortIdx[i]];
      }
      free(sortIdx);
      free((*features).features);
      free((*features).group);
      free(features);
      if (isVerbose)
      {
        printf("Counting...");
        fflush( stdout );
      }
      countAndUpdate(K,features_srt,group_srt,2,nfeat,nStr);
      free(features_srt);
      free(group_srt);
      if (isVerbose)
      {
        printf("done.");
      }
  }
  FILE *kernelfile;
  kernelfile = fopen("kernel.txt", "w");
  for (i = 0; i < nStr; ++i)
  {
     for (j = 0; j < nStr; ++j)
       fprintf(kernelfile, "%d ", K[i+j*nStr]);
     fprintf(kernelfile,"\n");
  }   
  fclose(kernelfile);
  free(K);
  return 0;
}
