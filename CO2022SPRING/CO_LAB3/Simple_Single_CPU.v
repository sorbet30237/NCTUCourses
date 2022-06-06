//Subject:     CO project 2 - Simple Single CPU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
module Simple_Single_CPU(
        clk_i,
		rst_i
		);
		
//I/O port
input         clk_i;
input         rst_i;

//Internal Signles
wire [31:0] pc_out, ins_memory, regtoalu, regt_o, signextend, muxtoalu, alu_result, data_out, writereg;
wire [31:0] sl2_o, add1_o, add2_o, pc_in, beqtojump, jumptojr,  jump_adress, sl2_jump_o;
wire [4:0]  muxtoreg;
wire [3:0]  alu_ctrl;
wire [2:0]  aluop;
wire [1:0] regdst, MemtoReg;
wire  regwrite, branch, alusrc, zero, adder_select, Jump, MemRead, MemWrite, jr;
assign adder_select = zero & branch;
assign jump_adress = {add1_o[31:28], sl2_jump_o[27:0]};
//Greate componentes
ProgramCounter PC(///vv
        .clk_i(clk_i),      
	    .rst_i (rst_i),     
	    .pc_in_i(pc_in) ,   
	    .pc_out_o(pc_out) 
	    );
	
Adder Adder1(///vv
        .src1_i(32'd4),     
	    .src2_i(pc_out),     
	    .sum_o(add1_o)    
	    );
	
Instr_Memory IM(///vv
        .pc_addr_i(pc_out),  
	    .instr_o(ins_memory)    
	    );

MUX_3to1 #(.size(5)) Mux_Write_Reg(///vv
        .data0_i(ins_memory[20:16]),
        .data1_i(ins_memory[15:11]),
        .data2_i(5'd31),
        .select_i(regdst),
        .data_o(muxtoreg)
        );
		
Reg_File Registers(
        .clk_i(clk_i),      
	    .rst_i(rst_i) ,     
        .RSaddr_i(ins_memory[25:21]) ,  
        .RTaddr_i(ins_memory[20:16]) ,  
        .RDaddr_i(muxtoreg) ,  
        .RDdata_i(writereg)  , 
        .RegWrite_i (regwrite),
        .RSdata_o(regtoalu) ,  
        .RTdata_o(regt_o)   
        );
	
Decoder Decoder(///vv
        .instr_op_i(ins_memory[31:26]), 
	    .RegWrite_o(regwrite), 
	    .ALU_op_o(aluop),   
	    .ALUSrc_o(alusrc),   
	    .RegDst_o(regdst),   
		.Branch_o(branch),
	    .Jump_o(jump),
        .MemRead_o(MemRead),
        .MemoryWrite_o(MemWrite),
        .MemtoReg_o(MemtoReg)
	    );

ALU_Ctrl AC(///vv
        .funct_i(ins_memory[5:0]),   
        .ALUOp_i(aluop),   
        .ALUCtrl_o(alu_ctrl),
        .jr_o(jr)
        );
	
Sign_Extend SE(///vv
        .data_i(ins_memory[15:0]),
        .data_o(signextend)
        );


		
ALU ALU(///vv
        .src1_i(regtoalu),
	    .src2_i(muxtoalu),
	    .ctrl_i(alu_ctrl),
	    .result_o(alu_result),
		.zero_o(zero)
	    );
		
Data_Memory Data_Memory(
	.clk_i(clk_i),
	.addr_i(alu_result),
	.data_i(regt_o),
	.MemRead_i(MemRead),
	.MemWrite_i(MemWrite),
	.data_o(data_out)
);

Adder Adder2(///vv
        .src1_i(add1_o),     
	    .src2_i(sl2_o),     
	    .sum_o(add2_o)      
	    );
		
Shift_Left_Two_32 Shifter(///vv
        .data_i(signextend),
        .data_o(sl2_o)
        );
        
Shift_Left_Two_32 Shifter_jump(///vv
        .data_i({6'b0, ins_memory[25:0]}),
        .data_o(sl2_jump_o)
        ); 	 		

MUX_2to1 #(.size(32)) Mux_ALUSrc(///vv
        .data0_i(regt_o),
        .data1_i(signextend),
        .select_i(alusrc),
        .data_o(muxtoalu)
        );	
		
MUX_2to1 #(.size(32)) Mux_PC_Source_beq(///vv
        .data0_i(add1_o),
        .data1_i(add2_o),
        .select_i(adder_select),
        .data_o(beqtojump)
        );
        
MUX_2to1 #(.size(32)) Mux_PC_Source_jump(///vv
        .data0_i(beqtojump),
        .data1_i(jump_adress),
        .select_i(jump),
        .data_o(jumptojr)
        );
MUX_2to1 #(.size(32)) Mux_PC_Source_jr(///vv
        .data0_i(jumptojr),
        .data1_i(regtoalu),
        .select_i(jr),
        .data_o(pc_in)
        );
        
MUX_3to1 #(.size(32)) Mux_Mem_to_Reg(///vv
        .data0_i(alu_result),
        .data1_i(data_out),
        .data2_i(add1_o),
        .select_i(MemtoReg),
        .data_o(writereg)
        );	
        
	
               


endmodule
		  

