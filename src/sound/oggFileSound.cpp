#include "oggFileSound.h"

#include <cstring>
#include "../core/virtualFileSystem.h"

bool oggFileSound::init(const std::string& filename) {
    return this->init(filename, 1.0f, 1.0f, glm::vec3{}, soundType::EFFECT, false, true);
}

bool oggFileSound::init(const std::string& filename, float pitch_, float gain_, const glm::vec3& position_, soundType type_, bool loop_, bool is3d_) {
    this->pitch = pitch_;
    this->gain = gain_;
    this->setPosition(position_);
    this->type = type_;
    this->loop = loop_;
    this->is3d = is3d_;

    this->audioData.filename = virtualFileSystem::getSoundPath(filename);
    this->audioData.file.open(this->audioData.filename, std::ios::binary);
    if (!this->audioData.file.is_open()) {
        chiraLogger::log(ERR, "OGG", "Could not open " + filename);
        return false;
    }
    this->audioData.file.seekg(0, std::ios_base::beg);
    this->audioData.file.ignore(std::numeric_limits<std::streamsize>::max());
    this->audioData.size = (ALsizei) this->audioData.file.gcount();
    this->audioData.file.clear();
    this->audioData.file.seekg(0, std::ios_base::beg);
    this->audioData.sizeConsumed = 0;

    ov_callbacks oggCallbacks;
    oggCallbacks.read_func = readOggVorbisCallback;
    oggCallbacks.close_func = nullptr;
    oggCallbacks.seek_func = seekOggVorbisCallback;
    oggCallbacks.tell_func = tellOggVorbisCallback;

    if (ov_open_callbacks(reinterpret_cast<void*>(&this->audioData), &this->audioData.oggVorbisFile, nullptr, -1, oggCallbacks) < 0) {
        chiraLogger::log(ERR, "OGG", "Could not open ov_open_callbacks for " + filename);
        return false;
    }

    vorbis_info* vorbisInfo = ov_info(&this->audioData.oggVorbisFile, -1);

    this->audioData.channels = vorbisInfo->channels;
    this->audioData.bitsPerSample = 16;
    this->audioData.sampleRate = vorbisInfo->rate;
    this->audioData.duration = (std::size_t) ov_time_total(&this->audioData.oggVorbisFile, -1);

    alCall(alGenSources, 1, &this->audioData.source);
    alCall(alSourcef, this->audioData.source, AL_PITCH, this->pitch);
    alCall(alSourcef, this->audioData.source, AL_GAIN, this->gain);
    if (!this->is3d) {
        alSourcei(this->audioData.source, AL_SOURCE_RELATIVE, AL_TRUE);
        alCall(alSource3f, this->audioData.source, AL_POSITION, 0, 0, 0);
    } else {
        alCall(alSource3f, this->audioData.source, AL_POSITION, this->position.x, this->position.y, this->position.z);
    }
    alCall(alSource3f, this->audioData.source, AL_VELOCITY, 0, 0, 0);
    alCall(alSourcei, this->audioData.source, AL_LOOPING, AL_FALSE); // Looping is handled manually later

    alCall(alGenBuffers, OGG_NUM_BUFFERS, &this->audioData.buffers[0]);

    if (this->audioData.file.eof()) {
        chiraLogger::log(ERR, "OGG", "Already reached EOF without loading data in " + filename);
        return false;
    } else if(this->audioData.file.fail()) {
        chiraLogger::log(ERR, "OGG", "Fail bit set in " + filename);
        return false;
    } else if (!this->audioData.file) {
        chiraLogger::log(ERR, "OGG", "File at " + filename + " is false");
        return false;
    }
    return this->readFile(this->audioData.filename);
}

