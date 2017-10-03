#include <string>
using std::string;

class Control
{
	public:
		Control();
		void setRegDst(char rd);
		void setJump(char j);
		void setBranch(char b);
		void setMemRead(char mr);
		void setMemToReg(char mtr);
		void setALUOp(string aop);
		void setMemWrite(char mw);
		void setALUSrc(char as);
		void setRegWrite(char rw);
		char getRegDst();
		char getJump();
		char getBranch();
		char getMemRead();
		char getMemToReg();
		string getALUOp();
		char getMemWrite();
		char getALUSrc();
		char getRegWrite();
		void establecerControl(char rd, char j, char b, char mr, char mtr, string aop, char mw, char as, char rw);
		void controlSign(char,string);
	private:
		char regDst;
		char jump;
		char branch;
		char memRead;
		char memToReg;
		string ALUOp;
		char memWrite;
		char ALUSrc;
		char regWrite;
};