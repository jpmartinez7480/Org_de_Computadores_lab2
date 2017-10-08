#include "Instruccion.cpp"
#include "Registro.cpp"

#include <list>
#include <string>
#include <iterator>

class Buffer
{
	public:
		Buffer();
		void setBuffer_if_id(Instruccion);
		void setBuffer_id_ex(int, int, string, string, string, string);
		void setBuffer_id_ex(int,string,string,string,string);
		void setBuffer_id_ex(string,int,string);
		void setBuffer_id_ex(int,int,string);
		void setBuffer_ex_mem(int,string,string);
		void setBuffer_mem_wb(string, string, int);
		void setInstruction(string);
		void setValueRs(int);
		void setValueRt(int);
		void setALU_result(int);
		void setRegistroRd(string);
		void setRegistroRt(string);
		void setRegistroRs(string);
		void setStatus(bool);
		int getValueRs();
		int getValueRt();
		int getALU_result();
		string getRegistroRd();
		string getRegistroRs();
		string getRegistroRt();
		string getInstruction();
		bool getStatus();
		Control getBufferControl();

	private:
		int valueRs;
		int valueRt;
		int valueALU_result;
		bool status;
		string rd;
		string rt;
		string rs;
		string instruction;
		Control bufferControl;
};