bool oggFileSound::readFile(const std::string& filename) {
    char* data = new char[OGG_BUFFER_SIZE];
    for (std::uint8_t i = 0; i < OGG_NUM_BUFFERS; ++i) {
        std::int32_t dataSoFar = 0;
        while (dataSoFar < OGG_BUFFER_SIZE) {
            std::int32_t result = ov_read(&this->audioData.oggVorbisFile, &data[dataSoFar], OGG_BUFFER_SIZE - dataSoFar, 0, 2, 1, reinterpret_cast<int*>(&this->audioData.oggCurrentSection));
            switch (result) {
                case OV_HOLE:
                    chiraLogger::log(ERR, "OGG", "OV_HOLE found in initial read of buffer " + std::to_string(i) + " in file " + filename);
                    break;
                case OV_EBADLINK:
                    chiraLogger::log(ERR, "OGG", "OV_EBADLINK found in initial read of buffer " + std::to_string(i) + " in file " + filename);
                    break;
                case OV_EINVAL:
                    chiraLogger::log(ERR, "OGG", "OV_EINVAL found in initial read of buffer " + std::to_string(i) + " in file " + filename);
                    break;
                case 0:
                    chiraLogger::log(ERR, "OGG", "EOF found in initial read of buffer " + std::to_string(i) + " in file " + filename);
                    break;
                default:
                    break;
            }
            dataSoFar += result;
        }
        if (this->audioData.channels == 1 && this->audioData.bitsPerSample == 8) {
            this->audioData.format = AL_FORMAT_MONO8;
        } else if (this->audioData.channels == 1 && this->audioData.bitsPerSample == 16) {
            this->audioData.format = AL_FORMAT_MONO16;
        } else if (this->audioData.channels == 2 && this->audioData.bitsPerSample == 8) {
            this->audioData.format = AL_FORMAT_STEREO8;
        } else if (this->audioData.channels == 2 && this->audioData.bitsPerSample == 16) {
            this->audioData.format = AL_FORMAT_STEREO16;
        } else {
            chiraLogger::log(ERR, "OGG", "Unrecognized OGG format with channels " + std::to_string(this->audioData.channels) + ", " + std::to_string(this->audioData.bitsPerSample) + "bps, in file " + filename);
            delete[] data;
            return false;
        }
        alCall(alBufferData, this->audioData.buffers[i], this->audioData.format, data, dataSoFar, this->audioData.sampleRate);
    }

    if (this->is3d && this->audioData.channels > 1) {
        chiraLogger::log(WARN, "OGG", "Stereo audio will not sound like it is playing from a 3D source");
    }

    alCall(alSourceQueueBuffers, this->audioData.source, OGG_NUM_BUFFERS, &this->audioData.buffers[0]);
    delete[] data;
    return true;
}

void oggFileSound::play() {
    if (!this->playing) {
        if (this->hasBeenPlayedPreviously) {
            this->seekBeginning();
            this->readFile(this->audioData.filename);
        }
        this->playing = true;
        alCall(alSourceStop, this->audioData.source);
        alCall(alSourcePlay, this->audioData.source);
    }
}

void oggFileSound::update() {
    ALint buffersProcessed = 0;
    alCall(alSourcef, this->audioData.source, AL_PITCH, this->pitch);
    alCall(alSourcef, this->audioData.source, AL_GAIN, this->gain);
    alCall(alSource3f, this->audioData.source, AL_POSITION, this->position.x, this->position.y, this->position.z);
    alCall(alSource3f, this->audioData.source, AL_VELOCITY, 0, 0, 0);
    alCall(alGetSourcei, this->audioData.source, AL_BUFFERS_PROCESSED, &buffersProcessed);
    if (buffersProcessed <= 0) {
        return;
    }
    while (buffersProcessed--) {
        ALuint buffer;
        alCall(alSourceUnqueueBuffers, this->audioData.source, 1, &buffer);
        char* data = new char[OGG_BUFFER_SIZE];
        memset(data, 0, OGG_BUFFER_SIZE);
        std::int32_t sizeRead = 0;
        while (sizeRead < OGG_BUFFER_SIZE) {
            std::int32_t result = ov_read(&this->audioData.oggVorbisFile, &data[sizeRead], OGG_BUFFER_SIZE - sizeRead, 0, 2, 1, reinterpret_cast<int*>(&this->audioData.oggCurrentSection));
            if (result == OV_HOLE) {
                chiraLogger::log(ERR, "OGG", "OV_HOLE found in update of buffer in " + this->audioData.filename);
                break;
            } else if (result == OV_EBADLINK) {
                chiraLogger::log(ERR, "OGG", "OV_EBADLINK found in update of buffer in " + this->audioData.filename);
                break;
            } else if (result == OV_EINVAL) {
                chiraLogger::log(ERR, "OGG", "OV_EINVAL found in update of buffer in " + this->audioData.filename);
                break;
            } else if (result == 0) {
                if (!this->loop) {
                    this->stop();
                    return;
                }
                this->seekBeginning();
            }
            sizeRead += result;
        }
        ALsizei dataSizeToBuffer = sizeRead;
        if (dataSizeToBuffer > 0) {
            alCall(alBufferData, buffer, this->audioData.format, data, dataSizeToBuffer, this->audioData.sampleRate);
            alCall(alSourceQueueBuffers, this->audioData.source, 1, &buffer);
        }
        if (dataSizeToBuffer < OGG_BUFFER_SIZE) {
            chiraLogger::log(WARN, "OGG", "Data missing in " + this->audioData.filename);
        }
        ALint state;
        alCall(alGetSourcei, this->audioData.source, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            alCall(alSourceStop, this->audioData.source);
            if (this->loop) {
                alCall(alSourcePlay, this->audioData.source);
            }
        }
        delete[] data;
    }
}

