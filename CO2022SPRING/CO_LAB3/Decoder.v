//Subject:     CO project 2 - Decoder
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      Luke
//----------------------------------------------
//Date:        2010/8/16
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------

module Decoder(
    instr_op_i,
	RegWrite_o,
	ALU_op_o,
	ALUSrc_o,
	RegDst_o,
	Branch_o,
	Jump_o,
    MemRead_o,
    MemoryWrite_o,
    MemtoReg_o
	);
     
//I/O ports
input  [6-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output [2-1:0] RegDst_o;
output         Branch_o;
output         Jump_o;
output         MemRead_o;
output         MemoryWrite_o;
output [2-1:0] MemtoReg_o;
 
//Internal Signals
reg    [3-1:0] ALU_op_o;
reg            ALUSrc_o;
reg            RegWrite_o;
reg    [2-1:0] RegDst_o;
reg            Branch_o;
reg            Jump_o;
reg            MemRead_o;
reg            MemoryWrite_o;
reg    [2-1:0] MemtoReg_o;

//Parameter

wire r, addi, slti, beq, lw, sw, jump, jal;


//Main function

assign r = (instr_op_i == 0);
assign addi = (instr_op_i == 8);
assign slti = (instr_op_i == 10);
assign beq = (instr_op_i == 4);
assign lw = (instr_op_i == 35);
assign sw = (instr_op_i == 43);
assign jump = (instr_op_i == 2);
assign jal = (instr_op_i == 3);

always@(*)begin
	if(r)
	   ALU_op_o = 1;
	else if(addi)
	   ALU_op_o = 2;
	else if(slti)
	   ALU_op_o = 3;
	else if(beq)
	   ALU_op_o = 4;
	else if(lw)
	   ALU_op_o = 5;
	else if(sw)
	   ALU_op_o = 6;
    	else
       	   ALU_op_o = 0;

    	if(jal)
	    RegDst_o = 2;
	else if(r)
	    RegDst_o = 1;
    	else
            RegDst_o = 0;





    	if(jal)
	    MemtoReg_o = 2;
	else if(lw)
	    MemtoReg_o = 1;
    	else
            MemtoReg_o = 0;

	Branch_o = beq;
        RegWrite_o = r | addi | slti | lw | jal;
	Jump_o = jal | jump;
	MemRead_o = lw;
	MemoryWrite_o = sw;
        ALUSrc_o = addi | slti | lw | sw;
end

endmodule





                    
                    