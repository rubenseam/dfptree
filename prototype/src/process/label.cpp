# include <watershed.h>
# include "label.hpp"


// RegisterFunction
RegisterFunction(Label);

// Constructor
Label::Label(void) {
}

// Destructor
Label::~Label(void) {
}

// Label function
int Label::GetLabel(Message& message, int num_instances) {
	return *(static_cast<unsigned int *>(message.GetData())) %
		num_instances;
}
