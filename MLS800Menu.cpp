#include "MLS800Menu.h"

MenuItem root = {
	"menu",
	0,
	&midi,
	&memClr,
	&midi,
	&menuDisplay,
	0,
	&menuExit,
	0,
	0
};

MenuItem midi = {
	"midi",
	&root,
	&midiR,
	&memClr,
	&dim,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem midiR = {
	"r   ",
	&midi,
	&midiR_Edit,
	&midiR,
	&midiR,
	&midiRDisplay,
	0,
	0,
	0,
	0
};

MenuItem midiR_Edit = {
	"r.   ",
	&midiR,
	0,
	0,
	0,
	&midiRDisplay,
	&midiRSave,
	&midiRBack,
	&midiRDown,
	&midiRUp
};

MenuItem dim = {
	"dim",
	&root,
	&dim_Edit,
	&midi,
	&memClr,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem dim_Edit = {
	"dim. ",
	&dim,
	0,
	0,
	0,
	&dimDisplay,
	&dimSave,
	&dimBack,
	&dimDown,
	&dimUp
};

MenuItem memClr = {
	"clr",
	&root,
	&memClrYes,
	&dim,
	&midi,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem memClrYes = {
	"yes",
	&memClr,
	0,
	&memClrNo,
	&memClrNo,
	&menuDisplay,
	&factoryReset,
	0,
	0,
	0
};

MenuItem memClrNo = {
	"no",
	&memClr,
	0,
	&memClrYes,
	&memClrYes,
	&menuDisplay,
	0,
	0,
	0,
	0
};

MenuItem memClrd = {
	"clrd",
	&memClr,
	0,
	0,
	0,
	&menuDisplay,
	0,
	0,
	0,
	0
};

Menu menu = Menu(&root);