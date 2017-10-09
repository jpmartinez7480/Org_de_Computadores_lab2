#include "Buffer.h"

using std::list;

Buffer::Buffer(){}

void Buffer::setBuffer_if_id(Instruccion i)
{
	setRegistroRd(i.getRegistro1());
	setRegistroRs(i.getRegistro2());
	setRegistroRt(i.getRegistro3());
	setInstruction(i.getOperando());
	setStatus(true);
}

void Buffer::setBuffer_id_ex(int rs, int rt, string rd, string i,string _rs, string _rt) // buffer_id_ex para R 
{
	setValueRs(rs); //valor
	setValueRt(rt); //valor
	setRegistroRd(rd); //reg de rd
	setInstruction(i);
	setRegistroRs(_rs);
	setRegistroRt(_rt);
	setStatus(true);
}

void Buffer::setBuffer_id_ex(int rs,string rt, string S_ext,string _rs,string i) //buffer_id_ex para (addi,subi)
{
	setValueRs(rs); //valor
	setRegistroRt(rt);
	setValueRt(atoi(S_ext.c_str()));
	setInstruction(i);
	setRegistroRs(_rs);
	setStatus(true);
}

void Buffer::setBuffer_id_ex(string rs, int offset, string i) //buffer_id_ex para lw,sw
{
	setRegistroRs(rs); 
	setValueRt(offset);
	setInstruction(i);
	setStatus(true);
}

void Buffer::setBuffer_id_ex(int rs, int rt, string label) //buffer_id_ex para beq
{
	setValueRs(rs);
	setValueRt(rt);
	setInstruction(label);
	setStatus(true);
}

void Buffer::setBuffer_ex_mem(int alr, string dest, string op) //buffer_ex_mem para R,addi,subi,lw,sw
{
	if(op!= "lw" && op!="sw"){
		setALU_result(alr);
		setRegistroRd(dest);	
	}
	else{
		setALU_result(alr);
		setRegistroRt(dest);
	}
	setStatus(true);
}

void Buffer::setBuffer_mem_wb(string rd, string op, int alr) //tipo R,addi,subi
{
	if(op != "lw"){
		setALU_result(alr);
		setRegistroRd(rd);
	}
	else{
		setALU_result(alr);
		setRegistroRt(rd);
	}
	setStatus(true);
}

void Buffer::setValueRs(int v){
	valueRs = v;
}

void Buffer::setValueRt(int v){
	valueRt = v;
}

void Buffer::setALU_result(int v){
	 valueALU_result= v;
}

void Buffer::setInstruction(string inst){
	instruction = inst;
}

void Buffer::setRegistroRd(string reg){
	rd = reg;
}

void Buffer::setRegistroRs(string reg){
	rs  = reg;
}

void Buffer::setRegistroRt(string reg){
	rt = reg;
}

void Buffer::setStatus(bool s){
	status = s;
}

int Buffer::getALU_result(){
	return valueALU_result;
}

int Buffer::getValueRs(){
	return valueRs;
}

int Buffer::getValueRt(){
	return valueRt;
}

string Buffer::getInstruction(){
	return instruction;
}

string Buffer::getRegistroRd(){
	return rd;
}

string Buffer::getRegistroRs(){
	return rs;
}

string Buffer::getRegistroRt(){
	return rt;
}

bool Buffer::getStatus(){
	return status;
}

Control Buffer::getBufferControl(){
	return bufferControl;
}
