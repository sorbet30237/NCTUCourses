`timescale 1ns / 1ps
//student's id: 109550112



module forward_unit(
    ex_mem_rd,
    mem_wb_rd,
    id_ex_rs,
    id_ex_rt,
    ex_mem_regwrite,
    mem_wb_regwrite,
    
    forwardA,
    forwardB
    );
    
input   [4:0]   ex_mem_rd;
input   [4:0]   mem_wb_rd;
input   [4:0]   id_ex_rs;
input   [4:0]   id_ex_rt;
input           ex_mem_regwrite;
input           mem_wb_regwrite;

output reg [1:0]   forwardA;
output reg [1:0]   forwardB;
    
always@(*)begin
    if(ex_mem_regwrite && (ex_mem_rd != 0) && (ex_mem_rd == id_ex_rs))
        forwardA = 2'b01;
    else if(mem_wb_regwrite && (mem_wb_rd != 0) && (mem_wb_rd == id_ex_rs))
        forwardA = 2'b10;
    else
        forwardA = 2'b00;
end
    
always@(*)begin
    if(ex_mem_regwrite && (ex_mem_rd != 0) && (ex_mem_rd == id_ex_rt))
        forwardB = 2'b01;
    else if(mem_wb_regwrite && (mem_wb_rd != 0) && (mem_wb_rd == id_ex_rt))
        forwardB = 2'b10;
    else
        forwardB = 2'b00;
end
    
    
endmodule