#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>

using namespace std;

// Internal ALU control codes.
#define ADDU (1)
#define SUBU (3)
#define AND (4)
#define OR  (5)
#define NOR (7)

// Memory size.
// In reality, the memory size should be 2^32, but for this lab and space reasons,
// we keep it as this large number, but the memory is still 32-bit addressable.
#define MemSize (65536)


class RF
{
  public:
    bitset<32> ReadData1, ReadData2; 
    RF()
    { 
      Registers.resize(32);  
      Registers[0] = bitset<32> (0);  
    }

    void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
    {   
      /**
       * @brief Reads or writes data from/to the Register.
       *
       * This function is used to read or write data from/to the register, depending on the value of WrtEnable.
       * Put the read results to the ReadData1 and ReadData2.
       */
      // TODO: implement!
      ReadData1 = Registers[RdReg1.to_ulong()];
      ReadData2 = Registers[RdReg2.to_ulong()];

      if (WrtEnable[0]) {
        Registers[WrtReg.to_ulong()] = WrtData;
      }
    }

    void OutputRF()
    {
      ofstream rfout;
      rfout.open("RFresult.txt",std::ios_base::app);
      if (rfout.is_open())
      {
        rfout<<"A state of RF:"<<endl;
        for (int j = 0; j<32; j++)
        {        
          rfout << Registers[j]<<endl;
        }

      }
      else cout<<"Unable to open file";
      rfout.close();

    }     
  private:
    vector<bitset<32> >Registers;
};


class ALU
{
  public:
    bitset<32> ALUresult;
    bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
    {   
      /**
       * @brief Implement the ALU operation here.
       *
       * ALU operation depends on the ALUOP, which are definded as ADDU, SUBU, etc. 
       */
      // TODO: implement!
      unsigned int ans;

      if (ALUOP==ADDU){
        ans=oprand1.to_ulong()+oprand2.to_ulong();
      }
      else if(ALUOP==SUBU){
        ans=oprand1.to_ulong()-oprand2.to_ulong();
      }
      else if (ALUOP==AND){
        ans=oprand1.to_ulong()&oprand2.to_ulong();
      }
      else if(ALUOP==OR){
        ans=oprand1.to_ulong()|oprand2.to_ulong();
      }
      else if (ALUOP==NOR){
        ans=~(oprand1.to_ulong()|oprand2.to_ulong());
      }

      ALUresult = bitset<32> (ans);
      return ALUresult;
    }            
};


class INSMem
{
  public:
    bitset<32> Instruction;
    INSMem()
    {       IMem.resize(MemSize); 
      ifstream imem;
      string line;
      int i=0;
      imem.open("imem.txt");
      if (imem.is_open())
      {
        while (getline(imem,line))
        {      
          IMem[i] = bitset<8>(line);
          i++;
        }

      }
      else cout<<"Unable to open file";
      imem.close();

    }

    bitset<32> ReadMemory (bitset<32> ReadAddress) 
    {    
      // TODO: implement!
      /**
       * @brief Read Instruction Memory (IMem).
       *
       * Read the byte at the ReadAddress and the following three byte,
       * and return the read result. 
       */
      // Read address is the Memory address, which when converted to decimal, is an index to the IMem vector.
      // address variable is used to store the integer value of ReadAddress, which is then used to index the IMem vector.
      unsigned int address = ReadAddress.to_ulong();

      // byte1, byte2, byte3, and byte4 are used to store the four bytes read from the IMem vector.
      unsigned long byte1, byte2, byte3, byte4;

      // The four bytes from the Imem vector are indexed and stored in the appropriate byte variable.
      // The first is the address itself and the remaining 3 are the next three consecutive addresses.
      byte1 = IMem[address].to_ulong();
      byte2 = IMem[address + 1].to_ulong();
      byte3 = IMem[address + 2].to_ulong();
      byte4 = IMem[address + 3].to_ulong();

      // The four byte variables are combined into one one 32-bit instruction variable.
      unsigned long bits = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

      // the bitsvariable is converted to a bitset of 32 bits and assigned to the Instruction variable.
      Instruction = bitset<32>(bits);

      // returns instruction variable
      std::cout << "Instruction: " << Instruction << std::endl;

      return Instruction;
    }     

  private:
    vector<bitset<8> > IMem;

};

