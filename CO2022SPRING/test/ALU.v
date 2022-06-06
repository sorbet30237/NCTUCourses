//Subject:     CO project 2 - ALU
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      109550112
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------

module ALU(
    src1_i,
	src2_i,
	ctrl_i,
	result_o,
	zero_o
	);
     
//I/O ports
input  [32-1:0]  src1_i;
input  [32-1:0]	 src2_i;
input  [4-1:0]   ctrl_i;

output [32-1:0]	 result_o;
output           zero_o;

//Internal signals
reg    [32-1:0]  result_o;
wire             zero_o;

//Parameter

//Main function


assign zero_o = (result_o == 0);

always@(*)
    begin
    result_o = 0;
    case(ctrl_i)
        0: result_o = src1_i & src2_i;
        1: result_o = src1_i | src2_i;
        2: result_o = src1_i + src2_i;
        6: result_o = src1_i - src2_i;
        7: if(src1_i<src2_i) 
             begin
                result_o = 1;
             end 
           else
             begin
                result_o = 0;
             end            
        12: result_o = ~(src1_i | src2_i);
    endcase    
    end

endmodule





                    
                    