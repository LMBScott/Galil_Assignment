#include "Galil.h"

// Default constructor
Galil::Galil() {
	
}

// Constructor with EmbeddedFunciton initialization
Galil::Galil(EmbeddedFunctions* Funcs, GCStringIn address) {
	
}

// Default destructor
Galil::~Galil() {
	
}

// DIGITAL OUTPUTS

// Write to all 16 bits of digital output, 1 command to the Galil
void Galil::DigitalOutput(uint16_t value) {
	
}

// Write to one byte, either high or low byte, as specified by user in 'bank'
// 0 = low, 1 = high
void Galil::DigitalByteOutput(bool bank, uint8_t value) {
	
}

// Write single bit to digital outputs. 'bit' specifies which bit
void Galil::DigitalBitOutput(bool val, uint8_t bit) {
	
}


// DIGITAL INPUTS

// Return the 16 bits of input data
// Query the digital inputs of the GALIL, See Galil command library @IN
uint16_t Galil::DigitalInput() {
	
}

// Read either high or low byte, as specified by user in 'bank'
// 0 = low, 1 = high
uint8_t Galil::DigitalByteInput(bool bank) {
	
}

// Read single bit from current digital inputs. Above functions
// may use this function
bool Galil::DigitalBitInput(uint8_t bit) {
	
}


// Check the string response from the Galil to check that the last 
// command executed correctly. 1 = succesful
bool Galil::CheckSuccessfulWrite() {
	
}

// ANALOG FUNCITONS

// Read Analog channel and return voltage
float Galil::AnalogInput(uint8_t channel) {
	
}

// Write to any channel of the Galil, send voltages as
// 2 decimal place in the command string	
void Galil::AnalogOutput(uint8_t channel, double voltage) {
	
}

// Configure the range of the input channel with
// the desired range code
void Galil::AnalogInputRange(uint8_t channel, uint8_t range) {
	
}

// ENCODER / CONTROL FUNCTIONS
// Manually Set the encoder value to zero
void Galil::WriteEncoder() {
	
}

// Read from Encoder
int Galil::ReadEncoder() {
	
}

// Set the desired setpoint for control loops, counts or counts/sec
void Galil::setSetPoint(int s) {
	
}

// Set the proportional gain of the controller used in controlLoop()
void Galil::setKp(double gain) {
	
}

// Set the integral gain of the controller used in controlLoop()
void Galil::setKi(double gain) {
	
}

// Set the derivative gain of the controller used in controlLoop()
void Galil::setKd(double gain) {
	
}

// Run the control loop. ReadEncoder() is the input to the loop. The motor is the output.
// The loop will run using the PID values specified in the data of this object, and has an 
// automatic timeout of 10s. You do not need to implement this function, it is defined in
// GalilControl.lib. debug = 1 for debugging (it will print some values).
void Galil::PositionControl(bool debug) {
	
}

// same as above. Setpoint interpreted as counts per second
void Galil::SpeedControl(bool debug) {
	
}

// Operator overload for '<<' operator. So the user can say cout << Galil; This function should print out the
// output of GInfo and GVersion, with two newLines after each.
std::ostream& operator<<(std::ostream& output, Galil& galil) {
	
}