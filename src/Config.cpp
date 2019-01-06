#include "MLS800.h"

void writeConfig()
{
	debugPrint("saved config :\ndim : ");
	debugPrintln(_config.displayDim);
	debugPrint("midi rx : ");
	debugPrintln(_config.rxChannel);
	_storage.writeBlock(CONFIG_ADDR, _config);
}

void resetConfig()
{
	debugPrint("Resetting configuration... ");
	Config config = {
		CONFIG_SEED,
		MLS800_VERSION,
		MIDI_CHANNEL_OMNI,
		MIDI_CHANNEL_OFF,
		0,
		0,
		8
	};

	_config = config;
	writeConfig();
	debugPrintln("Done !");
}

void readConfig()
{
	debugPrintln("Reading configuration...");
	_storage.readBlock(CONFIG_ADDR, _config);
	if (!String(CONFIG_SEED).equals(_config.seed)) {
		resetConfig();
	}
}