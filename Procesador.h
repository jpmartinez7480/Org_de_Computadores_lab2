#include "Control.cpp"
#include "Buffer.cpp"

#include <list> //definicion de la clase list
#include <algorithm>
#include <iterator>
using std::list;

class Procesador
{
	public:
		Procesador();
		~Procesador();
		int readFile_Mips(string nfile);
		int readFile_Registers(string);
		void showContent_Registros();
		int buscarRegistro(string r); //devuelve la pos del registro
		int ALU(int,int,string);
		int add(int,int);
		int sub(int,int);
		int mul(int,int);
		int div(int,int);
		int addi(int,int);
		int subi(int,int);
		void j(string);
		void sw(string,int);
		void ejecutar();
		void etapaIF();
		void etapaID();
		void etapaEX();
		void etapaMEM();
		void etapaWB();
		int Hazard_DataDetector();
		//bool Hazard_ControlDetector();
		void NOP();
		static const string regs[32];
		static const string operaciones[11];
	private:
		list<Instruccion>::iterator PC;
		list<Instruccion> instrucciones; //lista con las instrucciones mips
		Instruccion inst;
		Registro registros[32];
		Registro stackPointer[1000];
		Control lineControl;
		Buffer buffer_if_id;
		Buffer buffer_id_ex;
		Buffer buffer_ex_mem;
		Buffer buffer_mem_wb;
};

