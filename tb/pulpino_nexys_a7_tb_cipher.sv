module pulpino_nexys_a7_tb_cipher ();
    logic        clk100mhz;
    logic [15:0] sw;
    logic [15:0] led;
    logic        cpu_resetn;
    tri   [7:0]  ja;
    logic        uart_rxd_out;
    logic        uart_txd_in;

    // Clock generation
    initial begin
        clk100mhz = 0;
        forever
            #5 clk100mhz = ~clk100mhz;
    end

    // Reset generation
    initial begin
        cpu_resetn = 1;
        #10
        cpu_resetn = 0;
        #100
        cpu_resetn = 1;
    end

    // Buttons and switches
    initial begin
        sw  = '0;
    end

    // JTAG placeholder
    assign ja[3] = 1'b0;
    assign ja[4] = 1'b1;
    assign ja[0] = 1'b0;
    assign ja[1] = 1'b0;

    pulpino_nexys_a7
    //#(.DATA_RAM_INIT_FILE  (""),
    //  .INSTR_RAM_INIT_FILE (""))
    UUT (
        .clk100mhz    (clk100mhz),
        .cpu_resetn   (cpu_resetn),
        .sw           (sw),
        .led          (led),
        .ja           (ja),
        .uart_rxd_out (uart_rxd_out),
        .uart_txd_in  (uart_txd_in)
    );

    // ���������� ���������� UART ����������
    logic uart_rx_ready, uart_tx_req, uart_tx_busy;
    logic [7:0] uart_rx_data, uart_tx_data;
    logic uart_clk, uart_rst;
    always begin
        uart_clk = 0;
        #25;
        uart_clk = 1;
        #25;
    end
    initial begin
        uart_rst = 1'b0;
        #100;
        uart_rst = 1'b1;
    end
    uart_wrapper # (
        .CLOCK_FREQUENCY(20*1000*1000),
        .BAUDRATE(115200)
    ) uart_wrapper_i (
        // �������� �������� � ������ ������
        .clk(uart_clk),
        .rstn(uart_rst),

        // ���� ������
        .rx_data(uart_rx_data), 	// �������� ������, ������� ��� rx_ready=1
        .rx_ready(uart_rx_ready),	// ������������ � 1 �� 1 ����, ����� �������� ������ ������� (����� ���������� ����� �����)

        // �������� ������
        .tx_data(uart_tx_data),	// ������, ������� ����� ��������
        .tx_req(uart_tx_req),	// ����� ��������� � 1 �� 1 ����, ����� ������ �������� ������, ����� tx_busy=0
        .tx_busy(uart_tx_busy),	// ���� ��������� �����������

        // ��������� UART
        .rxd(uart_rxd_out),
        .txd(uart_txd_in)
    );


    // ������� ����� �� UART
    task uart_send_byte(input byte data);

        // ���, ���� ���������� UART �� ����� ��������
        while (uart_tx_busy)
            #1;

        // �������� ������ �� �������� ������
        uart_tx_data = data;
        @(posedge uart_clk);
            uart_tx_req = 1;
        @(posedge uart_clk);
            uart_tx_req = 0;
        @(posedge uart_clk);

        // ������� ��������� ���������
        while (uart_tx_busy)
            #1;
    endtask


    // ���� ����� �� UART
    task uart_receive_byte(output byte data);
        int timeout;

        // ��� ����������� ������
        timeout = 10000000;
        while (!uart_rx_ready && timeout!=0) begin
            #1;
            timeout = timeout - 1;
        end
        if (timeout == 0) begin
            $display("Error: time limit exceeded");
            $finish();
        end

        // ��������� ������
        data = uart_rx_data;

        // ���, ����� ����� ����� ����, ����� �� ������� 2 ���� ���� � �� �� ������
        while (uart_rx_ready)
            #1;
    endtask


    // ���������� 1 ����� ������ (�������� ������ �� UART � �������� ������)
    task encrypt_block(input logic [127:0] src_data, output logic [127:0] dst_data);
        logic [7:0] in_data, out_data;

        for (int i=0; i<16; i++) begin
            for (int j=0; j<8; j++)
                in_data[j] = src_data[i*8+j];
            uart_send_byte(in_data);
        end

        for (int i=0; i<16; i++) begin
            uart_receive_byte(out_data);
            for (int j=0; j<8; j++)
                dst_data[i*8+j] = out_data[j];
        end
        
        #10;
    endtask


    // ������������ ����������
    logic [127:0] data_to_cipher[11];
    logic [127:0] ciphered_data[11];
    logic [128*11-1:0] print_str;
    int init_clocks;
    initial begin

        // ������ ��� ����������
        data_to_cipher[00] <= 128'h3ee5c99f9a41c389ac17b4fe99c72ae4;
        data_to_cipher[01] <= 128'h79cfed3c39fa7677b970bb42a5631ccd;
        data_to_cipher[02] <= 128'h63a148b3d9774cede1c54673c68dcd03;
        data_to_cipher[03] <= 128'h2ed02c74160391fd9e8bd4ba21e79a9d;
        data_to_cipher[04] <= 128'h74f245305909226922ac9d24b9ed3b20;
        data_to_cipher[05] <= 128'h03dde21c095413db093bb8636d8fc082;
        data_to_cipher[06] <= 128'hbdeb379c9326a275c58c756885c40d47;
        data_to_cipher[07] <= 128'h2dcabdf6b6488f5f3d56c2fd3d2357b0;
        data_to_cipher[08] <= 128'h887adf8b545c4334e0070c63d2f344a3;
        data_to_cipher[09] <= 128'h23feeb9115fab3e4f9739578010f212c;
        data_to_cipher[10] <= 128'h53e0ebee97b0c1b8377ac5bce14cb4e8;

        // ��� ��������� ������������� ����������
        $display("Testbench has been started.\n");
        init_clocks = 10000;
        for (int i=0; i<init_clocks; i++)
            @(posedge clk100mhz);

        // ������� ������ ���� ������
        $display("Ciphering has been started.");
        for (int i=0; i<11; i++)
            encrypt_block(data_to_cipher[i], ciphered_data[i]);

        // ������� ���������
        $display("Ciphering has been finished.");
        $display("============================");
        $display("===== Ciphered message =====");
        $display("============================");
        print_str = {ciphered_data[0],
            ciphered_data[1],
            ciphered_data[2],
            ciphered_data[3],
            ciphered_data[4],
            ciphered_data[5],
            ciphered_data[6],
            ciphered_data[7],
            ciphered_data[8],
            ciphered_data[9],
            ciphered_data[10]
        };
        $display("%s", print_str);
        $display("============================");
        $finish();
    end

endmodule
