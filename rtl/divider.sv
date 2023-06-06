`timescale 1ns / 1ps

module divider(
    input logic clkin,
    input logic rst,
    output logic clkout
);

logic [3:0] counter;
parameter divider = 1;

always_ff @(posedge clkin, negedge rst) begin
    if (!rst) begin
        counter = 0;
        clkout = 1;
    end else begin
        if (counter < divider) begin
            counter = counter + 1;
        end else begin
            counter = 0;
            clkout = !clkout;
        end
    end
end

endmodule
