`timescale 1ns / 1ps
module audpwm (
    input logic clk,
    output logic aud_pwm,
    input logic start,
    input logic rst_i
);

parameter DATA_WIDTH = 8;

logic [DATA_WIDTH-1:0] count;
logic [DATA_WIDTH-1:0] duty;

logic [DATA_WIDTH-1:0] music [0:10000000];
logic [100:0] index = 0;

initial $readmemb("music.mem", music);
    // $readmemb("music.mem", music);

logic [2:0] state;

parameter IDLE = 0, READ = 1, PLAY = 2;
parameter MUSIC_LEN = 156249;

always_ff @(posedge clk, posedge start) begin
    if (rst_i) begin
        state = IDLE;
        count = 0;
        index = 0;
    end else begin
        case (state)
            IDLE: begin
                if (start) begin
                    state = READ;
                end
            end
            READ: begin
                if (index < MUSIC_LEN) begin
                    duty = music[index];
                    index = index + 1'b1;
                    state = PLAY;
                end else begin
                    state = IDLE;
                end
            end
            PLAY: begin
                if (&count == 1'b1) begin
                    count <= 1'b0;
                    state = READ;
                end else begin
                    count = count + 1'b1;
                end
            end
        endcase
    end
end

assign aud_pwm = (count <= duty);

endmodule