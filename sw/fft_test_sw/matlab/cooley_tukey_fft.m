function [A, w_out] = cooley_tukey_fft(a)

   A = bit_reverse_copy(a);

   n = length(a);
   
   w_m_mem = generate_coeff(n);

   w_out = [];
   
   for s = 1:1:log2(n)
    m = 2 ^ s;
    
    w_m = w_m_mem(s); % s-1
   
    for k = 0:m:(n-1)
        w = 1;
        
        for j = 0:1:(m/2 - 1)
            
            x2 = A(k + j + m/2 + 1); % k + j + m/2
            x1 = A(k + j + 1); % k + j 
            
            w_out = [w_out, w]; % DEBUG
            [y1, y2] = buttrefly2_dit(x1, x2, w);

            A(k + j + 1) = y1; % k + j
            A(k + j + m/2 + 1) = y2; % k + j + m/2
            w = w * w_m;
        end
    end
    
   end
   

end

