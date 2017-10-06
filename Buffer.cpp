#include "Buffer.h"

#include <list>
#include <iterator>

using std::list;

Buffer::Buffer(Instruccion i)
{
	setRegistroRd(i.getRegistro1());
	setRegistroRs(i.getRegistro2());
	setRegistroRt(i.getRegistro3());
	setInstruction(i.getOperando());
	setPC(PC);

}

Buffer::Buffer(){}

Buffer::setValueRs(int v){
	valueRs = v;
}

Buffer::setvalueRt(int v){
	valueRt = v;
}

void Buffer::setActivity(int v){
	activity = v;
}

void Buffer::setPC(list<Instruccion>::iterator pc){
	PC = pc;
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

int Buffer::getValue(){
	return value;
}

int Buffer::getActivity(){
	return activity;
}

Instruccion Buffer::getInstruction(){
	return instruction;
}

Instruccion Buffer::getPC(){
	return PC;
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


