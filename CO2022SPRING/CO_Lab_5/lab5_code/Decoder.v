//student id:109550112
//Subject:     CO project 2 - Decoder
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      109550112
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
    MemRead_o,
    MemoryWrite_o,
    MemtoReg_o,
    Branchtype
	);
     
//I/O ports
input  [6-1:0] instr_op_i;

output         RegWrite_o;
output [3-1:0] ALU_op_o;
output         ALUSrc_o;
output         RegDst_o;
output         Branch_o;
output         MemRead_o;
output         MemoryWrite_o;
output         MemtoReg_o;
output [1:0]   Branchtype;



 
//Internal Signals
reg         RegWrite_o;
reg [3-1:0] ALU_op_o;
reg         ALUSrc_o;
reg         RegDst_o;
reg         Branch_o;
reg         MemRead_o;
reg         MemoryWrite_o;
reg         MemtoReg_o;
reg   [1:0] Branchtype;
//Parameter
wire r, addi, slti, beq,lw,sw,bne,bge,bgt;

//Main function

assign r = (instr_op_i == 0);
assign addi = (instr_op_i == 8);
assign slti = (instr_op_i == 10);
assign beq = (instr_op_i == 4);
assign lw =  (instr_op_i == 35);
assign sw =  (instr_op_i == 43);
assign bne = (instr_op_i == 5);
assign bge = (instr_op_i == 1);
assign bgt = (instr_op_i == 7);

always@(*)begin
	if(r | addi | slti | lw)
	    RegWrite_o = 1;
    else
        RegWrite_o = 0;
end

always@(*)begin
	if(r)
	   ALU_op_o = 1;
	else if(addi)
	   ALU_op_o = 2;
	else if(slti|bge)
	   ALU_op_o = 3;
	else if(beq|bne)
	   ALU_op_o = 4;
	else if(lw)
	   ALU_op_o = 5;
	else if(sw)
	   ALU_op_o = 6;
	else if(bgt)   
	   ALU_op_o = 7;
    else
       ALU_op_o = 0;
end

always@(*)begin
	if(addi | slti | lw | sw)
	    ALUSrc_o = 1;
    else
        ALUSrc_o = 0;
end


always@(*)begin
    if(r)
	    RegDst_o = 1;
    else
        RegDst_o = 0;
end

always@(*)begin
	if(beq|bne|bge|bgt)
	    Branch_o = 1;
    else
        Branch_o = 0;
end

always@(*)begin
	if(bne)
	   Branchtype = 1;
	else if(bge)
	   Branchtype = 2;
	else if(bgt)
	   Branchtype = 3;
    else
       Branchtype = 0;
end

always@(*)begin
	if(lw)
	    MemRead_o = 1;
    else
        MemRead_o = 0;

end

always@(*)begin
	if(sw)
	    MemoryWrite_o = 1;
    else
        MemoryWrite_o = 0;
end

always@(*)begin
    if(lw)
	    MemtoReg_o = 0;
    else
        MemtoReg_o = 1;
end
endmodule






                    
                    