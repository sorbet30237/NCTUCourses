//Subject:     CO project 2 - Simple Single CPU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      109550112
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
`timescale 1ns/1ps
module Simple_Single_CPU(
        clk_i,
		rst_i
		);
		
//I/O port
input         clk_i;
input         rst_i;

//Internal Signles
wire [31:0] pc_in, pc_out, add1_out, ist_out, sign_out, shift_out, add2_out, regs_out, regt_out, aluresult, muxalu_out;
wire [4:0] muxreg1_out;
wire [3:0] aluctrl;
wire [2:0] aluop;
wire regdst,branch,regwrite,alusrc,zero, adders_select;
assign adders_select = branch & zero;
//Greate componentes
ProgramCounter PC(
        .clk_i(clk_i),      
	    .rst_i (rst_i),     
	    .pc_in_i(pc_in) ,   
	    .pc_out_o(pc_out) 
	    );
	
Adder Adder1(
        .src1_i(32'd4),     
	    .src2_i(pc_out),     
	    .sum_o(add1_out)    
	    );
	
Instr_Memory IM(
        .pc_addr_i(pc_out),  
	    .instr_o(ist_out)    
	    );

MUX_2to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(ist_out[20:16]),
        .data1_i(ist_out[15:11]),
        .select_i(regdst),
        .data_o(muxreg1_out)
        );	
		
Reg_File RF(
        .clk_i(clk_i),      
	    .rst_i(rst_i) ,     
        .RSaddr_i(ist_out[25:21]) ,  
        .RTaddr_i(ist_out[20:16]) ,  
        .RDaddr_i(muxreg1_out) ,  
        .RDdata_i(aluresult)  , 
        .RegWrite_i(regwrite),
        .RSdata_o(regs_out) ,  
        .RTdata_o(regt_out)   
        );
	
Decoder Decoder(
        .instr_op_i(ist_out[31:26]), 
	    .RegWrite_o(regwrite), 
	    .ALU_op_o(aluop),   
	    .ALUSrc_o(alusrc),   
	    .RegDst_o(regdst),   
		.Branch_o(branch)   
	    );

ALU_Ctrl AC(
        .funct_i(ist_out[5:0]),   
        .ALUOp_i(aluop),   
        .ALUCtrl_o(aluctrl) 
        );
	
Sign_Extend SE(
        .data_i(ist_out[15:0]),
        .data_o(sign_out)
        );

MUX_2to1 #(.size(32)) Mux_ALUSrc(
        .data0_i(regt_out),
        .data1_i(sign_out),
        .select_i(alusrc),
        .data_o(muxalu_out)
        );	
		
ALU ALU(
        .src1_i(regs_out),
	    .src2_i(muxalu_out),
	    .ctrl_i(aluctrl),
	    .result_o(aluresult),
		.zero_o(zero)
	    );
		
Adder Adder2(
        .src1_i(add1_out),     
	    .src2_i(shift_out),     
	    .sum_o(add2_out)      
	    );
		
Shift_Left_Two_32 Shifter(
        .data_i(sign_out),
        .data_o(shift_out)
        ); 		
		
MUX_2to1 #(.size(32)) Mux_PC_Source(
        .data0_i(add1_out),
        .data1_i(add2_out),
        .select_i(adders_select),
        .data_o(pc_in)
        );	

endmodule
		  


