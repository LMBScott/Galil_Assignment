#include "Galil.h"

Galil::Galil() {
	Functions = new EmbeddedFunctions();
	g = 0;
	Functions->GOpen(DEFAULT_ADDRESS, &g);
	ReadBytes = new GSize();
}

// Constructor with EmbeddedFunciton initialization
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	Functions = Funcs;
	g = 0;
	Functions->GOpen(address, &g);
	ReadBytes = new GSize();
}

// Default destructor
Galil::~Galil() {
	if (g) {
		GClose(g);
	}

	delete ReadBytes;
	delete Functions;
}

// DIGITAL OUTPUTS

// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalOutput(uint16_t value) {
	char Command[COM_STR_LEN] = "";

	for (int i = 0; i < 16; i++) {
		int bitVal = (value >> i) & 0b1; // Get ith bit of "value" input

		DigitalBitOutput(bitVal, i);
	}
}

// Write to one byte, either high or low byte, as specified by user in 'bank'
// 0 = low, 1 = high
void Galil::DigitalByteOutput(bool bank, uint8_t value) {
	char Command[COM_STR_LEN] = "";
	int offset = bank ? 8 : 0; // Offset bit index by 8 for high byte bank

	for (int i = 0; i < 8; i++) {
		int bitVal = (value >> i) & 0b1; // Get ith bit of "value" input
		
		DigitalBitOutput(bitVal, i + offset);
	}
}

// Write single bit to digital outputs. 'bit' specifies which bit
void Galil::DigitalBitOutput(bool val, uint8_t bit) {
	char Command[COM_STR_LEN] = "";

	if (val) {
		// val is 1, set corresponding digital output bit to 1
		sprintf_s(Command, "SB %d;", bit);
	}
	else {
		// val is 0, set corresponding digital output bit to 0
		sprintf_s(Command, "CB %d;", bit);
	}

	// Send command to Galil controller
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);
}


// DIGITAL INPUTS

// Return the 16 bits of input data
// Query the digital inputs of the GALIL, See Galil command library @IN
uint16_t Galil::DigitalInput() {
	char Command[COM_STR_LEN] = "";
	uint16_t output = 0;
	
	// Read each bit of digital input and store in a 16-bit integer
	for (int i = 0; i < 16; i++) {
		uint16_t bitVal = (int)DigitalBitInput(i); // Convert bit value string to integer

		output |= (bitVal << i); // Set corresponding bit of output to value of ith bit
	}

	return output;
}

// Read either high or low byte, as specified by user in 'bank'
// 0 = low, 1 = high
uint8_t Galil::DigitalByteInput(bool bank) {
	char Command[COM_STR_LEN] = "";
	uint8_t output = 0;

	int offset = bank ? 8 : 0; // Offset bit index by 8 for high byte bank

	// Read each bit of a given digital input byte and store in an 8-bit integer
	for (int i = 0; i < 8; i++) {
		uint8_t bitVal = (int)DigitalBitInput(i + offset); // Convert bit value string to integer

		output |= (bitVal << i); // Set corresponding bit of output to value of ith bit
	}

	return output;
}

// Read single bit from current digital inputs. Above functions
// may use this function
bool Galil::DigitalBitInput(uint8_t bit) {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "MG @IN[%d];", bit);

	// Send command to Galil controller
	// Get digital input value as a string
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);

	return (bool)std::stoi(ReadBuffer); // Convert digital input string to integer and return
}


// Check the string response from the Galil to check that the last 
// command executed correctly. 1 = succesful
bool Galil::CheckSuccessfulWrite() {
	return (ReadBuffer == SUCCESS_STRING);
}

// ANALOG FUNCITONS

// Read Analog channel and return voltage
float Galil::AnalogInput(uint8_t channel) {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "MG @AN[%d];", channel);

	// Send command to Galil controller
	// Get analog input value as a string
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);

	return std::stof(ReadBuffer); // Convert analog input string to float and return
}

// Write to any channel of the Galil, send voltages as
// 2 decimal place in the command string	
void Galil::AnalogOutput(uint8_t channel, double voltage) {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "AO %d,%.2f;", channel, voltage);

	// Send command to Galil controller
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);
}

// Configure the range of the input channel with
// the desired range code
void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "AQ %d,%d;", channel, range);

	// Send command to Galil controller
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);
}

// ENCODER / CONTROL FUNCTIONS
// Manually Set the encoder value to zero
void Galil::WriteEncoder() {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "WE 0,0;");

	// Send command to Galil controller
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);
}

// Read from Encoder
int Galil::ReadEncoder() {
	char Command[COM_STR_LEN] = "";

	sprintf_s(Command, "QE;");

	// Send command to Galil controller
	Functions->GCommand(g, Command, ReadBuffer, sizeof(ReadBuffer), ReadBytes);

	return std::stoi(ReadBuffer);
}

// Set the desired setpoint for control loops, counts or counts/sec
void Galil::setSetPoint(int s) {
	setPoint = s;
}

// Set the proportional gain of the controller used in controlLoop()
void Galil::setKp(double gain) {
	ControlParameters[0] = gain;
}

// Set the integral gain of the controller used in controlLoop()
void Galil::setKi(double gain) {
	ControlParameters[1] = gain;
}

// Set the derivative gain of the controller used in controlLoop()
void Galil::setKd(double gain) {
	ControlParameters[2] = gain;
}

// Operator overload for '<<' operator. So the user can say cout << Galil; This function should print out the
// output of GInfo and GVersion, with two newLines after each.
std::ostream& operator<<(std::ostream& output, Galil& galil) {
	GCStringOut infoBuffer = new char[INFO_STR_LEN];
	GCStringOut verBuffer = new char[INFO_STR_LEN];

	galil.Functions->GInfo(galil.g, infoBuffer, INFO_STR_LEN);
	galil.Functions->GVersion(verBuffer, INFO_STR_LEN);

	output << infoBuffer << std::endl << std::endl;
	output << verBuffer << std::endl << std::endl;

	delete [] infoBuffer;
	delete [] verBuffer;

	return output;
}