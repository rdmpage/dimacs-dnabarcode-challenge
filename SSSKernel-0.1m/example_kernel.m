%load sequence data
fprintf(1,'Loading sequences...\n');
load('example_seq.mat');
fprintf(1,'Loaded %d sequences.\n',length(seq));
fprintf(1,'Median length = %d .\n', ceil(mean(cellfun('length',seq))));
m = 0;
for i=1:length(seq)
 m = max(m,max(seq{i}));
end
na = m + 1;
fprintf(1,'Alphabet size = %d\n', na);
% compute triple(1,3) kernel
fprintf(1,'Computing triple(1,3) kernel...\n');
K = tripleKernel(seq,3,na);
save('Kernel.triple.mat', 'K');
fprintf(1, 'Saved as Kernel.triple.mat\n');
% compute double(1,5) kernel
fprintf(1,'Computing double(1,5) kernel...\n');
K = doubleKernel(seq,5,na);
save('Kernel.double.mat', 'K');
fprintf(1, 'Saved as Kernel.double.mat\n');
