`timescale 1ns / 1ps


`define START 32'h0000
`define RST 32'h0004


module aud_pwm_apb_wrapper (
    // Clock
    input logic pclk_i,

    // Reset
    input logic presetn_i,

    // Address
    input logic [31:0] paddr_i,

    // Control-status
    input logic psel_i,
    input logic penable_i,
    input logic pwrite_i,

    // Write
    input logic [3:0][7:0] pwdata_i,
    input logic [3:0]      pstrb_i,

    // Slave
    output logic        pready_o,
    output logic [31:0] prdata_o,
    output logic        pslverr_o,

    output logic aud_pwm
);

  ///////////////
  // Registers //
  ///////////////

  logic [31:0] start_reg;
  logic [31:0] rst_reg;

  /////////////////////////////////////
  // SPI Accelerometer instantiation //
  /////////////////////////////////////

  logic aud_start;
  assign aud_start = &start_reg;

  logic aud_rstn;
  assign aud_rstn = presetn_i && ~rst_reg[0];

  aud_pwm audpwm (
      .clk(pclk_i),
      .rstn(aud_rstn),
      .start(aud_start),
      .aud_pwm(aud_pwm)
  );

  // Control

  always_ff @(posedge pclk_i) begin
    pready_o <= psel_i;
  end

  logic psel_prev;


  typedef enum {
    NONE = 0,
    PENABLE = 1,
    PWRITE = 2,
    PSEL_PREV = 3,
    ADDRES = 4,
    READ_ONLY = 5,
    WRITE_ONLY = 6,
    REQUEST = 7,
    MISALIGN = 8
  } pslverr_causes_t;

  logic [2:0] pslverr_status;

  always_comb begin
    pslverr_o <= 0;
    psel_prev <= psel_i;

    pslverr_status <= NONE;

    // Wrong transaction phase

    if (penable_i && ~psel_i) begin
      pslverr_o <= 1;
      pslverr_status <= PENABLE;
    end
    if (pwrite_i && ~psel_i) begin
      pslverr_o <= 1;
      pslverr_status <= PWRITE;
    end
    if (~psel_prev && penable_i) begin
      pslverr_o <= 1;
      pslverr_status <= PSEL_PREV;
    end

    if (paddr_i > `DATA_READY) begin  // Register at the address doesn't exist
      pslverr_o <= 1;
      pslverr_status <= ADDRES;
    end

    if ((paddr_i <= `RESET) && ~pwrite_i && psel_i) begin  // Read from write-only register
      pslverr_o <= 1;
      pslverr_status <= WRITE_ONLY;
    end

    if (paddr_i[1:0]) begin  // Misaligned address
      pslverr_o <= 1;
      pslverr_status <= MISALIGN;
    end
  end

  // WRITE REGS
  always_ff @(posedge penable_i) begin
    pslverr_o <= 0;
    if (penable_i && pwrite_i) begin
      case (paddr_i)
        `START: begin
          start_reg[7:0]   <= pwdata_i[0];
          start_reg[15:8]  <= pwdata_i[1];
          start_reg[23:16] <= pwdata_i[2];
          start_reg[31:24] <= pwdata_i[3];
        end
        `RST: begin
          rst_reg[7:0]   <= pwdata_i[0];
          rst_reg[15:8]  <= pwdata_i[1];
          rst_reg[23:16] <= pwdata_i[2];
          rst_reg[31:24] <= pwdata_i[3];
        end
        default: begin
          start_reg <= 32'b0;
          rst_reg   <= 32'b0;
        end
      endcase
    end
  end

endmodule

