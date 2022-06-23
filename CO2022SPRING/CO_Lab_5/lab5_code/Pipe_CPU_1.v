//student id:109550112
`timescale 1ns / 1ps
//Subject:     CO project 4 - Pipe CPU 1
//--------------------------------------------------------------------------------
//Version:     1
//--------------------------------------------------------------------------------
//Writer:      109550112
//----------------------------------------------
//Date:        
//----------------------------------------------
//Description: 
//--------------------------------------------------------------------------------
module Pipe_CPU_1(
    clk_i,
    rst_i
    );
    
/****************************************
I/O ports
****************************************/
input clk_i;
input rst_i;

/****************************************
Internal signal
****************************************/
localparam ID_EX_CTRL = 10, EX_MEM_CTRL = 5, MEM_WB_CTRL = 2;


/**** IF stage ****/
wire [31:0] IF_pc_i, IF_pc_o, IF_ins_mem_o, IF_pc_source_1, IF_adder1_o;
wire        pc_src;

/**** ID stage ****/
wire [31:0] ID_adder1_o, ID_ins_mem_o, ID_ReadData1, ID_ReadData2, ID_SignExt_o;

//control signal
wire        ID_alusrc, ID_regdst, ID_branch, ID_memwrite, ID_memread, ID_memtoreg, ID_regwrite;
wire        ID_alusrc2, ID_regdst2, ID_branch2, ID_memwrite2, ID_memread2, ID_memtoreg2, ID_regwrite2;
wire [2:0]  ID_aluop;
wire [2:0]  ID_aluop2;

/**** EX stage ****/
wire [31:0] EX_adder1_o, EX_ReadData1, EX_ReadData2, EX_SignExt_o, EX_MUX_to_ALU, EX_ALU_result, EX_SL2_o, EX_adder2_o;
wire [31:0] Mux_forwardA_o, Mux_forwardB_o;
wire [4:0]  EX_regrs, EX_regrt, EX_regrd, EX_Write_Reg;
wire [3:0]  EX_ALU_ctrl;
wire        EX_zero;

//control signal
wire [2:0]  EX_aluop;
wire [2:0]  EX_aluop2;
wire [1:0]  forwardA, forwardB;
wire        EX_alusrc , EX_regdst , EX_branch , EX_memwrite , EX_memread , EX_memtoreg , EX_regwrite;
wire        EX_alusrc2, EX_regdst2, EX_branch2, EX_memwrite2, EX_memread2, EX_memtoreg2, EX_regwrite2;

/**** MEM stage ****/
wire [31:0] MEM_ALU_result, MEM_ReadDate2, MEM_Data_o;
wire [4:0]  MEM_Write_Reg;
wire        MEM_zero, MEM_pcsrc;

//control signal
wire        MEM_branch, MEM_memwrite, MEM_memread, MEM_memtoreg, MEM_regwrite;

/**** WB stage ****/
wire [31:0] WB_Data_o, WB_ALU_result, WB_write_data;
wire [4:0]  WB_Write_Reg;

//control signal
wire        WB_memtoreg, WB_regwrite;


wire        pc_write, IF_ID_write, IF_flush, ID_flush, EX_flush, IF_ID_rst;


wire        branchselect;
wire [1:0]  bratype;
/****************************************
Instantiate modules
****************************************/
assign pc_src = MEM_branch & MEM_zero;
assign branchselect = MEM_branch;
assign IF_ID_rst = (rst_i == 0 || IF_flush == 1) ? 0 : 1;
//Instantiate the components in IF stage
MUX_2to1 #(.size(32)) Mux_ALUSrc(///vv
        .data0_i(IF_adder1_o),
        .data1_i(IF_pc_source_1),
        .select_i(pc_src),
        .data_o(IF_pc_i)
        );

ProgramCounter PC(
        .clk_i(clk_i),      
	    .rst_i (rst_i),
        .write_i(pc_write),     
	    .pc_in_i(IF_pc_i) ,   
	    .pc_out_o(IF_pc_o) 
);

Instruction_Memory IM(
        .addr_i(IF_pc_o),  
	    .instr_o(IF_ins_mem_o)   
);
			
Adder Add_pc(
        .src1_i(IF_pc_o),     
	    .src2_i(4),     
	    .sum_o(IF_adder1_o)     
);

		
Pipe_Reg #(.size(32*2)) IF_ID(       //N is the total length of input/output
    .clk_i(clk_i),
    .rst_i(IF_ID_rst),
    .write_i(IF_ID_write),
    .data_i({IF_adder1_o, IF_ins_mem_o}),
    .data_o({ID_adder1_o, ID_ins_mem_o})
);


//Instantiate the components in ID stage
Reg_File RF(
        .clk_i(clk_i),      
	    .rst_i(rst_i) ,     
        .RSaddr_i(ID_ins_mem_o[25:21]) ,  
        .RTaddr_i(ID_ins_mem_o[20:16]) ,  
        .RDaddr_i(WB_Write_Reg) ,  
        .RDdata_i(WB_write_data)  , 
        .RegWrite_i (WB_regwrite),
        .RSdata_o(ID_ReadData1) ,  
        .RTdata_o(ID_ReadData2)   
);

MUX_2to1 #(.size(ID_EX_CTRL)) Mux_comtrol(///vv
        .data0_i({ID_alusrc, ID_regdst, ID_branch, ID_memwrite, ID_memread, ID_memtoreg, ID_regwrite, ID_aluop}),
        .data1_i(10'b0),
        .select_i(ID_flush),
        .data_o({ID_alusrc2, ID_regdst2, ID_branch2, ID_memwrite2, ID_memread2, ID_memtoreg2, ID_regwrite2, ID_aluop2})
        );

hazard_detect HD(
    .id_ex_memread(EX_memread),
    .id_ex_rt(EX_regrt),
    .if_id_rs(ID_ins_mem_o[25:21]),
    .if_id_rt(ID_ins_mem_o[20:16]),
    .pc_src(pc_src),
    .branchselect(branchselect),
    .typebranch(bratype),
    .pc_write(pc_write),
    .if_id_write(IF_ID_write),
    .if_flush(IF_flush),
    .id_flush(ID_flush),
    .ex_flush(EX_flush)
    );

Decoder Control(
        .instr_op_i(ID_ins_mem_o[31:26]), 
	    .RegWrite_o(ID_regwrite), 
	    .ALU_op_o(ID_aluop),   
	    .ALUSrc_o(ID_alusrc),   
	    .RegDst_o(ID_regdst),   
		.Branch_o(ID_branch),
        .MemRead_o(ID_memread),
        .MemoryWrite_o(ID_memwrite),
        .MemtoReg_o(ID_memtoreg),
        .Branchtype(bratype)
);

Sign_Extend Sign_Extend(
        .data_i(ID_ins_mem_o[15:0]),
        .data_o(ID_SignExt_o)
);	

Pipe_Reg #(.size(ID_EX_CTRL+32*4+5*3)) ID_EX(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .write_i(1'b1),
    .data_i({ID_alusrc2, ID_regdst2, ID_branch2, ID_memwrite2, ID_memread2, ID_memtoreg2, ID_regwrite2, ID_aluop2, ID_adder1_o, ID_ReadData1, ID_ReadData2, ID_SignExt_o, ID_ins_mem_o[25:21], ID_ins_mem_o[20:16], ID_ins_mem_o[15:11]}),
    .data_o({EX_alusrc , EX_regdst , EX_branch2, EX_memwrite2, EX_memread2, EX_memtoreg2, EX_regwrite2, EX_aluop , EX_adder1_o, EX_ReadData1, EX_ReadData2, EX_SignExt_o, EX_regrs           , EX_regrt           , EX_regrd           })
);


//Instantiate the components in EX stage	   
Shift_Left_Two_32 Shifter(
        .data_i(EX_SignExt_o),
        .data_o(EX_SL2_o)
);

ALU ALU(
        .src1_i(Mux_forwardA_o),
	    .src2_i(EX_MUX_to_ALU),
	    .ctrl_i(EX_ALU_ctrl),
	    .result_o(EX_ALU_result),
		.zero_o(EX_zero)
);

MUX_2to1 #(.size(EX_MEM_CTRL)) Mux_EXcomtrol(///vv
        .data0_i({EX_branch2,  EX_memwrite2,  EX_memread2,  EX_memtoreg2,  EX_regwrite2}),
        .data1_i(5'b0),
        .select_i(EX_flush),
        .data_o({EX_branch,  EX_memwrite,  EX_memread,  EX_memtoreg,  EX_regwrite})
        );
		
ALU_Ctrl ALU_Control(
        .funct_i(EX_SignExt_o[5:0]),   
        .ALUOp_i(EX_aluop),   
        .ALUCtrl_o(EX_ALU_ctrl)
);

MUX_3to1 #(.size(32)) Mux_forwardA(
        .data0_i(EX_ReadData1),
        .data1_i(MEM_ALU_result),
        .data2_i(WB_write_data),
        .select_i(forwardA),
        .data_o(Mux_forwardA_o)
);

MUX_3to1 #(.size(32)) Mux_forwardB(
        .data0_i(EX_ReadData2),
        .data1_i(MEM_ALU_result),
        .data2_i(WB_write_data),
        .select_i(forwardB),
        .data_o(Mux_forwardB_o)
);

forward_unit    fwd(
    .ex_mem_rd(MEM_Write_Reg),
    .mem_wb_rd(WB_Write_Reg),
    .id_ex_rs(EX_regrs),
    .id_ex_rt(EX_regrt),
    .ex_mem_regwrite(MEM_regwrite),
    .mem_wb_regwrite(WB_regwrite),
    
    .forwardA(forwardA),
    .forwardB(forwardB)
);

MUX_2to1 #(.size(32)) Mux1(
        .data0_i(Mux_forwardB_o),
        .data1_i(EX_SignExt_o),
        .select_i(EX_alusrc),
        .data_o(EX_MUX_to_ALU)
);
		
MUX_2to1 #(.size(5)) Mux_Write_Reg(
        .data0_i(EX_regrt),
        .data1_i(EX_regrd),
        .select_i(EX_regdst),
        .data_o(EX_Write_Reg)
);

Adder Add_pc_branch(
        .src1_i(EX_adder1_o),     
	    .src2_i(EX_SL2_o),     
	    .sum_o(EX_adder2_o)     
);

Pipe_Reg #(.size(EX_MEM_CTRL+1+32*3+5)) EX_MEM(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .write_i(1'b1),
    .data_i({EX_branch,  EX_memwrite,  EX_memread,  EX_memtoreg,  EX_regwrite,  EX_adder2_o,    EX_zero,  EX_ALU_result,  EX_ReadData2,  EX_Write_Reg }),
    .data_o({MEM_branch, MEM_memwrite, MEM_memread, MEM_memtoreg, MEM_regwrite, IF_pc_source_1, MEM_zero, MEM_ALU_result, MEM_ReadDate2, MEM_Write_Reg})
);


//Instantiate the components in MEM stage
Data_Memory DM(
	.clk_i(clk_i),
	.addr_i(MEM_ALU_result),
	.data_i(MEM_ReadDate2),
	.MemRead_i(MEM_memread),
	.MemWrite_i(MEM_memwrite),
	.data_o(MEM_Data_o)
);

Pipe_Reg #(.size(MEM_WB_CTRL+32*2+5)) MEM_WB(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .write_i(1'b1),
    .data_i({MEM_memtoreg, MEM_regwrite, MEM_Data_o, MEM_ALU_result, MEM_Write_Reg}),
    .data_o({ WB_memtoreg,  WB_regwrite,  WB_Data_o,  WB_ALU_result,  WB_Write_Reg})
);


//Instantiate the components in WB stage
MUX_2to1 #(.size(32)) Mux3(
        .data0_i(WB_Data_o),
        .data1_i(WB_ALU_result),
        .select_i(WB_memtoreg),
        .data_o(WB_write_data)
);

/****************************************
signal assignment
****************************************/

endmodule