#include "math.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  /*
  inputs:
    cell array of sequences
    parameters d1, d2
  outputs:
    extracted features
    feature labels
                number of features per sequence
  */
  bool isVerbose;
  const mxArray *cellS, *d1_arg, *d2_arg;
  int nStr, d1, d2;
  double *sBuf;
  int sLen;
  int i, j, c;
  int *sx, *g, *nf;
  int sumLen, numF;
  mxArray *sPtr;

  cellS=prhs[0];
  d1_arg=prhs[1];
  d2_arg=prhs[2];
  isVerbose = false;
  if (!mxIsCell(cellS))
  {
    mexErrMsgTxt("***Error: input is not a cell array!\n");
  }
  nStr=mxGetNumberOfElements(cellS);  
  d1 = (int)(mxGetScalar(d1_arg));
  d2 = (int)(mxGetScalar(d2_arg));
  if (isVerbose)
  {
     mexPrintf("Number of strings:%d\n", nStr);
     mexPrintf("d1=%d, d2=%d\n",d1,d2);  
  }
        
  plhs[2]=mxCreateNumericMatrix(nStr, 1, mxINT32_CLASS, mxREAL);
  nf= (int *)mxGetPr(plhs[2]);
  sumLen = 0;
  numF = 0;
  for (i = 0;i < nStr; ++i)
  {
    sPtr = mxGetCell(cellS, i);
    sLen = mxGetNumberOfElements(sPtr);
    sumLen += sLen;
    nf[i] = ((sLen>=d1+d2+1)?(sLen-d1-d2):0);
    numF += nf[i];
  }
  if (isVerbose)
  {
    mexPrintf("numF=%d, sumLen=%d\n", numF, sumLen);
  }
  plhs[0]=mxCreateNumericMatrix(numF, 3, mxINT32_CLASS, mxREAL);
  plhs[1]=mxCreateNumericMatrix(numF, 1, mxINT32_CLASS, mxREAL);
  sx = (int *)mxGetPr(plhs[0]);
  g = (int *)mxGetPr(plhs[1]);
  c = 0;
  for (i = 0;i < nStr; ++i)
  {
    sPtr = mxGetCell(cellS, i);
    sLen = mxGetNumberOfElements(sPtr);
    sBuf = mxGetPr(sPtr);
    for (j = 0;j < sLen-d1-d2; ++j)
    {
        sx[c+0*numF]=(int)sBuf[j];
        sx[c+1*numF]=(int)sBuf[j+d1];
        sx[c+2*numF]=(int)sBuf[j+d1+d2];
        g[c]=i;
        c++;
    }
  }
  if (isVerbose)
    mexPrintf("(%d,%d) Number of features extracted=%d\n", d1, d2, c);
  
  return;
}
