function [w] = generate_coeff(n)
%UNTITLED7 Summary of this function goes here
%   Detailed explanation goes here




for s = 1:1:log2(n)
    m = 2 ^ s;
    w(s) = exp(-2*pi*i/m);
end


%W_re = real(W);
%W_im = imag(W);

%W_re_fi = fi(W_re, 1, 16, 15);
%W_im_fi = fi(W_im, 1, 16, 15);


end