class DataMem    
{
  public:
    bitset<32> readdata;  
    DataMem()
    {
      DMem.resize(MemSize); 
      ifstream dmem;
      string line;
      int i=0;
      dmem.open("dmem.txt");
      if (dmem.is_open())
      {
        while (getline(dmem,line))
        {      
          DMem[i] = bitset<8>(line);
          i++;
        }
      }
      else cout<<"Unable to open file";
      dmem.close();

    }  
    bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
    {    
      /**
       * @brief Reads/writes data from/to the Data Memory.
       *
       * This function is used to read/write data from/to the DataMem, depending on the readmem and writemem.
       * First, if writemem enabled, WriteData should be written to DMem, clear or ignore the return value readdata,
       * and note that 32-bit WriteData will occupy 4 continious Bytes in DMem. 
       * If readmem enabled, return the DMem read result as readdata.
       */
      // TODO: implement!
      // Address is the Data address, which when converted to integer, is an
      // index to the Dmem vector.
      // MemAddress variable is used to store the integer value of Address,
      // which is then used to index the Dmem vector.
      unsigned int MemAddress = Address.to_ulong();

      // check if writemem is enabled, if it's enabled, write data to Dmem
      if (writemem.to_ulong() == 1) {
        // convert WriteData to an unsinged long number and store in variable
        unsigned long WriteValue = WriteData.to_ulong();

        // byte1, byte2, byte3, and byte4 are used to store the four bytes
        // gotten from the WriteData.
        unsigned long byte1, byte2, byte3, byte4;

        // shift the writeValue so only the appropriate 8-bit value (byte) is
        // left
        byte1 = (WriteValue >> 24) & 0xFF;
        byte2 = (WriteValue >> 16) & 0xFF;
        byte3 = (WriteValue >> 8) & 0xFF;
        byte4 = WriteValue & 0xFF;

        // Write to Dmem
        DMem[MemAddress] = bitset<8>(byte1);
        DMem[MemAddress + 1] = bitset<8>(byte2);
        DMem[MemAddress + 2] = bitset<8>(byte3);
        DMem[MemAddress + 3] = bitset<8>(byte4);
      }

      // check if readmem is enabled, if it's enabled, read data from Dmem and
      // store in readdata
      if (readmem.to_ulong() == 1) {
        // byte1, byte2, byte3, and byte4 are used to store the four bytes read
        // from the DMem vector.
        unsigned long byte1, byte2, byte3, byte4;

        // The four bytes from the DMem vector are indexed and stored in the
        // appropriate byte variable. The first is the Memory address itself and
        // the remaining 3 are the next three consecutive addresses.
        byte1 = DMem[MemAddress].to_ulong();
        byte2 = DMem[MemAddress + 1].to_ulong();
        byte3 = DMem[MemAddress + 2].to_ulong();
        byte4 = DMem[MemAddress + 3].to_ulong();

        // The four byte variables are combined into one one 32-bit data value.
        unsigned long bits = (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;

        // the bits variable is converted to a bitset of 32 bits and assigned to
        // the readdata variable.
        readdata = bitset<32>(bits);
      }

      return readdata;
    }   

    void OutputDataMem()
    {
      ofstream dmemout;
      dmemout.open("dmemresult.txt");
      if (dmemout.is_open())
      {
        for (int j = 0; j< 1000; j++)
        {     
          dmemout << DMem[j]<<endl;
        }

      }
      else cout<<"Unable to open file";
      dmemout.close();

    }             

  private:
    vector<bitset<8> > DMem;

};  



int main()
{
  RF myRF;
  ALU myALU;
  INSMem myInsMem;
  DataMem myDataMem;
  bitset<32> PC(0); // 32-bit program counter, initialized to 0

  while (1)  // TODO: implement!
  {
    // Fetch: fetch an instruction from myInsMem.
    bitset<32> Instruction = myInsMem.ReadMemory(PC);

    // If current instruction is "11111111111111111111111111111111", then break; (exit the while loop)
    if (Instruction.to_string() == "11111111111111111111111111111111"){
      break;
    }

    // decode(Read RF): get opcode and other signals from instruction, decode instruction
    // opCode
    int opCode = stoi(Instruction.to_string().substr(0, 6), nullptr, 2);

    // other signals
    int funct;
    bitset<5> rs, rt, rd, shamt;
    bitset<16> immediate;
    bitset<26> address;
    bitset<3> ALUOP;
    bitset<32> ALUReturnValue;

    // checks if opcode is R-type and get all signals
    if (opCode == 0x00) {

      // get values for other signals
      rs = bitset<5> (Instruction.to_string().substr(6, 5));
      rt = bitset<5> (Instruction.to_string().substr(11, 5));
      rd = bitset<5> (Instruction.to_string().substr(16, 5));
      shamt = bitset<5> (Instruction.to_string().substr(21, 5));

      // get the funct
      funct = stoi(Instruction.to_string().substr(26, 6), nullptr, 2);

      // decode the signals
      myRF.ReadWrite(rs, rt, bitset<5> (0), bitset<32> (0), bitset<1> (0));

      // check for specific instruction
      if (funct == 0x21) {
        // set the ALUOP value to ADDU
        ALUOP = bitset<3> ("000");

      } else if (funct == 0x23) {
        // set the ALUOP value to SUBU
        ALUOP = bitset<3> ("001");

      } else if (funct == 0x24) {
        // set the ALUOP value to AND
        ALUOP = bitset<3> ("010");

      } else if (funct == 0x25) {
        // set the ALUOP value to OR
        ALUOP = bitset<3> ("011");
        
      } else if (funct == 0x27) {
        // set the ALUOP value to NOR
        ALUOP = bitset<3> ("100");
      }

      // Execute: after decoding, ALU may run and return result
      ALUReturnValue =
          myALU.ALUOperation(ALUOP, myRF.ReadData1, myRF.ReadData2);

      // Write back to RF: some operations may write things to RF
      myRF.ReadWrite(rs, rt, rd, ALUReturnValue, bitset<1>(1));

      // Update PC
      PC = bitset<32>(PC.to_ulong() + 4);

    } else if (opCode == 0x09 || opCode == 0x04 || opCode == 0x23 || opCode == 0x2B) { // for I-type instructions

      // decode(Read RF): get opcode and other signals from instruction, decode instruction
      rs = bitset<5> (Instruction.to_string().substr(6, 5));
      rt = bitset<5> (Instruction.to_string().substr(11, 5));
      immediate = bitset<16> (Instruction.to_string().substr(16, 16));

      // Extend the immediate
        string immediateString = immediate.to_string();

        // check if MSB of immediate is 0 or 1
        if (immediateString[0] == '0') {
          // add 16 0s before the immediate
          immediateString = "0000000000000000" + immediateString;

        } else {
          // add 16 1s before the immediate
          immediateString = "1111111111111111" + immediateString;
        }

        bitset<32> signExtendedImmediate (immediateString);
          bitset<5> WriteReg=rt;
        // decode instruction
        myRF.ReadWrite(rs, rt, WriteReg, bitset<32>(0), bitset<1>(0));

      if (opCode == 0x09) { // addiu
        // Set ALUOP
        ALUOP = bitset<3> ("000");

        // Execute: after decoding, ALU may run and return result
        ALUReturnValue = myALU.ALUOperation(ALUOP, myRF.ReadData1, signExtendedImmediate);

        // Write back to RF: some operations may write things to RF
        myRF.ReadWrite(rs, rt, WriteReg, ALUReturnValue, bitset<1>(1));

        // Update PC
        PC = bitset<32>(PC.to_ulong() + 4); 

      } else if (opCode == 0x04) { // beq

        // Set ALUOP
        ALUOP = bitset<3> ("001");

         // Execute: after decoding, ALU may run and return result
        ALUReturnValue = myALU.ALUOperation(ALUOP, myRF.ReadData1, myRF.ReadData2);

        // Check if ALUReturnValue is equal to 0, indicating rt and rs are equal
        if (ALUReturnValue == bitset<32>(0)) {
          // branch by updating PC
          PC = bitset<32>(PC.to_ulong() + 4 + (signExtendedImmediate << 2).to_ulong()); // update this, not correct

        } else {
          // Update PC
          PC = bitset<32>(PC.to_ulong() + 4);

        }
      } else if (opCode == 0x23) { // lw instruction

        // Set ALUOP
        ALUOP = bitset<3> ("000"); // ADDU

        // Execute: after decoding, ALU may run and return result
        ALUReturnValue = myALU.ALUOperation(ALUOP, signExtendedImmediate, myRF.ReadData1);

        // Read/Write Mem: access data memory (myDataMem)
        bitset<32> dataMemValue = myDataMem.MemoryAccess(ALUReturnValue, bitset<32>(0), bitset<1>(1), bitset<1>(0));

        // Write back to RF: some operations may write things to RF
        myRF.ReadWrite(rs, rt, rt, dataMemValue, bitset<1>(1));

        // Update PC
        PC = bitset<32>(PC.to_ulong() + 4);
      } else if (opCode == 0x2B) { // sw instruction

        // Set ALUOP
        ALUOP = bitset<3> ("000"); // ADDU

        // Execute: after decoding, ALU may run and return result
        ALUReturnValue = myALU.ALUOperation(ALUOP, signExtendedImmediate, myRF.ReadData1);

        // Read/Write Mem: access data memory (myDataMem)
        myDataMem.MemoryAccess(ALUReturnValue, myRF.ReadData2, bitset<1>(0), bitset<1>(1));

        // Update PC
        PC = bitset<32>(PC.to_ulong() + 4);
      }
    }
    

    // Execute: after decoding, ALU may run and return result

    // Read/Write Mem: access data memory (myDataMem)

    // Write back to RF: some operations may write things to RF

    // Update PC


    /**** You don't need to modify the following lines. ****/
    myRF.OutputRF(); // dump RF;    
  }
  myDataMem.OutputDataMem(); // dump data mem

  return 0;
}
