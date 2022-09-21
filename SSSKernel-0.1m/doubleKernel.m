function K=doublePureSortinmexna(S,d,na)
%DOUBLEKERNEL double kernel for strings
%  K=doubleKernel(S,d,na) computes double(1,d) kernel for strings S
%  over alphabet [0:na-1].
%
%  INPUTS:
%    1. S - cell array of strings, each cell S{i} is
%    a double array with elements in the range [0, na-1], whera na is 
%    the alphabet size
%    2. d - max. distance between samples (eg. d=3)
%    3. na - alphabet size (eg. na=20)
%  OUTPUTS:  
%    1. K - kernel matrix (double matrix of size |S|-by-|S|)
%
%  Example:
%
%    S={[1 0 2 3 2 1],[1 1 2 0 1 1]}; % some sequences over [0:3] alphabet
%    K=doubleKernel(S,5,4);           % triple(1,3) kernel
%
%  Reference:
%
%  Pavel Kuksa, Pai-Hsi Huang, Vladimir Pavlovic. Fast Protein Homology
%  and Fold Detection with Sparse Spatial Sample Kernels. ICPR 2008.

num_strings = length(S);
K = zeros(num_strings, num_strings, 'int32');
for d1=1:d
  fprintf(1,'d=%d\n', d1);
  [sxy resgroup nf] = mexExtractDouble(S,d1);
  regroup = mexcntsrtna(sxy,2,na);
  sxy = sxy(regroup+1,:);
  resgroup = resgroup(regroup+1);
  K = K + mexextractuniquena_int(sxy, resgroup, 2, num_strings, na);
end
clear sxy resgroup regroup
K=double(K);
