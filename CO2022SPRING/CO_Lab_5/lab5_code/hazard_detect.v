`timescale 1ns / 1ps
//student's id: 109550112



module hazard_detect(
    id_ex_memread,
    id_ex_rt,
    if_id_rs,
    if_id_rt,
    pc_src,
    branchselect,
    typebranch,
    pc_write,
    if_id_write,
    if_flush,
    id_flush,
    ex_flush
    );

input           id_ex_memread;
input    [4:0]  id_ex_rt;
input    [4:0]  if_id_rs;
input    [4:0]  if_id_rt;
input           pc_src;
input           branchselect;
input    [1:0]  typebranch;  
output reg      pc_write;
output reg      if_id_write;
output reg      if_flush;
output reg      id_flush;
output reg      ex_flush;

always@(*)begin
    if(id_ex_memread && ((id_ex_rt == if_id_rs) || (id_ex_rt == if_id_rt)))///load_use_data_hazard
        pc_write = 0;
    else
        pc_write = 1;
end

always@(*)begin
    if(id_ex_memread && ((id_ex_rt == if_id_rs) || (id_ex_rt == if_id_rt)))///load_use_data_hazard
        if_id_write = 0;
    else
        if_id_write = 1;
end

always@(*)begin
    if((pc_src == 1 && typebranch == 0) || (pc_src == 0 && branchselect == 1 && typebranch == 1) || (pc_src == 1 && typebranch == 2) || (pc_src == 1 && typebranch == 3))///branch take(control hazard)
        if_flush = 1;
    else
        if_flush = 0;
end

always@(*)begin
    if(id_ex_memread && ((id_ex_rt == if_id_rs) || (id_ex_rt == if_id_rt)))///load_use_data_hazard
        id_flush = 1;
    else if((pc_src == 1 && typebranch == 0) || (pc_src == 0 && branchselect == 1 && typebranch == 1) || (pc_src == 1 && typebranch == 2) || (pc_src == 1 && typebranch == 3))///branch take(control hazard)
        id_flush = 1;
    else
        id_flush = 0;
end

always@(*)begin
    if((pc_src == 1 && typebranch == 0) || (pc_src == 0 && branchselect == 1 && typebranch == 1) || (pc_src == 1 && typebranch == 2) || (pc_src == 1 && typebranch == 3))///branch take(control hazard)
        ex_flush = 1;
    else
        ex_flush = 0;
end

endmodule