#include "Instruccion.h"


Instruccion::Instruccion()
{
	registro1 = "";
	registro2 = "";
	registro3 = "";
}


void Instruccion::setOperando(string op){
	operando = op;
}

void Instruccion::setRegistro1(string r){
	registro1 = r;
}

void Instruccion::setRegistro2(string r){
	registro2 = r;
}

void Instruccion::setRegistro3(string r){
	registro3 = r;
}

char Instruccion::getTipo(){
	return tipo;
}

string Instruccion::getOperando(){
	return operando;
}

string Instruccion::getRegistro1(){
	return registro1;
}

string Instruccion::getRegistro2(){
	return registro2;
}

string Instruccion::getRegistro3(){
	return registro3;
}

char Instruccion::Operacion(string op){
	if(op == "addi" || op == "subi" || op == "beq" || op == "lw" || op == "sw") return 'I';
	else if(op == "j") return 'J';
	else if (op == "add" || op == "sub" || op == "mul" || op =="div") return 'R';
	else return 'l';
}

void Instruccion::establecerInstruccion(string op, string r1,string r2, string r3)
{
	setTipo(Operacion(op));
	setOperando(op);
	setRegistro1(r1);
	setRegistro2(r2);
	setRegistro3(r3);
}

void Instruccion::establecerInstruccion(string r1)
{
	setTipo('J');
	setOperando("j");
	setRegistro3(r1+":");
}

void Instruccion::setTipo(char t){
	tipo = t;
}

void Instruccion::setInstruction(string linea, char t){
	setOperando(linea);
	setTipo(t);
}