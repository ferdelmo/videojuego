#include "Sonidos.h"

using namespace std;

mutex Sonidos::m; 
ALuint Sonidos::sources[256];
ALuint Sonidos::buffers[256];
unsigned int Sonidos::iBuf=0;
unsigned int Sonidos::iSource = 0;
ALCdevice * Sonidos::dev=nullptr;
ALCcontext * Sonidos::context=nullptr;

int Sonidos::loadWavFile(std::string filename, ALuint* buffer,
	ALsizei* size, ALsizei* frequency,
	ALenum* format) {
	//Local Declarations
	FILE* soundFile = NULL;
	WAVE_Format wave_format;
	RIFF_Header riff_header;
	WAVE_Data wave_data;
	unsigned char* data;
	m.lock();
	int viejaI = iBuf;
	iBuf++;
	if (iBuf > 255) {
		iBuf = 0;
	}
	cout << iBuf << viejaI << endl;
	m.unlock();
	//alDeleteBuffers(1, buffer);
	*size = wave_data.subChunk2Size;
	*frequency = wave_format.sampleRate;
	if (wave_format.numChannels == 1) {
		if (wave_format.bitsPerSample == 8)
			*format = AL_FORMAT_MONO8;
		else if (wave_format.bitsPerSample == 16)
			*format = AL_FORMAT_MONO16;
	}
	else if (wave_format.numChannels == 2) {
		if (wave_format.bitsPerSample == 8)
			*format = AL_FORMAT_STEREO8;
		else if (wave_format.bitsPerSample == 16)
			*format = AL_FORMAT_STEREO16;
	}

	try {
		soundFile = fopen(filename.c_str(), "rb");
		if (!soundFile)
			throw (filename);

		// Read in the first chunk into the struct
		fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

		//check for RIFF and WAVE tag in memeory
		if ((riff_header.chunkID[0] != 'R' ||
			riff_header.chunkID[1] != 'I' ||
			riff_header.chunkID[2] != 'F' ||
			riff_header.chunkID[3] != 'F') ||
			(riff_header.format[0] != 'W' ||
				riff_header.format[1] != 'A' ||
				riff_header.format[2] != 'V' ||
				riff_header.format[3] != 'E'))
			throw ("Invalid RIFF or WAVE Header");

		//Read in the 2nd chunk for the wave info
		fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
		//check for fmt tag in memory
		if (wave_format.subChunkID[0] != 'f' ||
			wave_format.subChunkID[1] != 'm' ||
			wave_format.subChunkID[2] != 't' ||
			wave_format.subChunkID[3] != ' ')
			throw ("Invalid Wave Format");

		//check for extra parameters;
		if (wave_format.subChunkSize > 16)
			fseek(soundFile, sizeof(short), SEEK_CUR);

		//Read in the the last byte of data before the sound file
		fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);

		//check for data tag in memory
		if (wave_data.subChunkID[0] != 'd' ||
			wave_data.subChunkID[1] != 'a' ||
			wave_data.subChunkID[2] != 't' ||
			wave_data.subChunkID[3] != 'a')
			throw ("Invalid data header");

		//Allocate memory for data
		data = new unsigned char[wave_data.subChunk2Size];

		// Read in the sound data into the soundData variable
		if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
			throw ("error loading WAVE data into struct!");

		//Now we set the variables that we passed in with the
		//data from the structs
		*size = wave_data.subChunk2Size;
		*frequency = wave_format.sampleRate;
		//The format is worked out by looking at the number of
		//channels and the bits per sample.
		if (wave_format.numChannels == 1) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_MONO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_MONO16;
		}
		else if (wave_format.numChannels == 2) {
			if (wave_format.bitsPerSample == 8)
				*format = AL_FORMAT_STEREO8;
			else if (wave_format.bitsPerSample == 16)
				*format = AL_FORMAT_STEREO16;
		}
		//create our openAL buffer and check for success
		alGenBuffers(1, buffer);
		if (alGetError() != AL_NO_ERROR) {
			std::cerr << "PENE " << alGetError() << std::endl;
		}
		//now we put our data into the openAL buffer and
		//check for success
		alBufferData(*buffer, *format, (void*)data,
			*size, *frequency);
		if (alGetError() != AL_NO_ERROR) {
			std::cerr << "PENE " << alGetError() << std::endl;
		}
		//clean up and return true if successful

		buffers[viejaI]=*buffer;
		fclose(soundFile);
		delete data;
		return viejaI;
	}
	catch (const char* error) {
		//our catch statement for if we throw a string
		std::cerr << error << " : trying to load "
			<< filename.c_str() << std::endl;
		//clean up memory if wave loading fails
		if (soundFile != NULL)
			fclose(soundFile);
		//return false to indicate the failure to load wave
		//delete data;
		return -1;
	}
}
void Sonidos::setListener(glm::vec3 pos, glm::vec3 dir) {
	ALfloat listenerOri[] = { 0.0f, 1.0f, 0.0f, dir.x, dir.y, dir.z };

	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	// check for errors
	alListener3f(AL_VELOCITY, 0, 0, 0);
	// check for errors
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void Sonidos::genBuffs() {
	alGenSources(256,sources);
}

void Sonidos::play(int * b, ALuint buf, glm::vec3 pos, string sf) {
	if (dev == nullptr) {
		const char * devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
		dev = alcOpenDevice(nullptr);
		if (!dev) {
			cout << "errorcito" << endl;
		}
		context = alcCreateContext(dev, NULL);
		if (!alcMakeContextCurrent(context)) {
			cout << "Error de contexto" << endl;
		}
	}
	m.lock();
	int viejaI = iSource;
	iSource++;
	if (iSource > 255) {
		iSource = 0;
	}
	cout << iSource << viejaI << endl;
	m.unlock();
	ALint state;
	ALsizei size;
	ALsizei frequency;
	ALenum format;
	
	//cout << "SONIDOS 0 " << alGetError() << endl;
	//SI DESCOMENTAS ESTA LINEA VA PERO TIENE Q IR LEYENDO ESO SOLO UNA VEZ
	if (*b == -1) {
		cout << "CARGAAAAAAAAAAAAAAAANDO" << endl;
		*b=Sonidos::loadWavFile(sf, &buf, &size, &frequency, &format);
	}
	//cout << "SONIDOS 1 " << alGetError() << endl;
	alGenSources((ALuint)1, &sources[viejaI]);
	//cout << "SONIDOS 2 " << alGetError() << endl;
	// check for errors
	alSourcef(sources[viejaI], AL_PITCH, 1);
	// check for errors
	alSourcef(sources[viejaI], AL_GAIN, 1);
	// check for errors
	alSource3f(sources[viejaI], AL_POSITION, pos.x, pos.y, pos.z);
	// check for errors
	alSource3f(sources[viejaI], AL_VELOCITY, 0, 0, 0);
	// check for errors
	alSourcei(sources[viejaI], AL_LOOPING, AL_FALSE);
	// check for errros
	alSourcei(sources[viejaI], AL_BUFFER, buffers[*b]);
	//cout << "SONIDOS 3 " << alGetError() << endl;
	alSourcePlay(sources[viejaI]);

	//cout << "SONIDOS 4 " << alGetError() << endl;

	do {
		alGetSourcei(sources[viejaI], AL_SOURCE_STATE, &state);
		this_thread::sleep_for(chrono::milliseconds(100));
	} while (state == AL_PLAYING);

	// Clean up sources and buffers;
	alDeleteSources(1, &sources[viejaI]);
	//delete our buffer
	//alDeleteBuffers(1, &buffers[viejaI]);
	/*dev = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(dev);*/
}