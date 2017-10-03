#include <string>

using std::string;

class Instruccion
{
	public:
		Instruccion();
		//~Instrucciones();
		void establecerInstruccion(string op, string r1,string r2, string r3);
		void establecerInstruccion(string r1);
		void setOperando(string);
		void setRegistro1(string r);
		void setRegistro2(string r);
		void setRegistro3(string r);
		void setInstruction(string,char);
		void setTipo(char);
		char Operacion(string op);
		char getTipo();
		string getOperando();
		string getRegistro1();
		string getRegistro2();
		string getRegistro3();
	private:
		char tipo;
		string operando;
		string registro1;
		string registro2;
		string registro3;
};