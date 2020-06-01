n = 128;

x = 1:n;

[f, w] = cooley_tukey_fft(x);


W_re = real(w);
W_im = imag(w);

W_re_fi = fi(W_re, 1, 16, 15);
W_im_fi = fi(W_im, 1, 16, 15);



for i = 1:length(W_re_fi)
    fprintf('{ %s, %s},\n', dec(W_im_fi(i)), dec(W_re_fi(i)));
end

