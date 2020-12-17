#include <cmath>
#include <cstdio>
#include <iostream>
#include <complex>
#include <vector>

// Структура, описывающая заголовок WAV файла.
struct WAVHEADER
{
    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    unsigned int chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned int subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned int sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    unsigned int byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    unsigned int subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};

void FFT(std::vector<std::complex<double> >& data) {
    unsigned int n = data.size();
    if (n == 1)  return;
    std::vector<std::complex<double> > data0 (n/2),  data1 (n/2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        data0[j] = data[i];
        data1[j] = data[i + 1];
    }
    FFT (data0);
    FFT (data1);
    std::complex<double> w (1),  wn (cos(2 * M_PI/n), sin(2 * M_PI/n));
    for (int i = 0; i < n / 2; ++i) {
        data[i] = data0[i] + w * data1[i];
        data[i + n / 2] = data0[i] - w * data1[i];
        w *= wn;
    }
}

void FFTReverse(std::vector<std::complex<double> >& data) {
    unsigned int n = data.size();
    if (n == 1)  return;
    std::vector<std::complex<double> > data0 (n/2),  data1 (n/2);
    for (int i = 0, j = 0; i < n; i += 2, ++j) {
        data0[j] = data[i];
        data1[j] = data[i + 1];
    }
    FFTReverse (data0);
    FFTReverse (data1);
    std::complex<double> w(1),  wn(cos(-2 * M_PI/n), sin(-2 * M_PI/n));
    for (int i = 0; i < n / 2; ++i) {
        data[i] = data0[i] + w * data1[i];
        data[i + n / 2] = data0[i] - w * data1[i];
        data[i] /= 2,  data[i + n / 2] /= 2;
        w *= wn;
    }
}

int Write(const char *filename, WAVHEADER *p, char *data, unsigned int subchunk2Size) {
    FILE * fp;
    char *c;
    int size = sizeof(WAVHEADER);
    if ((fp = fopen(filename, "wb")) == nullptr) {
        perror("Error");
        return 1;
    }
    c = (char *)p;
    for (int i = 0; i < size; i++) {
        putc(*c++, fp);
    }
    for (int i = 0;i < subchunk2Size; ++i) {
        putc(data[i], fp);
    }
    fclose(fp);
    return 0;
}

void Solve(char* data, WAVHEADER& header) {
    std::vector<std::complex<double> > Data(1LL << (long long)ceil((double)log(header.subchunk2Size)/log(2)), 0);
    for (int i = 0;i < header.subchunk2Size; ++i) {
        Data[i] = *data++;
    }
    std::cout << "Data is successfully loaded. " << std::endl;

    FFT(Data);

    for (size_t i = 0; i < Data.size(); ++i) { // обнуляем 75 процентов
        if (i % 4 != 1)
            Data[i] = 0;
    }

    FFTReverse(Data);
    for (int i = 0;i < header.subchunk2Size; ++i) {
        data[i] = Data[i].real();
    }
    Write("speech1.wav", &header, data, header.subchunk2Size);
}

int main() {
    FILE *file = fopen("speech.wav", "rb");
    if (!file)
    {
        std::cout << "Failed open file";
        return 0;
    }

    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, file);

    // Выводим полученные данные
    std::cout << header.chunkId[0] << header.chunkId[1] << header.chunkId[2] << header.chunkId[3] << std::endl;
    printf("Chunk size: %d\n", header.chunkSize);
    std::cout << header.format[0] << header.format[1] << header.format[2] << header.format[3] << std::endl;
    std::cout << header.subchunk1Id[0] << header.subchunk1Id[1] << header.subchunk1Id[2] << header.subchunk1Id[3] << std::endl;
    printf("SubChunkId1: %d\n", header.subchunk1Size);
    printf("Audio format: %d\n", header.audioFormat);
    printf("Channels: %d\n", header.numChannels);
    printf("Sample rate: %d\n", header.sampleRate);
    printf("Bits per sample: %d\n", header.bitsPerSample);
    std::cout << header.subchunk2Id[0] << header.subchunk2Id[1] << header.subchunk2Id[2] << header.subchunk2Id[3] << std::endl;

    // Посчитаем длительность воспроизведения в секундах
    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);

    char* data = new char[header.subchunk2Size];
    fread(data, header.subchunk2Size, 1, file);

    Solve(data, header);

    delete[] data;
    fclose(file);
    return 0;
}
