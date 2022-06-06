//student id:109550112
//Subject:     CO project 2 - ALU Controller
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      109550112
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------

module ALU_Ctrl(
          funct_i,
          ALUOp_i,
          ALUCtrl_o
          );
          
//I/O ports 
input      [6-1:0] funct_i;
input      [3-1:0] ALUOp_i;

output     [4-1:0] ALUCtrl_o;        
//Internal Signals
reg        [4-1:0] ALUCtrl_o;

//Parameter

wire [5:0] add, sub, And, Or, slt, slti;
assign add = 6'b100000;
assign sub = 6'b100010;
assign And = 6'b100100;
assign Or = 6'b100101;
assign slt = 6'b101010;




always@(*)
begin
	if(ALUOp_i == 1) //r-format
    begin
	   ALUCtrl_o[0] = (funct_i == Or) || (funct_i == slt);
	   ALUCtrl_o[1] = (funct_i == add) || (funct_i == sub) || (funct_i == slt);
	   ALUCtrl_o[2] = (funct_i == sub) || (funct_i == slt);
	end 

	else if(ALUOp_i == 2)//addi
        begin
	   ALUCtrl_o[2:0] = 3'b010;
	end 

	else if(ALUOp_i == 3)//slti
	begin
	   ALUCtrl_o[2:0] = 3'b111;
	end

	else if(ALUOp_i == 4)///beq
	begin
	   ALUCtrl_o[2:0] = 3'b110;
	end
 
	else if(ALUOp_i == 5)//lw
	begin
	   ALUCtrl_o[2:0] = 3'b010;
	end

	else if(ALUOp_i == 6)//sw
	begin
	   ALUCtrl_o[2:0] = 3'b010;
	end
	ALUCtrl_o[3] = 0;
end       
//Select exact operation

endmodule     





                    
                    