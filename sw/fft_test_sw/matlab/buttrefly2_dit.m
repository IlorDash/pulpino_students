function [y1, y2] = buttrefly2_dit(x1, x2, w)

mult_res = x2 * w;


y1 = x1 + mult_res;

y2 = x1 - mult_res;


end