void oggFileSound::seekBeginning() {
    std::int32_t seekResult = ov_raw_seek(&this->audioData.oggVorbisFile, 0);
    switch (seekResult) {
        case OV_ENOSEEK:
            chiraLogger::log(ERR, "OGG", "OV_ENOSEEK found when trying to loop through " + this->audioData.filename);
            break;
        case OV_EINVAL:
            chiraLogger::log(ERR, "OGG", "OV_EINVAL found when trying to loop through " + this->audioData.filename);
            break;
        case OV_EREAD:
            chiraLogger::log(ERR, "OGG", "OV_EREAD found when trying to loop through " + this->audioData.filename);
            break;
        case OV_EFAULT:
            chiraLogger::log(ERR, "OGG", "OV_EFAULT found when trying to loop through " + this->audioData.filename);
            break;
        case OV_EOF:
            chiraLogger::log(ERR, "OGG", "OV_EOF found when trying to loop through " + this->audioData.filename);
            break;
        case OV_EBADLINK:
            chiraLogger::log(ERR, "OGG", "OV_EBADLINK found when trying to loop through " + this->audioData.filename);
            break;
        default:
            if (seekResult != 0) {
                chiraLogger::log(ERR, "OGG", "Unknown error encountered in ov_raw_seek in " + this->audioData.filename);
            }
    }
}

void oggFileSound::stop() {
    alCall(alSourceStop, this->audioData.source);
    this->playing = false;
    this->hasBeenPlayedPreviously = true;
}

void oggFileSound::discard() {
    if (this->playing) {
        this->stop();
    }
    alCall(alDeleteSources, 1, &this->audioData.source);
    for (auto buffer : this->audioData.buffers) {
        alCall(alDeleteBuffers, 1, &buffer);
    }
}

std::size_t oggFileSound::readOggVorbisCallback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle) {
    auto* audioData = reinterpret_cast<oggStreamData*>(fileHandle);
    auto length = (ALsizei) (size1 * size2);
    if (audioData->sizeConsumed + length > audioData->size) {
        length = audioData->size - audioData->sizeConsumed;
    }
    if (!audioData->file.is_open()) {
        audioData->file.open(audioData->filename, std::ios::binary);
        if (!audioData->file.is_open()) {
            chiraLogger::log(ERR, "OGG", "Could not open " + audioData->filename);
            return 0;
        }
    }
    char* moreData = new char[length];
    audioData->file.clear();
    audioData->file.seekg(audioData->sizeConsumed);
    if (!audioData->file.read(&moreData[0], length)) {
        if (audioData->file.eof()) {
            audioData->file.clear();
        } else if (audioData->file.fail()) {
            chiraLogger::log(ERR, "OGG", "Fail bit set in " + audioData->filename);
            audioData->file.clear();
            return 0;
        } else if (audioData->file.bad()) {
            chiraLogger::log(ERR, "OGG", "File " + audioData->filename + " has bad bit set");
            audioData->file.clear();
            return 0;
        }
    }
    audioData->sizeConsumed += length;
    memcpy(destination, &moreData[0], length);
    delete[] moreData;
    audioData->file.clear();
    return length;
}

std::int32_t oggFileSound::seekOggVorbisCallback(void* fileHandle, ogg_int64_t to, std::int32_t type) {
    auto* audioData = reinterpret_cast<oggStreamData*>(fileHandle);
    switch (type) {
        case SEEK_CUR:
            audioData->sizeConsumed += (ALsizei) to;
            break;
        case SEEK_END:
            audioData->sizeConsumed = (ALsizei) (audioData->size - to);
            break;
        case SEEK_SET:
            audioData->sizeConsumed = (ALsizei) to;
            break;
        default:
            return -1;
    }
    if (audioData->sizeConsumed < 0) {
        audioData->sizeConsumed = 0;
        return -1;
    }
    if (audioData->sizeConsumed > audioData->size) {
        audioData->sizeConsumed = audioData->size;
        return -1;
    }
    return 0;
}

long int oggFileSound::tellOggVorbisCallback(void* fileHandle) {
    return reinterpret_cast<oggStreamData*>(fileHandle)->sizeConsumed;
}
