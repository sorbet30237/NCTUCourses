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




always@(*)begin
	if(ALUOp_i == 1) begin///r-format
	   case(funct_i)
	   6'b100000:  ALUCtrl_o[2:0] = 2;///add
	   6'b100010:  ALUCtrl_o[2:0] = 6;///sub
	   6'b100100:  ALUCtrl_o[2:0] = 0;///and
	   6'b100101:  ALUCtrl_o[2:0] = 1;///or
	   6'b101010:  ALUCtrl_o[2:0] = 7;///slt
	   6'b011000:  ALUCtrl_o[2:0] = 3;///mult
	   endcase

	end 
	else if(ALUOp_i == 4)begin///branch
	   ALUCtrl_o[2:0] = 3'b110;
	end 
	else if(ALUOp_i == 2)begin///addi
	   ALUCtrl_o[2:0] = 3'b010;
	end 
	else if(ALUOp_i == 3)begin///slti bge
	   ALUCtrl_o[2:0] = 3'b111;
	end
	else if(ALUOp_i == 5)begin///lw
	   ALUCtrl_o[2:0] = 3'b010;
	end
	else if(ALUOp_i == 6)begin///sw
	   ALUCtrl_o[2:0] = 3'b010;
	end   
	else if(ALUOp_i == 7)begin///sw
	   ALUCtrl_o[2:0] = 3'b100;
	end
	ALUCtrl_o[3] = 0;
end       
//Select exact operation

endmodule     





                    
                    