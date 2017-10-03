#include "Control.h"

Control::Control()
{
	setRegDst('0');
	setJump('0');
	setBranch('0');
	setMemRead('0');
	setMemToReg('0');
	setALUOp("00");
	setMemWrite('0');
	setALUSrc('0');
	setRegWrite('0');
}

void Control::setRegDst(char rd){
	regDst = rd;
}

void Control::setJump(char j){
	jump = j;
}

void Control::setBranch(char b){
	branch = b;
}

void Control::setMemRead(char mr){
	memRead = mr;
}

void Control::setMemToReg(char mtr){
	memToReg = mtr;
}

void Control::setALUOp(string aop){
	ALUOp = aop;
}

void Control::setMemWrite(char mw){
	memWrite = mw;
}

void Control::setALUSrc(char as){
	ALUSrc = as;
}

void Control::setRegWrite(char rw){
	regWrite = rw;
}

char Control::getRegDst(){
	return regDst;
}

char Control::getJump(){
	return jump;
}

char Control::getBranch(){
	return branch;
}

char Control::getMemRead(){
	return memRead;
}

char Control::getMemToReg(){
	return memToReg;
}

string Control::getALUOp(){
	return ALUOp;
}

char Control::getMemWrite(){
	return memWrite;
}

char Control::getALUSrc(){
	return ALUSrc;
}

char Control::getRegWrite(){
	return regWrite;
}

void Control::establecerControl(char rd, char j, char b, char mr, char mtr, string aop, char mw, char as, char rw){
	setRegDst(rd);
	setJump(j);
	setBranch(b);
	setMemRead(mr);
	setMemToReg(mtr);
	setALUOp(aop);
	setMemWrite(mw);
	setALUSrc(as);
	setRegWrite(rw);
}

void Control::controlSign(char type, string operation)
{
	switch(type)
	{
		case 'R':
			setRegDst('1');
			setRegWrite('1');
			setALUOp("10");
			break;
		case 'I':
			if(operation == "addi"){
				setALUSrc('1');
				setRegWrite('1');
			}
			else if(operation == "subi"){
				setALUOp("01");
				setALUSrc('1');
				setRegWrite('1');
			}
			else if(operation == "lw"){
				setMemRead('1');
				setMemToReg('1');
				setALUSrc('1');
				setRegWrite('1');
			}
			else if(operation == "sw"){
				setRegDst('x');
				setALUSrc('1');
				setMemToReg('x');
				setMemWrite('1');
			}
			else if(operation == "beq"){
				setRegDst('x');
				setBranch('1');
				setMemToReg('x');
				setALUOp("01");
			}
		break;

		case 'J':
			setJump('1');
			setALUSrc('1');
		break;

	}
}