#include "math.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  /*
  inputs:
    cell array of sequences
    parameter d1
  outputs:
    extracted features
    feature labels
                number of features per sequence
  */
  bool isVerbose;
  const mxArray *cellS, *d1_arg;
  int d1;
  int *sx, *g, *nf;
  int sumLen, numF, sLen, nStr;
  int i, j, c;
  double *sBuf;
  mxArray *sPtr;

  isVerbose = false;
  cellS = prhs[0];
  d1_arg = prhs[1];
  if (!mxIsCell(prhs[0]))
    mexErrMsgTxt("***Error: input is not a cell array!\n");
  nStr = mxGetNumberOfElements(cellS);  
  d1 = (int)(mxGetScalar(d1_arg));
  if (isVerbose)
  {
     mexPrintf("Number of strings:%d\n", nStr);
     mexPrintf("d=%d\n",d1);  
  }

  plhs[2] = mxCreateNumericMatrix(nStr, 1, mxINT32_CLASS, mxREAL);
  nf = (int *)mxGetPr(plhs[2]);
  sumLen = 0;
  numF = 0;
  for (i=0;i<nStr;i++)
  {
    sPtr = mxGetCell(cellS, i);
    sLen = mxGetNumberOfElements(sPtr);
    sumLen += sLen;
    nf[i] = ((sLen>=d1+1)?(sLen-d1):0);
    numF += nf[i];
  }

  if (isVerbose)
     mexPrintf("numF=%d, sumLen=%d\n", numF, sumLen);

  plhs[0] = mxCreateNumericMatrix(numF, 2, mxINT32_CLASS, mxREAL);
  plhs[1] = mxCreateNumericMatrix(numF, 1, mxINT32_CLASS, mxREAL);
  sx = (int *)mxGetPr(plhs[0]);
  g = (int *)mxGetPr(plhs[1]);
  c=0;
  for (i = 0; i < nStr; ++i)
  {
    sPtr = mxGetCell(cellS, i);
    sLen = mxGetNumberOfElements(sPtr);
    sBuf = mxGetPr(sPtr);
    for (j = 0;j < sLen-d1; ++j)
    {
        sx[c+0*numF]=(int)sBuf[j];
        sx[c+1*numF]=(int)sBuf[j+d1];
        g[c]=i;
        c++;
    }
  }
  if (isVerbose)
    mexPrintf("d=%d Number of features extracted=%d\n", d1, c);

  return;
}
