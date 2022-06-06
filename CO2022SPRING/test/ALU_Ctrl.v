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
`timescale 1ns/1ps
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
assign slt = 6'b100110;
assign slti = 6'b000000;
always@(*) 
    begin
      if(ALUOp_i[2] == 1)
        begin
            ALUCtrl_o[0] = (funct_i == slt) || (funct_i == Or);
            ALUCtrl_o[1] = (funct_i == add) || (funct_i == sub) || (funct_i == slt);
            ALUCtrl_o[2] = (funct_i == slt) || (funct_i == sub);
        end
        else if(ALUOp_i[1:0] == 2'b11)
        begin
            ALUCtrl_o[2:0] = 3'b110;
        end
        
        else if(ALUOp_i[1] == 1)
        begin
            ALUCtrl_o[2:0] = 3'b010;
        end
        
        else if(ALUOp_i[0] == 1)
        begin
            ALUCtrl_o[2:0] = 3'b111;
        end
      
        
      ALUCtrl_o[3] = 0;
    end   
//Select exact operation

endmodule     





                    
                    