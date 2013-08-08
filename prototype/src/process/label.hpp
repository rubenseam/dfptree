# include <watershed.h>


class Label : public LabelFunction {

public:

	// Constructor
	Label(void);

	// Destructor
	virtual ~Label(void);

	// Function
	int GetLabel(Message& message, int num_instances);

};
