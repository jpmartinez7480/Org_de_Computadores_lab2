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
	lineControl = Control();
	cantoReloj = 1;
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
	while(regs[cnt] != r)	cnt++;
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
	*PC = _i;
	ejecutar(_i);
}

int Procesador::ALU(int v1, int v2, string op)
{
	int res;
	if(op == "add" || op == "lw" || op == "sw")	res = add(v1,v2);
	else if(op == "sub" || op == "beq")	res = sub(v1,v2);
	else if(op == "mul") res = mul(v1,v2);
	else if(op == "div") res = div(v1,v2);
	else if(op == "addi") res = addi(v1,v2);
	else if(op == "subi") res = subi(v1,v2);
	return res;
}


void Procesador::etapaIF(){
	buffer_if_id.Buffer(inst);
}

void Procesador::etapaID()
{
	int rs = buscarRegistro(buffer_if_id.getRegistroRs());
	int rt = buscarRegistro(buffer_if_id.getRegistroRt());
	buffer_id_ex.Buffer(registros[rs],registros[rt],registros[rd],buffer_if_id.getInstruction());
}

void Procesador::etapaEX()
{
	int res = ALU(buffer_id_ex.getValueRs(),buffer_id_ex.getValueRt());
	if(buffer_id_ex.getInstruction().getTipo() == 'R')
		buffer_ex_mem.Buffer(res,buffer_id_ex.getRegistroRd())
	else if(buffer_id_ex.getInstruction().getOperando() == "beq" && res == 0)	
		lineControl.setBranch(1);
	else if(buffer_id_ex.getInstruction().getOperando() == "lw"){
		buffer_ex_mem(res,buffer_id_ex.getValueRt())
		lineControl.setMemRead(1);
	}
	else if(buffer_id_ex.getInstruction().getOperando() == "sw"){
		buffer_ex_mem(res,buffer_id_ex.getValueRt())
		lineControl.setMemWrite(1);	
	}

}

void Procesador::etapaMEM()
{
	if(lineControl.getMemWrite() == 1){
		sw(buffer_ex_mem.getRegistroRt(),buffer_ex_mem.getDir())
		lineControl.setMemToReg(0);
	}
	else if(lineControl.getMemRead() == 1){
		int value = stackPointer[999-buffer_ex_mem.getDir()/4].getRegistro();
		buffer_mem_wb.setRegistroRt(value);
		lineControl.setMemToReg(1);
	}
	else if(lineControl.getMemWrite() == 0 && lineControl.getMemRead() == 0){
		buffer_mem_wb(buffer_ex_mem.getValueRd(),buffer_ex_mem.getRegistroRd());
	}
}

void Procesador::etapaWB(){
	if(lineControl.getMemToReg() == 1){
		registros[buscarRegistro(buffer_mem_wb.getRegistroRd())].setRegistro();
		lineControl.setMemToReg(0);	
	}
}

void Procesador::ejecutar()
{
	PC = instrucciones.begin();
	while(PC != instrucciones.end()){
		inst = *PC;
		//ejecutarInstruccion(inst); se ejecuta una instruccion en un ciclo
		//PC++; //se pasa a la siguiente instruccion
		datapath(inst);
	}
}

void Procesador::datapath()
{
	etapaIF();
	etapaID();
	etapaEX();
	etapaMEM();
	etapaWB();
}

