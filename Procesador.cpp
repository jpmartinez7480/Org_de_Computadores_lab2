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
}

Procesador::~Procesador(){}


int Procesador::readFile_Mips(string nfile)
{
	fstream FILE;
	char *linea;
	FILE.open(nfile.c_str(),ios::in);
	if(!FILE)	return 0;
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
	if(!FILE)	return 0;
	else{
		string linea;
		getline(FILE,linea);
		while(FILE >> reg >> value)	registros[buscarRegistro(reg)].setRegistro(value);
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
	ejecutar();
}

int Procesador::ALU(int v1, int v2, string op)
{
	int res;
	if(op == "add")	res = add(v1,v2);
	else if(op == "sub" || op == "beq")	res = sub(v1,v2);
	else if(op == "mul") res = mul(v1,v2);
	else if(op == "div") res = div(v1,v2);
	else if(op == "addi") res = addi(v1,v2);
	else if(op == "subi") res = subi(v1,v2);
	return res;
}


void Procesador::etapaIF(){
	buffer_if_id.setBuffer_if_id(inst);
	buffer_id_ex.getBufferControl() = Control(); 
	cout << "IF: "<<buffer_if_id.getInstruction() << endl;
}

void Procesador::etapaID()
{
	if(buffer_if_id.getInstruction() != "sw" && buffer_if_id.getInstruction()!= "lw" && buffer_if_id.getInstruction() != "beq" && buffer_if_id.getInstruction()!= "j" && buffer_if_id.getInstruction()!= "addi" && buffer_if_id.getInstruction()!= "subi"){
		buffer_id_ex.setBuffer_id_ex(registros[buscarRegistro(buffer_if_id.getRegistroRs())].getRegistro(),registros[buscarRegistro(buffer_if_id.getRegistroRt())].getRegistro(),buffer_if_id.getRegistroRd(),buffer_if_id.getInstruction(),buffer_if_id.getRegistroRs(),buffer_if_id.getRegistroRt()); //int,int,string,string,string,string
		buffer_id_ex.getBufferControl().setRegDst(1);
	}
	else if(buffer_if_id.getInstruction() == "sw" || buffer_if_id.getInstruction()== "lw"){
		char rt = buffer_if_id.getRegistroRt()[0];
		int offset = rt;  
		buffer_id_ex.setBuffer_id_ex(buffer_if_id.getRegistroRs(),offset,buffer_if_id.getInstruction()); //string, int, string
		buffer_id_ex.getBufferControl().setALUSrc(1); //el segundo operando de la alu viende del signo extendido
	}
	else if(buffer_if_id.getInstruction() == "beq"){
		buffer_id_ex.setBuffer_id_ex(registros[buscarRegistro(buffer_if_id.getRegistroRs())].getRegistro(),registros[buscarRegistro(buffer_if_id.getRegistroRt())].getRegistro(),buffer_if_id.getRegistroRt()); //int, int, string
	}
	else if(buffer_if_id.getInstruction() == "addi" || buffer_if_id.getInstruction() == "subi"){
		buffer_id_ex.setBuffer_id_ex(registros[buscarRegistro(buffer_if_id.getRegistroRs())].getRegistro(), buffer_if_id.getRegistroRt(),buffer_if_id.getRegistroRd(),buffer_if_id.getRegistroRs(),buffer_if_id.getInstruction());		
		buffer_id_ex.getBufferControl().setALUSrc(1);	
	}
	buffer_ex_mem.getBufferControl() = Control();
	cout << "ID: " << buffer_id_ex.getInstruction() << endl;
}

void Procesador::etapaEX()
{
	Hazard_DataDetector();
	if(buffer_id_ex.getInstruction() != "sw"  && buffer_id_ex.getInstruction() != "beq" && buffer_id_ex.getInstruction() != "lw"){
		buffer_ex_mem.setBuffer_ex_mem(ALU(buffer_id_ex.getValueRs(),buffer_id_ex.getValueRt(),buffer_id_ex.getInstruction()),buffer_id_ex.getRegistroRd(),buffer_id_ex.getInstruction()); //string string int
		buffer_id_ex.getBufferControl().setRegWrite(1);
	}
	else if(buffer_id_ex.getInstruction() == "beq" && ALU(buffer_id_ex.getValueRs(),buffer_id_ex.getValueRt(),"sub") == 0){
		buffer_ex_mem.getBufferControl().setBranch(1);
	}	
	else if(buffer_id_ex.getInstruction() == "lw"){
		buffer_ex_mem.setBuffer_ex_mem(ALU(buffer_id_ex.getValueRs(),buffer_id_ex.getValueRt(),buffer_id_ex.getInstruction()),buffer_id_ex.getRegistroRd(),buffer_id_ex.getInstruction()); //string string int
		buffer_ex_mem.getBufferControl().setMemRead(1);
	}
	else if(buffer_id_ex.getInstruction() == "sw"){
		buffer_ex_mem.setBuffer_ex_mem(ALU(buffer_id_ex.getValueRs(),buffer_id_ex.getValueRt(),buffer_id_ex.getInstruction()),buffer_id_ex.getRegistroRd(),buffer_id_ex.getInstruction()); //string string int
		buffer_ex_mem.getBufferControl().setMemWrite(1);
	}
	buffer_mem_wb.getBufferControl() = Control();
	cout << "EX: " << buffer_id_ex.getInstruction() << endl;
}

void Procesador::etapaMEM()
{
	if(buffer_ex_mem.getBufferControl().getMemWrite() == 1)
		sw(buffer_ex_mem.getRegistroRs(),buffer_ex_mem.getALU_result());
	else if(buffer_ex_mem.getBufferControl().getMemRead() == 1){
		int value = stackPointer[999-buffer_ex_mem.getALU_result()/4].getRegistro();
		buffer_mem_wb.setBuffer_mem_wb(buffer_ex_mem.getRegistroRs(),"lw",value); //string string int
		buffer_mem_wb.getBufferControl().setMemToReg(1);
	}
	else if(buffer_ex_mem.getBufferControl().getMemWrite() == 0 && buffer_ex_mem.getBufferControl().getMemRead() == 0){
		buffer_mem_wb.setBuffer_mem_wb(buffer_ex_mem.getRegistroRd(),"",buffer_ex_mem.getALU_result());//int,string
		buffer_mem_wb.getBufferControl().setRegWrite(1);
	}
	cout << "MEM: " << buffer_id_ex.getInstruction() << endl;
}

void Procesador::etapaWB(){
	if(buffer_mem_wb.getBufferControl().getMemToReg() == 1){
		registros[buscarRegistro(buffer_ex_mem.getRegistroRs())].setRegistro(buffer_ex_mem.getALU_result());		
		buffer_mem_wb.getBufferControl().setMemToReg(0);
	}
	else if(buffer_mem_wb.getBufferControl().getRegDst() == 1){
		registros[buscarRegistro(buffer_mem_wb.getRegistroRd())].setRegistro(buffer_ex_mem.getALU_result());	
		buffer_mem_wb.getBufferControl().setRegDst(0);
	}
	cout << "WB: " << buffer_id_ex.getInstruction() << endl;
}

int Procesador::Hazard_DataDetector()  //recuerda modificar para forwarding
{
	//cout << "Hola estoy en hazard" << endl;
	if(buffer_ex_mem.getBufferControl().getRegWrite() == 1 &&
		buffer_ex_mem.getBufferControl().getRegDst() != 0 && 
		buffer_ex_mem.getRegistroRd() == buffer_id_ex.getRegistroRs()){
		cout << "Hazard: " << buffer_ex_mem.getRegistroRd() << endl;
		return 1;
	}
				
	else if(buffer_ex_mem.getBufferControl().getRegWrite() == 1 &&
			buffer_ex_mem.getBufferControl().getRegDst() != 0 && 
			buffer_ex_mem.getRegistroRd() == buffer_id_ex.getRegistroRt()){
				cout << "Hazard: " << buffer_ex_mem.getRegistroRd() << endl;
				return 2;
			}

	else if(buffer_mem_wb.getBufferControl().getRegWrite() == 1 &&
	   	buffer_mem_wb.getBufferControl().getRegDst() != 0 &&
	   	buffer_mem_wb.getRegistroRd() == buffer_ex_mem.getRegistroRs()){
			cout << "Hazard: " << buffer_ex_mem.getRegistroRd() << endl;
			return 3;
		}
				
	else if(buffer_mem_wb.getBufferControl().getRegWrite() == 1 &&
		buffer_mem_wb.getBufferControl().getRegDst() != 0 &&
		buffer_mem_wb.getRegistroRd() == buffer_id_ex.getRegistroRt()){
		cout << "Hazard: " << buffer_ex_mem.getRegistroRd() << endl;
		return 4;
	}
				
	else if(buffer_id_ex.getBufferControl().getMemRead() == 1 &&
		(buffer_id_ex.getRegistroRt() == buffer_if_id.getRegistroRs() || 
			buffer_id_ex.getRegistroRt() == buffer_if_id.getRegistroRt())){
		//poner en espera
		cout << "Hazard: " << buffer_ex_mem.getRegistroRd() << endl;
		return 5;}
}

/*void Procesador::NOP()
{
	
}*/

void Procesador::ejecutar()
{
	PC = instrucciones.begin();
	while(PC != instrucciones.end() && (buffer_mem_wb.getBufferControl().getMemToReg() == 0 || buffer_mem_wb.getBufferControl().getRegDst() == 0)){
		inst = *PC;
		if(buffer_mem_wb.getStatus())
			etapaWB();
		if(buffer_ex_mem.getStatus())
			etapaMEM();
		if(buffer_id_ex.getStatus())
			etapaEX();
		if(buffer_if_id.getStatus())
			etapaID();
		etapaIF();
		if(PC != instrucciones.end())
			PC++;
	}
}