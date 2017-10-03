#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "Procesador.h"

using namespace std;

const string Procesador::regs[32]  = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"};

const string Procesador::operaciones[11] = {"add","sub","mul","div","addi","subi","lw","sw","beq","j"};

Procesador::Procesador()
{
	for(int i = 0; i < 32; i++)
		registros[i] =Registro(); 
	for(int i = 0; i < 1000; i++)
		stackPointer[i] = Registro();
	for(int i = 0; i < 1000; i++)
		lines_control_sign[i] = Control();
}

Procesador::~Procesador(){}


int Procesador::readFile_Mips(string nfile)
{
	fstream FILE;
	char *linea;
	FILE.open(nfile.c_str(),ios::in);
	if(!FILE){
		cout << "No se pudo abrir el archivo. Comprueba el nombre del archivo o la existencia de este." << endl;
		return 0;
	}
	else
	{
		string line;
		int j = 0;
		while(!FILE.eof())
		{
			string inst[5];
			getline(FILE,line);
			if(line!=""){
				char *cstr = new char[line.length() + 1];
				strcpy(cstr,line.c_str());
				linea = strtok(cstr," ");
				if(line[line.length() - 1] == ':'){
					Instruccion i;
					i.setInstruction(linea,'L');
					instrucciones.push_back(i);
				}
				else if(line[line.length()-1]!=':'){
					while(linea){
						inst[j] = linea;
						j++;
						linea = strtok(NULL,", ");
					}	
					j = 0;
					Instruccion i;
					if(inst[0] != "j"){
						i.establecerInstruccion(inst[0],inst[1],inst[2],inst[3]);
						instrucciones.push_back(i);
					}
					else {
						i.establecerInstruccion(inst[1]);
						instrucciones.push_back(i);
					}
				}
			}
		}
		FILE.close();
		return 1;
	}
}

int Procesador::readFile_Registers(string nfile)
{
	fstream FILE;
	string reg;
	int value;
	FILE.open(nfile.c_str(),ios::in);
	if(!FILE){
		cout << "No se pudo abrir el archivo. Compruebe el nombre o la existencia de este archivo." << endl;
		return 0;
	}
	else
	{
		string linea;
		getline(FILE,linea);
		while(FILE >> reg >> value){
			registros[buscarRegistro(reg)].setRegistro(value);
		}
		FILE.close();
		return 1;
	}
}


void Procesador::showContent_Registros()
{
	cout << endl;
	cout << "********** Registros **********"<<endl;
	cout << "-------" << "-----" << endl; 
	cout << "|" <<regs[0] << setw(2) << "| " <<  registros[0].getRegistro() << "  |" <<endl;
	cout << "-------" << "-----" << endl;
	for(int i = 1; i < 32; i++){
		cout << "|" <<regs[i] << setw(4) << "| " <<registros[i].getRegistro() << "  |"<< endl;
		cout << "-------" << "-----" << endl;
	}
}

int Procesador::buscarRegistro(string r)
{
	int cnt = 0;
	while(regs[cnt] != r)
		cnt++;
	return cnt;
}

int Procesador::add(int r1,int r2){
	return r1+r2;
}

int Procesador::sub(int r1,int r2){
	return r1-r2;
}

int Procesador::mul(int r1, int r2){
	return r1*r2;
}

int Procesador::div(int r1, int r2){
	return r1/r2;
}

int Procesador::addi(int r1,int n){
	return r1 + n;
}

int Procesador::subi(int r1, int n){
	return r1 - n;
}

void Procesador::lw(string r, int pos){	
	registros[buscarRegistro(r)].setRegistro(stackPointer[999-pos/4].getRegistro());
}

void Procesador::sw(string r, int pos){
	stackPointer[999-pos/4].setRegistro(registros[buscarRegistro(r)].getRegistro());
}

bool Procesador::beq(int r1, int r2){
	return r1 == r2;
}

void Procesador::j(string label)
{
	int i = 0;
	Instruccion _i;
	list<Instruccion>::iterator aux;
	aux = instrucciones.begin();
	while(aux!=instrucciones.end())
	{
		_i = *aux;
		if(_i.getOperando() != label){
			i++;
			aux++;
		}		
		else {
			PC = aux++;	
			aux = instrucciones.end();
		}
	}
	LC = i;
	*PC = _i;
	ejecutarInstruccion(_i);
}

void Procesador::compilar()
{
	LC = 0;
	PC = instrucciones.begin();
	while(PC != instrucciones.end()){
		inst = *PC;
		ejecutarInstruccion(inst);
		PC++;
	}
}

void Procesador::ejecutarInstruccion(Instruccion _i)
{
	int r1;
	int r2;
	int r3;
	switch(_i.getTipo())
	{
		case 'R':
			r1 = buscarRegistro(_i.getRegistro1()); 
			r2 =  registros[buscarRegistro(_i.getRegistro2())].getRegistro();
			r3 = registros[buscarRegistro(_i.getRegistro3())].getRegistro();
			if(_i.getOperando() == "add")
				registros[r1].setRegistro(add(r2,r3));
			else if(_i.getOperando() == "sub")
				registros[r1].setRegistro(sub(r2,r3));
			else if(_i.getOperando() == "mul")
				registros[r1].setRegistro(mul(r2,r3));
			else if(_i.getOperando() == "div")
				registros[r1].setRegistro(div(r2,r3));
			lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
			controlSignal++;
			LC++;		
			break;

		case 'I':
			if(_i.getOperando() == "addi"){
				r1 = buscarRegistro(_i.getRegistro1());
				r2 = registros[buscarRegistro(_i.getRegistro2())].getRegistro();
				r3 = atoi(_i.getRegistro3().c_str()); //pasar a int
				registros[r1].setRegistro(addi(r2,r3));
				lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
				controlSignal++;
				LC++;
			}
			else if(_i.getOperando() == "subi"){
				r1 = buscarRegistro(_i.getRegistro1());
				r2 = registros[buscarRegistro(_i.getRegistro2())].getRegistro();
				r3 = atoi(_i.getRegistro3().c_str()); //pasar a int
				registros[r1].setRegistro(subi(r2,r3));
				lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
				controlSignal++;
				LC++;
			}
			else if(_i.getOperando() == "lw"){
				lw(_i.getRegistro1(),atoi(_i.getRegistro2().c_str()));
				lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
				controlSignal++;
				LC++;
			}
			else if(_i.getOperando() == "sw"){
				sw(_i.getRegistro1(),atoi(_i.getRegistro2().c_str()));
				lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
				controlSignal++;
				LC++;
			}
			else if(_i.getOperando() == "beq"){
				lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
				controlSignal++;
				LC++;
				if(beq(registros[buscarRegistro(_i.getRegistro1())].getRegistro(),registros[buscarRegistro(_i.getRegistro2())].getRegistro())){
					j(_i.getRegistro3()+":");
				}
			}	
			break;

			case 'J':
			j(_i.getRegistro3());
			lines_control_sign[controlSignal].controlSign(_i.getTipo(),_i.getOperando());
			controlSignal++;
			break;

	}
}

