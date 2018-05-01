#include <iostream>
#include <al.h>
#include <alc.h>
#include <mutex>

#include <glm/glm.hpp>

#pragma warning(disable : 4996)

using namespace std;


struct RIFF_Header {
	char chunkID[4] = { 0,0,0,0 };;
	long chunkSize;//size not including chunkSize or chunkID
	char format[4] = { 0,0,0,0 };;
};

/*
* Struct to hold fmt subchunk data for WAVE files.
*/
struct WAVE_Format {
	char subChunkID[4] = { 0,0,0,0 };
	long subChunkSize;
	short audioFormat;
	short numChannels;
	long sampleRate;
	long byteRate;
	short blockAlign;
	short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data {
	char subChunkID[4] = { 0,0,0,0 }; //should contain the word data
	long subChunk2Size; //Stores the size of the data block
};


struct valores {
	ALCdevice * dev = nullptr;
	ALCcontext * context = nullptr;
};

class Sonidos {


	public:

		static mutex m;
		static ALuint sources[256];
		static ALuint buffers[256];
		static unsigned int iBuf;
		static unsigned int iSource;
		static ALCdevice * dev;
		static ALCcontext * context;
		Sonidos() {
		}

		static int loadWavFile(std::string filename, ALuint* buffer,
			ALsizei* size, ALsizei* frequency,
			ALenum* format);


		static void setListener(glm::vec3 pos, glm::vec3 dir);

		static void genBuffs();

		static void play(int * b, ALuint buf, glm::vec3 pos);
};