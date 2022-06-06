//Subject:     CO project 2 - Decoder
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
module Decoder(
    instr_op_i,
	RegWrite_o,
	ALU_op_o,
	ALUSrc_o,
	RegDst_o,
	Branch_o
	);
     
//I/O ports
input  [6-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output         RegDst_o;
output         Branch_o;
 
//Internal Signals
reg    [3-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg            RegDst_o;
reg            Branch_o;

//Parameter
wire r,addi, slti, beq;

//Main function
assign r = (instr_op_i == 0);
assign addi = (instr_op_i == 8);
assign beq = (instr_op_i == 4);
assign slti = (instr_op_i == 10);

always@(*)
    begin
    RegWrite_o = addi | r | slti;
    ALUSrc_o = addi | slti;
    RegDst_o = r;
    Branch_o = beq;
    ALU_op_o[2] = r;
	ALU_op_o[1] = addi | beq;
	ALU_op_o[0] = beq | slti;
    end
endmodule





                    
                    