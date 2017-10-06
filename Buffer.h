#include "Instruccion.cpp"
#include "Registro.cpp"

#include <list>
#include <string>
#include <iterator>

class Buffer
{
	public:
		Buffer();
		~Buffer();
		void setInstruction(string);
		void setValueRs(int);
		void setValueRt(int);
		void setActivity(int);
		void setPC(list<Instruccion>::iterator);
		void setRegistroRd(string);
		void setRegistroRt(string);
		void setRegistroRs(string);
		int getValueRs();
		int getValueRt();
		int getActivity();
		string getRegistroRd();
		string getRegistroRs();
		string getRegistroRt();
		Instruccion getInstruction();
		Instruccion getPC();

	private:
		int valueRs;
		int valueRt;
		int activity;
		list<Instruccion>::iterator PC;
		string rd;
		string rt;
		string rs;
		string instruction;
}